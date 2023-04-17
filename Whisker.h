#pragma once
#include "WaypointManager.h"
#include "Vector2D.h"

class Whisker
{
public:
	Whisker(WaypointManager* waypointManager, Line line);
	Whisker(WaypointManager* waypointManager, Vector2D start, Vector2D end);

	bool intersectsBuilding();
	bool intersectsVehicle();

	void SetLine(Vector2D start, Vector2D end) { m_line.first = start, m_line.second = end; };
	void SetLine(Line line) { m_line = line; };

private:
	Line m_line;
	WaypointManager* m_waypointManager;
};

