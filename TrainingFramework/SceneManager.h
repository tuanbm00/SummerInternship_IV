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
<<<<<<< Updated upstream
=======
#include "Ground.h"
>>>>>>> Stashed changes

class SceneManager
{
private:
<<<<<<< Updated upstream
	std::vector<Object*> m_ListTerrain;
=======
	std::vector<Ground *> m_ListTerrain;
>>>>>>> Stashed changes
	MainCharacter* m_MainCharacter;
	std::vector<Bullet*> m_ListGun;
	std::vector<Bullet*> m_listBulletInWorld;
	std::vector<Enemy*> m_listEnemy;

	float m_direction;
	float m_Horizontal;
	float m_Vertical;
	float m_shoot;
	float m_changeGun;
	float m_time;
	float m_timeChangeGun;
	int keyPressed;

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

<<<<<<< Updated upstream
	void AddTerrain(Object* terrain);
=======
	void AddTerrain(Ground* terrain);
>>>>>>> Stashed changes
	void AddGun(Bullet* gun);
	void AddBullet(Bullet* bullet);
	void RemoveBullet(int index);
	void AddEnemy(Enemy* enemy);

<<<<<<< Updated upstream
	void Key(unsigned char key, bool isPressed);
=======
>>>>>>> Stashed changes
	void CheckMovement();
	
	void Shoot();
	void ChangeGun(bool isEmptyBullet = true);
	void SetStateHellGun(Bullet* hellBullet, float enemyWidth);
<<<<<<< Updated upstream
	void Update(float deltaTime);
=======
>>>>>>> Stashed changes

	void CleanUp();
	void ReadFile(FILE* f_SM);
	void SetFileManager(char* fileSM);

	//Object Fighting
	void SetIsFighting(bool IsFighting);
};





