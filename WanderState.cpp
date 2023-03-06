#include "WanderState.h"
#include "Vehicle.h"
#include "Messenger.h"


WanderState::WanderState()
{
}

void WanderState::Enter()
{
    m_destination = nullptr;
}

AIState* WanderState::Update(Vehicle* agent, float dt)
{
	if (m_destination = nullptr)	//if we have no waypoint currently set...
	{
		m_destination = agent->getWaypointManager()->getRandomWaypoint();	//get a random waypoint and set it as the destination
		agent->applyForceToPosition(m_destination->getPosition());	//Get a random waypoint and set the car to seek to that position.
	}

    return this;
}

void WanderState::DeliverMessage(Message message)
{
	if (message.type == MessageType::HERE_MESSAGE)	//When the object has arrived...
	{
		m_destination = nullptr;	//Remove the current destination
	}
}

AIState* WanderState::CheckTransition(Vehicle* agent)
{
	return nullptr;
}

void WanderState::Exit()
{
}
