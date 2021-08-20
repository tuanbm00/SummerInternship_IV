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

void Enemy::SetBulletID(int bulletID) {
	m_bulletID = bulletID;
}

void Enemy::setTransBox(float b1, float b2) {
	transPosBox.x = b1;
	transPosBox.y = b2;
}

int Enemy::GetBulletID() {
	return m_bulletID;
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
	if (m_left == m_right) m_body->SetLinearVelocity(b2Vec2(0, 0));
	
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
	float scale = m_ObjectID == 4 ? 0.25 : 1;
	if (m_time >= m_bullet->GetAttackSpeed() * scale) {
		return true;
	}
	return false;
}

void Enemy::SetBullet(Bullet* bullet) {
	m_time = 0;
	m_bullet = bullet;
}

void Enemy::SetBodyObject(b2World* world) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(m_Position.x + transPosBox.x, m_Position.y+transPosBox.y);
	m_body = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_spriteW/(2.0f), m_spriteH/(2.0f));
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 0;
//	fixtureDef.density = 1;
	fixtureDef.filter.categoryBits = CATEGORY_ENEMY;
	fixtureDef.filter.maskBits = MASK_ENEMY;
	fixtureDef.filter.groupIndex = -2;
	m_body->CreateFixture(&fixtureDef);
	m_body->SetLinearVelocity(b2Vec2(m_speedx, m_speedy));
}

Bullet* Enemy::GetBullet() {
	return m_bullet;
}

void Enemy::UpdateAnimation(float deltaTime) {
	if (m_current_anim == 2 || m_left == m_right) m_current_anim = abs(m_current_anim)*m_direction;
	else {
		m_current_anim = abs(m_current_anim) * ((m_body->GetLinearVelocity().x > 0) ? 1 : -1);
	}
	if (m_Model->b_IsAnimation == true) {
		m_Model->updateAnimation(deltaTime, m_current_anim);
	}
}