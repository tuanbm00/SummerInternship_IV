#include "stdafx.h"
#include "ButtonManager.h"
#include "ResourceManager.h"
#include "GameStateMachine.h"
#include "define.h"
#include "ComfirmBox.h"
#include "Camera.h"
#include "LoadingScreen.h"

ButtonManager::ButtonManager(char* srcButton, int currentLevel) {
	m_currentLevel = currentLevel;
	ReadFile(srcButton);
}

ButtonManager::~ButtonManager() {
}

int ButtonManager::ReadFile(char* srcButton) {
	Singleton<ComfirmBox>::GetInstance()->Init();
	FILE* f_M;
	fopen_s(&f_M,srcButton, "r");
	if (f_M == NULL) {
		return false;
	}

	int numberOfButtons;
	fscanf_s(f_M, "#Buttons: %d\n", &numberOfButtons);
	if (numberOfButtons <= 0)
		return false;

	Models * pModel = ResourceManager::GetInstance()->GetModelAtID(0);

	int id, shader, texture, W, H, active, comfirm;
	float X, Y;
	char type[20];
	for (int i = 0; i < numberOfButtons; ++i)
	{
		fscanf_s(f_M, "ID %d\n", &id);
		fscanf_s(f_M, "SHADER %d\n", &shader);
		fscanf_s(f_M, "TEXTURE %d\n", &texture);
		fscanf_s(f_M, "COORD %f %f %d %d\n", &X, &Y, &W, &H);
		fscanf_s(f_M, "ACTIVE %d\n", &active);
		fscanf_s(f_M, "COMFIRM %d\n", &comfirm);
		fscanf_s(f_M, "FUNC %s\n", type, _countof(type));

		auto button = std::make_shared<GameButton>(id++);
		button->setModel(pModel);
		button->setShader(ResourceManager::GetInstance()->GetShaderAtID(shader));
		button->SetTexture(ResourceManager::GetInstance()->GetTextureAtID(texture));
		button->Set2DPosition(X, Y);
		button->SetSize(W, H);
		button->SetActive(active);
		button->SetNeedComfirm(comfirm);
		AddFunction(type, button);		
		button->CalculateWVP();
		m_listButton.push_back(button);
	}
	fclose(f_M);
	return 0;
}

void ButtonManager::Update(float deltaTime)
{
	for (register int i = 0; i < size_as_int(m_listButton); ++i) {
		m_listButton[i]->Update(deltaTime);
	}
	//for (register int i = 0; i < (int)m_listButton.size(); ++i) {
	
	//}
}

void ButtonManager::CleanUp()
{
	for (register int i = 0; i < size_as_int(m_listButton); ++i) {
		m_listButton[i]->CleanUp();
	}
	Singleton<ComfirmBox>::GetInstance()->CleanUp();
}

void ButtonManager::Draw()
{
	for (register int i = 0; i < size_as_int(m_listButton); ++i) {
		m_listButton[i]->Draw();
	}
	Singleton<ComfirmBox>::GetInstance()->Draw();
}

void ButtonManager::AddFunction(char* type, std::shared_ptr<GameButton> button) {
	if (strcmp(type, "MAINMENU") == 0) {
		button->SetOnClick([]() {
			GameStateMachine::GetInstance()->PopState();
			if (GameStateMachine::GetInstance()->HasInstance()) {
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_MAINMENU);
			}
		});
	}
	else if (strcmp(type, "OPTIONS") == 0) {
		button->SetOnClick([]() {
			if (GameStateMachine::GetInstance()->HasInstance()) {
					GameStateMachine::GetInstance()->PushState(StateTypes::GS_OPTIONS);
			}
		});
	}
	else if (strcmp(type, "ABOUTUS") == 0) {
		button->SetOnClick([]() {
			if (GameStateMachine::GetInstance()->HasInstance()) {
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_ABOUTUS);
			}
		});
	}
	else if (strcmp(type, "LOADLEVEL") == 0) {
		button->SetOnClick([]() {
			if(GameStateMachine::GetInstance()->m_pActiveState->m_currentState != GSMAINMENU) GameStateMachine::GetInstance()->PopState();
			if (GameStateMachine::GetInstance()->HasInstance()) {
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_LOADLEVEL);
			}
		});
	}
	else if (strcmp(type, "LEVEL1") == 0) {
		button->SetOnClick([]() {
			Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
			Singleton<LoadingScreen>::GetInstance()->isDraw = false;
			GameStateMachine::GetInstance()->PopState();
			if (GameStateMachine::GetInstance()->HasInstance()) {
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL1);
			}
		});
	}
	else if (strcmp(type, "LEVEL2") == 0) {
		button->SetOnClick([]() {
			Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
			Singleton<LoadingScreen>::GetInstance()->isDraw = false; GameStateMachine::GetInstance()->PopState();
			if (GameStateMachine::GetInstance()->HasInstance()) {
				
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL2);
			}
		});
	}
	else if (strcmp(type, "LEVEL3") == 0) {
		button->SetOnClick([]() {
			Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
			Singleton<LoadingScreen>::GetInstance()->isDraw = false; GameStateMachine::GetInstance()->PopState();
			if (GameStateMachine::GetInstance()->HasInstance()) {
				
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL3);
			}
		});
	}
	else if (strcmp(type, "LEVEL4") == 0) {
		button->SetOnClick([]() {
			Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
			Singleton<LoadingScreen>::GetInstance()->isDraw = false; GameStateMachine::GetInstance()->PopState();
			if (GameStateMachine::GetInstance()->HasInstance()) {
				GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL4);
			}
		});
	}
	else if (strcmp(type, "RETRY") == 0) {
		//GameStateMachine::GetInstance()->PopState();
		switch (m_currentLevel) {
			case 1:
				button->SetOnClick([]() {
					Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
					Singleton<LoadingScreen>::GetInstance()->isDraw = false; GameStateMachine::GetInstance()->PopState();
					if (GameStateMachine::GetInstance()->HasInstance()) {
						GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL1);
					}
				});
				break;
			case 2:
				button->SetOnClick([]() {
					Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
					Singleton<LoadingScreen>::GetInstance()->isDraw = false; GameStateMachine::GetInstance()->PopState();
					if (GameStateMachine::GetInstance()->HasInstance()) {
						GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL2);
					}
				});
				break;
			case 3:
				button->SetOnClick([]() {
					Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
					Singleton<LoadingScreen>::GetInstance()->isDraw = false; GameStateMachine::GetInstance()->PopState();
					if (GameStateMachine::GetInstance()->HasInstance()) {
						GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL3);
					}
				});
				break;
			case 4:
				button->SetOnClick([]() {
					Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
					Singleton<LoadingScreen>::GetInstance()->isDraw = false; GameStateMachine::GetInstance()->PopState();
					if (GameStateMachine::GetInstance()->HasInstance()) {
						GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL4);
					}
				});
				break;
			default:
				break;
		}
		
	}
	else if (strcmp(type, "NEXTLEVEL") == 0) {
		//GameStateMachine::GetInstance()->PopState();
		switch (m_currentLevel+1) {
		case 1:
			button->SetOnClick([]() {
				Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
				Singleton<LoadingScreen>::GetInstance()->isDraw = false; GameStateMachine::GetInstance()->PopState();
				if (GameStateMachine::GetInstance()->HasInstance()) {
					GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL1);
				}
			});
			break;
		case 2:
			button->SetOnClick([]() {
				Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
				Singleton<LoadingScreen>::GetInstance()->isDraw = false; GameStateMachine::GetInstance()->PopState();
				if (GameStateMachine::GetInstance()->HasInstance()) {
					GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL2);
				}
			});
			break;
		case 3:
			button->SetOnClick([]() {
				Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
				Singleton<LoadingScreen>::GetInstance()->isDraw = false; GameStateMachine::GetInstance()->PopState();
				if (GameStateMachine::GetInstance()->HasInstance()) {
					GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL3);
				}
			});
			break;
		case 4:
			button->SetOnClick([]() {
				Singleton<LoadingScreen>::GetInstance()->SetIsLoading(true);
				Singleton<LoadingScreen>::GetInstance()->isDraw = false; GameStateMachine::GetInstance()->PopState();
				if (GameStateMachine::GetInstance()->HasInstance()) {
					GameStateMachine::GetInstance()->PushState(StateTypes::GS_LEVEL4);
				}
			});
			break;
		case 5:
			button->SetOnClick([]() {
				GameStateMachine::GetInstance()->PopState();
				if (GameStateMachine::GetInstance()->HasInstance()) {
					GameStateMachine::GetInstance()->PushState(StateTypes::GS_ABOUTUS);
				}
			});
			break;
		default:
			break;
		}

	}
	else if (strcmp(type, "POPSTATE") == 0) {
		button->SetOnClick([]() {
			if (GameStateMachine::GetInstance()->HasInstance()) {
				if(GameStateMachine::GetInstance()->GetIsCanPop())
				GameStateMachine::GetInstance()->PopState();
			}
		});
	}
	else if (strcmp(type, "SWITCHSOUND") == 0) {
		button->SetOnClick([]() {
			ResourceManager::GetInstance()->SwitchSound();
		});
	}
	else if (strcmp(type, "EXIT") == 0) {
		button->SetOnClick([]() {
			//exit(0);
			Camera::GetInstance()->is_exit = true;
		});
	}
}

void ButtonManager::OnMouseButtonUp(int X, int Y, char Button)
{
	switch (Button) {
	case LMB:
	{
		for (register int i = 0; i < size_as_int(m_listButton); ++i) {
			m_listButton[i]->OnMouseButtonUp(X, Y);
		}
	}
	break;
	case RMB:
	{
	}
	break;
	}
	Singleton<ComfirmBox>::GetInstance()->OnMouseButtonUp(X, Y, Button);
}

void ButtonManager::SetCurrentLevel(int currentLevel)
{
	m_currentLevel = currentLevel;
}
