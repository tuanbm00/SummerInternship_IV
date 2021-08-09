#include "MainCharacter.h"
#include "Camera.h"
#include "define.h"
<<<<<<< Updated upstream

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
	Camera::GetInstance()->SetTarget(m_Position.x, m_Position.y, 0.0f);
}

bool MainCharacter::isDie() {
	if (m_HP <= 0) {
		return true;
	}
	return false;
}


void MainCharacter::SetBodyObject(float positionX, float positionY, b2World* world) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(positionX, positionY);
	m_body = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_spriteW, m_spriteH);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	fixtureDef.filter.maskBits = MASK_PLAYER;
	m_body->CreateFixture(&fixtureDef);
=======

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
	Camera::GetInstance()->SetPosition(m_Position.x, m_Position.y, 1.0f);
	Camera::GetInstance()->SetTarget(m_Position.x, m_Position.y, 0.0f);
	if (m_Model->b_IsAnimation == true) {
		m_Model->updateAnimation(deltaTime, Idle);
	}
	m_CurrentTime += deltaTime;
	Matrix translationMatrix, scaleMatrix;
	translationMatrix.SetTranslation(m_Position);
	scaleMatrix.SetScale(m_Scale);
	m_WorldMatrix = scaleMatrix * translationMatrix;
	UpdateWVP();
>>>>>>> Stashed changes
}

bool MainCharacter::isDie() {
	if (m_HP <= 0) {
		return true;
	}
	return false;
}


void MainCharacter::SetBodyObject(float positionX, float positionY, b2World* world) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(positionX, positionY);
	m_body = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_spriteW, m_spriteH);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.filter.categoryBits = CATEGORY_PLAYER;
	fixtureDef.filter.maskBits = MASK_PLAYER;
	m_body->CreateFixture(&fixtureDef);
}
