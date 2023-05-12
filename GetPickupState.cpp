#include "GetPickupState.h"
#include "Vehicle.h"
#include "PickupManager.h"
#include "Tasking.h"

GetPickupState::GetPickupState()
{
	m_destination = nullptr;
	m_task = nullptr;
}

void GetPickupState::Enter(Vehicle* agent)
{
	//Get the nearest destination
	m_destination = agent->GetPickupManager()->GetNearestPickup(agent->getPosition(), m_type);
	//Have the agent start heading towards the nearest destination and store a  handle to the task to allow the destination to be updated if a newer thang spawns, or the old thang is collected ( in that case cancel it).
	if (m_destination != nullptr)
	{
		m_task = agent->Seek(m_destination->getPosition());
	}
}

void GetPickupState::Exit()
{
	m_task->Complete();
	m_task = nullptr;
	m_destination = nullptr;
}

void GetPickupState::Update(Vehicle* agent, float deltaTime)
{
	//Update the destination to a nearer passenger if one exists, just in case a new one spawns or the old one was collected
	m_destination = agent->GetPickupManager()->GetNearestPickup(agent->getPosition(), m_type);
	//if the destination has been collected
	if (m_destination == nullptr)
	{
		//complete the arrival task. theres nowhere to go.
		m_task->Complete();
	}
}

State* GetPickupState::Check(Vehicle* agent)
{
	bool check = m_task->Check();
	//If there is no destination, or the car has reached the destination (short-circuited to stop checking for destination positions when there is no destination.)
	if (m_destination == nullptr || m_task == nullptr || check)
	{
		//transition back to the superstate where a decision can be made, as there is no need for passenger now, or passenger cannot be acquired.
		return nullptr;
	}
	return this;
}
