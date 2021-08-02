#pragma once
#include "Object.h"

class Triangle
{
	float x, y, z, m, deltaX = 0;
	Shaders myShaders;
	GLuint iboId, vboId, textureId;
public:
	Triangle(float X, float Y, float Z, float M);
	~Triangle();
	int Init();
	void Draw();
	void CleanUp();
};

