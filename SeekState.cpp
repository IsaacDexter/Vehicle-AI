#include "SeekState.h"
#include "Vector2D.h"
#include "DrawableGameObject.h"
#include "StaticTarget.h"
#include "AttachedTarget.h"
#include "Vehicle.h"

SeekState::SeekState() : State()
{
	m_destination = nullptr;
}

void SeekState::Enter(Vehicle* agent)
{
	m_destination = new StaticTarget(agent->getWaypointManager()->getRandomWaypoint()->getPosition());
}

void SeekState::Exit()
{
	delete m_destination;
	m_destination = nullptr;
}

void SeekState::Update(Vehicle* agent, float deltaTime)
{
	Vector2D toDestination = m_destination->GetPosition() - agent->getPosition();
	toDestination.Normalize();	//Find the direction toward the destination
	agent->applyForceInDirection(toDestination);	//Apply force in direction if the destination.
}

State* SeekState::Check(Vehicle* agent)
{
	//Get the distance squared between this and the destination and compare it with the distance squared to change state at.
	float distanceSq = m_destination->GetPosition().DistanceSq(agent->getPosition());
	bool arrived = arrived = distanceSq < m_arriveRadiusSq;
	//if we've found our destination, hand in a nullptr to clear the state
	if (arrived)
	{
		return nullptr;
	}
	else
	{
		return this;
	}
}
