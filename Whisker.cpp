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

bool Whisker::intersectsBuilding(WaypointManager* waypointManager)
{
	bool result = waypointManager->doesLineCrossBuilding(m_line) != nullptr;
	if (result)
	{
		setTexture(m_redTexture);
	}
	else
	{
		setTexture(m_greenTexture);
	}
	return result;
}

bool Whisker::intersectsVehicle(WaypointManager* waypointManager)
{
	bool result = waypointManager->doesLineCrossVehicle(m_line) != nullptr;
	if (result)
	{
		setTexture(m_redTexture);
	}
	else
	{
		setTexture(m_greenTexture);
	}
	return result;
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
