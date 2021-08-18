#pragma once
#include "Object.h"
#include "Bullet.h"

class Boss :
	public Object
{
private:
	float m_MaxHP;
	float m_HP;
	float m_speedx;
	float m_speedy;
	std::vector<int> m_ListBulletID;
	float m_time;
	float m_left;
	float m_right;
	float m_high;
	float m_low;
	float m_destiny;
	int m_num;
	b2Body* m_target;
	std::vector<Bullet*> m_listBullet;
public:
	int m_direction;
	Boss(int ID) : Object(ID) {
		m_current_anim = Idle;
	}
	void SetHP(float hp);
	void SetMaxHP(float maxhp);
	float GetHP();
	void SetSpeed(float speedx, float speedy);
	void UploadSpeed();
	void UploadNum();
	Vector2 GetSpeed();
	void SetDestiny(float destiny);
	float GetDestiny();
	void SetTarget(b2Body* target);
	bool isDie();
	bool isAttack();
	int GetNumOfBullet();

	bool IsMove();
	void UpdateAttack(float deltaTime);
	void Update(float deltaTime);
	void SetLimit(float left, float right);
	void AddBulletID(int id);
	int GetBulletID(int id);
	void AddBullet(Bullet* bullet);
	void SwapGun();
	void SetBodyObject(float positionX, float positionY, b2World* world);

	Bullet* GetBullet();
};