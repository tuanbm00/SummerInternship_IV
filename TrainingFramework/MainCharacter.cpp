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
	m_Position.x = m_body->GetPosition().x;
	m_Position.y = m_body->GetPosition().y;
	Vector3 camPos = Camera::GetInstance()->GetPosition();
	int Y = camPos.y-m_Position.y;
	camPos.x = m_Position.x;
	if (Y > 500) camPos.y = m_Position.y + 500;
	if (Y < -500) camPos.y = m_Position.y - 500;
	Camera::GetInstance()->SetPosition(camPos);
	Camera::GetInstance()->SetTarget(camPos.x, camPos.y, 0.0f);
	
	Matrix translation, scale;
	translation.SetTranslation(m_Position);
	scale.SetScale(m_Scale);
	m_WorldMatrix = scale * translation;
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
	dynamicBox.SetAsBox(m_spriteW / 2.5, m_spriteH / 2);
	b2FixtureDef fixtureDef;
	float area = m_spriteW * m_spriteH * 4.0f;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	fixtureDef.filter.maskBits = MASK_PLAYER;
	fixtureDef.density = 6000.0f / area;
	m_body->CreateFixture(&fixtureDef);
}
void MainCharacter::resetAnimation(int type) {
	m_Model->getAnimation(type - 1)->resetAnimation();
}
void MainCharacter::resetGun() {
	m_Model->resetGun();
}
