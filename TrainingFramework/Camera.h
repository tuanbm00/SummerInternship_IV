#pragma once

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Math.h"


class Camera
{
private:
	static Camera* s_Instance;
	float m_FOV, m_Near, m_Far;
	bool flagX,flagY;
	
	Vector3 m_Up;
	Vector3 m_Target;
	Vector3 m_Position;

	Matrix m_ViewMatrix;
	Matrix Omatrix;
	float lerpX, lerpY;
	int past_dir;
public:
	GLuint iboId;
	Camera();
	~Camera();
	static Camera* GetInstance();
	int i_state;
	int m_iOption;
	Vector2 limitX, limitY;
	Matrix GetOrthographic();
	Matrix GetViewMatrix();

	void Init(float FOV, float Near, float Far, float Move_Speed, float Rotate_Speed);
	void Update(float deltaTime, float posX, float posY,int direction);
	void CleanUp();
	bool is_shoot;
	bool is_wound;
	bool is_dead;
	bool is_exit;
	int m_iUpdateFase;

	void setLimitX(float min, float max);
	void setLimitY(float min, float max);
	void initView();
	void initOrtho();
	void updateView(float x, float y);
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

