#include "GSAboutUs.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <iostream>
#include "TextManager.h"
#include "GameplayUI.h"

GSAboutUs::GSAboutUs() {

}

GSAboutUs::~GSAboutUs() {

}

void GSAboutUs::Init() {
	//Manager Initialize
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/TKKHT4.mp3", true);

	char* BM = "../Resources/Managers/BM_AboutUs.txt";
	m_BM = std::make_shared<ButtonManager>(BM, 0);

	//Background Initialize
	auto model = new Models(1, "../Resources/Models/Sprite2D.nfg");
	ResourceManager::GetInstance()->addModels(model);

	auto obj = std::make_shared<Sprite2D>(999);
	obj->setModel(model);
	obj->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	obj->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(36));
	obj->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	obj->SetSize(Globals::screenWidth, Globals::screenHeight);
	obj->CalculateWVP();
	m_listSprite.push_back(obj);

	//Frame
	obj = std::make_shared<Sprite2D>(999);
	obj->setModel(model);
	obj->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	obj->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(43));
	obj->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2 - 50);
	obj->SetSize(Globals::screenWidth / 4 * 3, Globals::screenHeight / 4 * 3);
	obj->CalculateWVP();
	m_listSprite.push_back(obj);

	//Logo Game
	auto logo = std::make_shared<Sprite2D>(0);
	logo->setModel(model);
	logo->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	logo->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(46));
	logo->Set2DPosition(Globals::screenWidth / 4 + 50, Globals::screenHeight / 2 - 100);
	logo->SetSize(Globals::screenWidth / 2, Globals::screenHeight / 5);
	logo->CalculateWVP();
	m_listSprite.push_back(logo);

	//Logo Studio
	auto logo2 = std::make_shared<Sprite2D>(0);
	logo2->setModel(model);
	logo2->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	logo2->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(44));
	logo2->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight - 250);
	logo2->SetSize(150, 150);
	logo2->CalculateWVP();
	m_listSprite.push_back(logo2);

	//Set Current State
	m_currentState = GSABOUTUS;
}

void GSAboutUs::Draw() {
	for (register int i = 0; i < m_listSprite.size(); i++) {
		m_listSprite[i]->Draw();
	}
	m_BM->Draw();

	Singleton<TextManager>::GetInstance()->RenderString("-- DEVELOPED BY --", Vector4(0.0f, 1.0f, 0.0f), Globals::screenWidth / 2 + 10.0f, 550.0f, 1.0f, 1.0f);
	Singleton<TextManager>::GetInstance()->RenderString("Nguyen Van Truong", Vector4(1.0f, 0.0f, 0.0f), Globals::screenWidth / 2 + 10.0f, 500.0f, 1.0f, 1.0f);
	Singleton<TextManager>::GetInstance()->RenderString("Bui Minh Tuan", Vector4(1.0f, 0.0f, 0.0f), Globals::screenWidth / 2 + 10.0f, 450.0f, 1.0f, 1.0f);
	Singleton<TextManager>::GetInstance()->RenderString("Phung Huu Phuoc Toai", Vector4(1.0f, 0.0f, 0.0f), Globals::screenWidth / 2 + 10.0f, 400.0f, 1.0f, 1.0f);
	Singleton<TextManager>::GetInstance()->RenderString("Ngo Quoc Khanh", Vector4(1.0f, 0.0f, 0.0f), Globals::screenWidth / 2 + 10.0f, 350.0f, 1.0f, 1.0f);
}

void GSAboutUs::Update(float deltaTime) {
	for (register int i = 0; i < m_listSprite.size(); i++) {
		m_listSprite[i]->Update(deltaTime);
	}
}

void GSAboutUs::CleanUp() {
	for (register int i = 0; i < m_listSprite.size(); i++) {
		m_listSprite[i]->CleanUp();
	}
	m_BM->CleanUp();
}

void GSAboutUs::Resume() {
	ResourceManager::GetInstance()->StopAllSound();
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/TKKHT4.mp3", true);
}

void GSAboutUs::Pause() {
	ResourceManager::GetInstance()->StopAllSound();
}


//Events
void GSAboutUs::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
	//m_SM->Key(key, bIsPressed);
	if (!bIsPressed) {
		switch (key) {

		}
	}
}

void GSAboutUs::OnMouseMoving(int X, int Y) {
}

void GSAboutUs::OnMouseButtonDown(int X, int Y, char Button) {
	//m_SM->OnMouseButtonDown(X, Y, Button);
}

void GSAboutUs::OnMouseButtonUp(int X, int Y, char Button) {
	m_BM->OnMouseButtonUp(X, Y, Button);
}

void GSAboutUs::OnMouseButtonMove(int X, int Y, char Button) {
	//m_SM->OnMouseButtonMove(X, Y, Button);
}
