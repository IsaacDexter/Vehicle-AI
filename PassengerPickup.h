#pragma once
#include "PickupItem.h"

class Passenger;

class PassengerPickup :
    public PickupItem
{
public:
    virtual HRESULT initMesh(ID3D11Device* pd3dDevice) override;
    virtual void pickup(Vehicle* collector) override;
protected:
    /// <summary>Storesd the d3dDevice to pass to the passenger to use to draw its destinations.</summary>
    ID3D11Device* m_pd3dDevice;
};

