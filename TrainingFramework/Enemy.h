#pragma once
#include "Object.h"
#include "Bullet.h"

class Enemy :
	public Object
{
private:
	float m_HP;
	float m_attackDame;
	float m_time;
	Bullet* m_bullet;
public:
	Enemy(int ID) : Object(ID) {}
	void SetHP(float hp);
	float GetHP();
	bool isDie();
	bool isAttack();

	void UpdateAttack(float deltaTime);
	void Update(float deltaTime);
	void SetBullet(Bullet* bullet);
	void SetBodyObject(float positionX, float positionY, b2World* world);

	Bullet* GetBullet();
};