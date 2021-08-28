#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Model.h"
#include "Vertex.h"
#include "ResourceManager.h"
#include <iostream>


Model::Model()
{
	b_IsAnimation = false;
}

Model::Model(Model * model) {
	InitSprite(model->m_posX, model->m_posY, model->m_spriteW, model->m_spriteH, model->m_textureW, model->m_textureH);
	std::vector<Animation*> v = model->getAnim();
	for (int i = 0; i < (int)v.size(); ++i) {
		Animation* anim = new Animation(v[i]);
		addAnimation(anim);
		ResourceManager::GetInstance()->addDumpAnim(anim);
	}
	if(v.size() > 0) b_IsAnimation = true;
}

Model::~Model() {
}

void Model::InitSprite(float spriteX, float spriteY, float spriteW, float spriteH, float textureW, float textureH, bool isNotHP)
{
	m_posX = spriteX; m_posY = spriteY; m_spriteW = spriteW; m_spriteH = spriteH;
	m_textureH = textureH; m_textureW = textureW;
	m_NumberOfVertices = 4;
	Vertex verticesData[4];
	if (isNotHP) {
		origin = Vector2((m_posX + m_spriteW) / 2, (m_posY + m_spriteH) / 2);
	}
	else {
		origin = Vector2(0, 0);
	}
	Vector3 delta = Vector3(origin.x - spriteW / 2, origin.y - spriteH / 2, 0.0);
	verticesData[0].pos = Vector3(-(float)spriteW / 2, -(float)spriteH / 2, 0.0f) - delta;
	verticesData[1].pos = Vector3((float)spriteW / 2, -(float)spriteH / 2, 0.0f) - delta;
	verticesData[2].pos = Vector3(-(float)spriteW / 2, (float)spriteH / 2, 0.0f) - delta;
	verticesData[3].pos = Vector3((float)spriteW / 2, (float)spriteH / 2, 0.0f) - delta;
	verticesData[0].uv = Vector2((float)spriteX / textureW, (float)(spriteY + spriteH) / textureH);
	verticesData[1].uv = Vector2((float)(spriteX + spriteW) / textureW, (float)(spriteY + spriteH) / textureH);
	verticesData[2].uv = Vector2((float)spriteX / textureW, (float)spriteY / textureH);
	verticesData[3].uv = Vector2((float)(spriteX + spriteW) / textureW, (float)spriteY / textureH);
	
	m_NumberOfIndices = 6;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_NumberOfVertices, verticesData, GL_STATIC_DRAW);
	
}

void Model::setOrigin(Vector2 ori)
{
	origin = Vector2((m_posX + m_spriteW) / 2, (m_posY + m_spriteH)/ 2);
}

void Model::addAnimation(Animation* anm)
{
	m_anim.push_back(anm);
	if (m_anim.size() == RunJump || m_anim.size() == Jump) {
		m_anim.back()->isJump = true;
	}
}

void Model::updateAnimation(float deltaTime, int type)
{
	bool revert = (type > 0) ? 0 : 1;
	type = abs(type);
	if (type > (int)m_anim.size()) type = m_anim.size();
	m_anim[type - 1]->play(&vboId, Vector2(m_textureW, m_textureH), origin, deltaTime, revert);
}

void Model::updateDead(float deltaTime)
{
	m_anim[dead - 1]->playDead(&vboId, Vector2(m_textureW, m_textureH), origin, deltaTime);
}

void Model::resetGun() {
	for (int i = 0; i < (int)m_anim.size(); ++i) {
		if (m_anim[i]->isGun) m_anim[i]->resetAnimation();
	}
}

void Model::resetTexture()
{
	Vector4 frame = getAnimation(0)->getTexture();
	float x = frame.x, y = frame.y, w = frame.z, h = frame.w;
	
	Vertex verticesData[4];
	Vector3 delta = Vector3(origin.x - w / 2, origin.y - h / 2, 0.0);
	verticesData[0].pos = Vector3(-(float)w / 2, -(float)h / 2, 0.0f) - delta;
	verticesData[1].pos = Vector3((float)w / 2, -(float)h / 2, 0.0f) - delta;
	verticesData[2].pos = Vector3(-(float)w / 2, (float)h / 2, 0.0f) - delta;
	verticesData[3].pos = Vector3((float)w / 2, (float)h / 2, 0.0f) - delta;

	x /= m_textureW;
	y /= m_textureH;
	w /= m_textureW;
	h /= m_textureH;

	verticesData[0].uv = Vector2(x, y + h);
	verticesData[1].uv = Vector2(x + w, y + h);
	verticesData[2].uv = Vector2(x, y);
	verticesData[3].uv = Vector2(x + w, y);


	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, verticesData, GL_STATIC_DRAW);
	
}

