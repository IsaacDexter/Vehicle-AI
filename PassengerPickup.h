#pragma once
#include "PickupItem.h"

class Passenger;

class PassengerPickup :
    public PickupItem
{
public:
    virtual HRESULT initMesh(ID3D11Device* pd3dDevice) override;
    virtual void pickup(Vehicle* collector) override;
};

