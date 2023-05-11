#pragma once
#include "DrawableGameObject.h"
#include "Collidable.h"

#include <Windows.h>

class Vehicle;

enum PickupType
{
    NONE = 0,
    PASSENGER,
    FUEL,
    SPEED
};

class PickupItem abstract:
    public DrawableGameObject, public Collidable
{
public:
    virtual HRESULT initMesh(ID3D11Device* pd3dDevice);
    virtual void pickup(Vehicle* collector) = 0;
    const PickupType& GetType() { return m_type; };
protected:
    PickupType m_type;
};


