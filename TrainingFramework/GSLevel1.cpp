#include "GSLevel1.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "stdlib.h"
#include "string.h"

GSLevel1::GSLevel1() {
	char* SM = "../Resources/Managers/SM1.txt";
	char* MAP = "../Resources/Map/maplv1.txt";
	Singleton<SceneManager>::GetInstance()->SetFileManager(SM, MAP);
}

GSLevel1::~GSLevel1() {
	
}

void GSLevel1::Init() {
	Singleton<SceneManager>::GetInstance()->Init();
	glEnable(GL_DEPTH_TEST);

	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/SkyCladNoKansokusha.mp3", true); // day, mp3 cung doc dc
}

void GSLevel1::Draw() {
	
	Camera::GetInstance()->i_state = 1;
	Singleton<SceneManager>::GetInstance()->Draw();

}

void GSLevel1::Update(float deltaTime) {
	Singleton<SceneManager>::GetInstance()->Update(deltaTime);
}

void GSLevel1::CleanUp() {
	Singleton<SceneManager>::GetInstance()->CleanUp();
	Singleton<SceneManager>::GetInstance()->FreeInstance();
}

void GSLevel1::Resume() {
	//Init();
	ResourceManager::GetInstance()->StopAllSound();
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/bg_MainMenu_Sound.wav", true);
}

void GSLevel1::Pause() {
	ResourceManager::GetInstance()->StopAllSound();
	//ResourceManager::GetInstance()->StopSound("../Resources/Sounds/bg_MainMenu_Sound.wav");
}


//Events
void GSLevel1::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
	Singleton<SceneManager>::GetInstance()->Key(key, bIsPressed);
	if (!bIsPressed) {
		switch (key) {
		case KEY_NEW_STATE:
		case KEY_NEW_STATE + 32:
		{
			if (GameStateMachine::GetInstance()->HasInstance()) {
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL2);
			}
		}
		break;
		case KEY_BACK_STATE:
		case KEY_BACK_STATE+32:
		{
			if (GameStateMachine::GetInstance()->GetIsCanPop()) {
				GameStateMachine::GetInstance()->PopState();
			}
		}
		break;
		}
	}
}

void GSLevel1::OnMouseMoving(int X, int Y) {
}

void GSLevel1::OnMouseButtonDown(int X, int Y, char Button) {
	Singleton<SceneManager>::GetInstance()->OnMouseButtonDown(X, Y, Button);
}

void GSLevel1::OnMouseButtonUp(int X, int Y, char Button) {
	Singleton<SceneManager>::GetInstance()->OnMouseButtonUp(X, Y, Button);
}

void GSLevel1::OnMouseButtonMove(int X, int Y, char Button) {
	Singleton<SceneManager>::GetInstance()->OnMouseButtonMove(X, Y, Button);
}
