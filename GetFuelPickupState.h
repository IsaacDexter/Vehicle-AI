#pragma once
#include "GetPickupState.h"

class GetFuelPickupState :
    public GetPickupState
{
public:
    /// <summary>Finds the nearest fuel pickup and heads to it. In the event of the fuel being picked up, by either car, it will exit this state, back to the decision making layer.</summary>
    GetFuelPickupState() : GetPickupState() { m_type = PickupType::FUEL; };
};