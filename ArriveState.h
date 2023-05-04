#pragma once

#include "HierarchicalState.h"

class Target;
class Vector2D;
class DrawableGameObject;

class ArriveState :
	public HierarchicalState
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

class ArriveState_Moving :
	public State
{
public:
	ArriveState_Moving(Target* destination);

	virtual void Enter(Vehicle* agent) override;
	virtual void Exit() override;
	virtual void Update(Vehicle* agent, float deltaTime) override;
	virtual State* Check(Vehicle* agent) override;
protected:
	Target* m_destination;
	/// <summary>The radius within which to switch to the braking state.</summary>
	const float m_brakeRadiusSq = 1000.0f;
};

class ArriveState_Braking :
	public ArriveState_Moving
{
public:
	ArriveState_Braking(Target* destination);

	virtual void Update(Vehicle* agent, float deltaTime) override;
	virtual State* Check(Vehicle* agent) override;
protected:
	/// <summary>The radius to deem as having arrived at the location</summary>
	const float arriveRadiusSq = 100.0f;
};