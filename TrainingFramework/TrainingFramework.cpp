// TrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Application.h"
#include "Globals.h"
#include "Camera.h"

int Init(ESContext* esContext){	
	if (Application::GetInstance()->HasInstance()) {
		Application::GetInstance()->Init();
	}
	int *indices = new int[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;
	glGenBuffers(1, &Camera::GetInstance()->iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Camera::GetInstance()->iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), indices, GL_STATIC_DRAW);


	delete[] indices;
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
	glDeleteBuffers(1, &Camera::GetInstance()->iboId);
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

	esCreateWindow(&esContext, "Four", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

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

