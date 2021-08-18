#include "ButtonManager.h"
#include <iostream>
#include "ResourceManager.h"
#include "GameStateMachine.h"
#include "define.h"

ButtonManager::ButtonManager(char* srcButton) {
	ReadFile(srcButton);
}

ButtonManager::~ButtonManager() {

}

int ButtonManager::ReadFile(char* srcButton) {
	FILE* f_M;
	f_M = fopen(srcButton, "r+");
	if (f_M == NULL) {
		return false;
	}

	int numberOfButtons;
	fscanf(f_M, "#Buttons: %d\n", &numberOfButtons);
	if (numberOfButtons <= 0)
		return false;
	auto pModel = new Models(1, "../Resources/Models/Sprite2D.nfg");
	int id, shader, texture, X, Y, W, H;
	char type[20];
	for (int i = 0; i < numberOfButtons; ++i)
	{
		fscanf(f_M, "ID %d\n", &id);
		fscanf(f_M, "SHADER %d\n", &shader);
		fscanf(f_M, "TEXTURE %d\n", &texture);
		fscanf(f_M, "COORD %d %d %d %d\n", &X, &Y, &W, &H);
		fscanf(f_M, "FUNC %s\n", type);

		auto button = std::make_shared<GameButton>(id++);
		button->setModel(pModel);
		button->setShader(ResourceManager::GetInstance()->GetShaderAtID(shader));
		button->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(texture));
		button->Set2DPosition(X, Y);
		button->SetSize(W, H);
		AddFunction(type, button);		
		m_listButton.push_back(button);
	}
}

void ButtonManager::Update(float deltaTime)
{
	for (register int i = 0; i < m_listButton.size(); i++) {
		m_listButton[i]->Update(deltaTime);
	}
}

void ButtonManager::CleanUp()
{
	for (register int i = 0; i < m_listButton.size(); i++) {
		m_listButton[i]->CleanUp();
	}
}

void ButtonManager::Draw()
{
	for (register int i = 0; i < m_listButton.size(); i++) {
		m_listButton[i]->Draw();
	}
}

void ButtonManager::AddFunction(char* type, std::shared_ptr<GameButton> button) {
	if (strcmp(type, "LEVEL1") == 0) {
		button->SetOnClick([]() {
			if (GameStateMachine::GetInstance()->HasInstance()) {
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL1);
			}
		});
	}
	else if (strcmp(type, "LEVEL2") == 0) {
		button->SetOnClick([]() {
			if (GameStateMachine::GetInstance()->HasInstance()) {
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL2);
			}
		});
	}
}

void ButtonManager::OnMouseButtonUp(int X, int Y, char Button)
{
	switch (Button) {
	case LMB:
	{
		for (register int i = 0; i < m_listButton.size(); i++) {
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
