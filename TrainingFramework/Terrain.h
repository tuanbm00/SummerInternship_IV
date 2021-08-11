#pragma once
#include "Object.h"
#include "define.h"

class Terrain : 
	public Object 
{
private:

public:
	Terrain(int ID) : Object(ID) {}

	void InitTerrain(char *filename);
	void SetBodyObject(float spriteW, float spriteH, b2World* world);
};