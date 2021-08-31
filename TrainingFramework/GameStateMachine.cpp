
#include "GameStateMachine.h"
#include "GameStateBase.h"

GameStateMachine::GameStateMachine()
{
	m_running = true;
	m_pActiveState = 0;
	m_pNextState = 0;
	m_StatesStack.clear();
}


GameStateMachine::~GameStateMachine()
{
}


void GameStateMachine::Cleanup()
{
	// cleanup the all states
	while (!m_StatesStack.empty()) {
		m_StatesStack.back()->CleanUp();
		m_StatesStack.pop_back();
	}
}

void GameStateMachine::ChangeState(StateTypes stt)
{
	std::shared_ptr<GameStateBase> gstb = GameStateBase::CreateState(stt);

	// cleanup the current state
	if (!m_StatesStack.empty()) {
		m_StatesStack.back()->CleanUp();
		m_StatesStack.pop_back();
	}

	m_pNextState = gstb;
}

void GameStateMachine::ChangeState(std::shared_ptr<GameStateBase> state)
{
	// cleanup the current state
	if (!m_StatesStack.empty()) {
		m_StatesStack.back()->CleanUp();
		m_StatesStack.pop_back();
	}

	m_pNextState = state;
}


void GameStateMachine::PushState(StateTypes stt)
{
	std::shared_ptr<GameStateBase> state = GameStateBase::CreateState(stt);
	// pause current state
	if (!m_StatesStack.empty()) {
		m_StatesStack.back()->Pause();
	}
	// store and init the new state
	m_pNextState = state;
}

void GameStateMachine::PushState(StateTypes stt, bool bIsVictory, int currentLevel)
{
	std::shared_ptr<GameStateBase> state = GameStateBase::CreateState(stt, bIsVictory, currentLevel);
	// pause current state
	if (!m_StatesStack.empty()) {
		m_StatesStack.back()->Pause();
		
			m_pActiveState->CleanUp();
			m_StatesStack.pop_back();
		
	}

	// store and init the new state
	m_pNextState = state;
	PerformStateChange();
}

void GameStateMachine::PopState()
{
	// cleanup the current state
	if (!m_StatesStack.empty()) {
		m_StatesStack.back()->Pause();
		m_StatesStack.back()->CleanUp();
		m_StatesStack.pop_back();	
	}

	// resume previous state
	if (!m_StatesStack.empty()) {
		m_pActiveState = m_StatesStack.back();
		m_StatesStack.back()->Resume();
	}
}

void  GameStateMachine::PerformStateChange()
{
	if (m_pNextState != 0)
	{
		// store and init the new state
		m_StatesStack.push_back(m_pNextState);
		m_StatesStack.back()->Init();
		m_pActiveState = m_pNextState;
		m_pNextState = 0;
	}
}