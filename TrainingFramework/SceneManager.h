#pragma once
#include <fstream>
#include <crtdbg.h>
#include "Object.h"
#include <vector>
#include "box2d/b2_world.h"
#include"box2d/b2_body.h"
#include"box2d/b2_fixture.h"
#include"box2d/b2_circle_shape.h"
#include"box2d/b2_polygon_shape.h"
#include "MainCharacter.h"
#include "NPC.h"

class SceneManager
{
private:
	std::vector<Object *> m_Object;
	bool m_bIsFighting = false;
	Vector2 m_oTarget;
	char* m_fileSM = NULL;
	b2World * myWorld;
	
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

	void AddObject(Object * obj);
	void CleanUp();
	void ReadFile(FILE* f_SM);
	void SetFileManager(char* fileSM);

	//Object Fighting
	void SetIsFighting(bool IsFighting);
};





