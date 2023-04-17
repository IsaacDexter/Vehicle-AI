#include "Whisker.h"

Whisker::Whisker(WaypointManager* waypointManager, Line line)
{
	m_line = line;
	m_waypointManager = waypointManager;
}

Whisker::Whisker(WaypointManager* waypointManager, Vector2D start, Vector2D end)
{
	m_line = Line(start, end);
	m_waypointManager = waypointManager;
}

bool Whisker::intersectsBuilding()
{
	return m_waypointManager->doesLineCrossBuilding(m_line) != nullptr;
}

bool Whisker::intersectsVehicle()
{
	return m_waypointManager->doesLineCrossVehicle(m_line) != nullptr;
}
