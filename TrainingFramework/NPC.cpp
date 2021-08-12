#include "NPC.h"
#include "Camera.h"

void NPC::Update(float deltaTime)
{
	AnimationType typ = Idle;
	if (m_Model->b_IsAnimation == true) {
		m_Model->updateAnimation(deltaTime, typ);
	}
	if (m_bIsTarget) {
		SetPosition(Camera::GetInstance()->GetTarget());
	}
	Matrix translationMatrix, scaleMatrix;
	translationMatrix.SetTranslation(m_Position);
	scaleMatrix.SetScale(m_Scale);
	m_WorldMatrix = scaleMatrix * translationMatrix;
	UpdateWVP();
}
