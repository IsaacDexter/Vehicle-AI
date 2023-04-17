#include "Whisker.h"

HRESULT	Whisker::init(ID3D11Device* pd3dDevice)
{
	m_visible = true;

	float s = 10;
	m_scale = XMFLOAT3(s, s, s);

	setTextureName(L"Resources\\green.dds");

	// create the visible mesh to draw
	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	return hr;
}

bool Whisker::intersectsBuilding(WaypointManager* waypointManager)
{
	return waypointManager->doesLineCrossBuilding(m_line) != nullptr;
}

bool Whisker::intersectsVehicle(WaypointManager* waypointManager)
{
	return waypointManager->doesLineCrossVehicle(m_line) != nullptr;
}
