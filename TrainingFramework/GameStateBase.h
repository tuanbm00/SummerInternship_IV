#pragma once
#include <iostream>
#include "GameStateMachine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "ButtonManager.h"

class GameStateBase
{
public:
	GameStateBase(void) {}
	virtual ~GameStateBase(void) {}

	virtual void Init() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void CleanUp() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) = 0;
	virtual void OnMouseButtonDown(int X, int Y, char Button) = 0;
	virtual void OnMouseButtonUp(int X, int Y, char Button) = 0;
	virtual void OnMouseButtonMove(int X, int Y, char Button) = 0;
	virtual void OnMouseMoving(int X, int Y) = 0;

	virtual void Draw() = 0;


	static std::shared_ptr<GameStateBase> CreateState(StateTypes stt);
};

