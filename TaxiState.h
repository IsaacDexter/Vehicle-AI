#pragma once
#include "HierarchicalState.h"
#include <array>

enum class Substates
{
    FUEL,
    SPEED,
    PASSENGER,
    DELIVER,
    PARK 
};

class TaxiState :
    public HierarchicalState
{
public:
    /// <summary><para>The main hierarchical state that decides which states to enter and exit. It has priority over all the other taxis states.</para>
    /// <para>The goal of this state is to maximise the money made, from delivering passengers to their destination.</para>
    /// <para>Collecting passengers is kind of the number one priority, but in the event of low fuel, fuel will be sought out.</para>
    /// <para>If there are speed boosts near enough nearby and no desperate rush it'll go after them.</para>
    /// <para>The two taxis are competing so states have functions to update according to the world around them.</para></summary>
    TaxiState();

    /*
        each frame:
            -   calculate priorities of various actions
            -   take the appropriate actions
            -   the priority of collecting fuel is inversely proportional to how much fuel there is, and how far the nearest fuel boost is
            -   the priority of collecting speed boosts is proportional to how nearby they are
            -   the priority of picking up passengers is inversely proportional to how many passengers are held and how nearby the nearest passenger is
            -   the priority of delivering passengers is proportional to how nearby the destinations are, and how full the car is.
            -   if nothing can be done, i.e. there is no fuel, speed boosts, passengers to collect or deliver, the car will idle
    */

    virtual void Enter(Vehicle* agent) override;
    virtual void Exit() override;
    virtual void Update(Vehicle* agent, float deltaTime) override;
    virtual State* Check(Vehicle* agent) override;

protected:
    virtual State* GetBestState(Vehicle* agent);
    virtual float Divide(float numerator, float denominator)
    {
        if (denominator == 0 || numerator == 0)
        {
            return 0;
        }
        return numerator / denominator;
    }
};

