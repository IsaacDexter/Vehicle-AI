#pragma once

#include "Target.h"
#include "DrawableGameObject.h"

class AttatchedTarget :
	public Target
{
public:
	/// <summary>Targets act as a handle for seek and other behaviours to track both static and dynamic objects.
	/// Creating and cleaning up a target at the position means the seek behaviour can save checks in its update function</summary>
	/// <param name="position">The dynamic object for the target's position to track to</param>
	AttatchedTarget(DrawableGameObject* parent) : Target() { m_parent = parent; };
	/// <summary>Updates the position to be that of the parent's</summary>
	virtual void Update() { if (m_parent != nullptr) m_position = m_parent->getPosition(); };

protected:
	DrawableGameObject* m_parent;
};