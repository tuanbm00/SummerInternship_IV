#pragma once
#include <memory>
#include <list>
#include "Singleton.h"

class GameStateBase;

enum StateTypes {
	GS_INVALID,
	GS_INTRO,
	GS_MAINMENU,
	GS_LOADLEVEL,
	GS_LEVEL1,
	GS_LEVEL2,
	GS_LEVEL3,
	GS_LEVEL4,
	GS_RESULT,
	GS_OPTION,
	GS_CREDIT
};

class GameStateMachine : public Singleton<GameStateMachine>
{
public:
	
	GameStateMachine();
	~GameStateMachine();
	int Init();
	void Render();

	void Cleanup();

	void ChangeState(std::shared_ptr<GameStateBase> state);
	void ChangeState(StateTypes stt);
	void PushState(StateTypes stt);
	void PushState(StateTypes stt, bool bIsVictory, int currentLevel);
	void PopState();

	bool isRunning() { return m_running; }
	void Quit() { m_running = false; }
	void PerformStateChange();

	inline std::shared_ptr<GameStateBase>	CurrentState()const
	{
		return m_pActiveState;
	}

	inline bool	HasState()const
	{
		return m_StatesStack.size() > 0;
	}

	inline bool	GetIsCanPop()const
	{
		return m_StatesStack.size() > 1;
	}

private:

	std::list<std::shared_ptr<GameStateBase>> m_StatesStack;
	std::shared_ptr<GameStateBase>	m_pActiveState;
	std::shared_ptr<GameStateBase>	m_pNextState;
	bool m_running;
	bool m_fullscreen;
};

