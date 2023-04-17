#pragma once
#include "WaypointManager.h"
#include "Vector2D.h"

class Whisker
{
public:
	Whisker();
	Whisker(Line line);
	Whisker(Vector2D start, Vector2D end);

	bool intersectsBuilding(WaypointManager* waypointManager);
	bool intersectsVehicle(WaypointManager* waypointManager);

	void SetLine(Vector2D start, Vector2D end) { m_line.first = start, m_line.second = end; };
	void SetLine(Line line) { m_line = line; };

private:
	Line m_line;
};

