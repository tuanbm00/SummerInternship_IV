#pragma once
#include "Object.h"
#include "define.h"
#include "b2_world.h"
#include "b2_body.h"
#include "b2_fixture.h"
#include "b2_polygon_shape.h"

class Terrain 
{
private:
	int m_iID;
	b2Body * m_body;
	b2Vec2 m_Position;
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef;
	b2PolygonShape staticBox;
public:
	bool m_bIsTerrain;
	Terrain(int ID) {
		m_iID = ID;
		isDef = false;
		m_bIsTerrain = true;
		bodyDef.type = b2_staticBody;
		staticBox.SetAsBox(99, 99);
		fixtureDef.shape = &staticBox;
		fixtureDef.friction = 0;
		bodyDef.fixedRotation = true;
	}
	void SetPosition(float x, float y);
	b2Body * getBody();
	bool isDef;
	void InitTerrain();
	void SetBodyObject(b2World* world);
};