#pragma once

#include "HierarchicalState.h"

class Target;
class Vector2D;
class DrawableGameObject;

class ArriveState :
	public HierarchicalState
{

#pragma region Substates

protected:
	class MoveState :
		public State
	{
	public:
		MoveState(Target* destination);

		virtual void Enter(Vehicle* agent) override;
		virtual void Exit() override;
		virtual void Update(Vehicle* agent, float deltaTime) override;
		virtual State* Check(Vehicle* agent) override;
	protected:
		Target* m_destination;
		/// <summary>The radius within which to switch to the braking state.</summary>
		const float m_brakeRadiusSq = 2048.0f;
	};

	class BrakeState :
		public MoveState
	{
	public:
		BrakeState(Target* destination);

		virtual void Update(Vehicle* agent, float deltaTime) override;
		virtual State* Check(Vehicle* agent) override;
	protected:
		/// <summary>The radius to deem as having arrived at the location</summary>
		const float arriveRadiusSq = 8.0f;
	};

#pragma endregion

public:
	ArriveState();

	virtual void Enter(Vehicle* agent) override;
	virtual void Exit() override;
	virtual void Update(Vehicle* agent, float deltaTime) override;
	virtual State* Check(Vehicle* agent) override;

protected:
	Target* m_destination;
};