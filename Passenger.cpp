#include "Passenger.h"
#include "Vehicle.h"

void Passenger::Pickup(Vehicle* driver)
{

	m_driver = driver;
	//Get a random destination
	m_destination = m_driver->getWaypointManager()->getRandomWaypoint()->getPosition();
	//try to get the collector to pick up this passenger.
	if (m_driver->PickupPassenger(this, destination))
	{
		//Calculate a tip in advanced based on the vehcicles direct distance to the target.
		m_tip = (destination - m_driver->getPosition()).Length();
		//If such an operation was successful, increase the fuel consumption of the vehicle
		m_driver->SetFuelConsumption(m_driver->GetFuelConsumption() + m_fuelConsumptionIncrease);
	}

}

void Passenger::Dropoff()
{
	//Tip the driver
	m_driver->GiveTip(m_tip);
	//Reduce the drivers fuel consumption now the passenger is delivered.
	m_driver->SetFuelConsumption(m_driver->GetFuelConsumption() - m_fuelConsumptionIncrease);
}