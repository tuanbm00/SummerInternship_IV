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
	fopen_s(&f_M,srcButton, "r+");
	if (f_M == NULL) {
		return false;
	}

	int numberOfButtons;
	fscanf_s(f_M, "#Buttons: %d\n", &numberOfButtons);
	if (numberOfButtons <= 0)
		return false;
	auto pModel = new Models(1, "../Resources/Models/Sprite2D.nfg");
	int id, shader, texture, X, Y, W, H;
	char type[20];
	for (int i = 0; i < numberOfButtons; ++i)
	{
		fscanf_s(f_M, "ID %d\n", &id);
		fscanf_s(f_M, "SHADER %d\n", &shader);
		fscanf_s(f_M, "TEXTURE %d\n", &texture);
		fscanf_s(f_M, "COORD %d %d %d %d\n", &X, &Y, &W, &H);
		fscanf_s(f_M, "FUNC %s\n", type, _countof(type));

		auto button = std::make_shared<GameButton>(id++);
		button->setModel(pModel);
		button->setShader(ResourceManager::GetInstance()->GetShaderAtID(shader));
		button->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(texture));
		button->Set2DPosition(X, Y);
		button->SetSize(W, H);
		AddFunction(type, button);		
		m_listButton.push_back(button);
	}
	return 0;
}

void ButtonManager::Update(float deltaTime)
{
	for (register int i = 0; i < (int)m_listButton.size(); i++) {
		m_listButton[i]->Update(deltaTime);
	}
}

void ButtonManager::CleanUp()
{
	for (register int i = 0; i < (int)m_listButton.size(); i++) {
		m_listButton[i]->CleanUp();
	}
}

void ButtonManager::Draw()
{
	for (register int i = 0; i < (int)m_listButton.size(); i++) {
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
	else if (strcmp(type, "EXIT") == 0) {
		button->SetOnClick([]() {
			exit(0);
		});
	}
}

void ButtonManager::OnMouseButtonUp(int X, int Y, char Button)
{
	switch (Button) {
	case LMB:
	{
		for (register int i = 0; i < (int)m_listButton.size(); i++) {
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
