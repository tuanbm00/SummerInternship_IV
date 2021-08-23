#pragma once
#include "GameStateBase.h"
class GSLevel4 :
    public GameStateBase
{
	std::shared_ptr<SceneManager> m_SM;
public:
	GSLevel4(void);
	~GSLevel4(void);

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

