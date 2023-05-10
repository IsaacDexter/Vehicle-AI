#pragma once
#include "DrawableGameObject.h"
#include "Collidable.h"

#include <Windows.h>

class Vehicle;


class PickupItem abstract:
    public DrawableGameObject, public Collidable
{
public:
    virtual HRESULT initMesh(ID3D11Device* pd3dDevice);
    virtual void pickup(Vehicle* collector) = 0;
protected:

};


