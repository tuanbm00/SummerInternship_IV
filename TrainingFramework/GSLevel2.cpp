#include "GSLevel2.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"

GSLevel2::GSLevel2() {
	char* SM = "../Resources/Managers/SM2.txt";
	char* MAP = "../Resources/Map/maplv2.txt";
	Singleton<SceneManager>::GetInstance()->SetFileManager(SM, MAP);
}

GSLevel2::~GSLevel2() {

}

void GSLevel2::Init() {
	Camera::GetInstance()->i_state = 2;
	Singleton<SceneManager>::GetInstance()->Init();
	//ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/WindyHill.mp3", true);


}

void GSLevel2::Draw() {
	Singleton<SceneManager>::GetInstance()->Draw();
}

void GSLevel2::Update(float deltaTime) {
	Singleton<SceneManager>::GetInstance()->Update(deltaTime);
}

void GSLevel2::CleanUp() {
	ResourceManager::GetInstance()->CleanDump();
	Singleton<SceneManager>::GetInstance()->CleanUp();
	Singleton<SceneManager>::GetInstance()->FreeInstance();
}

void GSLevel2::Resume() {
	ResourceManager::GetInstance()->StopAllSound();
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/bg_Play_Sound.wav", true);
}

void GSLevel2::Pause() {
	ResourceManager::GetInstance()->StopAllSound();
}


//Events
void GSLevel2::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
	Singleton<SceneManager>::GetInstance()->Key(key, bIsPressed);
	if (!bIsPressed) {
		switch (key) {
		case KEY_NEW_STATE:
		case KEY_NEW_STATE + 32:
		{
			if (GameStateMachine::GetInstance()->HasInstance()) {
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL1);
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

void GSLevel2::OnMouseMoving(int X, int Y) {
}

void GSLevel2::OnMouseButtonDown(int X, int Y, char Button) {
	Singleton<SceneManager>::GetInstance()->OnMouseButtonDown(X, Y, Button);
}

void GSLevel2::OnMouseButtonUp(int X, int Y, char Button) {
	Singleton<SceneManager>::GetInstance()->OnMouseButtonUp(X, Y, Button);
}

void GSLevel2::OnMouseButtonMove(int X, int Y, char Button) {
	Singleton<SceneManager>::GetInstance()->OnMouseButtonMove(X, Y, Button);
}
