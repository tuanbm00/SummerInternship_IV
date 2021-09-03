#include "stdafx.h"
#include "Terrain.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

void Terrain::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

b2Body * Terrain::getBody()
{
	return m_body;
}

void Terrain::InitTerrain()
{
	bodyDef.position.Set(m_Position.x, m_Position.y);
	if (m_bIsTerrain) {
		fixtureDef.filter.categoryBits = CATEGORY_TERRAIN;
		fixtureDef.filter.maskBits = MASK_TERRAIN;
	}
	else {
		fixtureDef.filter.categoryBits = CATEGORY_SLOW_TRAP;
		fixtureDef.filter.maskBits = MASK_SLOW_TRAP;
	}
}

void Terrain::SetBodyObject(b2World* world) {
	m_body = world->CreateBody(&bodyDef);
	m_body->CreateFixture(&fixtureDef);
}