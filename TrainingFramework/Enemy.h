#pragma once
#include "Object.h"
#include "Bullet.h"

class Enemy :
	public Object
{
private:
	float m_HP;
	float m_speedx;
	float m_speedy;
	float m_attackDame;
	float m_left;
	float m_right;
	int m_bulletID;
	Bullet* m_bullet;
public:
	float m_time;
	int cnt;
	int m_direction;
	Enemy(int ID) : Object(ID) {
		m_direction = 1;
		cnt = 0;
	}
	void SetHP(float hp);
	void SetBulletID(int bulletID);
	int GetBulletID();
	float GetHP();
	void SetSpeed(float speedx, float speedy);
	Vector2 GetSpeed();
	bool isDie();
	bool isAttack();

	void UpdateAttack(float deltaTime);
	void Update(float deltaTime);
	void SetLimit(float left, float right);
	void SetBullet(Bullet* bullet);
	void SetBodyObject(float positionX, float positionY, b2World* world, float scale);
	void UpdateAnimation(float deltaTime);

	Bullet* GetBullet();
};