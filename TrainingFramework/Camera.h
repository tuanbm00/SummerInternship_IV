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
	Vector3 m_TargetPosition;

	Matrix m_ViewMatrix;
	Matrix Omatrix;

	int keyPressed = 0;
public:
	GLuint iboId;
	Camera();
	~Camera();
	static Camera* GetInstance();
	int i_state;
	Matrix GetOrthographic();
	Matrix GetViewMatrix();

	void Init(float FOV, float Near, float Far, float Move_Speed, float Rotate_Speed);
	void Update(float deltaTime);
	void CleanUp();


	void initOrtho();
	void SetPosition(float X, float Y, float Z);
	void SetPosition(float X, float Y);
	void SetPosition(Vector3 Position);
	Vector3 GetPosition();
	void SetTarget(Vector3 Target);
	void SetTarget(float X, float Y, float Z);
	Vector3 GetTarget();
	void SetUp(Vector3 Up) { m_Up = Up; }
	Vector3 GetUp() { return m_Up; }	
};

