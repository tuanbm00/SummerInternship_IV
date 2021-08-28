// TrainingFramework.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"
#include "Application.h"
#include "Globals.h"
#include "Camera.h"


int Init(ESContext* esContext){	
	if (Application::GetInstance()->HasInstance()) {
		Application::GetInstance()->Init();
	}
	return 0;
}

void Draw(ESContext* esContext)
{
	if (Application::GetInstance()->HasInstance()) {
		Application::GetInstance()->Render();
	}
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void CleanUp()
{
	if (Application::GetInstance()->HasInstance()) {
		Application::GetInstance()->Exit();
	}
	Camera::GetInstance()->CleanUp();

	exit(0);
}

void Update(ESContext* esContext, float deltaTime)
{
	if (Camera::GetInstance()->is_exit) {
		CleanUp();
	}
	if (Application::GetInstance()->HasInstance()) {
		Application::GetInstance()->Update(deltaTime);
	}
}

void KeyEvents(ESContext* esContext, unsigned char key, int X, int Y, bool bIsPressed)
{
	if (Application::GetInstance()->HasInstance()) {
		Application::GetInstance()->HandleKeyEvents(key, X, Y, bIsPressed);
	}
}

void OnMouseMoving(ESContext* esContext, int X, int Y)
{
	if (Application::GetInstance()->HasInstance()) {
		Application::GetInstance()->OnMouseMoving(X, Y);
	}
}

void OnMouseButtonDown(ESContext* esContext, int X, int Y, char Button)
{
	if (Application::GetInstance()->HasInstance()) {
		Application::GetInstance()->OnMouseButtonDown(X, Y, Button);
	}
}

void OnMouseButtonUp(ESContext* esContext, int X, int Y, char Button)
{
	if (Application::GetInstance()->HasInstance()) {
		Application::GetInstance()->OnMouseButtonUp(X, Y, Button);
	}
}

void OnMouseButtonMove(ESContext* esContext, int X, int Y, char Button)
{
	if (Application::GetInstance()->HasInstance()) {
		Application::GetInstance()->OnMouseButtonMove(X, Y, Button);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "Four - Angel of Deaths", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;
	
	esRegisterKeyFunc(&esContext, KeyEvents);
	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterMouseDownFunc(&esContext, OnMouseButtonDown);
	esRegisterMouseUpFunc(&esContext, OnMouseButtonUp);
	esRegisterMouseMoveFunc(&esContext, OnMouseButtonMove);
	esRegisterMouseFunc(&esContext, OnMouseMoving);
	

	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();
	//identifying memory leaks
	//MemoryDump();
	return 0;
}

