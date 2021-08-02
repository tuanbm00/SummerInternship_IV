#include <iostream>
#include "stdafx.h"
#include "Object.h"
#include "Vertex.h"
#include "Camera.h"
#include "defines.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

Object::Object(int ID, Shaders *shader, Model *model) {
	m_ObjectID = ID;
	m_Model = model;
	m_Shader = shader;
}

void Object::SetTexture(Texture* texture) {
	m_Texture.push_back(texture);
	GLuint i;
	textureId.push_back(i);
	m_isTexture = true;
}

void Object::SetCubeTexture(CubeTextures* cubeTexture) {
	m_CubeTexture = cubeTexture;
	GLuint i;
	textureId.push_back(i);
	m_isCubeTexture = true;
}
Object::~Object() {
}

int Object::Init() {
	m_CurrentTime = 0;

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Model->GetNumberofVertices(), m_Model->GetVertices(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Model->GetNumberofIndices() * sizeof(int), m_Model->GetIndices(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	
	for (int i = 0; i < 4; i++) {
		Vector4 v = Vector4(m_Model->GetVertices()[i].pos, 1.0) * m_WVP;
		printf("%f %f %f %f\n", v.x, v.y, v.z, v.w);
	}

	if (m_isTexture) {
		for (register int i = 0; i < textureId.size(); i++) {
			glGenTextures(1, &textureId[i]);
			glBindTexture(GL_TEXTURE_2D, textureId[i]);
			m_Texture[i]->BufferTexture();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	if (m_isCubeTexture) {
		for (register int i = 0; i < textureId.size(); i++) {
			glGenTextures(1, &textureId[i]);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureId[i]);
			m_CubeTexture->BufferTexture();
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	}

	return m_Shader->Init();
}

void Object::Draw() {

	glUseProgram(m_Shader->program);

	if (m_Shader->m_Cull_Face != 0) {
		glEnable(GL_CULL_FACE);
	} else glDisable(GL_CULL_FACE);
	if (m_Shader->m_Depth_Test != 0) {
		glEnable(GL_DEPTH_TEST);
	}else glDisable(GL_DEPTH_TEST);
	if (m_Shader->m_Blend != 0) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	} else glDisable(GL_BLEND);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
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

	//Set LerpValue
	m_DistanceToCamera = (m_Position - Camera::GetInstance()->GetPosition()).Length();
	if (m_Shader->m_uDistanceToCamera != -1)
	{
		glEnableVertexAttribArray(m_Shader->m_uDistanceToCamera);
		glUniform1f(m_Shader->m_uDistanceToCamera, m_DistanceToCamera);
	}

	//Setting Texture Uniform
	if (m_isTexture) {
		for (register int i = 0; i < textureId.size(); i++) {
			if (m_Shader->m_uTextures[i] != -1) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, TEXTURE0 + i);
				glUniform1i(m_Shader->m_uTextures[i], i);
			}
		}
	}

	if (m_isCubeTexture) {
		if (m_Shader->m_uCubeTexture != -1) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureId[0]);
			glUniform1i(m_Shader->m_uCubeTexture, 0);
		}
	}

	if (m_Shader->m_uTime != -1)
	{
		glEnableVertexAttribArray(m_Shader->m_uTime);
		glUniform1f(m_Shader->m_uTime, m_CurrentTime);
	}

	glDrawElements(GL_TRIANGLES, m_Model->GetNumberofIndices(), GL_UNSIGNED_INT, 0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Object::Update(float deltaTime) {
	m_CurrentTime += deltaTime;
	if (m_bIsTarget) {
		SetPosition(Camera::GetInstance()->GetTarget());
	}
	UpdateWVP();
}

void Object::UpdateWVP() {
	Matrix Rx, Ry, Rz;
	Matrix translationMatrix, scaleMatrix, rotationMatrix;
	translationMatrix.SetTranslation(m_Position);
	scaleMatrix.SetScale(m_Scale);
	rotationMatrix = Rz.SetRotationZ(m_Rotation.z * float(PI / 180.0f)) * Rx.SetRotationX(m_Rotation.x * float(PI / 180.0f)) * Ry.SetRotationY(m_Rotation.y * float(PI / 180.0f));
	m_WorldMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	if(Camera::GetInstance()->i_state == 1) m_WVP = m_WorldMatrix * Camera::GetInstance()->GetViewMatrix() * Camera::GetInstance()->GetPerspective();
	else if(Camera::GetInstance()->i_state == 2) m_WVP = m_WorldMatrix * Camera::GetInstance()->GetViewMatrix() * Camera::GetInstance()->GetOrthographic();
}

void Object::CleanUp() {
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &iboId);
	for (register int i = 0; i < textureId.size(); i++) {
		glDeleteBuffers(1, &textureId[i]);
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
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