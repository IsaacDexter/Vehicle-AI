#include "Whisker.h"

HRESULT	Whisker::init(ID3D11Device* pd3dDevice)
{
	m_visible = true;

	float s = 10;
	m_scale = XMFLOAT3(s, s, s);

	setTextureName(m_currentTexture);

	// create the visible mesh to draw
	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	loadTexture(pd3dDevice, m_greenTexture);
	loadTexture(pd3dDevice, m_redTexture);

	return hr;
}

BoundingBox* Whisker::intersectsBuilding(WaypointManager* waypointManager)
{
	//get the bounding box of whatever building we intersected
	BoundingBox* intersected = waypointManager->doesLineCrossBuilding(m_line);
	if (intersected != nullptr)
	{
		setTexture(m_redTexture);
	}
	else
	{
		setTexture(m_greenTexture);
	}
	return intersected;
}

std::vector<Vehicle*>* Whisker::intersectsVehicle(WaypointManager* waypointManager)
{
	std::vector<Vehicle*>* crossed = waypointManager->doesLineCrossVehicle(m_line);
	return crossed;
}

void Whisker::setTexture(std::wstring texture)
{
	//if (m_pTextureResourceViews.at(m_redTexture) == m_pTextureResourceViews.at(m_greenTexture))
	//{
	//	OutputDebugStringA("The two textures are identical!\n");
	//}
	if (texture != m_currentTexture)
	{
		m_currentTexture = texture;
		switchTexture(m_currentTexture);
	}
}
