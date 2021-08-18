#pragma once
#include <fstream>
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include <vector>

class Models
{
private:

	Vertex* verticesData;
	int* indices;

	int m_Id;

	GLuint		m_iNumIndices;
	GLuint		m_iVboID; 
	GLuint		m_iIboID;
public:
	Models(int ID, char* srcModel);
	~Models();
	int		LoadModel(char* srcModel);
	GLuint		GetVertexObject();
	GLuint		GetIndiceObject();
	GLuint		GetNumIndiceObject();
	void		SetModelId(GLuint id);
	GLuint		GetModelId();
};