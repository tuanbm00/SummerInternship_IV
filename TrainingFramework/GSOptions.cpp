#include "stdafx.h"
#include "GSOptions.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "TextManager.h"
#include "GameplayUI.h"

GSOptions::GSOptions() {

}

GSOptions::~GSOptions() {

}

void GSOptions::Init() {
	//Manager Initialize
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/TKKHT2.mp3", true);
	
	char* BM = "../Resources/Managers/BM_Options.txt";
	m_BM = std::make_shared<ButtonManager>(BM, 0);

	//Set Texture for Audio Button
	for (register int i = 0; i < size_as_int(m_BM->m_listButton); i++) {
		if (m_BM->m_listButton[i]->GetID() == 0) {
			if (ResourceManager::GetInstance()->GetGlobalVolume() > 0) {
				m_BM->m_listButton[i]->SetNewTexture(ResourceManager::GetInstance()->GetTextureAtID(40));
			}
			else m_BM->m_listButton[i]->SetNewTexture(ResourceManager::GetInstance()->GetTextureAtID(39));
		}
	}

	//Background Initialize
	Models * model = ResourceManager::GetInstance()->GetModelAtID(0);

	auto obj = std::make_shared<Sprite2D>(999);
	obj->setModel(model);
	obj->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	obj->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(36));
	obj->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	obj->SetSize(Globals::screenWidth, Globals::screenHeight);
	obj->CalculateWVP();
	m_listSprite.push_back(obj);

	obj = std::make_shared<Sprite2D>(999);
	obj->setModel(model);
	obj->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	if (GameStateMachine::GetInstance()->CurrentState()->GetCurrentState() == GSLEVEL4) {
		obj->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(41)); // MAP LEVEL4
	}
	else obj->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(37));
	obj->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2 - 100);
	obj->SetSize(600, 500);
	obj->CalculateWVP();
	m_listSprite.push_back(obj);

	//Set Current State
	m_currentState = GSOPTIONS;
}

void GSOptions::Draw() {
	for (register int i = 0; i < size_as_int(m_listSprite); i++) {
		m_listSprite[i]->Draw();
	}
	m_BM->Draw();
}

void GSOptions::Update(float deltaTime) {
	for (register int i = 0; i < size_as_int(m_listSprite); i++) {
		m_listSprite[i]->Update(deltaTime);
	}
	for (register int i = 0; i < size_as_int(m_BM->m_listButton); i++) {
		if ((m_BM->m_listButton[i]->GetID() == 0) && m_BM->m_listButton[i]->IsHandle()) {
			static bool isA = true;
			if (isA) {
				m_BM->m_listButton[i]->SetNewTexture(ResourceManager::GetInstance()->GetTextureAtID(39));
				isA = false;
			}
			else {
				m_BM->m_listButton[i]->SetNewTexture(ResourceManager::GetInstance()->GetTextureAtID(40));
				isA = true;
			}
			m_BM->m_listButton[i]->SetIsHandle(false);
		}
	}
}

void GSOptions::CleanUp() {
	for (register int i = 0; i < size_as_int(m_listSprite); i++) {
		m_listSprite[i]->CleanUp();
	}
	m_BM->CleanUp();
}

void GSOptions::Resume() {
	ResourceManager::GetInstance()->StopAllSound();
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/TKKHT2.mp3", true);
}

void GSOptions::Pause() {
	ResourceManager::GetInstance()->StopAllSound();
}


//Events
void GSOptions::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
	//m_SM->Key(key, bIsPressed);
	
}

void GSOptions::OnMouseMoving(int X, int Y) {
}

void GSOptions::OnMouseButtonDown(int X, int Y, char Button) {
	//m_SM->OnMouseButtonDown(X, Y, Button);
}

void GSOptions::OnMouseButtonUp(int X, int Y, char Button) {
	m_BM->OnMouseButtonUp(X, Y, Button);
}

void GSOptions::OnMouseButtonMove(int X, int Y, char Button) {
	//m_SM->OnMouseButtonMove(X, Y, Button);
}
