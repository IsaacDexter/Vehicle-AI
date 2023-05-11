#pragma once
#include "State.h"
#include "Vector2D.h"

class Task;

class DeliverState :
    public State
{
public:
	/// <summary>Gets the fare in the agent with the closest destination and heads towards it.</summary>
	DeliverState();

	virtual void Enter(Vehicle* agent) override;
	virtual void Exit() override;
	virtual void Update(Vehicle* agent, float deltaTime) override;
	virtual State* Check(Vehicle* agent) override;
protected:
	Vector2D m_destination;
	/// <summary>Handle to the task to move the agent to its destination, allowing the state to cancel it early.</summary>
	Task* m_task;
};

