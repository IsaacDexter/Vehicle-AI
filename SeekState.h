#pragma once

#include "State.h"

class Target;
class Vector2D;
class DrawableGameObject;

class SeekState :
    public State
{
public:
	SeekState();

	virtual void Enter(Vehicle* agent) override;
	virtual void Exit() override;
	virtual void Update(Vehicle* agent, float deltaTime) override;
	virtual State* Check(Vehicle* agent) override;

protected:
	Target* m_destination;
	const float m_arriveRadiusSq = 8.0f;
};

