#include <iostream>
#include "stdafx.h"
#include "Object.h"
#include "Vertex.h"
#include "Camera.h"
#include "define.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

Object::Object(int ID) {
	m_ObjectID = ID;
	m_current_anim = Idle;
}

int Object::GetID() {
	return m_ObjectID;
}

b2Body* Object::getBody() {
	return m_body;
}

Vector2 Object::GetBox() {
	return Vector2(m_spriteW, m_spriteH);
}

void Object::SetTexture(Texture* texture) {
	m_Texture.push_back(texture);
	GLuint i;
	textureId.push_back(i);
	m_isTexture = true;
}


Object::~Object() {

}

void Object::InitWVP()
{
	m_Model->setOrigin(Vector2(m_Position.x, m_Position.y));
	Matrix Rx, Ry, Rz;
	Matrix translationMatrix, scaleMatrix, rotationMatrix;
	translationMatrix.SetTranslation(m_Position);
	scaleMatrix.SetScale(m_Scale);
	rotationMatrix = Rz.SetRotationZ(m_Rotation.z * float(PI / 180.0f)) * Rx.SetRotationX(m_Rotation.x * float(PI / 180.0f)) * Ry.SetRotationY(m_Rotation.y * float(PI / 180.0f));
	m_WorldMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	m_WVP = m_WorldMatrix * Camera::GetInstance()->GetViewMatrix() * Camera::GetInstance()->GetOrthographic();
}

void Object::Draw() {
	UpdateWVP();

	glUseProgram(m_Shader->program);
	

	glBindBuffer(GL_ARRAY_BUFFER, m_Model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Camera::GetInstance()->iboId);
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
	if (m_isTexture) {
		for (register int i = 0; i < m_Texture.size(); i++) {
			if (m_Shader->m_uTextures[i] != -1) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_Texture[i]->mTextureId);
				glUniform1i(m_Shader->m_uTextures[i], i);
			}
		}
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Object::Update(float deltaTime) {
	Matrix translationMatrix, scaleMatrix;
	translationMatrix.SetTranslation(m_Position);
	scaleMatrix.SetScale(m_Scale);
	m_WorldMatrix = scaleMatrix * translationMatrix;
	UpdateWVP();
}

void Object::UpdateWVP() {
	m_WVP = m_WorldMatrix * Camera::GetInstance()->GetViewMatrix() * Camera::GetInstance()->GetOrthographic();
}

void Object::CleanUp() {
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &iboId);
	for (register int i = 0; i < textureId.size(); i++) {
//		glDeleteBuffers(1, &textureId[i]);
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Object::setModel(Model* mmodel)
{
	this->m_spriteH = mmodel->m_spriteH;
	this->m_spriteW = mmodel->m_spriteW;
	m_Model = mmodel;
}

void Object::setShader(Shaders* mshader)
{
	m_Shader = mshader;
}

void Object::SetPosition(float X, float Y, float Z) {
	m_Position = Vector3(X, Y, Z);
}

void Object::SetPosition(Vector3 Position) {
	m_Position = Position;
}

Vector3 Object::GetPosition() {
	return m_Position;
}
void Object::SetScale(float X, float Y, float Z) {
	m_Scale = Vector3(X, Y, Z);
}

void Object::SetScale(Vector3 Scale) {
	m_Scale = Scale;
}

Vector3 Object::GetScale() {
	return m_Scale;
}
void Object::SetRotation(float X, float Y, float Z) {
	m_Rotation = Vector3(X, Y, Z);
}

void Object::SetRotation(Vector3 Rotation) {
	m_Rotation = Rotation;
}
Vector3 Object::GetRotation() {
	return m_Rotation;
}

void Object::SetBodyObject(b2World* world) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(m_Position.x, m_Position.y);
	m_body = world->CreateBody(&bodyDef);
	b2PolygonShape staticBox;
	staticBox.SetAsBox(m_spriteW/2, m_spriteH/2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &staticBox;
	fixtureDef.filter.categoryBits = CATEGORY_TERRAIN;
	fixtureDef.filter.maskBits = MASK_TERRAIN;
	m_body->CreateFixture(&fixtureDef);
}