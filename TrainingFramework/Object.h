#pragma once

#include <conio.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"
#include "CubeTextures.h"

class Object
{
private:
	GLuint iboId, vboId;
	std::vector<GLuint> textureId;
	std::vector<Texture*> m_Texture;
	CubeTextures* m_CubeTexture;
	Shaders* m_Shader;
	Model* m_Model;

	Vector3 m_Position;
	Vector3 m_Scale;
	Vector3 m_Rotation;

	Matrix m_WorldMatrix;
	Matrix m_ScaleMatrix;
	Matrix m_WVP;
	float m_DistanceToCamera;
	float m_CurrentTime;

	int m_ObjectID;
	bool m_bIsTarget = false, m_isTexture = false, m_isCubeTexture = false;
	
public:
	Object(int ID, Shaders* Shader, Model* Model);
	~Object();
	int Init();
	void Draw();
	void Update(float deltaTime);
	void CleanUp();

	void UpdateWVP();

	void LookUp();
	void LookAround();

	void SetTexture(Texture* Texture);
	void SetCubeTexture(CubeTextures* CubeTexture);
	void SetPosition(float X, float Y, float Z);
	void SetPosition(Vector3 Position);
	Vector3 GetPosition();
	void SetScale(float X, float Y, float Z);
	void SetScale(Vector3 Scale);
	Vector3 GetScale();
	void SetRotation(float X, float Y, float Z);
	void SetRotation(Vector3 Rotation);
	Vector3 GetRotation();
	void SetIsTarget(bool Target) { m_bIsTarget = Target; }
	bool GetIsTarget() { return m_bIsTarget; }
};

