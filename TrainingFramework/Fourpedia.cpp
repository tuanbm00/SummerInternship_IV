#include "stdafx.h"
#include "Fourpedia.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <iostream>
#include "TextManager.h"

Fourpedia::Fourpedia() {

}

Fourpedia::~Fourpedia() {

}

void Fourpedia::Init() {

	//Button
	m_buttonExit = std::make_shared<GameButton>(999);
	m_buttonExit->setModel(ResourceManager::GetInstance()->GetModelAtID(0));
	m_buttonExit->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	m_buttonExit->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(38));
	m_buttonExit->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight - 100);
	m_buttonExit->SetSize(150, 80);
	m_buttonExit->SetActive(true);
	m_buttonExit->SetNeedComfirm(false);
	m_buttonExit->SetOnClick([]() {
		if (GameStateMachine::GetInstance()->HasInstance()) {
			if (GameStateMachine::GetInstance()->GetIsCanPop())
				GameStateMachine::GetInstance()->PopState();
		}
	});
	m_buttonExit->CalculateWVP();

	//Sprite
	auto obj = std::make_shared<Sprite2D>(999);
	obj->setModel(ResourceManager::GetInstance()->GetModelAtID(0));
	obj->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	obj->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(36));
	obj->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	obj->SetSize(Globals::screenWidth, Globals::screenHeight);
	obj->CalculateWVP();
	m_listSprite.push_back(obj);

	obj = std::make_shared<Sprite2D>(999);
	obj->setModel(ResourceManager::GetInstance()->GetModelAtID(0));
	obj->setShader(ResourceManager::GetInstance()->GetShaderAtID(0));
	obj->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(56));
	obj->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	obj->SetSize(600, 350);
	obj->CalculateWVP();
	m_listSprite.push_back(obj);


	//Set Current State
	m_currentState = GSFOURPEDIA;
}

void Fourpedia::Draw() {
	for (register int i = 0; i < size_as_int(m_listSprite); i++) {
		m_listSprite[i]->Draw();
	}
	m_buttonExit->Draw();
}


void Fourpedia::Update(float deltaTime) {

}

void Fourpedia::CleanUp() {
	for (register int i = 0; i < size_as_int(m_listSprite); i++) {
		m_listSprite[i]->CleanUp();
	}
	m_buttonExit->CleanUp();
}

void Fourpedia::Resume() {

}

void Fourpedia::Pause() {

}


//Events
void Fourpedia::HandleKeyEvents(unsigned char key, int X, int Y, bool bIsPressed) {
	//m_SM->Key(key, bIsPressed);

}

void Fourpedia::OnMouseMoving(int X, int Y) {
}

void Fourpedia::OnMouseButtonDown(int X, int Y, char Button) {
	//m_SM->OnMouseButtonDown(X, Y, Button);
}

void Fourpedia::OnMouseButtonUp(int X, int Y, char Button) {
	switch (Button) {
	case LMB:
		m_buttonExit->OnMouseButtonUp(X, Y);
		break;
	default:
		break;
	}
}

void Fourpedia::OnMouseButtonMove(int X, int Y, char Button) {
	//m_SM->OnMouseButtonMove(X, Y, Button);
}