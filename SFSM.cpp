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
	//If we're trying to exit a state by entering no state
	if (state == nullptr)
	{
		//Pop from the stack if we can
		ExitState();
	}
	//Otherwise, if we're trying to enter a state
	else
	{
		//Push it to the stack if it isn't a state we're in already
		EnterState(state);
	}
}

void SFSM::ExitState()
{
	//If there is anything on the top of the stack
	if (!m_states.empty())
	{
		//exit the top state
		m_states.top()->Exit();
		//delete it then pop it.
		delete m_states.top();
		m_states.pop();
	}
}

void SFSM::EnterState(State* state)
{
	//If we are currently in a state
	if (!m_states.empty())
	{
		//And its this state
		if (state == m_states.top())
		{
			//Do nothing
			return;
		}
	}

	//Otherwise, as the state is not nullptr, push it to the top of the stack
	m_states.push(state);
	//And enter it
	state->Enter(m_agent);
}
