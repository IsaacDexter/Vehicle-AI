#pragma once
#include "GetPickupState.h"
class GetSpeedPickupState :
    public GetPickupState
{
public:
    GetSpeedPickupState() { m_type = PickupType::SPEED; };
};