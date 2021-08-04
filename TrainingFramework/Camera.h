#pragma once

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Math.h"


class Camera
{
private:
	static Camera* s_Instance;
	float m_FOV, m_Near, m_Far;
	bool m_bIsChange;
	bool m_bIsChangePers;
	
	Vector3 m_Up;
	Vector3 m_Target;
	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_TargetPosition;

	Matrix m_WorldMatrix;
	Matrix m_ViewMatrix;
	Matrix m_Perspective;

	int keyPressed = 0;
public:
	Camera();
	~Camera();
	float m_Horizontal, m_Vertical, m_Speed = 1.0f, m_rSpeed = 1.0f;
	float m_rHorizontal, m_rVertical;
	static Camera* GetInstance();
	int i_state;
	int getKeyPressed();
	Matrix GetPerspective();
	Matrix GetOrthographic();
	Matrix GetViewMatrix();

	void Init(float FOV, float Near, float Far, float Move_Speed, float Rotate_Speed);
	void Update(float deltaTime);
	void CleanUp();

	void Key(unsigned char key, bool isPressed);
	void CheckMovement();
	void MoveForward(float deltaTime);
	void MoveRight(float dis, int r);
	void LookUp(float deltaTime);
	void LookAround(float deltaTime);
	Matrix RotationMatrixAroundX(float Angle);
	Matrix RotationMatrixAroundY(float Angle);
	Matrix GetWorldMatrix();

	void SetHorizontal(float H);
	void SetVertical(float V);
	void SetPosition(float X, float Y, float Z);
	void SetPosition(Vector3 Position);
	Vector3 GetPosition();
	void SetRotation(float X, float Y, float Z);
	void SetRotation(Vector3 Rotation);
	Vector3 GetRotation();
	void SetTarget(Vector3 Target);
	void SetTarget(float X, float Y, float Z);
	Vector3 GetTarget();
	void SetUp(Vector3 Up) { m_Up = Up; }
	Vector3 GetUp() { return m_Up; }	
};

