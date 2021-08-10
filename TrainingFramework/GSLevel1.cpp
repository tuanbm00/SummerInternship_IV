#include "GSLevel1.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Camera.h"

GSLevel1::GSLevel1() {
	char* SM = "../Resources/Managers/SM1.txt";
	m_SM = std::make_shared<SceneManager>(SM);
}

GSLevel1::~GSLevel1() {
	
}

void GSLevel1::Init() {
	printf("State init\n");
	m_SM->Init();
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/fatima.mp3", true); // day, mp3 cung doc dc
}

void GSLevel1::Draw() {

	Camera::GetInstance()->i_state = 1;
	m_SM->Draw();
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
		case 'K':
		case 'k':
		{
			if (GameStateMachine::GetInstance()->HasInstance()) {
				std::cout << "PushStates\n";
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL2);
			}
		}
		break;
		case 'J':
		case 'j':
		{
			if (GameStateMachine::GetInstance()->GetIsCanPop()) {
				std::cout << "PopStates\n";
				GameStateMachine::GetInstance()->PopState();
			}else std::cout << "Cannot PopStates\n";
		}
		break;
		}
	}
}

void GSLevel1::OnMouseMoving(int X, int Y) {
	printf("GSLevel1 - Mouse At: %d - %d!\n", X, Y);
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
