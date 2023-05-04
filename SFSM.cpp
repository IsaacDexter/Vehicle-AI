#include "SFSM.h"
#include "SFSM.h"
#include "State.h"

SFSM::SFSM(Vehicle* agent) : SM()
{
	m_agent = agent;
}

void SFSM::Update(float dt)
{
	//Assuming we have a state set
	if (!m_states.empty())
	{
		//Update it
		m_states.top()->Update(m_agent, dt);
		//Check if its trying to transition. if it is, call it's own cleanup handling.
		SetState(m_states.top()->Check(m_agent));
	}
}

void SFSM::SetState(State* state)
{
	//exiting:
	//If there are states in the stack
	if (!m_states.empty())
	{
		//And the state trying to be set isnt the one at the top of the state
		if (state != m_states.top())
		{
			//Exit current state to allow it to clean itself up
			m_states.top()->Exit();
		}
		//remove up memory bound to top state itself.
		delete m_states.top();
		//pop the top
		m_states.pop();
	}

	//entering:
	//if the user is trying to set no state, leave the stack empty
	if (state != nullptr)
	{
		//otherwise if the user is trying to set a state, push it to the stack 
		//Enter and push the new state
		m_states.push(state);
		state->Enter(m_agent);
	}
}
