#include "stdafx.h"
#include <iostream>
#include "Camera.h"
#include "Globals.h"
#include "defines.h"

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

void Camera::CheckMovement() {
	if (keyPressed & MOVE_FORWARD) {
		m_Vertical = 1;
	}
	if (keyPressed & MOVE_BACKWARD) {
		m_Vertical = -1;
	}
	
	if (keyPressed & ROTATE_UP) {
		m_rVertical = 1;
	}
	if (keyPressed & ROTATE_DOWN) {
		m_rVertical = -1;
	}
	if (keyPressed & ROTATE_RIGHT) {
		m_rHorizontal = 1;
	}
	if (keyPressed & ROTATE_LEFT) {
		m_rHorizontal = -1;
	}
}

void Camera::Key(unsigned char key, bool bIsPressed) {
	if (bIsPressed) {
		switch (key)
		{
		case 'W':
		case 'w':
			keyPressed = keyPressed | MOVE_FORWARD;
			break;
		case 'S':
		case 's':
			keyPressed = keyPressed | MOVE_BACKWARD;
			break;
		case 'A':
		case 'a':
			keyPressed = keyPressed | MOVE_LEFT;
			break;
		case 'D':
		case 'd':
			keyPressed = keyPressed | MOVE_RIGHT;
			break;
		case 'K':
		case 'k':
			m_TargetPosition = m_Target;
			break;

		case LEFT_ARROW:
			keyPressed = keyPressed | ROTATE_LEFT;
			break;
		case UP_ARROW:
			keyPressed = keyPressed | ROTATE_UP;
			break;
		case RIGHT_ARROW:
			
			keyPressed = keyPressed | ROTATE_RIGHT;
			break;
		case DOWN_ARROW:
			keyPressed = keyPressed | ROTATE_DOWN;
			break;
		}
	}
	else {
		switch (key)
		{
		case 'W':
		case 'w':
			keyPressed = keyPressed ^ MOVE_FORWARD;
			m_Vertical = 0;
			break;
		case 'S':
		case 's':
			keyPressed = keyPressed ^ MOVE_BACKWARD;
			m_Vertical = 0;
			break;
		case 'A':
		case 'a':
			keyPressed = keyPressed ^ MOVE_LEFT;
			m_Horizontal = 0;
			break;
		case 'D':
		case 'd':
			keyPressed = keyPressed ^ MOVE_RIGHT;
			m_Horizontal = 0;
			break;

		case LEFT_ARROW:
			keyPressed = keyPressed ^ ROTATE_LEFT;
			m_rHorizontal = 0;
			break;
		case UP_ARROW:
			keyPressed = keyPressed ^ ROTATE_UP;
			m_rVertical = 0;
			break;					
		case RIGHT_ARROW:			
			keyPressed = keyPressed ^ ROTATE_RIGHT;
			m_rHorizontal = 0;
			break;					
		case DOWN_ARROW:			
			keyPressed = keyPressed ^ ROTATE_DOWN;
			m_rVertical = 0;
			break;
		}
	}
}

void Camera::MoveRight(float dis, int r) {
	m_Position.x += dis*r;
	m_Target.x = m_Position.x;
}

void Camera::MoveForward(float deltaTime) {
	if (m_Vertical != 0) {
		m_bIsChange = true;
		Vector3 deltaMove = -(m_Position - m_Target).Normalize() * m_Speed * deltaTime * m_Vertical;
		m_Position += deltaMove;
		m_Target += deltaMove;

		//TPS
		//m_TargetPosition += deltaMove;
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

		//TPS
		//m_Position += deltaMove;
		//m_TargetPosition += deltaMove;

	}
}

void Camera::LookAround(float deltaTime) {
	if (m_rHorizontal != 0) {
		m_bIsChange = true;
		Vector4 Target = Vector4(0, 0, -(m_Position - m_Target).Length(), 1);				//Local Target
		Target = Target * RotationMatrixAroundY(-m_rHorizontal * deltaTime * m_rSpeed);		//New Local Target
		Target = Target * GetWorldMatrix();													//New World Target
		m_Target = Vector3(Target.x, Target.y, Target.z);


		//FPS - 2
		//Vector3 deltaMove = (m_Up.Cross((m_Position - m_Target).Normalize())).Normalize() * m_rSpeed * 10 * deltaTime * m_rHorizontal;
		//m_Target += deltaMove;

		//TPS
		//m_Position += deltaMove;
		//m_TargetPosition = m_Target;
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


	//Return CameraLocalView
		//Camera Local Space
	/*Matrix r1 = Ry.SetRotationY(-m_Rotation.y * float(PI / 180.0f)) * Rx.SetRotationX(-m_Rotation.x * float(PI / 180.0f)) * Rz.SetRotationZ(-m_Rotation.z * float(PI / 180.0f)) ;
	Matrix t1;
	t1.SetTranslation(-m_Position.x, -m_Position.y, -m_Position.z);

	Matrix m_iWorldMatrix;
	m_iWorldMatrix = t1 * r1;*/
	//return m_iWorldMatrix;
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

Matrix Camera::RotationMatrixAroundY(float Angle) {
	Matrix rotation;
	Vector4 rotateAxis = Vector4(0, 1, 0, 0);
	rotateAxis = rotateAxis * GetViewMatrix();
	return rotation.SetRotationAngleAxis(Angle, rotateAxis.x, rotateAxis.y, rotateAxis.z);
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

void Camera::SetHorizontal(float H) {
	m_Horizontal = H;
}

void Camera::SetVertical(float V) {
	m_Vertical = V;
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