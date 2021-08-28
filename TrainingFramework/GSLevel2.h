#pragma once
#include "GameStateBase.h"
#include "Bullet.h"

class GSLevel2 :
	public GameStateBase
{
	SceneManager * m_SM;
	std::vector<std::shared_ptr<Bullet>> m_ListBullet;

public:
	GSLevel2(void);
	~GSLevel2(void);

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

