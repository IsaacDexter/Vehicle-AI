#pragma once

class State;
class Vehicle;

class StateManager
{
public:
	StateManager(Vehicle* agent);
	void Update(float dt);
	void SetState(State* state);

private:
	Vehicle* m_agent;
	State* m_state;
};

