#include "TaxiState.h"
#include "ParkState.h"
#include "DeliverState.h"
#include "GetFuelPickupState.h"
#include "GetPassengerPickupState.h"
#include "GetSpeedPickupState.h"
#include "FSM.h"
#include "Vehicle.h"
#include "Passenger.h"

TaxiState::TaxiState() : HierarchicalState()
{
	
}

void TaxiState::Enter(Vehicle* agent)
{
	//initialise the FSM
	HierarchicalState::Enter(agent);

	
}

void TaxiState::Exit()
{
}

void TaxiState::Update(Vehicle* agent, float deltaTime)
{
	
}

State* TaxiState::Check(Vehicle* agent)
{
	//If we completed our last state...
	if (!m_pStateManager->HasState())
	{
		//Find a new state to instanciate
	}
	return this;
}

State* TaxiState::GetBestState(Vehicle* agent)
{
	/*
	- the priority of collecting fuel is inversely proportional to how much fuel there is, and how far the nearest fuel boost is
	- the priority of collecting speed boosts is inversely proportional to how far the nearest speed boost is
	- the priority of picking up passengers is inversely proportional to how many passengers are held and how far the nearest passenger is
	- the priority of delivering passengers is inversely proportional to how far the nearest destination are, and how empty the car is.
	- if nothing can be done, i.e.there is no fuel, speed boosts, passengers to collect or deliver, the car will idle
	*/

	//Cache pickup manager & position
	PickupManager* pPickupManager = agent->GetPickupManager();
	Vector2D position = agent->getPosition();

	std::array<float, 4> distancesSq = { 0.0f, 0.0f, 0.0f, 0.0f };

	//Get the shortest distance to each option
	pPickupManager->GetNearestPickup(position, PickupType::FUEL, distancesSq.at(0));
	pPickupManager->GetNearestPickup(position, PickupType::SPEED, distancesSq.at(1));
	pPickupManager->GetNearestPickup(position, PickupType::PASSENGER, distancesSq.at(2));
	//agent->GetNearestFare();
	agent->GetNearestFare(distancesSq.at(3));

	//Get the type of the highest priority
	Substates highestPriority = Substates(std::distance(distancesSq.begin(), max_element(distancesSq.begin(), distancesSq.end())));
	
	switch (highestPriority)
	{
	case Substates::FUEL:
	{
		break;
	}
	case Substates::SPEED:
	{
		break;
	}
	case Substates::PASSENGER:
	{
		break;
	}
	case Substates::DELIVER:
	{
		break;
	}
	case Substates::PARK:
	{
		break;
	}
	default:
	{
		break;
	}
	}
	return nullptr;
}
