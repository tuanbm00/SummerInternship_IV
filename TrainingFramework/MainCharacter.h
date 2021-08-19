#pragma once
#include "Object.h"
class MainCharacter :
    public Object
{
private:
    float m_HP;
public:
    MainCharacter(int ID) : Object(ID) {
		m_HP = 0;
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

