#pragma once
#include "State.h"
#include "Waypoint.h"

class WanderState :
    public AIState
{
public:
	WanderState();

	virtual void Enter();
	virtual AIState* Update(Vehicle* agent, float dt);
	virtual void DeliverMessage(Message message);
private:
	virtual AIState* CheckTransition(Vehicle* agent);
	virtual void Exit();

	Waypoint* m_destination;
};

