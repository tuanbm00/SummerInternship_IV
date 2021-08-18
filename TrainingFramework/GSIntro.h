#pragma once
#include "GameStateBase.h"
class GSIntro :
	public GameStateBase
{
	std::vector<std::shared_ptr<Sprite2D>> m_listSprite2D;
	float m_currentTime;
public:
	GSIntro(void);
	~GSIntro(void);

	void Init();
	void Update(float deltaTime);
	void CleanUp();

	void Pause();
	void Resume();

	void HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed);
	void OnMouseButtonDown(int X, int Y, char Button);
	void OnMouseButtonUp(int X, int Y, char Button);
	void OnMouseButtonMove(int X, int Y, char Button);
	void OnMouseMoving(int X, int Y);

	void Draw();
};

