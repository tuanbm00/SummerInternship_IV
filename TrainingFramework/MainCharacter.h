#pragma once
#include "Object.h"
class MainCharacter :
    public Object
{
private:
    float m_HP;
	b2Vec2 m_PrevPos;
	bool m_CameraChange;
public:
    MainCharacter(int ID) : Object(ID) {
		m_HP = 0;
		m_PrevPos = b2Vec2(0, 0);
		m_CameraChange = false;
	}
	void SetHP(float hp);
	float GetHP();
	bool isDie();
	void playDead(float deltaTime);
    void Update(float deltaTime);
	void SetBodyObject(float positionX, float positionY, b2World* world);
	void resetAnimation(int type);
	void resetGun();

};

