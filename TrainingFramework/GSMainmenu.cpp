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
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/FutariNoKimochi.mp3", true);
	char* BM = "../Resources/Managers/BM_Mainmenu.txt";
	m_BM = std::make_shared<ButtonManager>(BM, 0);

	//Background Initialize
	m_Background = std::make_shared<Sprite2D>(0);
	auto model = new Models(1, "../Resources/Models/Sprite2D.nfg");
	ResourceManager::GetInstance()->addModels(model);
	m_Background->setModel(model);
	m_Background->setShader(ResourceManager::GetInstance()->GetShaderAtID(1));
	m_Background->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(10));
	m_Background->Set2DPosition(Globals::screenWidth/2, Globals::screenHeight/2);
	m_Background->SetSize(Globals::screenWidth, Globals::screenHeight);
	
	m_Background->CalculateWVP();
}

void GSMainmenu::Draw() {
	m_Background->Draw();
	m_BM->Draw();
	char name[7] = "Fourth";
	Singleton<TextManager>::GetInstance()->RenderString(name, Vector4(0.0f, 1.0f, 0.0f), Globals::screenWidth / 2 - 100.0f, 600.0f, 3.0f, 3.0f);
}

void GSMainmenu::Update(float deltaTime) {
	
}

void GSMainmenu::CleanUp() {
	m_BM->CleanUp();
	
}

void GSMainmenu::Resume() {
	ResourceManager::GetInstance()->StopAllSound();
	ResourceManager::GetInstance()->PlaySound("../Resources/Sounds/FutariNoKimochi.mp3", true);
}

void GSMainmenu::Pause() {
	ResourceManager::GetInstance()->StopAllSound();
}


//Events
void GSMainmenu::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
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
