#include "ArriveState.h"
#include "Vector2D.h"
#include "DrawableGameObject.h"
#include "StaticTarget.h"
#include "AttachedTarget.h"
#include "Vehicle.h"
#include "FSM.h"

ArriveState::ArriveState() : HierarchicalState()
{
	m_destination = nullptr;
}

void ArriveState::Enter(Vehicle* agent)
{
	HierarchicalState::Enter(agent);	//Call the parent's enter to set up the  
	m_destination = new StaticTarget(agent->getWaypointManager()->getRandomWaypoint()->getPosition());	//Set the destination
	m_pStateManager->SetState(new MoveState(m_destination));	//Set the initial substate
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
	//If there is not currently a set state, the finite state machine has completed 
	if (!m_pStateManager->HasState())
	{
		//return new ArriveState();
		return nullptr;
	}
	else
	{
		return this;
	}
}



ArriveState::MoveState::MoveState(Target* destination) : State()
{
	m_destination = destination;
}

void ArriveState::MoveState::Enter(Vehicle* agent)
{
}

void ArriveState::MoveState::Exit()
{
	//Don't delete the destination as its common with the other state
	m_destination = nullptr;
}

void ArriveState::MoveState::Update(Vehicle* agent, float deltaTime)
{
	Vector2D toDestination = m_destination->GetPosition() - agent->getPosition();
	toDestination.Normalize();	//Find the direction toward the destination
	agent->applyForceInDirection(toDestination);	//Apply force in direction if the destination.
}

State* ArriveState::MoveState::Check(Vehicle* agent)
{
	//Get the distance squared between this and the destination and compare it with the distance squared to change state at.
	float distanceSq = m_destination->GetPosition().DistanceSq(agent->getPosition());
	bool arrived = arrived = distanceSq < m_brakeRadiusSq;
	//If we're within braking distance, switch to the braking substate
	if (arrived)
	{
		return new BrakeState(m_destination);
		//return nullptr;
	}
	else
	{
		return this;
	}
}

ArriveState::BrakeState::BrakeState(Target* destination) : MoveState(destination)
{
	
}

void ArriveState::BrakeState::Update(Vehicle* agent, float deltaTime)
{
	Vector2D toDestination = m_destination->GetPosition() - agent->getPosition();
	float distanceSq = toDestination.LengthSq();
	toDestination.Normalize();	//Find the direction away from the destination
	agent->applyForceInDirection(toDestination);	//Apply force in direction if the destination.
	toDestination *= -1;

	float brakePercentage = max(0.0f, ((m_brakeRadiusSq)-distanceSq) / (m_brakeRadiusSq));	//Calculate a percentage of how much of the brake area has been covered
	Vector2D brakeForce = toDestination * (brakePercentage / 1.2);	//Apply a brake force in the opposite direction  to the destination according to how close the car is to the location													
	agent->getForceMotion()->accumulateForce(brakeForce);
}

State* ArriveState::BrakeState::Check(Vehicle* agent)
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
