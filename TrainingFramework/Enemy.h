#pragma once
#include "Object.h"
#include "Bullet.h"

class Enemy :
	public Object
{
private:
	int m_HP;
	float m_attackDame;
	Bullet* m_bullet;
public:
	Enemy(int ID) : Object(ID) {}
	void SetHP(float hp);
	float GetHP();
	bool isDie();
	void Update(float deltaTime);
	void SetBodyObject(float positionX, float positionY, b2World* world);
};