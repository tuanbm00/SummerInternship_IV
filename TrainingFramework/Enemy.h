#pragma once
#include "Object.h"
class Enemy :
	public Object
{
private:
	int m_HP;
public:
	Enemy(int ID) : Object(ID) {}
	void SetHP(float hp);
	float GetHP();
	void Update(float deltaTime);
	void SetBodyObject(float positionX, float positionY, b2World* world);
};