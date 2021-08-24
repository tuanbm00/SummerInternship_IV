#include "GSResult.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <iostream>
#include "TextManager.h"
#include "GameplayUI.h"

GSResult::GSResult(bool isVictory, StateTypes stt) {
	m_bIsVictory = isVictory;
}

GSResult::~GSResult() {

}

void GSResult::Init() {
	//Manager Initialize
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/FutariNoKimochi.mp3", true);
	char* BM = "../Resources/Managers/BM_Result.txt";
	m_BM = std::make_shared<ButtonManager>(BM);

	//Background Initialize
	m_Background = std::make_shared<Sprite2D>(0);
	auto model = new Models(1, "../Resources/Models/Sprite2D.nfg");
	m_Background->setModel(model);
	m_Background->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	m_Background->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(10));
	m_Background->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	m_Background->SetSize(Globals::screenWidth, Globals::screenHeight);

	m_Background->CalculateWVP();
}

void GSResult::Draw() {
	m_Background->Draw();
	m_BM->Draw();
}

void GSResult::Update(float deltaTime) {

}

void GSResult::CleanUp() {
	m_Background->CleanUp();
	m_BM->CleanUp();

}

void GSResult::Resume() {
	ResourceManager::GetInstance()->StopAllSound();
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/bg_Play_Sound.wav", true);
}

void GSResult::Pause() {
	ResourceManager::GetInstance()->StopAllSound();
}


//Events
void GSResult::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
	//m_SM->Key(key, bIsPressed);
	if (!bIsPressed) {
		switch (key) {
		case KEY_NEW_STATE:
		case KEY_NEW_STATE + 32:
		{

		}
		break;
		case KEY_BACK_STATE:
		case KEY_BACK_STATE + 32:
		{

		}
		break;
		}
	}
}

void GSResult::OnMouseMoving(int X, int Y) {
}

void GSResult::OnMouseButtonDown(int X, int Y, char Button) {
	//m_SM->OnMouseButtonDown(X, Y, Button);
}

void GSResult::OnMouseButtonUp(int X, int Y, char Button) {
	m_BM->OnMouseButtonUp(X, Y, Button);
}

void GSResult::OnMouseButtonMove(int X, int Y, char Button) {
	//m_SM->OnMouseButtonMove(X, Y, Button);
}
