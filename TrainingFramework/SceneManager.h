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
#include "Bullet.h"
#include "Enemy.h"

class SceneManager
{
private:
	std::vector<Object*> m_ListTerrain;
	std::shared_ptr<MainCharacter> m_MainCharactor;
	std::vector<Bullet*> m_ListGun;
	std::vector<Bullet*> m_listBulletInWorld;
	std::vector<Enemy*> m_listEnemy;
	bool m_bIsFighting = false;
	Vector2 m_oTarget;
	char* m_fileSM = NULL;
	b2World * m_world;
	
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

	void AddTerrain(Object* terrain);
	void AddGun(Bullet* gun);
	void AddBullet(Bullet* bullet);
	void RemoveBullet(int index);
	void AddEnemy(Enemy* enemy);
	void CleanUp();
	void ReadFile(FILE* f_SM);
	void SetFileManager(char* fileSM);

	//Object Fighting
	void SetIsFighting(bool IsFighting);
};





