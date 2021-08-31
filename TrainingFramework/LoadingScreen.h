#pragma once
#include "GameStateBase.h"
class LoadingScreen :
	public GameStateBase
{
	std::vector<std::shared_ptr<Sprite2D>> m_listSprite;
	bool m_isLoading;
	float m_currentTime;
public:
	LoadingScreen(void);
	~LoadingScreen(void);

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

	inline void SetIsLoading(bool isLoading) { m_isLoading = isLoading; }
	inline bool GetIsLoading() { return m_isLoading; }
};

