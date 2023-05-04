#include "HierarchicalState.h"
#include "StateManager.h"

HierarchicalState::HierarchicalState()
{
	m_pStateManager = nullptr;
}

void HierarchicalState::Enter(Vehicle* agent)
{
	m_pStateManager = new StateManager(agent);
}

void HierarchicalState::Exit()
{
	m_pStateManager->SetState(nullptr);	//Exit the current state to clear it's memory
	delete m_pStateManager;	//Delete the state manager to clear its memory
	m_pStateManager = nullptr;
}

void HierarchicalState::Update(Vehicle* agent, float deltaTime)
{
	m_pStateManager->Update(deltaTime);
}
