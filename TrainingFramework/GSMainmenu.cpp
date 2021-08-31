#include "GSMainmenu.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <iostream>
#include "TextManager.h"
#include "GameplayUI.h"

GSMainmenu::GSMainmenu() {
	
}

GSMainmenu::~GSMainmenu() {

}

void GSMainmenu::Init() {
	//Manager Initialize
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/TKKHT7.mp3", true);
	char* BM = "../Resources/Managers/BM_Mainmenu.txt";
	m_BM = std::make_shared<ButtonManager>(BM, 0);

	auto model = new Models(1, "../Resources/Models/Sprite2D.nfg");
	ResourceManager::GetInstance()->addModels(model);

	//Background Initialize
	auto bg = std::make_shared<Sprite2D>(0);
	bg->setModel(model);
	bg->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	bg->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(10));
	bg->Set2DPosition(Globals::screenWidth/2, Globals::screenHeight/2);
	bg->SetSize(Globals::screenWidth, Globals::screenHeight);
	bg->CalculateWVP();
	m_listSprite.push_back(bg);

	//Logo Game
	auto logo = std::make_shared<Sprite2D>(0);
	logo->setModel(model);
	logo->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	logo->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(45));
	logo->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 4);
	logo->SetSize(829/3 * 2, 291/3*2);
	logo->CalculateWVP();
	m_listSprite.push_back(logo);

	//Logo Studio
	auto logo2 = std::make_shared<Sprite2D>(0);
	logo2->setModel(model);
	logo2->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	logo2->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(44));
	logo2->Set2DPosition(70, Globals::screenHeight - 70);
	logo2->SetSize(100, 100);
	logo2->CalculateWVP();
	m_listSprite.push_back(logo2);


	//Set Current State
	m_currentState = GSMAINMENU;
}

void GSMainmenu::Draw() {
	for (register int i = 0; i < m_listSprite.size(); i++) {
		m_listSprite[i]->Draw();
	}
	m_BM->Draw();
}

void GSMainmenu::Update(float deltaTime) {
	
}

void GSMainmenu::CleanUp() {
	m_BM->CleanUp();
	for (register int i = 0; i < m_listSprite.size(); i++) {
		m_listSprite[i]->CleanUp();
	}
}

void GSMainmenu::Resume() {
	ResourceManager::GetInstance()->StopAllSound();
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/TKKHT7.mp3", true);
}

void GSMainmenu::Pause() {
	ResourceManager::GetInstance()->StopAllSound();
}


//Events
void GSMainmenu::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
	//m_SM->Key(key, bIsPressed);
	if (!bIsPressed) {
		switch (key) {

		}
	}
}

void GSMainmenu::OnMouseMoving(int X, int Y) {
}

void GSMainmenu::OnMouseButtonDown(int X, int Y, char Button) {
	//m_SM->OnMouseButtonDown(X, Y, Button);
}

void GSMainmenu::OnMouseButtonUp(int X, int Y, char Button) {
	m_BM->OnMouseButtonUp(X, Y, Button);
}

void GSMainmenu::OnMouseButtonMove(int X, int Y, char Button) {
	//m_SM->OnMouseButtonMove(X, Y, Button);
}
