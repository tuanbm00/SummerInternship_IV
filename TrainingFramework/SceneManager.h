#pragma once
#include <fstream>
#include <crtdbg.h>
#include "Object.h"
#include <vector>

class SceneManager
{
private:
	std::vector<std::shared_ptr<Object>> m_Object;
	bool m_bIsFighting = false;
	Vector2 m_oTarget;
	char* m_fileSM = NULL;
public:

	SceneManager(char* fileSM);
	~SceneManager();

	void Init();
	void Update(float deltaTime);
	void Draw();
	void Key(unsigned char key, bool bIsPressed);
	void OnMouseButtonDown(int X, int Y, char Button);
	void OnMouseButtonUp(int X, int Y, char Button);
	void OnMouseButtonMove(int X, int Y, char Button);

	void AddObject(std::shared_ptr<Object> obj);
	void CleanUp();
	void ReadFile(FILE* f_SM);
	void SetFileManager(char* fileSM);

	//Object Fighting
	void SetIsFighting(bool IsFighting);
};





