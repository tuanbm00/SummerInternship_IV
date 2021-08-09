#pragma once
#include "Object.h"
class MainCharacter :
    public Object
{
private:
    int m_HP;
public:
	AnimationType m_current_anim;
    MainCharacter(int ID) : Object(ID) {
		m_current_anim = Idle;
		m_HP = 0;
	}
	void SetHP(float hp);
	float GetHP();
	bool isDie();
    void Update(float deltaTime);
	void SetBodyObject(float positionX, float positionY, b2World* world);
};

