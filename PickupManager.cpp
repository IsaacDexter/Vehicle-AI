#include "PickupManager.h"
#include "PassengerPickup.h"
#include "FuelPickup.h"
#include "SpeedPickup.h"
#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Waypoint.h"
#include "Vehicle.h"

PickupManager::PickupManager(ID3D11Device* pd3dDevice, WaypointManager* waypointManager)
{
	m_pickups = new std::vector<PickupItem*>();
	m_pd3dDevice = pd3dDevice;
	m_waypointManager = waypointManager;
    CreatePickup();
}

PickupManager::~PickupManager()
{
    for (PickupItem* pu : *m_pickups)
    {
        delete pu;
    }
    m_pickups->clear();
    delete m_pickups;
    m_pickups = nullptr;
}

void PickupManager::Update(float dt)
{
    UpdatePickups(dt);
    m_counter += dt;
    if (m_counter > m_spawntime)
    {
        m_counter = 0.0f;
        CreatePickup();
    }
}

void PickupManager::CreatePickup()
{
	// NOTE!! for fuel and speedboost - you will need to create these here yourself!
	// create a pickup item
    PickupItem* pickup = nullptr;
    unsigned int type = rand() % 3;
    switch (type)
    {
    case 0:
    {
        pickup = new PassengerPickup();
        break;
    }
    case 1:
    {
        pickup = new SpeedPickup();
        break;
    }
    case 2:
    {
        pickup = new FuelPickup();
        break;
    }
    default:
        break;
    }

    //initialise it and push it back
	pickup->initMesh(m_pd3dDevice);
	m_pickups->push_back(pickup);
	//Set it to a random point in the wor;d
	pickup->setPosition(m_waypointManager->getRandomWaypoint()->getPosition());
}

void PickupManager::UpdatePickups(float dt)
{
	for (PickupItem* pickup : *m_pickups)
	{
		pickup->update(dt);
	}
}

bool PickupManager::CheckCollisions(Vehicle* other)
{
    XMVECTOR dummy;

    // get the position and scale of the car and store in dx friendly xmvectors
    XMVECTOR carPos;
    XMVECTOR carScale;
    XMMatrixDecompose(
        &carScale,
        &dummy,
        &carPos,
        XMLoadFloat4x4(other->getTransform())
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
    bool collision = false;
    //For each pickup (done this way to allow deletion from the list without iterating past summit)
    //Start at the first pickup
    auto it = m_pickups->begin();
    //as long as this isnt the end of the list
    while (it != m_pickups->end())
    {
        XMVECTOR puPos;
        XMVECTOR puScale;
        XMMatrixDecompose(
            &puScale,
            &dummy,
            &puPos,
            XMLoadFloat4x4((*it)->getTransform())
        );

        // bounding sphere for pickup item
        XMStoreFloat3(&scale, puScale);
        BoundingSphere boundingSpherePU;
        XMStoreFloat3(&boundingSpherePU.Center, puPos);
        boundingSpherePU.Radius = scale.x;

        // does the car bounding sphere collide with the pickup bounding sphere?
        if (boundingSphereCar.Intersects(boundingSpherePU))
        {
            //if it does, collect and delete the pickup.
            other->CollectPickup((*it));

            delete (*it);
            it = m_pickups->erase(it);

            //and mark that theres been a collision
            collision = true;

            //continue to avoid iteration
            continue;
        }
        ++it;
    }

    return collision;
}
