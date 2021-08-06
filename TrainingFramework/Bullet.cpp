#include "Bullet.h"
#include "define.h"

void Bullet::Init(int numberOfBullet, float attackSpeed, float speedOfBullet, float maxOfLength) {
	m_NumberOfBullet = numberOfBullet;
	m_AttackSpeed = attackSpeed;
	m_SpeedOfBullet = speedOfBullet;
	m_MaxOfLength = maxOfLength;
	m_CurrentLength = 0;
}

// hàm kiểm tra sẽ được gọi sau khi thực hiện việc bắn đạn nên lượng đạn -1
// nếu súng đã hết đạn -> đổi súng
bool Bullet::IsEmptyBullet() {
	m_NumberOfBullet--;
	if (m_NumberOfBullet > 0) {
		return false;
	}
	return true;
}

bool Bullet::IsOverLength() {
	if (m_CurrentLength < m_MaxOfLength) {
		return false;
	}
	return true;
}

float Bullet::GetAttackSpeed() {
	return m_AttackSpeed;
}

float Bullet::GetSpeedOfBullet() {
	return m_SpeedOfBullet;
}

float Bullet::GetMaxOfLength() {
	return m_MaxOfLength;
}

void Bullet::Update(float deltaTime)
{
	m_CurrentLength += deltaTime * m_SpeedOfBullet;
}

void Bullet::SetBodyObject(float positionX, float positionY, float width, float height, bool isPlayer = true) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 400.0f);
	//m_body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(240.0f, 240.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	if (isPlayer) {
		fixtureDef.filter.categoryBits = CATEGORY_BULLET_PLAYER;
		fixtureDef.filter.maskBits = MASK_BULLET_PLAYER;
	}
	else {
		fixtureDef.filter.categoryBits = CATEGORY_BULLET_ENEMY;
		fixtureDef.filter.maskBits = MASK_BULLET_ENEMY;
	}
	m_body->CreateFixture(&fixtureDef);
	m_body->SetLinearVelocity(b2Vec2(00.0f, -100.0f));
}
