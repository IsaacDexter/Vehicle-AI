#pragma once
#include "GetPickupState.h"
class GetSpeedPickupState :
    public GetPickupState
{
public:
    GetSpeedPickupState() : GetPickupState() { m_type = PickupType::SPEED; };
};