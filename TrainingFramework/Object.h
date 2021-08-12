#pragma once

#include <conio.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"
#include "CubeTextures.h"
#include "b2_body.h"
#include <b2_world.h>
#include <b2_polygon_shape.h>
#include <b2_fixture.h>
#include <b2_circle_shape.h>
#include <b2_edge_shape.h>
#include <b2_contact.h>

class Object
{
protected:
	GLuint iboId, vboId;
	std::vector<GLuint> textureId;
	std::vector<Texture*> m_Texture;
	Shaders* m_Shader;
	Model* m_Model;


	float m_spriteX, m_spriteY, m_spriteW, m_spriteH;

	b2Body* m_body;

	Vector3 m_Position;
	Vector3 m_Scale;
	Vector3 m_Rotation;

	Matrix m_WorldMatrix;
	Matrix m_ScaleMatrix;
	Matrix m_WVP;
	float m_CurrentTime;

	int m_ObjectID;
	bool m_bIsTarget = false, m_isTexture = false, m_isCubeTexture = false;
	
public:
	int m_current_anim;
	Object(int ID);
	~Object();
	int Init();
	void InitWVP();
	void Draw();
	virtual void Update(float deltaTime);
	void CleanUp();
	void setModel(Model* mmodel);
	void setShader(Shaders* mshader);
	void UpdateWVP();

	void SetTexture(Texture* Texture);
	void SetPosition(float X, float Y, float Z);
	void SetPosition(Vector3 Position);
	Vector3 GetPosition();
	void SetScale(float X, float Y, float Z);
	void SetScale(Vector3 Scale);
	Vector3 GetScale();
	void SetRotation(float X, float Y, float Z);
	void SetRotation(Vector3 Rotation);
	Vector3 GetRotation();
	Vector2 GetBox();
	int GetID();
	b2Body* getBody();
	void SetIsTarget(bool Target) { m_bIsTarget = Target; }
	bool GetIsTarget() { return m_bIsTarget; }
	void SetBodyObject(b2World* world);
};

