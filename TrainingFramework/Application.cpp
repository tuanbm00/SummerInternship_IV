#include "stdafx.h"
#include "Application.h"
#include "GameStateMachine.h"
#include "GameStatebase.h"
#include "TextManager.h"
#include "ResourceManager.h"
#include "LoadingScreen.h"

Application::Application()
{
}


Application::~Application()
{
}


void Application::Init()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, 960, 720);
	Singleton<TextManager>::GetInstance()->Initialize();
	ResourceManager::GetInstance()->Init();
	GameStateMachine::GetInstance()->ChangeState(StateTypes::GS_INTRO);
	Singleton<LoadingScreen>::GetInstance()->Init();
}

void Application::Update(GLfloat deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (Singleton<LoadingScreen>::GetInstance()->isDraw == false) return;
	GameStateMachine::GetInstance()->PerformStateChange();
	if (Singleton<LoadingScreen>::GetInstance()->GetIsLoading()) return;
	if (GameStateMachine::GetInstance()->HasState())
		GameStateMachine::GetInstance()->CurrentState()->Update(deltaTime);
}

void Application::Render()
{
	Singleton<LoadingScreen>::GetInstance()->Draw();
	if (Singleton<LoadingScreen>::GetInstance()->GetIsLoading()) return;
	if (GameStateMachine::GetInstance()->HasState())
		GameStateMachine::GetInstance()->CurrentState()->Draw();
}


void Application::HandleKeyEvents(unsigned char key, GLint X, GLint Y, bool bIsPressed) {
	if (GameStateMachine::GetInstance()->HasState())
		GameStateMachine::GetInstance()->CurrentState()->HandleKeyEvents(key, X, Y, bIsPressed);
}
void Application::OnMouseButtonDown(GLint X, GLint Y, char Button) {
	if (GameStateMachine::GetInstance()->HasState())
	GameStateMachine::GetInstance()->CurrentState()->OnMouseButtonDown(X, Y, Button);
		
}
void Application::OnMouseButtonUp(GLint X, GLint Y, char Button) {
	if (GameStateMachine::GetInstance()->HasState())
	GameStateMachine::GetInstance()->CurrentState()->OnMouseButtonUp(X, Y, Button);
}
void Application::OnMouseButtonMove(GLint X, GLint Y, char Button) {
	if (GameStateMachine::GetInstance()->HasState())
	GameStateMachine::GetInstance()->CurrentState()->OnMouseButtonMove(X, Y, Button);
}
void Application::OnMouseMoving(GLint X, GLint Y) {
	if (GameStateMachine::GetInstance()->HasState())
	GameStateMachine::GetInstance()->CurrentState()->OnMouseMoving(X, Y);
}


void Application::Exit()
{
	GameStateMachine::GetInstance()->Cleanup();
	ResourceManager::GetInstance()->CleanDump();
	ResourceManager::GetInstance()->CleanUp();
	delete ResourceManager::GetInstance();
	Singleton<TextManager>::GetInstance()->FreeInstance();

}
