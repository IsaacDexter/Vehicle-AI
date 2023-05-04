#pragma once
#include "State.h"

class StateManager;

class HierarchicalState abstract : 
	public State
{
public:
	HierarchicalState();

	virtual void Enter(Vehicle* agent);
	virtual void Exit();
	virtual void Update(Vehicle* agent, float deltaTime);
	virtual State* Check(Vehicle* agent) = 0;

protected:
	//Used to handle this state's child states
	StateManager* m_pStateManager;
};