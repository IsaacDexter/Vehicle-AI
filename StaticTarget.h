#pragma once

#include "Target.h"

class StaticTarget :
	public Target
{
public:
	/// <summary>Targets act as a handle for seek and other behaviours to track both static and dynamic objects.
	/// Creating and cleaning up a target at the position means the seek behaviour can save checks in its update function</summary>
	/// <param name="position">The static position for the target to track to</param>
	StaticTarget(Vector2D position) : Target() { m_position = position; };
	/// <summary>Static targets do not move, so lack an update function, they are static</summary>
	virtual void Update() { ; };
};