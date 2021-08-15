#include "GSLevel1.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "TextManager.h"
#include "stdlib.h"
#include "string.h"

GSLevel1::GSLevel1() {
	char* SM = "../Resources/Managers/SM1.txt";
	char* MAP = "../Resources/Map/tilemap.txt";
	m_SM = std::make_shared<SceneManager>(SM, MAP);
}

GSLevel1::~GSLevel1() {
	
}

void GSLevel1::Init() {
	m_SM->Init();

	Singleton<TextManager>::GetInstance()->Initialize();
	glEnable(GL_DEPTH_TEST);

	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/SkyCladNoKansokusha.mp3", true); // day, mp3 cung doc dc
}

void GSLevel1::Draw() {
	DWORD start, end;
	start = GetTickCount();

	Camera::GetInstance()->i_state = 1;
	m_SM->Draw();

	end = GetTickCount();
	DWORD frameTime = end - start;
	int fps = frameTime;
	char buffer[5];
	itoa(fps, buffer, 10);
	char s[9] = "FPS: ";
	strcat(s, buffer);

	Singleton<TextManager>::GetInstance()->RenderString(s, Vector4(0.5f, 0.8f, 0.2f), 1.0f, 700.0f, 1.0f, 1.0f);
}

void GSLevel1::Update(float deltaTime) {
	m_SM->Update(deltaTime);
}

void GSLevel1::CleanUp() {
	m_SM->CleanUp();
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
	m_SM->Key(key, bIsPressed);
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
	m_SM->OnMouseButtonDown(X, Y, Button);
}

void GSLevel1::OnMouseButtonUp(int X, int Y, char Button) {
	m_SM->OnMouseButtonUp(X, Y, Button);
}

void GSLevel1::OnMouseButtonMove(int X, int Y, char Button) {
	m_SM->OnMouseButtonMove(X, Y, Button);
}
