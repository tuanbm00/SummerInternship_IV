#pragma once
#include "Object.h"
class MainCharacter :
    public Object
{
private:
    int m_HP;
public:
    MainCharacter(int ID) : Object(ID) {}
	void SetHP(float hp);
	float GetHP();
	bool isDie();
    void Update(float deltaTime);
	void SetBodyObject(float positionX, float positionY, b2World* world);
};

