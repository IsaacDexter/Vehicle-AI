#pragma once
#include "GetPickupState.h"
#include "Tasking.h"

class GetPassengerPickupState :
    public GetPickupState
{
public:
    /// <summary>Finds the nearest passenger pickup and heads to it. In the event of the passenger being picked up, by either car, it will exit this state, back to the decision making layer.</summary>
    GetPassengerPickupState() : GetPickupState() {  m_type = PickupType::PASSENGER; };
};

