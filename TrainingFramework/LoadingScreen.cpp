#include "LoadingScreen.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <iostream>
#include "TextManager.h"

LoadingScreen::LoadingScreen() {

}

LoadingScreen::~LoadingScreen() {

}

void LoadingScreen::Init() {
	//Manager Initialize
	//Singleton<TextManager>::GetInstance()->Initialize();
	m_isLoading = false;
	isDraw = true;
	m_currentTime = 0;
	Models * model = ResourceManager::GetInstance()->GetModelAtID(0);

	//Background Initialize
	auto obj = std::make_shared<Sprite2D>(222);
	obj->setModel(model);
	obj->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	obj->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(48));
	obj->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	obj->SetSize(Globals::screenWidth, Globals::screenHeight);
	m_listSprite.push_back(obj);

	obj = std::make_shared<Sprite2D>(222);
	obj->setModel(model);
	obj->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	obj->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(49));
	obj->Set2DPosition(Globals::screenWidth - Globals::screenWidth/5, Globals::screenHeight - Globals::screenHeight/10);
	obj->SetSize(200, 48);
	m_listSprite.push_back(obj);

	obj = std::make_shared<Sprite2D>(222);
	obj->setModel(model);
	obj->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	obj->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(46));
	obj->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	obj->SetSize(829 / 1.5f, 291 / 1.5f);
	m_listSprite.push_back(obj);

}

void LoadingScreen::Draw() {
	if (!m_isLoading) return;
	for (register int i = 0; i < m_listSprite.size(); i++) {
		m_listSprite[i]->Draw();
	}
	isDraw = true;
}

void LoadingScreen::Update(float deltaTime) {
	if (!m_isLoading) return;
	m_currentTime += deltaTime;
	if (m_currentTime > 3.0f) m_isLoading = false;
}

void LoadingScreen::CleanUp() {
	for (register int i = 0; i < m_listSprite.size(); i++) {
		m_listSprite[i]->CleanUp();
	}
}

void LoadingScreen::Resume() {

}

void LoadingScreen::Pause() {

}


//Events
void LoadingScreen::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
	//m_SM->Key(key, bIsPressed);
	if (!bIsPressed) {
		switch (key) {

		}
	}
}

void LoadingScreen::OnMouseMoving(int X, int Y) {
}

void LoadingScreen::OnMouseButtonDown(int X, int Y, char Button) {
	//m_SM->OnMouseButtonDown(X, Y, Button);
}

void LoadingScreen::OnMouseButtonUp(int X, int Y, char Button) {
}

void LoadingScreen::OnMouseButtonMove(int X, int Y, char Button) {
	//m_SM->OnMouseButtonMove(X, Y, Button);
}