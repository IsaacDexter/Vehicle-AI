#include "Waypoint.h"

HRESULT	Waypoint::init(ID3D11Device* pd3dDevice, bool isOnTrack, bool checkPointID)
{
	m_visible = false;
	m_checkpointID = checkPointID;
	m_isOnTrack = isOnTrack;

	float s = 10;
	m_scale = XMFLOAT3(s, s, s);

	if (m_isOnTrack)
	{
		setTextureName(L"Resources\\blue.dds");
	}
	else {
		setTextureName(L"Resources\\red.dds");
	}

	setTextureName(m_currentTexture);

	// create the visible mesh to draw
	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	loadTexture(pd3dDevice, m_greenTexture);
	loadTexture(pd3dDevice, m_blueTexture);
	loadTexture(pd3dDevice, m_redTexture);

	return hr;
}

float Waypoint::distanceToWaypoint(Waypoint* waypoint)
{
	if (waypoint == nullptr)
		return FLT_MAX;

	return (float)waypoint->getPosition().Distance(getPosition());
}

void Waypoint::setTexture(std::wstring texture)
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
