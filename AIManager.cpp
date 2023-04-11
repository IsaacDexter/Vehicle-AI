#include "AIManager.h"
#include "Vehicle.h"
#include "DrawableGameObject.h"
#include "PickupItem.h"
#include "Waypoint.h"
#include "main.h"
#include "constants.h"
#include "ForceHelper.h"
#include "Tasking.h"

// AI Manager

AIManager::AIManager()
{
    m_pRedCar = nullptr;
    m_pBlueCar = nullptr;
    m_pTaskManager = nullptr;
}

AIManager::~AIManager()
{
    release();
}

void AIManager::release()
{
    clearDrawList();

    for (PickupItem* pu : m_pickups)
    {
        delete pu;
    }
    m_pickups.clear();

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

    //Set up the task manager
    m_pTaskManager = new TaskManager();
    m_pRedCar->setTaskManager(m_pTaskManager);
    m_pBlueCar->setTaskManager(m_pTaskManager);

    // create a passenger pickup item
    PickupItem* pPickupPassenger = new PickupItem();
    hr = pPickupPassenger->initMesh(pd3dDevice, pickuptype::Passenger);
    m_pickups.push_back(pPickupPassenger);

    // NOTE!! for fuel and speedboost - you will need to create these here yourself!

    // (needs to be done after waypoint setup)
    setRandomPickupPosition(pPickupPassenger);

    //m_pBlueCar->Wander();

    return hr;
}


void AIManager::update(const float fDeltaTime)
{
    for (unsigned int i = 0; i < m_waypointManager.getWaypointCount(); i++) {
        m_waypointManager.getWaypoint(i)->update(fDeltaTime);
        //AddItemToDrawList(m_waypointManager.getWaypoint(i)); // if you uncomment this, it will display the waypoints
    }

    for (int i = 0; i < m_waypointManager.getQuadpointCount(); i++)
    {
        Waypoint* qp = m_waypointManager.getQuadpoint(i);
        qp->update(fDeltaTime);
        //AddItemToDrawList(qp); // if you uncomment this, it will display the quad waypoints
    }

    // update and display the pickups
    for (unsigned int i = 0; i < m_pickups.size(); i++) {
        m_pickups[i]->update(fDeltaTime);
        AddItemToDrawList(m_pickups[i]);
    }

    // draw the waypoints nearest to the red car

    /*Waypoint* wp = m_waypointManager.getNearestWaypoint(m_pRedCar->getPosition());
    if (wp != nullptr)
    {
        vecWaypoints vwps = m_waypointManager.getNeighbouringWaypoints(wp);
        for (Waypoint* wp : vwps)
        {
            AddItemToDrawList(wp);
        }
    }*/

    m_pTaskManager->MaintainTasks(fDeltaTime);

    // update and draw the red car (and check for pickup collisions)
    if (m_pRedCar != nullptr)
    {
        m_pRedCar->update(fDeltaTime);
        checkForCollisions();
        AddItemToDrawList(m_pRedCar);
        //OutputDebugStringA(("FPS: " + std::to_string( 1000.0f / fDeltaTime) + ", ").c_str());
        //OutputDebugStringA(("Red Car: Velocity:" + std::to_string(m_pRedCar->getForceMotion()->getVelocity().Length()) + " m/s, Force:" + std::to_string(m_pRedCar->getForceMotion()->getForce().Length()) + " N\n").c_str());

    }

    // update and draw the blue car (and check for pickup collisions)
    if (m_pBlueCar != nullptr)
    {
        m_pBlueCar->update(fDeltaTime);
        checkForCollisions();
        AddItemToDrawList(m_pBlueCar);
        //OutputDebugStringA(("Blue Car:\nVelocity:" + std::to_string(m_pBlueCar->getForceMotion()->getVelocity().Length()/fDeltaTime) + " m/s\nForce:" + std::to_string(m_pBlueCar->getForceMotion()->getForce().Length()) + " N\n").c_str());
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
    m_pickups[0]->setPosition(Vector2D(x, y));
}

void AIManager::keyUp(WPARAM param)
{
    // hint 65-90 are a-z
    const WPARAM key_a = 65;
    const WPARAM key_e = 69;
    const WPARAM key_f = 70;
    const WPARAM key_i = 73;
    const WPARAM key_r = 82;
    const WPARAM key_s = 83;
    const WPARAM key_w = 87;
    const WPARAM key_space = 32;

    switch (param)
    {
    case VK_LEFT:
    {
        OutputDebugStringA("Currently controlling Red.\n");
        m_pCurrentCar = m_pRedCar;
        m_pOtherCar = m_pBlueCar;
        break;
    }
    case VK_RIGHT:
    {
        OutputDebugStringA("Currently controlling Blue.\n");
        m_pCurrentCar = m_pBlueCar;
        m_pOtherCar = m_pRedCar;
        break;
    }
    case key_a:
    {
        m_pCurrentCar->Arrive(m_pickups[0]->getPosition());
        break;
    }
    case key_e:
    {
        m_pCurrentCar->Evade(m_pOtherCar);
        break;
    }
    case key_f:
    {
        m_pCurrentCar->Flee(m_pOtherCar);
        break;
    }
    case key_i:
    {
        m_pCurrentCar->Intercept(m_pOtherCar);
        break;
    }
    case key_r:
    {
        m_pCurrentCar->Ramble();
        break;
    }
    case key_s:
    {
        m_pCurrentCar->Seek(m_pOtherCar);
        break;
    }
    case key_w:
    {
        m_pCurrentCar->Wander();
        break;
    }
    case key_space:
    {
        m_pTaskManager->Clear();
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

void AIManager::setRandomPickupPosition(PickupItem* pickup)
{
    if (pickup == nullptr)
        return;

    int x = (rand() % SCREEN_WIDTH) - (SCREEN_WIDTH / 2);
    int y = (rand() % SCREEN_HEIGHT) - (SCREEN_HEIGHT / 2);

    Waypoint* wp = m_waypointManager.getNearestWaypoint(Vector2D(x, y));
    if (wp) {
        pickup->setPosition(wp->getPosition());
    }
}

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
    if (m_pickups.size() == 0)
        return false;

    XMVECTOR dummy;

    // get the position and scale of the car and store in dx friendly xmvectors
    XMVECTOR carPos;
    XMVECTOR carScale;
    XMMatrixDecompose(
        &carScale,
        &dummy,
        &carPos,
        XMLoadFloat4x4(m_pRedCar->getTransform())
    );

    // create a bounding sphere for the car
    XMFLOAT3 scale;
    XMStoreFloat3(&scale, carScale);
    BoundingSphere boundingSphereCar;
    XMStoreFloat3(&boundingSphereCar.Center, carPos);
    boundingSphereCar.Radius = scale.x;

    // do the same for a pickup item
    // a pickup - !! NOTE it is only referring the first one in the list !!
    // to get the passenger, fuel or speedboost specifically you will need to iterate the pickups and test their type (getType()) - see the pickup class
    XMVECTOR puPos;
    XMVECTOR puScale;
    XMMatrixDecompose(
        &puScale,
        &dummy,
        &puPos,
        XMLoadFloat4x4(m_pickups[0]->getTransform())
    );

    // bounding sphere for pickup item
    XMStoreFloat3(&scale, puScale);
    BoundingSphere boundingSpherePU;
    XMStoreFloat3(&boundingSpherePU.Center, puPos);
    boundingSpherePU.Radius = scale.x;

    // does the car bounding sphere collide with the pickup bounding sphere?
    if (boundingSphereCar.Intersects(boundingSpherePU))
    {
        OutputDebugStringA("A collision has occurred!\n");
        m_pickups[0]->hasCollided();
        setRandomPickupPosition(m_pickups[0]);

        // you will need to test the type of the pickup to decide on the behaviour
        // m_pRedCar->dosomething(); ...

        return true;
    }

    return false;
}