#include "Terrain.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

void Terrain::InitTerrain(char * filename)
{
}

void Terrain::SetBodyObject(float spriteW, float spriteH, b2World* world, bool isTerrain) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(m_Position.x, m_Position.y);
	m_body = world->CreateBody(&bodyDef);
	b2PolygonShape staticBox;
	staticBox.SetAsBox(spriteW / 2, spriteH / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &staticBox;
	if (isTerrain) {
		fixtureDef.filter.categoryBits = CATEGORY_TERRAIN;
		fixtureDef.filter.maskBits = MASK_TERRAIN;
	}
	else {
		fixtureDef.filter.categoryBits = CATEGORY_SLOW_TRAP;
		fixtureDef.filter.maskBits = MASK_SLOW_TRAP;
	}
	m_body->CreateFixture(&fixtureDef);
	isDef = true;
}