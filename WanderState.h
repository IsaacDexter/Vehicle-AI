#pragma once
#include "SeekState.h"

class Target;
class Vector2D;

class WanderState :
    public SeekState
{
public:
	WanderState();

	virtual void Enter(Vehicle* agent) override;
	virtual void Update(Vehicle* agent, float deltaTime) override;
	virtual State* Check(Vehicle* agent) override;

protected:
	virtual Vector2D GetRandomDirection();
	virtual Vector2D GetRandomDirectionInGrid(Vehicle* agent);
	virtual Vector2D GetWanderPosition(Vehicle* agent);


	const float m_radius = 560.0f;
	const float m_interval = 0.6f;
};

