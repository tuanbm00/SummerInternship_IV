#pragma once
#include "GameButton.h"

class ButtonManager
{
private:
	int m_currentLevel = 0;
public:
	ButtonManager(char *srcButton, int currentLevel);
	~ButtonManager();



	std::vector<std::shared_ptr<GameButton>> m_listButton;

	int ReadFile(char* srcButton);
	void Update(float deltaTime);
	void CleanUp();

	void Pause();
	void Resume();

	//void HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed);
	//void OnMouseButtonDown(int X, int Y, char Button);
	void OnMouseButtonUp(int X, int Y, char Button);
	//void OnMouseButtonMove(int X, int Y, char Button);
	//void OnMouseMoving(int X, int Y);

	void Draw();

	

	void AddFunction(char* type, std::shared_ptr<GameButton> button);
	void SetCurrentLevel(int currentLevel);
};

