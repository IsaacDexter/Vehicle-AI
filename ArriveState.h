#pragma once

#include "HierarchicalState.h"

class Target;
class Vector2D;
class DrawableGameObject;

/// <summary>The Arrive State is a hierarchical state with two states below it, arrive and move. it's first state moves towards the destination, then the second starts slowing down once its getting close.</summary>
class ArriveState :
	public HierarchicalState
{

#pragma region Substates

protected:
	/// <summary>The move state is the substate in use when the superstate is first moving to the destination</summary>
	class MoveState :
		public State
	{
	public:
		/// <summary>The move state is the substate in use when the superstate is first moving to the destination</summary>
		/// <param name="destination">The target to navigate towards and update</param>
		MoveState(Target* destination);

		/// <param name="agent">The vehicle in this state</param>
		virtual void Enter(Vehicle* agent) override;
		virtual void Exit() override;
		/// <summary>Moves towards the destination and updates said destination</summary>
		/// <param name="agent">The vehicle in this state</param>
		/// <param name="deltaTime">time elapsed this frame</param>
		virtual void Update(Vehicle* agent, float deltaTime) override;
		/// <summary>Checks if within the braking radius. If so, switch to the braking state</summary>
		/// <param name="agent">The vehicle in this state</param>
		/// <returns>nullptr to exit this state, or another state to enter, or itself to remain</returns>
		virtual State* Check(Vehicle* agent) override;
	protected:
		/// <returns>The normalised direction to the destination</returns>
		virtual Vector2D GetDirection(Vehicle* agent);
		/// <summary>Checks to see if the we are within the radius of the destination</summary>
		/// <param name="agent">The vehicle in this state</param>
		/// <param name="radius">The radius of the destination. m_brakeRadiusSq</param>
		/// <returns>whether the vehicle is within the radius</returns>
		virtual bool Arrived(Vehicle* agent, const float& radius);

		/// <summary>The target being headed towards</summary>
		Target* m_destination;
		/// <summary>The radius within which to switch to the braking state.</summary>
		const float m_brakeRadius = 120.0f;
		const float m_brakeRadiusSq = m_brakeRadius * m_brakeRadius;
	};

	/// <summary>The brake state is the substate in use when the superstate is secondly moving to the destination and slowing down</summary>
	class BrakeState :
		public MoveState
	{
	public:
		/// <summary>The brake state is the substate in use when the superstate is secondly moving to the destination and slowing down</summary>
		/// <param name="destination">The target to navigate towards and update</param>
		BrakeState(Target* destination);
		/// <summary>Moves towards the destination, then applies a smaller force away from it and updates said destination</summary>
		/// <param name="agent">The vehicle in this state</param>
		/// <param name="deltaTime">time elapsed this frame</param>
		virtual void Update(Vehicle* agent, float deltaTime) override;
		/// <summary>Checks if within the arrived radius. If so, exit this state</summary>
		/// <param name="agent">The vehicle in this state</param>
		/// <returns>nullptr to exit this state, or another state to enter, or itself to remain</returns>
		virtual State* Check(Vehicle* agent) override;
	protected:
		/// <summary>Finds the portion of the brake radius covered</summary>
		/// <param name="coveredDistance">The distance from the destination, Sq</param>
		/// <param name="totalDistance">Braking distance is used rather than a variable</param>
		/// <param name="direction">The direction to apply the force in</param>
		/// <returns>The force to be applied</returns>
		virtual Vector2D GetBrakeForce(const float& distanceSq, Vehicle* agent, float dt);
		/// <summary>The radius to deem as having arrived at the location</summary>
		const float arriveRadiusSq = 900.0f;
	};

#pragma endregion

public:
	/// <summary>The Arrive State is a hierarchical state with two states below it, arrive and move. it's first state moves towards the destination, then the second starts slowing down once its getting close.</summary>
	ArriveState();
	/// <summary>Enters the first state and sets up the destination</summary>
	/// <param name="agent">The vehicle in this state</param>
	virtual void Enter(Vehicle* agent) override;
	/// <summary>Cleans up the two substates and exits</summary>
	virtual void Exit() override;
	/// <summary>Updates whichever of its substates is active</summary>
	/// <param name="agent">The vehicle in this state</param>
	/// <param name="deltaTime">Time since last frame</param>
	virtual void Update(Vehicle* agent, float deltaTime) override;
	/// <summary>Checks if each substate has completed. If so, exit this state</summary>
	/// <param name="agent">The vehicle in this state</param>
	/// <returns>nullptr to exit this state, or another state to enter, or itself to remain</returns>
	virtual State* Check(Vehicle* agent) override;

protected:
	Target* m_destination;
};