#pragma once
#include "HierarchicalState.h"
#include <map>
#include <functional>

typedef std::function<void()> Transition;
typedef std::function<float()> Priority;
typedef std::map<Transition*, Priority*> OptionMap;
typedef std::pair<Transition*, Priority*> Option;

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
    /// <summary>Made public to allow lambdas to instanciate new tasks</summary>
    virtual FSM* GetStateManager() const { return m_pStateManager; };

#pragma region PriorityCalculations

    /// <returns>The priority to switch into park state. To be called by function pointers. Can't decide if this belongs in here or in the state. Doing it in the state would require an instanciated one. Which feels counter intuitive.</returns>
    virtual float CalculateParkStatePriority();
    /// <returns>The priority to switch into the deliver state, to be called by function pointers. proportional to how nearby the destinations are, and how full the car is.</returns>
    virtual float CalculateDeliverStatePriority();
    /// <returns>The priority to switch into the pickup fuel state, to be called by function pointers. inversely proprotional to the amount of fuel and distance to the nearest fuel pickup.</returns>
    virtual float CalculateGetFuelPickupStatePriority();
    /// <returns>The priority to switch into the pickup passenger state, to be called by function pointers. inversely proportional to how many passengers are held and how nearby the nearest passenger is</returns>
    virtual float CalculateGetPassengerPickupStatePriority();
    /// <returns>The priority to switch into the pickup speed state, to be called by function pointers. proportional to how nearby they are</returns>
    virtual float CalculateGetSpeedPickupStatePriority();

#pragma endregion

protected:
    virtual void CalculatePriorities();
    virtual Transition* GetBestTransition();

    template<typename T>
    Transition* MakeTransition();
    
    /// <summary>The priority queue of functions which can be instanciated in memory with the new keyword. each of which will call set another state.</summary>
    OptionMap m_options;
    /// <summary>The previous transition made. To stop making the same transition twice.</summary>
    Transition* m_lastTransition = nullptr;


};

/// <summary>Instanciates a state of type T and sets it when this task </summary>
/// <typeparam name="T">The type of state for this task to instanciate. It must be a child of state or this will chuck up errors.</typeparam>
/// <returns>The function lambda thing used in the tasking.h view there for more indepth info. But calling this function will queue up a task of x type.</returns>
template<typename T>
inline Transition* TaxiState::MakeTransition()
{
    Transition  function = [this, T] { this->GetStateManager()->SetState(new T()); };	//Head towards that position
    //Return a handle to the task for early completion etc.
    return new Transition(function);
}
