#include "Enemy.h"
#include "Camera.h"
#include "define.h"

void Enemy::SetHP(float hp) {
	m_HP = hp;
}

float Enemy::GetHP() {
	return m_HP;
}

void Enemy::Update(float deltaTime)
{
	m_Position.x = m_body->GetPosition().x;
	m_Position.y = m_body->GetPosition().y;
<<<<<<< Updated upstream
=======
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

bool Enemy::isDie() {
	if (m_HP <= 0) {
		return true;
	}
	return false;
}

void Enemy::SetBodyObject(float positionX, float positionY, b2World* world) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(positionX, positionY);
	m_body = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_spriteW, m_spriteH);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = m_attackDame;
	fixtureDef.filter.categoryBits = CATEGORY_ENEMY;
	fixtureDef.filter.maskBits = MASK_ENEMY;
	m_body->CreateFixture(&fixtureDef);
}
