#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Model.h"
#include "Vertex.h"
#include <iostream>


Model::Model(int ID, char* srcModel) {
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

	/*FILE* f_M;
	f_M = fopen(m_srcModel, "r+");
	if (f_M == NULL) {
		return false;
	}

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	int numberOfVertices;
	fscanf(f_M, "NrVertices: %d\n", &numberOfVertices);
	if (numberOfVertices <= 0)
		return false;

	Vertex* verticesData = new Vertex[numberOfVertices];

	for (int i = 0; i < numberOfVertices; ++i)
	{
		fscanf(f_M, "  %*d. pos:[%f, %f, %f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f, %f];\n",
			&verticesData[i].pos.x, &verticesData[i].pos.y, &verticesData[i].pos.z,
			&verticesData[i].uv.x, &verticesData[i].uv.y);
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numberOfVertices, verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] verticesData;

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	int numberOfIndices;
	fscanf(f_M, "NrIndices: %d\n", &numberOfIndices);
	if (numberOfIndices <= 0)
	{
		return 0;
	}
	int* indices = new int[numberOfIndices];
	for (int i = 0; i < numberOfIndices; i += 3)
	{
		fscanf(f_M, "   %*d.    %d,    %d,    %d\n", &indices[i], &indices[i + 1], &indices[i + 2]);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numberOfIndices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] indices;

	m_NumberOfIndices = numberOfIndices;

	fclose(f_M);*/
}

