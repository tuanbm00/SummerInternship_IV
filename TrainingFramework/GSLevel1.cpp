#include "GSLevel1.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "stdlib.h"
#include "string.h"

GSLevel1::GSLevel1() {
	
}

GSLevel1::~GSLevel1() {
}

void GSLevel1::Init() {
	
	//Init SM
	m_SM = new SceneManager(1);
	char* SM = "../Resources/Managers/SM1.txt";
	char* MAP = "../Resources/Map/maplv1.txt";
	m_SM->SetFileManager(SM, MAP);
	m_SM->Init();

	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/FutariNoKimochi.mp3", true); // day, mp3 cung doc dc

	//Set Current State
	m_currentState = GSLEVEL1;
}

void GSLevel1::Draw() {
	
	m_SM->Draw();

}

void GSLevel1::Update(float deltaTime) {
	m_SM->Update(deltaTime);
}

void GSLevel1::CleanUp() {
	m_SM->CleanUp();
	delete m_SM;

}

void GSLevel1::Resume() {
	//Init();
	ResourceManager::GetInstance()->StopAllSound();
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/FutariNoKimochi.mp3", true);
}

void GSLevel1::Pause() {
	ResourceManager::GetInstance()->StopAllSound();
}


//Events
void GSLevel1::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
	m_SM->Key(key, bIsPressed);
	if (!bIsPressed) {
		switch (key) {
		case KEY_NEW_STATE:
		case KEY_NEW_STATE + 32:
		{
		}
		break;
		case KEY_BACK_STATE:
		case KEY_BACK_STATE+32:
		{
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
