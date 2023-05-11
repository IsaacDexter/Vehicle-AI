#pragma once
#include "State.h"
class ParkState :
    public State
{
public:
	/// <summary>This is the idle state. While in this state, the car will not move, to conserve fuel. Its only really worth doing when there is nothin else to do. It cannot exit itself.</summary>
	ParkState() { ; };

	virtual void Enter(Vehicle* agent) override { ; };
	virtual void Exit() override { ; };
	virtual void Update(Vehicle* agent, float deltaTime) override { ; };
	virtual State* Check(Vehicle* agent) override { return this; };
};

