#include "TaxiState.h"
#include "ParkState.h"
#include "DeliverState.h"
#include "GetFuelPickupState.h"
#include "GetPassengerPickupState.h"
#include "GetSpeedPickupState.h"
#include "FSM.h"

TaxiState::TaxiState() : HierarchicalState()
{
	//create the priority queue of functions that call transitions to new states of templated types for this
	m_options = OptionMap();

	Priority priority;

	//populate it with a transition to each type of state. They are all of priority 0 except the ParkState, who's priority will never change, and will always be just above 0.

	//Push back the transition and priority recalculation to the park state
	priority = [this] { return this->CalculateParkStatePriority(); };
	m_options.emplace(	MakeTransition<ParkState>(), new Priority(priority));

	//ditto with Deliver state
	priority = [this] { return this->CalculateDeliverStatePriority(); };
	m_options.emplace(MakeTransition<DeliverState>(), new Priority(priority));
	
	//ditto with Get Fuel pickup state
	priority = [this] { return this->CalculateGetFuelPickupStatePriority(); };
	m_options.emplace(MakeTransition<GetFuelPickupState>(), new Priority(priority));
	
	//ditto with Get passenger pickup state
	priority = [this] { return this->CalculateGetPassengerPickupStatePriority(); };
	m_options.emplace(MakeTransition<GetPassengerPickupState>(), new Priority(priority));
	
	//and lastly, ditto with Get speed pickup state
	priority = [this] { return this->CalculateGetSpeedPickupStatePriority(); };
	m_options.emplace(MakeTransition<GetSpeedPickupState>(), new Priority(priority));
}

void TaxiState::Enter(Vehicle* agent)
{
	//initialise the FSM
	HierarchicalState::Enter(agent);

	//Update the priorities of all of the options. Store the option with the highest priority as you do this.
	m_lastTransition = GetBestTransition();
	//Call the transition, setting the state to a new State
	(*m_lastTransition)();
}

void TaxiState::Exit()
{
}

void TaxiState::Update(Vehicle* agent, float deltaTime)
{
	//Update the priorities of all the transitions and get the highest priority transition
	Transition* bestTransition = GetBestTransition();
	//If the best transition that could be made isnt to the one we previously made (i.e. we arent already in the best possible state):
	if (bestTransition != m_lastTransition)
	{
		//Store the best transition as the last transition we made
		m_lastTransition = bestTransition;
		//Call the transition, setting the state to a new State
		(*m_lastTransition)();
	}
}

State* TaxiState::Check(Vehicle* agent)
{
	return this;
}

float TaxiState::CalculateParkStatePriority()
{
	//return a flat value thats always 0. we never really wanna switch into park state.
	return 0.01f;
}

float TaxiState::CalculateDeliverStatePriority()
{
	return 0.0f;
}

float TaxiState::CalculateGetFuelPickupStatePriority()
{
	return 0.0f;
}

float TaxiState::CalculateGetPassengerPickupStatePriority()
{
	return 0.0f;
}

float TaxiState::CalculateGetSpeedPickupStatePriority()
{
	return 0.0f;
}

void TaxiState::CalculatePriorities()
{
	//Calculate the priorities of each possible transition.
	
}

Transition* TaxiState::GetBestTransition()
{
	float highestPrioity = 0.0f;
	Transition* bestOption = nullptr;
	for (auto it = m_options.begin(); it != m_options.end(); ++it)
	{
		//Update the priority of this transition
		float priority = (*it->second)();
		//if this priority is higher than the current highest priority
		if (priority > highestPrioity)
		{
			//Make this the new highest priority
			highestPrioity = priority;
			bestOption = it->first;
		}
	}
	return bestOption;
}
