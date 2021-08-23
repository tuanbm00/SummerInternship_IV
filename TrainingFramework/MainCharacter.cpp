#include "MainCharacter.h"
#include "Camera.h"
#include "define.h"


void MainCharacter::SetHP(float hp) {
	m_HP = hp;
	if (hp < 0) {
		m_redHp->SetScale(0);
	}
	else {
		float scalex = m_HP / m_MaxHP * m_redHp->GetScaleX();
		m_redHp->SetScale(scalex);
	}
}

void MainCharacter::SetMaxHP(float maxhp) {
	m_MaxHP = maxhp;
	m_HP = maxhp;
}

float MainCharacter::GetHP() {
	return m_HP;
}

void MainCharacter::Update(float deltaTime)
{
	b2Vec2 pos = m_body->GetPosition();
	
	m_Position.x = pos.x;
	m_Position.y = pos.y;
		
	Vector3 posCamera = Camera::GetInstance()->GetPosition();
	float scalew = 1250;
	float scaleh = 900;

	m_whiteHp->SetPosition(posCamera.x - scalew, posCamera.y - scaleh, posCamera.z);
	m_redHp->SetPosition(posCamera.x - scalew, posCamera.y - scaleh, m_Position.z);

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

void MainCharacter::SetHPTexture(Healthy* healthy, bool isWhite) {
	if (isWhite) {
		m_whiteHp = healthy;
	}
	else {
		m_redHp = healthy;
	}
}

void MainCharacter::DrawHP() {
	m_whiteHp->Draw();
	m_redHp->Draw();
}