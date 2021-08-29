#include <iostream>
#include "stdafx.h"
#include "Object.h"
#include "Vertex.h"
#include "Camera.h"
#include "define.h"
#include "Globals.h"
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
	m_isTexture = true;
}

Texture* Object::getTexture() {
	return m_Texture[0];
}


Object::~Object() {

}

void Object::InitWVP()
{
	m_Model->setOrigin(Vector2(m_Position.x, m_Position.y));
	m_WorldMatrix.SetScale(m_Scale);
	UpdateWorld();

	m_WVP = m_WorldMatrix * Camera::GetInstance()->GetViewMatrix();
}

void Object::Draw() {
	UpdateWVP();

	glBindBuffer(GL_ARRAY_BUFFER, m_Model->vboId);
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
		glBindTexture(GL_TEXTURE_2D, m_Texture[0]->mTextureId);
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	//
	//
}

void Object::Update(float deltaTime) {
	Vector3 camPos = Camera::GetInstance()->GetPosition();
	float temp = (camPos.x * (1 - parallaxEffect));
	float dist = camPos.x * parallaxEffect;
	m_Position.x = startPos+dist;
	m_Position.y = camPos.y-100.0f;
	if (temp > startPos + length) startPos += length;
	else if (temp < startPos - length) startPos -= length;
	UpdateWorld();
}

void Object::UpdateWVP() {
	Matrix ma = Camera::GetInstance()->GetViewMatrix();
	m_WVP.m[0][0] = m_Scale.x * ma.m[0][0];
	m_WVP.m[3][0] = m_WorldMatrix.m[3][0] * ma.m[0][0] + ma.m[3][0];
	m_WVP.m[3][1] = m_WorldMatrix.m[3][1] * ma.m[1][1] + ma.m[3][1];
}

void Object::UpdateWorld()
{
	m_WorldMatrix.m[3][0] = m_Position.x;
	m_WorldMatrix.m[3][1] = m_Position.y;
	m_WorldMatrix.m[3][2] = m_Position.z;
}

void Object::CleanUp() {
	
}

void Object::setModel(Model* mmodel)
{
	this->m_spriteH = mmodel->m_spriteH;
	this->m_spriteW = mmodel->m_spriteW;
	m_Model = mmodel;
}

Model* Object::getModel() {
	return m_Model;
}

void Object::setShader(Shaders* mshader)
{
	m_Shader = mshader;
}

Shaders* Object::getShaders() {
	return m_Shader;
}

void Object::SetPosition(float X, float Y, float Z) {
	m_Position = Vector3(X, Y, Z);
	UpdateWorld();
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

void Object::SetScale(float X)
{
	m_Scale.x = X;
}

void Object::SetScale(Vector3 Scale) {
	m_Scale = Scale;
}

bool Object::checkDraw()
{
	Vector3 pos = Camera::GetInstance()->GetPosition();
	if (m_Position.x <= (pos.x - Globals::screenWidth * 2) || m_Position.x >= (pos.x + Globals::screenWidth * 2)) return false;
	if (m_Position.y <= (pos.y - Globals::screenHeight * 2) || m_Position.y >= (pos.y + Globals::screenHeight * 2)) return false;
	return true;
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
void Object::UpdateAnimation(float deltaTime) {
	if (m_Model->b_IsAnimation == true) {
		m_Model->updateAnimation(deltaTime, m_current_anim);
	}
}

void Object::start(float para, float len)
{
	startPos = m_Position.x;
	parallaxEffect = para;
	length = len*m_Scale.y;
}

void Object::deleteTex()
{
	glDeleteTextures(1, &m_Texture[0]->mTextureId);
}

bool Object::getDead()
{
	return m_Model->m_isDead;
}
