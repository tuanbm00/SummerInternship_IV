#include "MainCharacter.h"
#include "Camera.h"
#include "define.h"


void MainCharacter::SetHP(float hp) {
	m_HP = hp;
}

float MainCharacter::GetHP() {
	return m_HP;
}

void MainCharacter::Update(float deltaTime)
{
	b2Vec2 pos = m_body->GetPosition();
	
	m_Position.x = pos.x;
	m_Position.y = pos.y;
		
	UpdateWorld();
}

bool MainCharacter::isDie() {
	if (m_HP <= 0) {
		return true;
	}
	return false;
}

void MainCharacter::playDead(float deltaTime) {
	m_Model->updateDead(deltaTime);
}


void MainCharacter::SetBodyObject(float positionX, float positionY, b2World* world) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(positionX, positionY);
	m_body = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_spriteW / 2.5f, m_spriteH / 2.0f);
	b2FixtureDef fixtureDef;
	float area = m_spriteW * m_spriteH * 4.0f;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	fixtureDef.filter.maskBits = MASK_PLAYER;
	fixtureDef.filter.groupIndex = -1;
	fixtureDef.density = 6000.0f / area;
	m_body->CreateFixture(&fixtureDef);
}
void MainCharacter::resetAnimation(int type) {
	m_Model->getAnimation(type - 1)->resetAnimation();
}
void MainCharacter::resetGun() {
	m_Model->resetGun();
}
