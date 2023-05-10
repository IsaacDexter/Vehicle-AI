#pragma once
#include "PickupItem.h"

class PassengerPickup :
    public PickupItem
{
public:
    virtual HRESULT initMesh(ID3D11Device* pd3dDevice) override;
    virtual void pickup(Vehicle* collector) override;

    void dropoff();

protected:
    Vehicle* m_driver;
    /// <summary>
    /// How much to increase t</summary>
    const float m_fuelConsumptionIncrease = 0.125f;
    float m_tip;
};

