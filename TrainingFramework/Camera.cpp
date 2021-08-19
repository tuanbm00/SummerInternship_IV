#include "stdafx.h"
#include <iostream>
#include "Camera.h"
#include "Globals.h"
#include "define.h"

Camera* Camera::s_Instance = NULL;

Camera::Camera(void)
{
	m_ViewMatrix.SetZero();
	m_bIsChange = true;
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
	initOrtho();
	initView();
}

void Camera::Update(float deltaTime) {

	
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
	initView();
}

Vector3 Camera::GetTarget() {
	return m_TargetPosition;
}

void Camera::initView()
{
	Vector3 xaxis, yaxis, zaxis;
	zaxis = (m_Position - m_Target).Normalize();
	xaxis = (m_Up.Cross(zaxis)).Normalize();
	yaxis = (zaxis.Cross(xaxis)).Normalize();

	m_ViewMatrix.m[0][0] = xaxis.x;
	m_ViewMatrix.m[0][1] = yaxis.x;
	m_ViewMatrix.m[0][2] = zaxis.x;
	m_ViewMatrix.m[0][3] = 0;

	m_ViewMatrix.m[1][0] = xaxis.y;
	m_ViewMatrix.m[1][1] = yaxis.y;
	m_ViewMatrix.m[1][2] = zaxis.y;
	m_ViewMatrix.m[1][3] = 0;

	m_ViewMatrix.m[2][0] = xaxis.z;
	m_ViewMatrix.m[2][1] = yaxis.z;
	m_ViewMatrix.m[2][2] = zaxis.z;
	m_ViewMatrix.m[2][3] = 0;

	m_ViewMatrix.m[3][0] = -m_Position.Dot(xaxis);
	m_ViewMatrix.m[3][1] = -m_Position.Dot(yaxis);
	m_ViewMatrix.m[3][2] = -m_Position.Dot(zaxis);
	m_ViewMatrix.m[3][3] = 1;
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