#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Models.h"
#include "Vertex.h"
#include <iostream>


Models::Models(int ID, char* srcModel) {
	m_Id = ID;
	this->LoadModel(srcModel);
}

Models::~Models() {
	delete[] verticesData;
	delete[] indices;
}


int Models::LoadModel(char * srcModel)
{
	FILE* f_M;
	f_M = fopen(srcModel, "r+");
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

	m_iNumIndices = numberOfIndices;

	//buffer object
	glGenBuffers(1, &m_iVboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVboID);
	glBufferData(GL_ARRAY_BUFFER, numberOfVertices * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//if(m_iIboID == 0)
	{
		glGenBuffers(1, &m_iIboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iNumIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	fclose(f_M);
}

GLuint Models::GetVertexObject()
{
	return m_iVboID;
}
GLuint Models::GetIndiceObject()
{
	return m_iIboID;
}
GLuint Models::GetNumIndiceObject()
{
	return m_iNumIndices;
}
