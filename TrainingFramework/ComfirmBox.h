#pragma once
#include "GameStateBase.h"

class ComfirmBox : public Singleton<ComfirmBox>
{
	std::vector<std::shared_ptr<GameButton>> m_listButton;
	std::shared_ptr<Sprite2D> m_Frame;
	bool m_bIsActive;

public:
	ComfirmBox();
	~ComfirmBox();

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

	inline void SetActive(bool isActive) { m_bIsActive = isActive; }
	inline bool GetActive() { return m_bIsActive; }
	inline std::shared_ptr<GameButton> GetButtonYes() { return m_listButton[0]; }
	inline std::shared_ptr<GameButton> GetButtonNo() { return m_listButton[1]; }
};
