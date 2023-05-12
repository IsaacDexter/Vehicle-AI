#include "AIManager.h"
#include "Vehicle.h"
#include "DrawableGameObject.h"
#include "PassengerPickup.h"
#include "PickupManager.h"
#include "Waypoint.h"
#include "main.h"
#include "constants.h"
#include "ForceHelper.h"
#include "Tasking.h"
#include "Controls.h"
#include "SeekState.h"
#include "ArriveState.h"
#include "TaxiState.h"
#include "FSM.h"

#include <iomanip>
#include <sstream>

// AI Manager

AIManager::AIManager()
{
    m_pRedCar = nullptr;
    m_pBlueCar = nullptr;
    m_pTaskManager = nullptr;
    m_currentMode = Controls::KEY_MOVEMENT;
}

AIManager::~AIManager()
{
    release();
}

void AIManager::release()
{
    clearDrawList();

    delete m_pRedCar;
    m_pRedCar = nullptr;
    delete m_pBlueCar;
    m_pBlueCar = nullptr;
}

HRESULT AIManager::initialise(ID3D11Device* pd3dDevice)
{
    srand(static_cast <unsigned> (time(0)));

    // create the red vehicle 
    float xPos = -500;
    float yPos = 300;

    m_pRedCar = new Vehicle();
    HRESULT hr = m_pRedCar->initMesh(pd3dDevice, carColour::redCar);
    m_pRedCar->setPosition(Vector2D(xPos, yPos));
    if (FAILED(hr))
        return hr;
    m_pCurrentCar = m_pRedCar;

    // create the blue vehicle 
    xPos = 500;
    yPos = -300;

    m_pBlueCar = new Vehicle();
    hr = m_pBlueCar->initMesh(pd3dDevice, carColour::blueCar);
    m_pBlueCar->setPosition(Vector2D(xPos, yPos));
    if (FAILED(hr))
        return hr;
    m_pOtherCar = m_pBlueCar;

    // setup the waypoints
    m_waypointManager.createWaypoints(pd3dDevice);
    m_pRedCar->setWaypointManager(&m_waypointManager);
    m_pBlueCar->setWaypointManager(&m_waypointManager);
    m_waypointManager.createDynamicBoundingBox(m_pRedCar, m_pRedCar->getPositionAddress(), m_pRedCar->getScale());
    m_waypointManager.createDynamicBoundingBox(m_pBlueCar, m_pBlueCar->getPositionAddress(), m_pBlueCar->getScale());

    //Set up the task manager
    m_pTaskManager = new TaskManager();
    m_pRedCar->setTaskManager(m_pTaskManager);
    m_pBlueCar->setTaskManager(m_pTaskManager);

    //Set the state managers/machines
    m_pRedCar->setStateManager(new FSM(m_pRedCar));
    m_pBlueCar->setStateManager(new FSM(m_pBlueCar));

    //Set up the pickup manager
    m_pickupManager = new PickupManager(pd3dDevice, &m_waypointManager);
    m_pBlueCar->SetPickupManager(m_pickupManager);
    m_pRedCar->SetPickupManager(m_pickupManager);

    return hr;
}


void AIManager::update(const float fDeltaTime)
{
    for (unsigned int i = 0; i < m_waypointManager.getWaypointCount(); i++) {
        m_waypointManager.getWaypoint(i)->update(fDeltaTime);
        if (m_waypointManager.getWaypoint(i)->isVisible())
        {
            //draw only visible waypoints
            AddItemToDrawList(m_waypointManager.getWaypoint(i)); // if you uncomment this, it will display the waypoints

        }
    }

    for (int i = 0; i < m_waypointManager.getQuadpointCount(); i++)
    {
        Waypoint* qp = m_waypointManager.getQuadpoint(i);
        qp->update(fDeltaTime);
        //AddItemToDrawList(qp); // if you uncomment this, it will display the quad waypoints
    }

    

    // draw the waypoints nearest to the red car

    Waypoint* wp = m_waypointManager.getNearestWaypoint(m_pCurrentCar->getPosition());
    if (wp != nullptr)
    {
        vecWaypoints vwps = m_waypointManager.getNeighbouringWaypoints(wp);
        for (Waypoint* wp : vwps)
        {
            //AddItemToDrawList(wp);
        }
    }

    m_pTaskManager->MaintainTasks(fDeltaTime);

    // update and draw the red car (and check for pickup collisions)
    if (m_pRedCar != nullptr)
    {
        m_pRedCar->update(fDeltaTime);
        AddItemToDrawList(m_pRedCar);
        //OutputDebugStringA(("FPS: " + std::to_string( 1000.0f / fDeltaTime) + ", ").c_str());
        //OutputDebugStringA(("Red Car: Velocity:" + std::to_string(m_pRedCar->getForceMotion()->getVelocity().Length()) + " m/s, Force:" + std::to_string(m_pRedCar->getForceMotion()->getForce().Length()) + " N\n").c_str());

    }

    // update and draw the blue car (and check for pickup collisions)
    if (m_pBlueCar != nullptr)
    {
        m_pBlueCar->update(fDeltaTime);
        AddItemToDrawList(m_pBlueCar);
        //OutputDebugStringA(("Blue Car:\nVelocity:" + std::to_string(m_pBlueCar->getForceMotion()->getVelocity().Length()/fDeltaTime) + " m/s\nForce:" + std::to_string(m_pBlueCar->getForceMotion()->getForce().Length()) + " N\n").c_str());
    }

    if (m_pCurrentCar != nullptr)
    {
        for (Whisker* whisker : m_pCurrentCar->getWhiskers())
        {
            whisker->update(fDeltaTime);
            if (whisker->isVisible())
            {
                AddItemToDrawList(whisker);
            }
        }
    }
    //update pickups
    m_pickupManager->Update(fDeltaTime);

    checkForCollisions();

    //Add pickups to the draw list.
    std::vector<PickupItem*>* pickups = m_pickupManager->GetPickups();
    for (auto it = pickups->begin(); it != pickups->end(); ++it)
    {
        AddItemToDrawList(*it);
    }
}

/// <summary>
/// 
/// </summary>
/// <param name="x">The X coordinate, with 0 being the centre, ~-500 for the left border and ~500 for the right.</param>
/// <param name="y">The Y coordinate, with 0 being the centre, ~400 for the top border and ~-400 for the bottom.</param>
void AIManager::mouseUp(int x, int y)
{
    // HINT you will find this useful later on...
    //Waypoint* wp = m_waypointManager.getNearestWaypoint(Vector2D(x, y));
    //if (wp == nullptr)
    //	return;

    // Applies a directional force to the car from its current position to (x,y). SEEK_MESSAGE is used to determine when the car is at it's destination.
    //m_pRedCar->applyForceToPosition(Vector2D(x, y), ARRIVE_MESSAGE);
    //m_pickups[0]->setPosition(Vector2D(x, y));
    m_clickPos = Vector2D(x, y);
}

void AIManager::keyUp(WPARAM param)
{


    switch (m_currentMode)
    {
    case Controls::KEY_STATES:
    {
        //Handle the specified movement system controls
        switch (param)
        {
        case Controls::KEY_SEEK:
        {
            m_pBlueCar->setState(new SeekState());
            break;
        }
        case Controls::KEY_ARRIVE:
        {
            m_pBlueCar->setState(new ArriveState());
            break;
        }
        case Controls::KEY_WANDER:
        {
            break;
        }
        case Controls::KEY_PURSUIT:
        {
            break;
        }
        case Controls::KEY_FLEE:
        {
            break;
        }
        case Controls::KEY_OBSTACLE_AVOIDANCE:
        {
            break;
        }
        case Controls::KEY_CLEAR:
        {
            m_pBlueCar->setState(nullptr);
            m_pRedCar->setState(nullptr);
            break;
        }
        case Controls::KEY_MOVEMENT:
        {
            m_currentMode = Controls::KEY_MOVEMENT;
            break;
        }
        case Controls::KEY_DECISION:
        {
            m_currentMode = Controls::KEY_DECISION;
            break;
        }
        case Controls::KEY_STATES:
        {
            m_currentMode = Controls::KEY_STATES;
            break;
        }
        default:
            break;
        }
        break;
    }
    case Controls::KEY_DECISION:
    {
        //m_pBlueCar->setState(new TaxiState());
        
        switch (param)
        {
        case VK_LEFT:
        {
            m_pRedCar->setState(new TaxiState());
            break;
        }
        case VK_RIGHT:
        {
            m_pBlueCar->setState(new TaxiState());
            break;
        }
        case Controls::KEY_MONEY:
        {
            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << m_pRedCar->GetMoney();
                std::string s = stream.str();
                OutputDebugStringA(("Red Car has £" + s + "\n").c_str());
            }

            {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << m_pBlueCar->GetMoney();
                std::string s = stream.str();
                OutputDebugStringA(("Blue Car has £" + s + "\n").c_str());
            }
        
            break;
        }
        case Controls::KEY_MOVEMENT:
        {
            m_currentMode = Controls::KEY_MOVEMENT;
            break;
        }
        case Controls::KEY_DECISION:
        {
            m_currentMode = Controls::KEY_DECISION;
            break;
        }
        case Controls::KEY_STATES:
        {
            m_currentMode = Controls::KEY_STATES;
            break;
        }
        default:
            break;
        }
        break;
    }
    case Controls::KEY_MOVEMENT:
    {
        //handle the original tasking system mode
        switch (param)
        {
        case VK_LEFT:
        {
            //OutputDebugStringA("Currently controlling Red.\n");
            m_pCurrentCar = m_pRedCar;
            m_pOtherCar = m_pBlueCar;
            break;
        }
        case VK_RIGHT:
        {
            //OutputDebugStringA("Currently controlling Blue.\n");
            m_pCurrentCar = m_pBlueCar;
            m_pOtherCar = m_pRedCar;
            break;
        }
        case Controls::KEY_ARRIVE:
        {
            m_pCurrentCar->Arrive(m_clickPos);
            break;
        }
        case Controls::KEY_EVADE:
        {
            m_pCurrentCar->Evade(m_pOtherCar);
            break;
        }
        case Controls::KEY_FLEE:
        {
            m_pCurrentCar->Flee(m_pOtherCar);
            break;
        }
        case Controls::KEY_INTERCEPT:
        {
            m_pCurrentCar->Intercept(m_pOtherCar);
            break;
        }
        case Controls::KEY_RAMBLE:
        {
            m_pCurrentCar->Ramble();
            break;
        }
        case Controls::KEY_SEEK:
        {
            m_pCurrentCar->Seek(m_pOtherCar);
            break;
        }
        case Controls::KEY_WANDER:
        {
            m_pCurrentCar->Wander();
            break;
        }
        case Controls::KEY_CLEAR:
        {
            m_pTaskManager->Clear();
            break;
        }
        case Controls::KEY_OBSTACLE_AVOIDANCE:
        {
            m_pCurrentCar->ObstaceAvoidance(m_clickPos);
            break;
        }
        case Controls::KEY_BUILDING_AVOIDANCE:
        {
           // m_pCurrentCar->BuildingAvoidance();
            break;
        }
        case Controls::KEY_PURSUIT:
        {
            m_pCurrentCar->Pursuit(m_pOtherCar);
            break;
        }
        case Controls::KEY_MOVEMENT:
        {
            m_currentMode = Controls::KEY_MOVEMENT;
            break;
        }
        case Controls::KEY_DECISION:
        {
            m_currentMode = Controls::KEY_DECISION;
            break;
        }
        case Controls::KEY_STATES:
        {
            m_currentMode = Controls::KEY_STATES;
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

/// <summary>
/// 
/// </summary>
/// <param name="param"></param>
void AIManager::keyDown(WPARAM param)
{

}

//void AIManager::setRandomPickupPosition(PickupItem* pickup)
//{
//    if (pickup == nullptr)
//        return;
//
//    int x = (rand() % SCREEN_WIDTH) - (SCREEN_WIDTH / 2);
//    int y = (rand() % SCREEN_HEIGHT) - (SCREEN_HEIGHT / 2);
//
//    Waypoint* wp = m_waypointManager.getNearestWaypoint(Vector2D(x, y));
//    if (wp) {
//        pickup->setPosition(wp->getPosition());
//    }
//}

/*
// hello. This is hopefully the only time you may need to use and alter directx code
// the relevant #includes are already in place, but if you create your own collision class (or use this code anywhere else)
// make sure you have the following:

#include <d3d11_1.h> // this has the appropriate directx structures / objects
#include <DirectXCollision.h> // this is the dx collision class helper
using namespace DirectX; // this means you don't need to put DirectX:: in front of objects like XMVECTOR and so on.
*/

bool AIManager::checkForCollisions()
{
    bool collided = false;
    if (m_pBlueCar != nullptr)
    {
        collided = m_pickupManager->CheckCollisions(m_pBlueCar);
    }
    if (m_pRedCar != nullptr)
    {
        collided = m_pickupManager->CheckCollisions(m_pRedCar);
    }
    return collided;
}