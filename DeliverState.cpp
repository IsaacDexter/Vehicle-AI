#include "DeliverState.h"
#include "Tasking.h"
#include "Vehicle.h"
#include "Passenger.h"

DeliverState::DeliverState()
{
	m_task = nullptr;
}

void DeliverState::Enter(Vehicle* agent)
{
	//Get the fare with the nearest destination and store it.
	m_destination = agent->GetNearestFare()->GetDestination();
	//Send the agent to head to that destination and store a handle to it just in case things need changing.
	m_task = agent->Seek(m_destination);
}

void DeliverState::Exit()
{
	//complete the task
	m_task->Complete();
	m_task = nullptr;
}

void DeliverState::Update(Vehicle* agent, float deltaTime)
{
	//No need to try to update the destination, the chances of randomly picking up a fare on the way are pretty low so its worth just sticking to a destination - they can't get nabbed either
}

State* DeliverState::Check(Vehicle* agent)
{
	//If there is no destination, or the car has reached the destination (short-circuited to stop checking for destination positions when there is no destination.)
	if (m_task == nullptr || m_task->Check())
	{
		agent->DeliverPassenger(m_destination);
		//transition back to the superstate where a decision can be made, as there is no need to do anything else now, as the passenger has been delivered..
		return nullptr;
	}
	return this;
}
