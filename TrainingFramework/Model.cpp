#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Model.h"
#include "Vertex.h"
#include <iostream>


Model::Model()
{
	b_IsAnimation = false;
}


Model::Model(int ID, char* srcModel) {
	b_IsAnimation = false;
	m_Id = ID;
	strcpy(m_srcModel, srcModel);
	this->Init();
}

Model::~Model() {
	delete[] verticesData;
	delete[] indices;
}


int Model::Init() {
	LoadModel();
	return 0;
}

void Model::InitSprite(float spriteX, float spriteY, float spriteW, float spriteH, float textureW, float textureH)
{
	m_posX = spriteX; m_posY = spriteY; m_spriteW = spriteW; m_spriteH = spriteH;
	m_textureH = textureH; m_textureW = textureW;
	m_NumberOfVertices = 4;
	verticesData = new Vertex[m_NumberOfVertices];
	origin = Vector2((m_posX + m_spriteW) / 2, (m_posY + m_spriteH) / 2);
	Vector3 delta = Vector3(origin.x - spriteW / 2, origin.y - spriteH / 2, 0.0);
	verticesData[0].pos = Vector3(-(float)spriteW / 2, -(float)spriteH / 2, 0.0f) - delta;
	verticesData[1].pos = Vector3((float)spriteW / 2, -(float)spriteH / 2, 0.0f) - delta;
	verticesData[2].pos = Vector3(-(float)spriteW / 2, (float)spriteH / 2, 0.0f) - delta;
	verticesData[3].pos = Vector3((float)spriteW / 2, (float)spriteH / 2, 0.0f) - delta;
	verticesData[0].uv = Vector2((float)spriteX / textureW, (float)(spriteY + spriteH) / textureH);
	verticesData[1].uv = Vector2((float)(spriteX + spriteW) / textureW, (float)(spriteY + spriteH) / textureH);
	verticesData[2].uv = Vector2((float)spriteX / textureW, (float)spriteY / textureH);
	verticesData[3].uv = Vector2((float)(spriteX + spriteW) / textureW, (float)spriteY / textureH);

//	for (int i = 0; i < m_NumberOfVertices; i++) {
//		printf("first uv pos %f %f\n", verticesData[i].pos.x, verticesData[i].pos.y);
//	}

	indices = new int[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;
	
	m_NumberOfIndices = 6;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_NumberOfVertices, verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumberOfIndices * sizeof(int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


int Model::LoadModel()
{
	FILE* f_M;
	f_M = fopen(m_srcModel, "r+");
	if (f_M == NULL) {
		return false;
	}

	int numberOfVertices;
	fscanf(f_M, "NrVertices: %d\n", &numberOfVertices);
	if (numberOfVertices <= 0)
		return false;

	verticesData = new Vertex[numberOfVertices];

	for (int i = 0; i < numberOfVertices; ++i)
	{
		fscanf(f_M, "  %*d. pos:[%f, %f, %f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f, %f];\n",
			&verticesData[i].pos.x, &verticesData[i].pos.y, &verticesData[i].pos.z,
			&verticesData[i].uv.x, &verticesData[i].uv.y);
	}

	m_NumberOfVertices = numberOfVertices;

	int numberOfIndices;
	fscanf(f_M, "NrIndices: %d\n", &numberOfIndices);
	if (numberOfIndices <= 0)
	{
		return 0;
	}
	indices = new int[numberOfIndices];
	for (int i = 0; i < numberOfIndices; i += 3)
	{
		fscanf(f_M, "   %*d.    %d,    %d,    %d\n", &indices[i], &indices[i + 1], &indices[i + 2]);
	}

	m_NumberOfIndices = numberOfIndices;
	fclose(f_M);
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_NumberOfVertices, verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumberOfIndices * sizeof(int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::setOrigin(Vector2 ori)
{
	origin = Vector2((m_posX + m_spriteW) / 2, (m_posY + m_spriteH)/ 2);
}

void Model::addAnimation(Animation* anm)
{
	printf("add animation %d\n", anm->GetID());
	m_anim.push_back(anm);
}

void Model::updateAnimation(float deltaTime, int type)
{
	if (type >= 0) m_anim[type - 1]->play(&vboId, Vector2(m_textureW, m_textureH), origin, deltaTime);
	else {
		type = -type;
		m_anim[type - 1]->play(&vboId, Vector2(m_textureW, m_textureH), origin, deltaTime, true);
	}
}

