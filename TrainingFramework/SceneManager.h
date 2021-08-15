#pragma once
#include <fstream>
#include <crtdbg.h>
#include <vector>
#include <map>
#include "box2d/b2_world.h"
#include"box2d/b2_body.h"
#include"box2d/b2_fixture.h"
#include"box2d/b2_circle_shape.h"
#include"box2d/b2_polygon_shape.h"
#include "Object.h"
#include "MainCharacter.h"
#include "NPC.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Terrain.h"
#include "Ground.h"

class SceneManager
{
private:
	int wlow, whigh, hlow, hhigh;

	Ground * groundTest;
	std::vector<std::vector<Terrain*>> m_listTerrain;
	std::vector<Terrain*> m_ListBackground;
	std::vector<std::vector<int>> map;
	std::map<std::pair<int, int>, int> mapEnemy;
	std::map<std::pair<int, int>, Enemy*> m_mapEnemy;
	std::vector<std::vector<int>> isInit;
	MainCharacter* m_MainCharacter;
	std::vector<Bullet*> m_ListGun;
	std::vector<Bullet*> m_listBulletInWorld;
	std::vector<Enemy*> m_listEnemy;
	std::vector<Enemy*> m_listEnemyInWorld;

	float m_direction;
	float m_Horizontal;
	float m_Vertical;
	float m_shoot;
	float m_changeGun;
	float m_time;
	float m_timeChangeGun;
	int keyPressed;

	bool m_bIsFighting = false;
	bool is_in_ground;
	Vector2 m_oTarget;
	char* m_fileSM = NULL;
	char* m_fileMAP = NULL;
	b2World * m_world;
	int jumpstep;
	int numJump;
public:
	int * indices;
	SceneManager(char* fileSM, char* fileMAP);
	~SceneManager();

	void Init();
	void Update(float deltaTime);
	void Draw();
	void Key(unsigned char key, bool bIsPressed);
	void OnMouseButtonDown(int X, int Y, char Button);
	void OnMouseButtonUp(int X, int Y, char Button);
	void OnMouseButtonMove(int X, int Y, char Button);

	void AddGun(Bullet* gun);
	void AddBullet(Bullet* bullet);
	void RemoveBullet(int index);
	void AddEnemy(Enemy* enemy);


	void CheckMovement();
	
	void Shoot();
	void ChangeGun(bool isEmptyBullet = true);
	void SetStateHellGun(Bullet* hellBullet, float enemyWidth);
	void EnemyAttack(Enemy* enemy);

	void CleanUp();
	void ReadFile(FILE* f_SM);
	void ReadMap(FILE* f_MAP);
	void SetFileManager(char* fileSM, char* fileMAP);

	//Object Fighting
	void SetIsFighting(bool IsFighting);
};





