#include "ArriveState.h"
#include "Vector2D.h"
#include "DrawableGameObject.h"
#include "StaticTarget.h"
#include "AttachedTarget.h"
#include "Vehicle.h"

ArriveState::ArriveState() : State()
{
	m_destination = nullptr;
}

void ArriveState::Enter(Vehicle* agent)
{
	m_destination = new StaticTarget(agent->getWaypointManager()->getRandomWaypoint()->getPosition());
}

void ArriveState::Exit()
{
	delete m_destination;
	m_destination = nullptr;
}

void ArriveState::Update(Vehicle* agent, float deltaTime)
{
	//Move the target if needs be.
	m_destination->Update();
	agent->applyForceToPosition(m_destination->GetPosition());
}

State* ArriveState::Check(Vehicle* agent)
{
	//if we've found our destination, start braking, hand in a new state with a new random position
	if (agent->isArrived(m_destination->GetPosition()))
	{
		return new ArriveState();
	}
	else
	{
		return this;
	}
}
