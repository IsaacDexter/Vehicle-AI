#pragma once

#include "State.h"

class Target;
class Vector2D;
class DrawableGameObject;

/// <summary>The seek state moves towards the destination and immediately stops when it is there</summary>
class SeekState :
    public State
{
public:
	/// <summary>The seek state moves towards the destination and immediately stops when it is there</summary>
	SeekState();

	/// <summary>Sets the destination</summary>
	/// <param name="agent">The vehicle in this state</param>
	virtual void Enter(Vehicle* agent) override;
	/// <summary>Cleans up the target used as destination</summary>
	virtual void Exit() override;
	/// <summary>Updates the destination, calculates a vector to it, and applies that as a force</summary>
	/// <param name="agent">The vehicle in this state</param>
	/// <param name="deltaTime">The time since last frame</param>
	virtual void Update(Vehicle* agent, float deltaTime) override;
	/// <summary>Checks if within the arrived radius. If so, return no state to exit</summary>
	/// <param name="agent">The vehicle in this state</param>
	/// <returns>nullptr to exit this state, or another state to enter, or itself to remain</returns>
	virtual State* Check(Vehicle* agent) override;

protected:
	/// <summary>The destination being navigated towards</summary>
	Target* m_destination;
	/// <summary>How close one has to be to the destination to be considered there</summary>
	const float m_arriveRadiusSq = 900.0f;
};

