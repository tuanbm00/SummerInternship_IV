#include "Enemy.h"
#include "Camera.h"
#include "define.h"

void Enemy::SetHP(float hp) {
	m_HP = hp;
}

float Enemy::GetHP() {
	return m_HP;
}

void Enemy::SetSpeed(float speedx, float speedy) {
	m_speedx = speedx;
	m_speedy = speedy;
}

Vector2 Enemy::GetSpeed() {
	return Vector2(m_speedx, m_speedy);
}

void Enemy::UpdateAttack(float deltaTime) {
	m_time += deltaTime;
}

void Enemy::Update(float deltaTime)
{
	m_Position.x = m_body->GetPosition().x;
	m_Position.y = m_body->GetPosition().y;

	if (m_Position.x <= m_left || m_Position.x >= m_right) {
		m_body->SetLinearVelocity(b2Vec2(-m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y));
	}

	if (m_Model->b_IsAnimation == true) {
		m_Model->updateAnimation(deltaTime, Idle);
	}
	Matrix translationMatrix, scaleMatrix;
	translationMatrix.SetTranslation(m_Position);
	scaleMatrix.SetScale(m_Scale);
	m_WorldMatrix = scaleMatrix * translationMatrix;
}

void Enemy::SetLimit(float left, float right) {
	m_left = left;
	m_right = right;
}

bool Enemy::isDie() {
	if (m_HP <= 0) {
		return true;
	}
	return false;
}

bool Enemy::isAttack() {
	if (m_time >= m_bullet->GetAttackSpeed()) {
		m_time = 0;
		return true;
	}
	return false;
}

void Enemy::SetBullet(Bullet* bullet) {
	m_time = 0;
	m_bullet = bullet;
}

void Enemy::SetBodyObject(float positionX, float positionY, b2World* world) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(positionX, positionY);
	m_body = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_spriteW/2, m_spriteH/2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 0;
//	fixtureDef.density = 1;
	fixtureDef.filter.categoryBits = CATEGORY_ENEMY;
	fixtureDef.filter.maskBits = MASK_ENEMY;
	m_body->CreateFixture(&fixtureDef);
	m_body->SetLinearVelocity(b2Vec2(m_speedx, m_speedy));
}
bool Enemy::checkDraw() {
	Vector3 pos = Camera::GetInstance()->GetPosition();
	if (m_Position.x <= (pos.x - 1280) || m_Position.x >= (pos.x + 1280)) return false;
	if (m_Position.y <= (pos.y - 960) || m_Position.y >= (pos.y + 960)) return false;
	return true;
}

Bullet* Enemy::GetBullet() {
	return m_bullet;
}