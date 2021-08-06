#pragma once
#include "Singleton.h"
#include "define.h"


class Application : public Singleton<Application>
{
public:
	Application();
	~Application();
	void Init();
	void Update(GLfloat deltaTime);
	void Exit();

	void HandleKeyEvents(unsigned char key, GLint X, GLint Y, bool bIsPressed);
	void OnMouseButtonDown(GLint X, GLint Y, char Button);
	void OnMouseButtonUp(GLint X, GLint Y, char Button);
	void OnMouseButtonMove(GLint X, GLint Y, char Button);
	void OnMouseMoving(GLint X, GLint Y);

	void Render();

private:


};

