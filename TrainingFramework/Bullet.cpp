#include "Bullet.h"
#include "define.h"

void Bullet::Init(int numberOfBullet, float attackDame ,float attackSpeed, float speedOfBulletX, float speedOfBulletY, float maxOfLength) {
	m_NumberOfBullet = numberOfBullet;
	m_AttackDame = attackDame;
	m_AttackSpeed = attackSpeed;
	m_SpeedOfBulletX = speedOfBulletX;
	m_SpeedOfBulletY = speedOfBulletY;
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

float Bullet::GetAttackDame() {
	return m_AttackDame;
}

float Bullet::GetAttackSpeed() {
	return m_AttackSpeed;
}

Vector2 Bullet::GetSpeedOfBullet() {
	return Vector2(m_SpeedOfBulletX, m_SpeedOfBulletY);
}

float Bullet::GetMaxOfLength() {
	return m_MaxOfLength;
}

void Bullet::Update(float deltaTime)
{
	if (m_ObjectID == CATEGORY_BAZOKA) {
		b2Vec2 v = m_body->GetLinearVelocity();
		m_body->SetLinearVelocity(b2Vec2(v.x, v.y + 9.8*deltaTime));
	}
	m_CurrentLength += deltaTime * m_SpeedOfBulletX;
	m_Position.x = m_body->GetPosition().x;
	m_Position.y = m_body->GetPosition().y;
}

void Bullet::SetBodyObject(float positionX, float positionY, b2World* world, bool isPlayer = true) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(positionX, positionY);
	m_body = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_spriteW, m_spriteH);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	if (isPlayer) {
		fixtureDef.filter.categoryBits = CATEGORY_BULLET_PLAYER;
		fixtureDef.filter.maskBits = MASK_BULLET_PLAYER;
	}
	else {
		fixtureDef.filter.categoryBits = CATEGORY_BULLET_ENEMY;
		fixtureDef.filter.maskBits = MASK_BULLET_ENEMY;
	}
	m_body->CreateFixture(&fixtureDef);
	m_body->SetLinearVelocity(b2Vec2(m_SpeedOfBulletX, m_SpeedOfBulletY));
}
