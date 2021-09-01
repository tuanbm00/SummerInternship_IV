#include "stdafx.h"
#include "ComfirmBox.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "TextManager.h"

ComfirmBox::ComfirmBox() {

}

ComfirmBox::~ComfirmBox() {

}

void ComfirmBox::Init() {
	m_bIsActive = false;
	m_listButton.clear();
	//Manager Initialize
	//Singleton<TextManager>::GetInstance()->Initialize();

	Models * pModel = ResourceManager::GetInstance()->GetModelAtID(0);
	auto button = std::make_shared<GameButton>(555);
	button->setModel(pModel);
	button->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	button->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(42));
	button->Set2DPosition(Globals::screenWidth/2 - 100 , Globals::screenHeight/2 + 50);
	button->SetSize(150, 80);
	button->SetActive(1);
	button->SetNeedComfirm(false);
	button->CalculateWVP();
	m_listButton.push_back(button);

	button = std::make_shared<GameButton>(666);
	button->setModel(pModel);
	button->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	button->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(38));
	button->Set2DPosition(Globals::screenWidth / 2 + 100, Globals::screenHeight / 2 + 50);
	button->SetSize(150, 80);
	button->SetActive(1);
	button->SetNeedComfirm(false);
	button->SetOnClick([]() {
		Singleton<ComfirmBox>::GetInstance()->SetActive(false);
	});
	button->CalculateWVP();
	m_listButton.push_back(button);

	//Frame Initialize
	m_Frame = std::make_shared<Sprite2D>(0);
	m_Frame->setModel(pModel);
	m_Frame->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	m_Frame->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(43));
	m_Frame->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	m_Frame->SetSize(Globals::screenWidth / 2, Globals::screenHeight / 2 - 100);
}


void ComfirmBox::Draw() {
	if (!m_bIsActive) {
		return;
	}
	m_Frame->Draw();
	for (register int i = 0; i < size_as_int(m_listButton); i++) {
		m_listButton[i]->Draw();
	}
	
	//MainCharacter

	Singleton<TextManager>::GetInstance()->RenderString("ARE YOU SURE???", Vector4(1.0f, 0.0f, 0.0f), Globals::screenWidth/2.0f - 150.0f, Globals::screenHeight/2.0 + 50.0f, 1.5f, 1.5f);
}

void ComfirmBox::Update(float deltaTime) {
}

void ComfirmBox::CleanUp() {
	for (register int i = 0; i < size_as_int(m_listButton); i++) {
		m_listButton[i]->CleanUp();
	}
	m_Frame->CleanUp();
}

void ComfirmBox::Resume() {

}

void ComfirmBox::Pause() {

}


//Events
void ComfirmBox::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
}

void ComfirmBox::OnMouseMoving(int X, int Y) {
}


void ComfirmBox::OnMouseButtonUp(int X, int Y, char Button) {
	if (!m_bIsActive)
		return;
	switch (Button) {
	case LMB:
	{
		for (register int i = 0; i < size_as_int(m_listButton); i++) {
			m_listButton[i]->OnMouseButtonUp(X, Y);
		}
	}
	break;
	case RMB:
	{
	}
	break;
	}
}

void ComfirmBox::OnMouseButtonMove(int X, int Y, char Button) {
	//m_SM->OnMouseButtonMove(X, Y, Button);
}