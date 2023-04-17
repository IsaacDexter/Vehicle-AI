#include "Whisker.h"

Whisker::Whisker()
{
	m_line = Line(Vector2D(), Vector2D());
}

Whisker::Whisker(Line line)
{
	m_line = line;
}

Whisker::Whisker(Vector2D start, Vector2D end)
{
	m_line = Line(start, end);
}

bool Whisker::intersectsBuilding(WaypointManager* waypointManager)
{
	return waypointManager->doesLineCrossBuilding(m_line) != nullptr;
}

bool Whisker::intersectsVehicle(WaypointManager* waypointManager)
{
	return waypointManager->doesLineCrossVehicle(m_line) != nullptr;
}
