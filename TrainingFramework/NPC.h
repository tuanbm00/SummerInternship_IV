#pragma once
#include "Object.h"
class NPC :
    public Object
{
public:
    NPC(int ID) : Object(ID) {}
    void Update(float deltaTime);
private:
    
};

