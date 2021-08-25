#include "stdafx.h"
#include <iostream>
#include "Camera.h"
#include "Globals.h"
#include "define.h"
#define CAM_MAX_SPEED 600

Camera* Camera::s_Instance = NULL;

Camera::Camera(void)
{
	m_ViewMatrix.SetZero();
	m_bIsChange = false;
	i_state = 0;
	is_shoot = false;
	is_wound = false;
	is_dead = false;
}

Camera* Camera::GetInstance()
{
	if (!s_Instance)
		s_Instance = new Camera();
	return s_Instance;
}


Camera::~Camera() {

}

void Camera::Init(float FOV, float Near, float Far, float Move_Speed, float Rotate_Speed) {
	m_FOV = FOV;
	m_Near = Near;
	m_Far = Far;
	m_fVelocityX = 1.0f;
	m_fVelocityY = 200.0f;
	initOrtho();
}

void Camera::Update(float deltaTime, float posX, float posY,int direction) {
	//posX += 500 * direction;
	//if (checkRect(posX, posY)) return;
	m_Position.x += (posX - m_Position.x)*deltaTime;
	m_Position.y += (posY - m_Position.y)*deltaTime;
	if (m_Position.x - 1280 < limitX.x) m_Position.x = limitX.x + 1280;
	if (m_Position.x + 1280 > limitX.y) m_Position.x = limitX.y-1280;
	if (m_Position.y - 600 < limitY.x) m_Position.y = limitY.x + 600;
	if (m_Position.y + 1000 > limitY.y) m_Position.y = limitY.y - 1000;
	updateView(m_Position.x, m_Position.y);
}

Matrix Camera::GetViewMatrix() {
	return m_ViewMatrix;
}

Matrix Camera::GetOrthographic() {
	return Omatrix;
}

void Camera::CleanUp() {
	if (s_Instance)
	{
		delete s_Instance;
		s_Instance = NULL;
	}
}

void Camera::SetTarget(Vector3 Target) {
	m_Target = Target;
	m_TargetPosition = Target;
	initView();
}

void Camera::SetTarget(float X, float Y, float Z) {
	m_Target = Vector3(X, Y, Z);
}

Vector3 Camera::GetTarget() {
	return m_TargetPosition;
}

bool Camera::checkRect(float posx, float posy)
{
	if (posx < m_Position.x - 600 || posx > m_Position.x + 600) return false;
	if (posy < m_Position.y - 600 || posy > m_Position.y + 600) return false;

	return true;
}

void Camera::setLimitX(float min, float max)
{
	limitX.x = min; limitX.y = max;
}

void Camera::setLimitY(float min, float max)
{
	limitY.x = min; limitY.y = max;
}

void Camera::initView()
{
	m_ViewMatrix.m[0][0] = 1;
	m_ViewMatrix.m[0][1] = 0;
	m_ViewMatrix.m[0][2] = 0;
	m_ViewMatrix.m[0][3] = 0;

	m_ViewMatrix.m[1][0] = 0;
	m_ViewMatrix.m[1][1] = 1;
	m_ViewMatrix.m[1][2] = 0;
	m_ViewMatrix.m[1][3] = 0;

	m_ViewMatrix.m[2][0] = 0;
	m_ViewMatrix.m[2][1] = 0;
	m_ViewMatrix.m[2][2] = 1;
	m_ViewMatrix.m[2][3] = 0;

	m_ViewMatrix.m[3][0] = -m_Position.x;
	m_ViewMatrix.m[3][1] = -m_Position.y;
	m_ViewMatrix.m[3][2] = -m_Position.z;
	m_ViewMatrix.m[3][3] = 1;
	m_ViewMatrix = m_ViewMatrix * Omatrix;
}

void Camera::initOrtho()
{
	float left = -960.0f;
	float right = 960.0f;
	float top = -720.0f;
	float bottom = 720.0f;
	float cfar = m_Far, cnear = m_Near;
	Omatrix.m[0][0] = 2 / (right - left);
	Omatrix.m[0][1] = 0;
	Omatrix.m[0][2] = 0;
	Omatrix.m[0][3] = -(right + left) / (right - left);

	Omatrix.m[1][0] = 0;
	Omatrix.m[1][1] = 2 / (top - bottom);
	Omatrix.m[1][2] = 0;
	Omatrix.m[1][3] = -(top + bottom) / (top - bottom);

	Omatrix.m[2][0] = 0;
	Omatrix.m[2][1] = 0;
	Omatrix.m[2][2] = -2 / (cfar - cnear);
	Omatrix.m[2][3] = -(cfar + cnear) / (cfar - cnear);

	Omatrix.m[3][0] = 0;
	Omatrix.m[3][1] = 0;
	Omatrix.m[3][2] = 0;
	Omatrix.m[3][3] = 1;
}

void Camera::updateView(float x, float y)
{
	m_ViewMatrix.m[3][0] = -x * Omatrix.m[0][0];
	m_ViewMatrix.m[3][1] = -y * Omatrix.m[1][1];
}

void Camera::SetPosition(float X, float Y, float Z) {
	m_Position = Vector3(X, Y, Z);
}

void Camera::SetPosition(float X, float Y)
{
	m_Position.x = X;
	m_Position.y = Y;
}

void Camera::SetPosition(Vector3 Position) {
	m_Position = Position;
}

Vector3 Camera::GetPosition() {
	return m_Position;
}