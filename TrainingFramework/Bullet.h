#pragma once
#include "Object.h"
class Bullet :
    public Object
{
public:
    Bullet(int ID) : Object(ID) {}
    void Update(float deltaTime);
private:
    float m_bullet_speed;
    float m_bullet_distance;
};

