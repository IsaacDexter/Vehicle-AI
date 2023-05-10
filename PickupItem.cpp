#include "PickupItem.h"

HRESULT	PickupItem::initMesh(ID3D11Device* pd3dDevice)
{
	m_scale = XMFLOAT3(20, 20, 1);

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	setPosition(Vector2D(0, 0));

	return hr;
}