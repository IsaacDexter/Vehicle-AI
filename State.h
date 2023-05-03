#pragma once

class Vehicle;

class State abstract
{
public:
	State();

	virtual void Enter(Vehicle* agent) = 0;
	virtual void Exit() = 0;
	virtual void Update(Vehicle* agent, float deltaTime) = 0;
	virtual State* Check(Vehicle* agent) = 0;
};