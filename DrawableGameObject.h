#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include <iostream>
#include "structures.h"
#include "Vector2D.h"
#include <map>

using namespace DirectX;

class DrawableGameObject
{
public:
	DrawableGameObject();
	virtual ~DrawableGameObject();

	void								release();
	virtual HRESULT						initMesh(ID3D11Device* pd3dDevice);
	virtual void						update(const float t);
	void								draw(ID3D11DeviceContext* pContext);
	ID3D11ShaderResourceView**			getTextureResourceView() { return &m_pTextureResourceView; 	}
	XMFLOAT4X4*							getTransform() { return &m_world; }
	ID3D11SamplerState**				getTextureSamplerState() { return &m_pSamplerLinear; }

	void								setXMPosition(DirectX::XMFLOAT3 position);
	DirectX::XMFLOAT3*					getXMPosition() { return &m_position; }
	DirectX::XMFLOAT3*					getDirection() { return &m_direction; }

	// AI framework friendly functions
	void								setPosition(Vector2D position);
	Vector2D							getPosition();

protected:

	void								setDirection(DirectX::XMFLOAT3 direction);
	void								setTextureName(wstring texName) { m_textureName = texName; }
	bool								loadTexture(ID3D11Device* pd3dDevice, wstring texName);
	bool								switchTexture(wstring textureName);

	// helper functions
	DirectX::XMFLOAT3							addFloat3(DirectX::XMFLOAT3& f1, DirectX::XMFLOAT3& f2);
	DirectX::XMFLOAT3							subtractFloat3(DirectX::XMFLOAT3& f1, DirectX::XMFLOAT3& f2);
	DirectX::XMFLOAT3							normaliseFloat3(DirectX::XMFLOAT3& f1);
	float								magnitudeFloat3(DirectX::XMFLOAT3& f1);
	DirectX::XMFLOAT3							multiplyFloat3(DirectX::XMFLOAT3& f1, const float scalar);
	DirectX::XMFLOAT3							divideFloat3(DirectX::XMFLOAT3& f1, const float scalar);

protected: // protected variables
	XMFLOAT3							m_scale;
	float								m_radianRotation;
	/// <summary>A map pairing paths with textures, useful as basic identifiers for textures and to prevent duplicate loading.</summary>
	map<std::wstring, ID3D11ShaderResourceView*>	m_pTextureResourceViews;

private: // private variables
	
	XMFLOAT4X4							m_world;
	ID3D11Buffer*						m_pVertexBuffer;
	ID3D11Buffer*						m_pIndexBuffer;
	ID3D11ShaderResourceView*			m_pTextureResourceView;
	ID3D11SamplerState *				m_pSamplerLinear;
	DirectX::XMFLOAT3							m_position;

	DirectX::XMFLOAT3							m_direction;
	wstring								m_textureName;
	
	
};

