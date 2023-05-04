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
	//Move the target if needs be.
	m_destination->Update();
	agent->applyForceToPosition(m_destination->GetPosition());
}

State* SeekState::Check(Vehicle* agent)
{
	//if we've found our destination, hand in a new state with a new random position
	if (agent->isPassed(m_destination->GetPosition()))
	{
		return new SeekState();
	}
	else
	{
		return this;
	}
}