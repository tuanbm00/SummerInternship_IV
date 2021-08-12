#pragma once
#include "Object.h"
#include "define.h"

class Bullet :
    public Object
{
private:
	int m_NumberOfBullet;
	int m_isChange;
	float m_AttackDame;
	float m_AttackSpeed;
	float m_SpeedOfBulletX;
	float m_SpeedOfBulletY;
	float m_MaxOfLength;
	float m_CurrentLength;
public:
    Bullet(int ID) : Object(ID) {}

	void InitA(float attackDame, float attackSpeed, float speedOfBulletX, float speedOfBulletY, float maxOfLength);
	void ResetBullet();
	void SetCurrLength(float curr);
	void ReverseV();
	bool IsEmptyBullet();
	bool IsOverLength();
	void SetIsChange();

	bool IsChange();
	float GetAttackDame();
	float GetAttackSpeed();
	float GetCurrLength();

	Vector2 GetSpeedOfBullet();
	float GetMaxOfLength();
    void Update(float deltaTime);
	void SetBodyObject(float positionX, float positionY, b2World* world, bool isPlayer = true);
};

