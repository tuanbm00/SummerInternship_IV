#include "MainCharacter.h"
#include "Camera.h"
#include "define.h"

void MainCharacter::Update(float deltaTime)
{
	AnimationType typ = Idle;
	int key = Camera::GetInstance()->getKeyPressed();
	float dis = 500 * deltaTime;
	if (key & MOVE_RIGHT) {
		typ = RunFW;
		m_Position.x += dis;

		Camera::GetInstance()->MoveRight(dis, 1);
	}
	if (key & MOVE_LEFT) {
		typ = RunBW;
		m_Position.x -= dis;

		Camera::GetInstance()->MoveRight(dis, -1);
	}
	if (m_Model->b_IsAnimation == true) {
		m_Model->updateAnimation(deltaTime, typ);
	}
	m_CurrentTime += deltaTime;
	if (m_bIsTarget) {
		SetPosition(Camera::GetInstance()->GetTarget());
	}
	Matrix translationMatrix, scaleMatrix;
	translationMatrix.SetTranslation(m_Position);
	scaleMatrix.SetScale(m_Scale);
	m_WorldMatrix = scaleMatrix * translationMatrix;
	UpdateWVP();
}
