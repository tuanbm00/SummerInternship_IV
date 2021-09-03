#include "stdafx.h"
#include "Healthy.h"

void Healthy::SetOriginScale(float x, float y, float z) {
	m_scaleX = x;
	m_Scale = Vector3(x, y, z);
}

void Healthy::SetOriginScale(Vector3 scale) {
	m_scaleX = scale.x;
	m_Scale = Vector3(scale);
}

float Healthy::GetScaleX() {
	return m_scaleX;
}