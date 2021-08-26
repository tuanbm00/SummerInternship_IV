#include "GSIntro.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <iostream>
#include "TextManager.h"

GSIntro::GSIntro() {

}

GSIntro::~GSIntro() {

}

void GSIntro::Init() {
	m_currentTime = 0;
	//Manager Initialize
	//Singleton<TextManager>::GetInstance()->Initialize();

	//Sprite2D Initialize
	auto model = new Models(1, "../Resources/Models/Sprite2D.nfg");
	//Logo
	auto logo = std::make_shared<Sprite2D>(1);
	logo->setModel(model);
	logo->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	logo->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(12));
	logo->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	logo->SetSize(400, 400);
	logo->CalculateWVP();
	m_listSprite2D.push_back(logo);

	//Background
	auto bg = std::make_shared<Sprite2D>(0);
	bg->setModel(model);
	bg->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	bg->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(11));
	bg->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	bg->SetSize(Globals::screenWidth, Globals::screenHeight);
	bg->CalculateWVP();
	m_listSprite2D.push_back(bg);
}

void GSIntro::Draw() {
	char name[5] = "Four";
	Singleton<TextManager>::GetInstance()->RenderString(name, Vector4(0.0f, 1.0f, 0.0f), Globals::screenWidth / 2, 700.0f, 1.0f, 1.0f);
	for (register int i = m_listSprite2D.size()-1; i >= 0; --i) {
		m_listSprite2D[i]->Draw();
	}
}

void GSIntro::Update(float deltaTime) {
	m_currentTime += deltaTime;
	if (m_currentTime >= 3.0f) {
		if (GameStateMachine::GetInstance()->HasInstance()) {
			GameStateMachine::GetInstance()->PushState(StateTypes::GS_MAINMENU);
		}
	}
}

void GSIntro::CleanUp() {
	for (register int i = 0; i < (int)m_listSprite2D.size(); ++i) {
		m_listSprite2D[i]->CleanUp();
	}
}

void GSIntro::Resume() {

}

void GSIntro::Pause() {
	//ResourceManager::GetInstance()->StopAllSound();
}


//Events
void GSIntro::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {

}

void GSIntro::OnMouseMoving(int X, int Y) {
}

void GSIntro::OnMouseButtonDown(int X, int Y, char Button) {
	//m_SM->OnMouseButtonDown(X, Y, Button);
}

void GSIntro::OnMouseButtonUp(int X, int Y, char Button) {
	//m_BM->OnMouseButtonUp(X, Y, Button);
}

void GSIntro::OnMouseButtonMove(int X, int Y, char Button) {
	//m_SM->OnMouseButtonMove(X, Y, Button);
}
