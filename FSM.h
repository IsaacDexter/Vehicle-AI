#pragma once
#include "SM.h"

class State;
class Vehicle;

class FSM :
	public SM
{
public:
	FSM(Vehicle* agent);
	virtual void Update(float dt) override;
	virtual void SetState(State* state) override;
	virtual bool HasState() override { return m_state != nullptr; };

protected:
	Vehicle* m_agent;
	State* m_state;
};

