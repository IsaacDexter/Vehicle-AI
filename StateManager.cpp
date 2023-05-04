#include "StateManager.h"
#include "State.h"

StateManager::StateManager(Vehicle* agent)
{
	m_agent = agent;
	m_state = nullptr;
}

void StateManager::Update(float dt)
{
	//Assuming we have a state set
	if (m_state != nullptr)
	{
		//Update it
		m_state->Update(m_agent, dt);
		//Check if its trying to transition. if it is, call it's own cleanup handling.
		SetState(m_state->Check(m_agent));
	}
}

void StateManager::SetState(State* state)
{
	if (state != m_state)
	{
		//allow the user to come out of no state
		if (m_state != nullptr)
		{
			//Exit current state to allow it to clean itself up
			m_state->Exit();
		}
		//remove up memory bound to state itself.
		delete m_state;
		m_state = nullptr;
		//allow the user to set the state to nullptr
		if (state != nullptr)
		{
			//Enter and store the new state
			m_state = state;
			state->Enter(m_agent);
		}
	}
}
