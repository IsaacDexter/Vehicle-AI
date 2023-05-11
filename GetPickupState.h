#pragma once
#include "State.h"
#include "PickupItem.h"

struct Task;

class GetPickupState abstract
	: public State
{
public:
	/// <summary>Abstract pickup state that heads towards a pickup of a specified type, with information on the matter gleaned from the agent's pickup manager.</summary>
	GetPickupState();

	virtual void Enter(Vehicle* agent) override;
	virtual void Exit() override;
	virtual void Update(Vehicle* agent, float deltaTime) override;
	virtual State* Check(Vehicle* agent) override;

protected:
	PickupType m_type;
	/// <summary>The nearest fuel pickup to the agent. If this is ever nullptr, it will try to transition to a different state so as not to waste time.</summary>
	PickupItem* m_destination;
	/// <summary>Stores a handle to the agents arrive to task, allowing it to be customised with different paramters (destinations) over the state's lifecycle.</summary>
	Task* m_task;
};

