#pragma once
#include "Object.h"
#include "define.h"
#include "b2_world.h"
#include "b2_body.h"
#include "b2_polygon_shape.h"

class Terrain 
{
private:
	int m_iID;
	b2Body * m_body;
	b2Vec2 m_Position;
public:
	bool m_bIsTerrain;
	Terrain(int ID) {
		m_iID = ID;
		isDef = false;
		m_bIsTerrain = true;
	}
	void SetPosition(float x, float y);
	b2Body * getBody();
	bool isDef;
	void InitTerrain(char *filename);
	void SetBodyObject(float spriteW, float spriteH, b2World* world);
};