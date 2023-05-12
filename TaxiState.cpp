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
	HierarchicalState::Exit();
}

void TaxiState::Update(Vehicle* agent, float deltaTime)
{
	HierarchicalState::Update(agent, deltaTime);
}

State* TaxiState::Check(Vehicle* agent)
{
	//If we completed our last state...
	if (!m_pStateManager->HasState())
	{
		//Find a new state to instanciate
		m_pStateManager->SetState(GetBestState(agent));
	}
	return this;
}

State* TaxiState::GetBestState(Vehicle* agent)
{
	State* bestState = nullptr;
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

	std::array<float, 5> priorities = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	//Get the shortest distance to each option
	pPickupManager->GetNearestPickup(position, PickupType::FUEL, priorities.at(int(Substates::FUEL)));
	pPickupManager->GetNearestPickup(position, PickupType::SPEED, priorities.at(int(Substates::SPEED)));
	pPickupManager->GetNearestPickup(position, PickupType::PASSENGER, priorities.at(int(Substates::PASSENGER)));
	agent->GetNearestFare(priorities.at(int(Substates::DELIVER)));

	for (auto it = priorities.begin(); it != priorities.end(); ++it)
	{
		if (*it == FLT_MAX)
		{
			*it = 0.0f;
		}
		*it = Divide(1, sqrt(*it));	//Set each object to be inversely proporitonal to its distance.
	}

	priorities.at(int(Substates::PARK)) = FLT_MIN;

	//multiply fuel priority to the inverse of the fuel remaining
	priorities.at(int(Substates::FUEL)) *= Divide(15.0f, agent->GetFuel());
	//multiply passenger priority by the inverse of how many passengers there are
	priorities.at(int(Substates::PASSENGER)) *= Divide(2, agent->GetPassengerCount() + 1);
	//multiply delivering passengers by how many passengers there are
	priorities.at(int(Substates::DELIVER)) *= agent->GetPassengerCount();

	//Get the substate associated with the index of the lowest priority
	Substates highestPriority = Substates(std::distance(priorities.begin(), max_element(priorities.begin(), priorities.end())));
	
	switch (highestPriority)
	{
	case Substates::FUEL:
	{
		bestState = new GetFuelPickupState();
		break;
	}
	case Substates::SPEED:
	{
		bestState = new GetSpeedPickupState();
		break;
	}
	case Substates::PASSENGER:
	{
		bestState = new GetPassengerPickupState();
		break;
	}
	case Substates::DELIVER:
	{
		bestState = new DeliverState();
		break;
	}
	//Park will be picked if all the other states were abominably low, like 0
	case Substates::PARK:
	{
		bestState = new ParkState();
		break;
	}
	default:
	{
		bestState = new ParkState();
		break;
	}
	}
	return bestState;
}
