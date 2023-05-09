#pragma once
#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Vector2D.h"

class Whisker : public DrawableGameObject
{
public:
	virtual HRESULT init(ID3D11Device* pd3dDevice);

	bool intersectsBuilding(WaypointManager* waypointManager);
	Vehicle* intersectsVehicle(WaypointManager* waypointManager);

	void setLine(Vector2D start, Vector2D end) { m_line.first = start, m_line.second = end; };
	void setLine(Line line) { m_line = line; };

	void    setVisible(const bool visible) { m_visible = visible; }
	bool    isVisible() { return m_visible; }
private:
	Line m_line;
	bool m_visible;

	const std::wstring m_greenTexture = L"Resources\\green.dds";
	const std::wstring m_redTexture = L"Resources\\red.dds";

	void setTexture(std::wstring texture);
	std::wstring m_currentTexture = m_greenTexture;
};

