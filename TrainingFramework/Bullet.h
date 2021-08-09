#pragma once
#include "Object.h"
#include "define.h"

class Bullet :
    public Object
{
private:
	int m_NumberOfBullet;
	float m_AttackDame;
	float m_AttackSpeed;
	float m_SpeedOfBulletX;
	float m_SpeedOfBulletY;
	float m_MaxOfLength;
	float m_CurrentLength;
public:
    Bullet(int ID) : Object(ID) {}
<<<<<<< Updated upstream
	void Init(int numberOfBullet, float attackDame, float attackSpeed, float speedOfBulletX, float speedOfBulletY, float maxOfLength);
=======
	void InitA(int numberOfBullet, float attackDame, float attackSpeed, float speedOfBulletX, float speedOfBulletY, float maxOfLength);
>>>>>>> Stashed changes
	bool IsEmptyBullet();
	bool IsOverLength();
	float GetAttackDame();
	float GetAttackSpeed();
	Vector2 GetSpeedOfBullet();
	float GetMaxOfLength();
    void Update(float deltaTime);
	void SetBodyObject(float positionX, float positionY, b2World* world, bool isPlayer = true);
};

