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
	m_options = std::priority_queue<std::pair<float, Option*>, std::less<float>>();

	Priority priority;

	//populate it with a transition to each type of state. They are all of priority 0 except the ParkState, who's priority will never change, and will always be just above 0.

	//Push back the transition and priority recalculation to the park state
	priority = [this] { return this->CalculateParkStatePriority(); };
	m_options.push(make_pair(0.01f,	Option(MakeTransition<ParkState>(), new Priority(priority))));

	//ditto with Deliver state
	priority = [this] { return this->CalculateDeliverStatePriority(); };
	m_options.push(make_pair(0.0f,	Option(MakeTransition<DeliverState>(), new Priority(priority))));
	
	//ditto with Get Fuel pickup state
	priority = [this] { return this->CalculateGetFuelPickupStatePriority(); };
	m_options.push(make_pair(0.0f,	Option(MakeTransition<GetFuelPickupState>(), new Priority(priority))));
	
	//ditto with Get passenger pickup state
	priority = [this] { return this->CalculateGetPassengerPickupStatePriority(); };
	m_options.push(make_pair(0.0f,	Option(MakeTransition<GetPassengerPickupState>(), new Priority(priority))));
	
	//and lastly, ditto with Get speed pickup state
	priority = [this] { return this->CalculateGetSpeedPickupStatePriority(); };
	m_options.push(make_pair(0.0f,	Option(MakeTransition<GetSpeedPickupState>(), new Priority(priority))));
}

void TaxiState::Enter(Vehicle* agent)
{
	//initialise the FSM
	HierarchicalState::Enter(agent);

	//Store the last transition made, i.e. the transition to the parked state. this stops transitions being made to states the machine is already in.
	m_chosenOption = m_options.top();
	//Call the transition, setting the state to a new ParkState as a default.
	(*(m_chosenOption->transition))();
}

void TaxiState::Exit()
{
}

void TaxiState::Update(Vehicle* agent, float deltaTime)
{
}

State* TaxiState::Check(Vehicle* agent)
{
	return nullptr;
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
