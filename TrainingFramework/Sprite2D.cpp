#include "Sprite2D.h"
#include <iostream>
#include "stdafx.h"
#include "Vertex.h"
#include "Camera.h"
#include "define.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Globals.h"

Sprite2D::Sprite2D(int ID) {
	m_SpriteID = ID;
	m_Rotation = Vector3(0, 0, 0);
	m_Position = Vector3(0, 0, 0);
	m_iHeight = 50;
	m_iWidth = 100;
	m_Scale = Vector3((float)m_iWidth / Globals::screenWidth, (float)m_iHeight / Globals::screenHeight, 1);
}



void Sprite2D::SetTexture(Texture* texture) {
	m_Texture.push_back(texture);
	GLuint i;
	textureId.push_back(i);
	m_isTexture = true;
}

void Sprite2D::SetNewTexture(Texture* texture) {
	m_Texture[0] = texture;
}

Texture* Sprite2D::getTexture() {
	return m_Texture[0];
}


Sprite2D::~Sprite2D() {
}

void Sprite2D::CalculateWVP()
{
	Matrix Rx, Ry, Rz;
	Matrix translationMatrix, scaleMatrix, rotationMatrix;
	translationMatrix.SetTranslation(m_Position);
	scaleMatrix.SetScale(m_Scale);
	rotationMatrix = Rz.SetRotationZ(m_Rotation.z * float(PI / 180.0f)) * Rx.SetRotationX(m_Rotation.x * float(PI / 180.0f)) * Ry.SetRotationY(m_Rotation.y * float(PI / 180.0f));
	m_WorldMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	m_WVP = m_WorldMatrix;
}

void Sprite2D::Draw() {
	if (!m_bIsActive) return;

	glUseProgram(m_Shader->program);

	glBindBuffer(GL_ARRAY_BUFFER, m_Model->GetVertexObject());
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Model->GetIndiceObject());
	//Set Position
	if (m_Shader->m_aPosition != -1)
	{
		glEnableVertexAttribArray(m_Shader->m_aPosition);
		glVertexAttribPointer(m_Shader->m_aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	}

	//Set UV
	if (m_Shader->m_aUV != -1)
	{
		glEnableVertexAttribArray(m_Shader->m_aUV);
		glVertexAttribPointer(m_Shader->m_aUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}

	//Set World Matrix
	if (m_Shader->m_uWVP != -1)
	{
		glEnableVertexAttribArray(m_Shader->m_uWVP);
		glUniformMatrix4fv(m_Shader->m_uWVP, 1, GL_FALSE, (GLfloat*)m_WVP.m);
	}

	//Setting Texture Uniform
	if (m_Shader->m_uTextures[0] != -1) {
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]->mTextureId);
		//glUniform1i(m_Shader->m_uTextures[0], 0);
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	/*
	
	*/
}

void Sprite2D::Update(float deltaTime) {
	/*Matrix translationMatrix, scaleMatrix;
	translationMatrix.SetTranslation(m_Position);
	scaleMatrix.SetScale(m_Scale);
	m_WorldMatrix = scaleMatrix * translationMatrix;*/
	//CalculateWVP();
}

void Sprite2D::CleanUp() {
	/*glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);*/
}

void Sprite2D::setModel(Models* mmodel)
{
	m_Model = mmodel;
}

Models* Sprite2D::getModel() {
	return m_Model;
}

void Sprite2D::setShader(Shaders* mshader)
{
	m_Shader = mshader;
}

Shaders* Sprite2D::getShaders() {
	return m_Shader;
}


void Sprite2D::Set2DScale(float X, float Y) {
	m_Scale = Vector3(X, Y, 1.0f);
}

void Sprite2D::Set2DScale(Vector2 Scale) {
	m_Scale = Vector3(Scale.x, Scale.y, 1.0f);
}

Vector2 Sprite2D::Get2DScale() {
	return Vector2(m_Scale.x, m_Scale.y);
}

void Sprite2D::SetPosition(GLfloat X, GLfloat Y, GLfloat Z)
{
	m_Position = Vector3(X, Y, Z);
	CalculateWVP();
}

void Sprite2D::Set2DPosition(GLfloat X, GLfloat Y)
{
	m_Vec2DPos = Vector2(X, Y);

	float xx = (2 * m_Vec2DPos.x) / Globals::screenWidth - 1.0f;
	float yy = 1.0f - (2.0f * m_Vec2DPos.y) / Globals::screenHeight;
	m_Position = Vector3(xx, yy, 0.0);
	CalculateWVP();
}

void Sprite2D::SetRotationZ(GLfloat Z)
{
	m_Rotation.z = Z;
	CalculateWVP();
}

void Sprite2D::Set2DPosition(Vector2 pos)
{
	m_Vec2DPos = pos;

	float xx = (2 * m_Vec2DPos.x) / Globals::screenWidth - 1.0f;
	float yy = 1.0f - (2.0f * m_Vec2DPos.y) / Globals::screenHeight;
	m_Position = Vector3(xx, yy, 0.0);
	CalculateWVP();
}

Vector2 Sprite2D::Get2DPosition()
{
	float xx = (2 * m_Vec2DPos.x) / Globals::screenWidth - 1.0f;
	float yy = 1.0f - (2.0f * m_Vec2DPos.y) / Globals::screenHeight;
	std::cout << xx << " - " << yy << std::endl;
	return m_Vec2DPos;
}


void Sprite2D::SetSize(GLint W, GLint H)
{
	m_iWidth = W;
	m_iHeight = H;
	m_Scale = Vector3((float)m_iWidth / Globals::screenWidth, (float)m_iHeight / Globals::screenHeight, 1);
	CalculateWVP();
}