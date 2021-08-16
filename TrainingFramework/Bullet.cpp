#include "Bullet.h"
#include "define.h"


void Bullet::InitA(float attackDame ,float attackSpeed, float speedOfBulletX, float speedOfBulletY, float maxOfLength) {
	m_isChange = 0;
	m_NumberOfBullet = NUM_BULLET;
	m_AttackDame = attackDame;
	m_AttackSpeed = attackSpeed;
	m_SpeedOfBulletX = speedOfBulletX;
	m_SpeedOfBulletY = speedOfBulletY;
	m_MaxOfLength = maxOfLength;
	m_CurrentLength = 0;
}

void Bullet::ResetBullet() {
	m_NumberOfBullet = NUM_BULLET;
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

void Bullet::SetCurrLength(float curr) {
	m_CurrentLength = curr;
}

float Bullet::GetCurrLength() {
	return m_CurrentLength;
}

float Bullet::GetMaxOfLength() {
	return m_MaxOfLength;
}

void Bullet::SetIsChange() {
	m_isChange = 1;
}

bool Bullet::IsChange() {
	if (m_isChange == 0) {
		return false;
	}
	m_isChange = 0;
	return true;
}

void Bullet::ReverseV() {
	m_current_anim = -m_current_anim;
	m_SpeedOfBulletX = -m_SpeedOfBulletX;
	m_body->SetLinearVelocity(b2Vec2(m_SpeedOfBulletX, m_SpeedOfBulletY));
}

void Bullet::Update(float deltaTime)
{
	if (m_ObjectID == CATEGORY_BAZOKA) {
		b2Vec2 v = m_body->GetLinearVelocity();
		m_body->SetLinearVelocity(b2Vec2(v.x, v.y + 9.8*deltaTime*0.2));
	}
	float dir = m_SpeedOfBulletX > 0 ? 1 : -1;
	m_CurrentLength += (m_body->GetPosition().x  - m_Position.x) * dir;
	m_Position.x = m_body->GetPosition().x;
	m_Position.y = m_body->GetPosition().y;
	if (m_Model->b_IsAnimation == true) {
		m_Model->updateAnimation(deltaTime, m_current_anim);
	}
	Matrix translation, scale;
	translation.SetTranslation(m_Position);
	scale.SetScale(m_Scale);
	m_WorldMatrix = scale * translation;
}


void Bullet::SetBodyObject(float positionX, float positionY, b2World* world, bool isPlayer) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(positionX, positionY);
	m_body = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(m_spriteW, m_spriteH);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = m_AttackDame;
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
