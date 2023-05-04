#pragma once

class State;
class Vehicle;

class StateManager
{
public:
	StateManager(Vehicle* agent);
	void Update(float dt);
	void SetState(State* state);
	bool HasState() { return m_state != nullptr; };

private:
	Vehicle* m_agent;
	State* m_state;
};

