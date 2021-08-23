#include "GSLevel4.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "stdlib.h"
#include "string.h"

GSLevel4::GSLevel4() {
	char* SM = "../Resources/Managers/SM1.txt";
	char* MAP = "../Resources/Map/maplv1.txt";
	Singleton<SceneManager>::GetInstance()->SetFileManager(SM, MAP);
}

GSLevel4::~GSLevel4() {

}

void GSLevel4::Init() {
	Camera::GetInstance()->i_state = 1;
	Singleton<SceneManager>::GetInstance()->Init();

	//ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/WindyHill.mp3", true); // day, mp3 cung doc dc
}

void GSLevel4::Draw() {

	Singleton<SceneManager>::GetInstance()->Draw();

}

void GSLevel4::Update(float deltaTime) {
	std::cout << "This is Level 4!" << std::endl;

	Singleton<SceneManager>::GetInstance()->Update(deltaTime);
}

void GSLevel4::CleanUp() {
	ResourceManager::GetInstance()->CleanDump();
	Singleton<SceneManager>::GetInstance()->CleanUp();
	Singleton<SceneManager>::GetInstance()->FreeInstance();
}

void GSLevel4::Resume() {
	//Init();
	ResourceManager::GetInstance()->StopAllSound();
	//ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/bg_MainMenu_Sound.wav", true);
}

void GSLevel4::Pause() {
	ResourceManager::GetInstance()->StopAllSound();
	//ResourceManager::GetInstance()->StopSound("../Resources/Sounds/bg_MainMenu_Sound.wav");
}


//Events
void GSLevel4::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
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
		case KEY_BACK_STATE + 32:
		{
			if (GameStateMachine::GetInstance()->GetIsCanPop()) {
				GameStateMachine::GetInstance()->PopState();
			}
		}
		break;
		}
	}
}

void GSLevel4::OnMouseMoving(int X, int Y) {
}

void GSLevel4::OnMouseButtonDown(int X, int Y, char Button) {
	Singleton<SceneManager>::GetInstance()->OnMouseButtonDown(X, Y, Button);
}

void GSLevel4::OnMouseButtonUp(int X, int Y, char Button) {
	Singleton<SceneManager>::GetInstance()->OnMouseButtonUp(X, Y, Button);
}

void GSLevel4::OnMouseButtonMove(int X, int Y, char Button) {
	Singleton<SceneManager>::GetInstance()->OnMouseButtonMove(X, Y, Button);
}
