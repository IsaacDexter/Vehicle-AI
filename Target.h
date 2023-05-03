#pragma once

#include "Vector2D.h"

class Target abstract
{
public:
	Target() { m_position = Vector2D(); };
	virtual void Update() = 0;

	Vector2D GetPosition() const { return m_position; };
protected:
	void SetPosition(Vector2D position) { m_position = position; };
	Vector2D m_position;
};