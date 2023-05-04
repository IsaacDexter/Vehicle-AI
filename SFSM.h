#pragma once
#include "SM.h"
#include <stack>

class State;
class Vehicle;

/// <summary>Stack based Finite State Machine</summary>
class SFSM :
    public SM
{
public:
	SFSM(Vehicle* agent);
	virtual void Update(float dt) override;
	virtual void SetState(State* state) override;
	virtual bool HasState() override { return !m_states.empty(); };

protected:
	Vehicle* m_agent;
	std::stack<State*> m_states;
};

