#pragma once
#include "Object.h"

class Bullet :
    public Object
{
private:
	int m_NumberOfBullet;
	float m_AttackSpeed;
	float m_SpeedOfBullet;
	float m_MaxOfLength;
	float m_CurrentLength;
public:
    Bullet(int ID) : Object(ID) {}
	void Init(int numberOfBullet, float attackSpeed, float speedOfBullet, float maxOfLength);
	bool IsEmptyBullet();
	bool IsOverLength();
	float GetAttackSpeed();
	float GetSpeedOfBullet();
	float GetMaxOfLength();
    void Update(float deltaTime);
	void SetBodyObject(float positionX, float positionY, float width, float height, bool isPlayer);
};

