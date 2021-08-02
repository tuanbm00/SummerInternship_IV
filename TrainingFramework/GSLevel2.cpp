#include "GSLevel2.h"
#include "SceneManager.h"
#include "ResourceManager.h"

GSLevel2::GSLevel2() {
	char* SM = "../Resources/Managers/SM2.txt";
	m_SM = m_SM = std::make_shared<SceneManager>(SM);
}

GSLevel2::~GSLevel2() {
}

void GSLevel2::Init() {
	m_SM->SetFileManager("../Resources/Managers/SM2.txt");
	m_SM->Init();
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/bg_Play_Sound.wav", true);
}

void GSLevel2::Draw() {
	m_SM->Draw();
}

void GSLevel2::Update(float deltaTime) {
	m_SM->Update(deltaTime);
}

void GSLevel2::CleanUp() {
	m_SM->CleanUp();
	
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
	m_SM->Key(key, bIsPressed);
	if (!bIsPressed) {
		switch (key) {
		case 'K':
		case 'k':
		{
			if (GameStateMachine::GetInstance()->HasInstance()) {
				std::cout << "PushStates\n";
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL1);
			}
		}
		break;
		case 'J':
		case 'j':
		{
			if (GameStateMachine::GetInstance()->GetIsCanPop()) {
				std::cout << "PopStates\n";
				GameStateMachine::GetInstance()->PopState();
			}
			else std::cout << "Cannot PopStates\n";
		}
		break;
		}
	}
}

void GSLevel2::OnMouseMoving(int X, int Y) {
	printf("GSLevel2 - Mouse At: %d - %d!\n", X, Y);
}

void GSLevel2::OnMouseButtonDown(int X, int Y, char Button) {
	m_SM->OnMouseButtonDown(X, Y, Button);
}

void GSLevel2::OnMouseButtonUp(int X, int Y, char Button) {
	m_SM->OnMouseButtonUp(X, Y, Button);
}

void GSLevel2::OnMouseButtonMove(int X, int Y, char Button) {
	m_SM->OnMouseButtonMove(X, Y, Button);
}
