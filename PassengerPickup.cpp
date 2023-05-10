#include "PassengerPickup.h"
#include "Vehicle.h"

HRESULT PassengerPickup::initMesh(ID3D11Device* pd3dDevice)
{
	setTextureName(L"Resources\\pickup_passenger.dds");
	//// set the graphic for the type of pickup
	//if (m_type == pickuptype::Fuel)
	//	setTextureName(L"Resources\\pickup_fuel.dds");
	//else if (m_type == pickuptype::Passenger)
	//	setTextureName(L"Resources\\pickup_passenger.dds");
	//else
	//	setTextureName(L"Resources\\pickup_speedboost.dds");
	return PickupItem::initMesh(pd3dDevice);
}

void PassengerPickup::pickup(Vehicle* collector)
{
	m_driver = collector;
	//Get a random destination
	Vector2D destination = m_driver->getWaypointManager()->getRandomWaypoint()->getPosition();
	//try to get the collector to pick up this passenger.
	if (m_driver->PickupPassenger(this, destination))
	{
		//Calculate a tip in advanced based on direct distance to the target.
		m_tip = (destination - getPosition()).Length();
		//If such an operation was successful, increase the fuel consumption of the vehicle
		m_driver->SetFuelConsumption(m_driver->GetFuelConsumption() +  m_fuelConsumptionIncrease);
	}
}

void PassengerPickup::dropoff()
{
	//Tip the driver
	m_driver->GiveTip(m_tip);
	//Reduce the drivers fuel consumption now the passenger is delivered.
	m_driver->SetFuelConsumption(m_driver->GetFuelConsumption() - m_fuelConsumptionIncrease);
}
