#pragma once
#include "Object.h"
class MainCharacter :
    public Object
{
private:
    int m_HP;
public:
    MainCharacter(int ID) : Object(ID) {}
    void Update(float deltaTime);
};

