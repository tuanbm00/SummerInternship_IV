#pragma once

#include <conio.h>
#include "Shaders.h"
#include "Models.h"
#include "Texture.h"

class Sprite2D
{
protected:
	std::vector<GLuint> textureId;
	std::vector<Texture*> m_Texture;
	Shaders* m_Shader;
	Models* m_Model;

	Vector3 m_Position;
	Vector3 m_Scale;
	Vector3 m_Rotation;

	Matrix m_WorldMatrix;
	Matrix m_ScaleMatrix;
	Matrix m_WVP;

	Vector2 m_Vec2DPos;
	int m_iWidth, m_iHeight;

	int m_SpriteID;
	bool m_bIsTarget = false, m_isTexture = false;
	bool m_bIsActive = true;

public:
	int m_current_anim;
	Sprite2D(int ID);
	~Sprite2D();
	void CalculateWVP();
	void Draw();
	virtual void Update(float deltaTime);
	void CleanUp();

	void setModel(Models* mmodel);
	void setShader(Shaders* mshader);
	void SetTexture(Texture* Texture);
	void Set2DPosition(GLfloat X, GLfloat Y);
	void SetPosition(GLfloat X, GLfloat Y, GLfloat Z);
	void Set2DPosition(Vector2 Position);
	Vector2 Get2DPosition();
	void SetSize(int W, int H);
	void Set2DScale(GLfloat X, GLfloat Y);
	void Set2DScale(Vector2 Scale);
	Vector2 Get2DScale();
	void SetRotationZ(GLfloat Z);
	GLfloat GetRotationZ();
	int GetID();
	inline void SetActive(bool isActive) { m_bIsActive = isActive; }
	inline bool GetActive() { return m_bIsActive; }

	Texture* getTexture();
	Shaders* getShaders();
	Models* getModel();
	

};


