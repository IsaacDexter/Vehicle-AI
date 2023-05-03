#pragma once

#include "State.h"

class Target;
class Vector2D;
class DrawableGameObject;

class ArriveState :
	public State
{
public:
	ArriveState();

	virtual void Enter(Vehicle* agent) override;
	virtual void Exit() override;
	virtual void Update(Vehicle* agent, float deltaTime) override;
	virtual State* Check(Vehicle* agent) override;

protected:
	Target* m_destination;
};

