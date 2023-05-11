#pragma once
#include "PickupItem.h"
class SpeedPickup :
    public PickupItem
{
public:
    virtual HRESULT initMesh(ID3D11Device* pd3dDevice) override;
    virtual void pickup(Vehicle* collector) override;
private:
    /// <summary>The amount of fuel to restore to the collector.</summary>
    const float m_speedIncrease = 0.075f;
};

