#pragma once
#include "Object.h"
#include "Healthy.h"
class MainCharacter :
    public Object
{
private:
    float m_HP;
	float m_MaxHP;
	b2Vec2 m_PrevPos;
	Healthy* m_whiteHp;
	Healthy* m_redHp;
	bool m_CameraChange;
public:
    MainCharacter(int ID) : Object(ID) {
		m_HP = 0;
		m_PrevPos = b2Vec2(0, 0);
		m_CameraChange = false;
	}
	void SetHP(float hp);
	void SetMaxHP(float hp);
	float GetHP();
	bool isDie();
	void playDead(float deltaTime);
    void Update(float deltaTime);
	void SetBodyObject(float positionX, float positionY, b2World* world);
	void resetAnimation(int type);
	void resetGun();
	void SetHPTexture(Healthy* healthy, bool isWhite = true);
	void DrawHP();
	void cleanHP();
};

