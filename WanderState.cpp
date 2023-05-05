#include "WanderState.h"

#include "StaticTarget.h"
#include "Vector2D.h"
#include "Vehicle.h"

WanderState::WanderState()
{
	m_destination = nullptr;

}

void WanderState::Enter(Vehicle* agent)
{
	m_destination = new StaticTarget(GetWanderPosition(agent));
}

void WanderState::Update(Vehicle* agent, float deltaTime)
{
	m_destination->Update();
	Vector2D toDestination = m_destination->GetPosition() - agent->getPosition();
	toDestination.Normalize();	//Find the direction toward the destination
	agent->applyForceInDirection(toDestination);	//Apply force in direction if the destination.
}

State* WanderState::Check(Vehicle* agent)
{
	//Get the distance squared between this and the destination and compare it with the distance squared to change state at.
	float distanceSq = m_destination->GetPosition().DistanceSq(agent->getPosition());
	bool arrived = arrived = distanceSq < m_arriveRadiusSq;
	//if we've found our destination, hand in a nullptr to clear the state
	if (arrived)
	{	//Simply reinitialise this state with a new destination
		Exit();
		Enter(agent);
	}
	return this;	//The AI will wander indefinitely until another state is put on top or it is explitly exited, it is a sort of idle state.
}

Vector2D WanderState::GetRandomDirection()
{
	float randomFloat = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
	float theta = std::acos(randomFloat);	//Get a random angle by generating a random float between -1 and 1, and finding the arccos of it.
	Vector2D direction = Vec2DfromAngle(theta);
	return direction;
}

Vector2D WanderState::GetRandomDirectionInGrid(Vehicle* agent)
{
	Vector2D position = agent->getWaypointManager()->getRandomWaypoint()->getPosition();
	Vector2D direction = position - agent->getPosition();
	direction.Normalize();
	return direction;
}

Vector2D WanderState::GetWanderPosition(Vehicle* agent)
{
	Vector2D wanderDirection = GetRandomDirectionInGrid(agent);	//Get a random waypoint's direction
	wanderDirection *= m_radius;
	Vector2D wanderPosition = agent->getPosition() += wanderDirection;
	return wanderPosition;
}
