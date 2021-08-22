#pragma once
#include "Object.h"

class Healthy :
	public Object
{
private:
	float m_scaleX;
public:
	Healthy(int ID) : Object(ID) {}
	void SetOriginScale(float X, float Y, float Z);
	void SetOriginScale(Vector3 scale);
	float GetScaleX();
};