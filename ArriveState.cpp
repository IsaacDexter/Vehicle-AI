#include "ArriveState.h"
#include "Vector2D.h"
#include "DrawableGameObject.h"
#include "StaticTarget.h"
#include "AttachedTarget.h"
#include "Vehicle.h"

ArriveState::ArriveState() : HierarchicalState()
{
	m_destination = nullptr;
}

void ArriveState::Enter(Vehicle* agent)
{
	HierarchicalState::Enter(agent);	//Call the parent's enter to set up the  
	m_destination = new StaticTarget(agent->getWaypointManager()->getRandomWaypoint()->getPosition());
}

void ArriveState::Exit()
{
	//Delete the destination
	delete m_destination;
	m_destination = nullptr;
	HierarchicalState::Exit();	//Delete the necessary things in the FSM
}

void ArriveState::Update(Vehicle* agent, float deltaTime)
{
	//Move the target if needs be.
	m_destination->Update();
	HierarchicalState::Update(agent, deltaTime);
}

State* ArriveState::Check(Vehicle* agent)
{
	//if we've found our destination, start braking, hand in a new state with a new random position
	if (agent->isArrived(m_destination->GetPosition()))
	{
		return new ArriveState();
		//return nullptr;
	}
	else
	{
		return this;
	}
}



ArriveState_Moving::ArriveState_Moving(Target* destination) : State()
{
	m_destination = destination;
}

void ArriveState_Moving::Enter(Vehicle* agent)
{
}

void ArriveState_Moving::Exit()
{
	//Don't delete the destination as its common with the other state
	m_destination = nullptr;
}

void ArriveState_Moving::Update(Vehicle* agent, float deltaTime)
{
	agent->applyForceToPosition(m_destination->GetPosition());
}

State* ArriveState_Moving::Check(Vehicle* agent)
{
	//Get the distance squared between this and the destination and compare it with the distance squared to change state at.
	float distanceSq = m_destination->GetPosition().DistanceSq(agent->getPosition());
	bool arrived = arrived = distanceSq < brakeRadiusSq;
	//If we're within braking distance, switch to the braking substate
	if (arrived)
	{
		return new ArriveState_Braking(m_destination);
		//return nullptr;
	}
	else
	{
		return this;
	}
}

ArriveState_Braking::ArriveState_Braking(Target* destination) : State()
{
	m_destination = destination;
}

void ArriveState_Braking::Enter(Vehicle* agent)
{

}

void ArriveState_Braking::Exit()
{
	//Don't delete the destination as its common with the other state
	m_destination = nullptr;
}

void ArriveState_Braking::Update(Vehicle* agent, float deltaTime)
{
	agent->applyForceToPosition(m_destination->GetPosition());
	agent->brake(m_destination->GetPosition());
}

State* ArriveState_Braking::Check(Vehicle* agent)
{
	//Get the distance squared between this and the destination and compare it with the distance squared to change state at.
	float distanceSq = m_destination->GetPosition().DistanceSq(agent->getPosition());
	bool arrived = arrived = distanceSq < arriveRadiusSq;
	//If we're close enough to have arrived, return nothing so we may leave the state entirely
	if (arrived)
	{
		return nullptr;
	}
	else
	{
		return this;
	}
}
