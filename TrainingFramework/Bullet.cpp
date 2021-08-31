#include "Bullet.h"
#include "define.h"
#include "Camera.h"


void Bullet::InitA(float attackDame ,float attackSpeed, float speedOfBulletX, float speedOfBulletY, float maxOfLength) {
	m_isChange = 0;
	m_NumberOfBullet = NUM_BULLET;
	m_AttackDame = attackDame;
	m_AttackSpeed = attackSpeed;
	m_SpeedOfBulletX = speedOfBulletX;
	m_SpeedOfBulletY = speedOfBulletY;
	m_MaxOfLength = maxOfLength;
	m_CurrentLength = 0;
	m_target = NULL;
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

void Bullet::UpdateAnimation(float deltaTime)
{
	if (m_Model->m_iUpdateFase != Camera::GetInstance()->m_iUpdateFase) {
		m_Model->m_iUpdateFase = Camera::GetInstance()->m_iUpdateFase;
		Object::UpdateAnimation(deltaTime);
	}
}

float Bullet::GetMaxOfLength() {
	return m_MaxOfLength;
}

void Bullet::SetOldPos(float oldpos) {
	m_OldPos = oldpos;
}

float Bullet::GetOldPos() {
	return m_OldPos;
}

void Bullet::SetLengthBoomerang(float length) {
	m_lengthBoomerang = length;
}

float Bullet::GetLengthBoomerang() {
	return m_lengthBoomerang;
}

void Bullet::SetIsChange() {
	m_isChange = 1;
}

void Bullet::SetTarget(b2Body* target) {
	m_target = target;
}

b2Body* Bullet::GetTarget() {
	return m_target;
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
		if (!m_target) {
			m_body->SetLinearVelocity(b2Vec2(v.x, v.y + 9.8*deltaTime*0.2));
		}
		else {
			float dir = m_target->GetPosition().x > m_body->GetPosition().x ? 1.0f : -1.0f;
			v.x = v.x > 0 ? v.x : -v.x;
			if ((m_body->GetPosition().x == m_target->GetPosition().x)) {
				m_body->SetLinearVelocity(b2Vec2(m_SpeedOfBulletX, m_SpeedOfBulletY));
			}
			else {
				float scale = (m_body->GetPosition().y - m_target->GetPosition().y + 30.0f) / (m_body->GetPosition().x - m_target->GetPosition().x);
				m_body->SetLinearVelocity(b2Vec2(v.x * dir, v.x * scale * dir));
			}
		}
	}
	else if (m_ObjectID == CATEGORY_BAZOKA_ENEMY) {
		b2Vec2 v = m_body->GetLinearVelocity();
		m_body->SetLinearVelocity(b2Vec2(v.x, v.y + 9.8*deltaTime*0.2));
	}
	else {
		m_body->SetLinearVelocity(b2Vec2(m_SpeedOfBulletX, m_SpeedOfBulletY));
	}
	float dir = m_SpeedOfBulletX > 0 ? 1 : -1;
	m_CurrentLength += (m_body->GetPosition().x  - m_Position.x) * dir;
	m_Position.x = m_body->GetPosition().x;
	m_Position.y = m_body->GetPosition().y;
	
	UpdateWorld();
}


void Bullet::SetBodyObject(float positionX, float positionY, b2World* world, bool isPlayer, bool notBoss) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(positionX, positionY);
	m_body = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	if (notBoss) {
		dynamicBox.SetAsBox(m_spriteW*0.35, m_spriteH*0.35);
	}
	else {
		dynamicBox.SetAsBox(m_spriteW*5, m_spriteH*5);
	}
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = m_AttackDame;
	if (isPlayer) {
		fixtureDef.filter.categoryBits = CATEGORY_BULLET_PLAYER;
		fixtureDef.filter.maskBits = MASK_BULLET_PLAYER;
		fixtureDef.filter.groupIndex = -1;
	}
	else {
		if (notBoss) {
			fixtureDef.filter.categoryBits = CATEGORY_BULLET_ENEMY;
			fixtureDef.filter.maskBits = MASK_BULLET_ENEMY;
		}
		else {
			fixtureDef.filter.categoryBits = CATEGORY_BULLET_BOSS;
			fixtureDef.filter.maskBits = MASK_BULLET_BOSS;
		}
		fixtureDef.filter.groupIndex = -2;
	}
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
	m_body->CreateFixture(&fixtureDef);
	m_body->SetLinearVelocity(b2Vec2(m_SpeedOfBulletX, m_SpeedOfBulletY));
}
