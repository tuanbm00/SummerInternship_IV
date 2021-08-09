#include "stdafx.h"
#include <iostream>
#include "Camera.h"
#include "Globals.h"
#include "define.h"

Camera* Camera::s_Instance = NULL;

Camera::Camera(void)
{
	m_ViewMatrix.SetZero();
	m_WorldMatrix.SetZero();
	m_bIsChange = true;
	i_state = 0;
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
	m_Speed = Move_Speed;
	m_rSpeed = Rotate_Speed;
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_Horizontal = 0;
	m_Vertical = 0;
	m_rHorizontal = 0;
	m_rVertical = 0;
}

void Camera::Update(float deltaTime) {
	CheckMovement();
	if (keyPressed) {
		//Move
		MoveForward(deltaTime);
		//MoveRight(deltaTime);

		//Look
		LookAround(deltaTime);
		LookUp(deltaTime);
		
	}
}


void Camera::LookUp(float deltaTime) {
	if (m_rVertical != 0) {	
		//FPS
		m_bIsChange = true;
		Matrix matR;
		matR.SetRotationX(m_rSpeed * deltaTime * m_rVertical);
		Vector4 Target = Vector4(0, 0, -(m_Position - m_Target).Length(), 1);	//Local Target
		Target = Target * matR;													//New Local Target
		Target = Target * GetWorldMatrix();										//New World Target
		m_Target = Vector3(Target.x, Target.y, Target.z);
	}
}

void Camera::LookAround(float deltaTime) {
	if (m_rHorizontal != 0) {
		m_bIsChange = true;
		Vector4 Target = Vector4(0, 0, -(m_Position - m_Target).Length(), 1);				//Local Target
		Target = Target * RotationMatrixAroundY(-m_rHorizontal * deltaTime * m_rSpeed);		//New Local Target
		Target = Target * GetWorldMatrix();													//New World Target
		m_Target = Vector3(Target.x, Target.y, Target.z);
	}

	
}

Matrix Camera::GetWorldMatrix() {
	Vector3 xaxis, yaxis, zaxis;
	zaxis = (m_Position - m_Target).Normalize();
	xaxis = (m_Up.Cross(zaxis)).Normalize();
	yaxis = (zaxis.Cross(xaxis)).Normalize();
	Matrix RotationMatrix, TranslationMatrix;
	RotationMatrix.m[0][0] = xaxis.x;
	RotationMatrix.m[0][1] = xaxis.y;
	RotationMatrix.m[0][2] = xaxis.z;
	RotationMatrix.m[0][3] = 0;

	RotationMatrix.m[1][0] = yaxis.x;
	RotationMatrix.m[1][1] = yaxis.y;
	RotationMatrix.m[1][2] = yaxis.z;
	RotationMatrix.m[1][3] = 0;

	RotationMatrix.m[2][0] = zaxis.x;
	RotationMatrix.m[2][1] = zaxis.y;
	RotationMatrix.m[2][2] = zaxis.z;
	RotationMatrix.m[2][3] = 0;

	RotationMatrix.m[3][0] = 0;
	RotationMatrix.m[3][1] = 0;
	RotationMatrix.m[3][2] = 0;
	RotationMatrix.m[3][3] = 1;

	TranslationMatrix.m[0][0] = 1;
	TranslationMatrix.m[0][1] = 0;
	TranslationMatrix.m[0][2] = 0;
	TranslationMatrix.m[0][3] = 0;

	TranslationMatrix.m[1][0] = 0;
	TranslationMatrix.m[1][1] = 1;
	TranslationMatrix.m[1][2] = 0;
	TranslationMatrix.m[1][3] = 0;

	TranslationMatrix.m[2][0] = 0;
	TranslationMatrix.m[2][1] = 0;
	TranslationMatrix.m[2][2] = 1;
	TranslationMatrix.m[2][3] = 0;

	TranslationMatrix.m[3][0] = m_Position.x;
	TranslationMatrix.m[3][1] = m_Position.y;
	TranslationMatrix.m[3][2] = m_Position.z;
	TranslationMatrix.m[3][3] = 1;

	return m_WorldMatrix = RotationMatrix * TranslationMatrix;
}

Matrix Camera::GetViewMatrix() {
	if (m_bIsChange) {
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
				
	return m_ViewMatrix;
}

int Camera::getKeyPressed()
{
	return keyPressed;
}

Matrix Camera::GetPerspective() {
	if (m_bIsChangePers) {
		float aspect = Globals::screenWidth / Globals::screenHeight;
		m_Perspective.SetPerspective(m_FOV, aspect, m_Near, m_Far);
		m_bIsChangePers = false;
	}
	return m_Perspective;
}

Matrix Camera::GetOrthographic() {
	Matrix Omatrix;
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
}

void Camera::SetTarget(float X, float Y, float Z) {
	m_Target = Vector3(X, Y, Z);
}

Vector3 Camera::GetTarget() {
	return m_TargetPosition;
}

void Camera::SetPosition(float X, float Y, float Z) {
	m_Position = Vector3(X, Y, Z);
}

void Camera::SetPosition(Vector3 Position) {
	m_Position = Position;
}

Vector3 Camera::GetPosition() {
	return m_Position;
}

void Camera::SetRotation(float X, float Y, float Z) {
	m_Rotation = Vector3(X, Y, Z);
}

void Camera::SetRotation(Vector3 Rotation) {
	m_Rotation = Rotation;
}

Vector3 Camera::GetRotation() {
	return m_Rotation;
}