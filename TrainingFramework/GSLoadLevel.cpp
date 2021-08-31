#include "GSLoadLevel.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <iostream>
#include "TextManager.h"
#include "GameplayUI.h"

GSLoadLevel::GSLoadLevel() {

}

GSLoadLevel::~GSLoadLevel() {

}

void GSLoadLevel::Init() {
	//Manager Initialize

	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/bg_LoadLevel_Sound.wav", false);
	char* BM = "../Resources/Managers/BM_LOADLEVEL.txt";
	m_BM = std::make_shared<ButtonManager>(BM, 0);

	Models * model = ResourceManager::GetInstance()->GetModelAtID(0);

	//Background Initialize
	m_Background = std::make_shared<Sprite2D>(0);
	m_Background->setModel(model);
	m_Background->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	m_Background->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(10));
	m_Background->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	m_Background->SetSize(Globals::screenWidth, Globals::screenHeight);

	//Frame Initialize
	m_Frame = std::make_shared<Sprite2D>(0);
	m_Frame->setModel(model);
	m_Frame->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	m_Frame->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(26));
	m_Frame->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	m_Frame->SetSize(Globals::screenWidth/2, Globals::screenHeight/2);

	//Set Current State
	m_currentState = GSLOADLEVEL;
}

void GSLoadLevel::Draw() {
	m_Background->Draw();
	m_Frame->Draw();
	m_BM->Draw();
}

void GSLoadLevel::Update(float deltaTime) {
	m_Background->Update(deltaTime);
	m_Frame->Update(deltaTime);
	m_BM->Update(deltaTime);
}

void GSLoadLevel::CleanUp() {
	m_Frame->CleanUp();
	m_BM->CleanUp();

}

void GSLoadLevel::Resume() {
	ResourceManager::GetInstance()->StopAllSound();
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/bg_LoadLevel_Sound.wav", true);
}

void GSLoadLevel::Pause() {
	ResourceManager::GetInstance()->StopAllSound();
}


//Events
void GSLoadLevel::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
	//m_SM->Key(key, bIsPressed);
	if (!bIsPressed) {
		switch (key) {

		}
	}
}

void GSLoadLevel::OnMouseMoving(int X, int Y) {
}

void GSLoadLevel::OnMouseButtonDown(int X, int Y, char Button) {
	//m_SM->OnMouseButtonDown(X, Y, Button);
}

void GSLoadLevel::OnMouseButtonUp(int X, int Y, char Button) {
	m_BM->OnMouseButtonUp(X, Y, Button);
}

void GSLoadLevel::OnMouseButtonMove(int X, int Y, char Button) {
	//m_SM->OnMouseButtonMove(X, Y, Button);
}
