#pragma once
#include <fstream>
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h";

class Model
{
private:
	int m_NumberOfVertices = 0;
	int m_NumberOfIndices = 0;
	
	Vertex* verticesData;
	int* indices;

	int m_Id;
	char m_srcModel[250];
	
public:
	
	Model();
	Model(int ID, char* srcModel);
	~Model();
	int Init();
	int LoadModel();

	int GetNumberofVertices() {
		return m_NumberOfVertices;
	}
	int GetNumberofIndices() {
		return m_NumberOfIndices;
	}
	int* GetIndices() { return indices; }
	Vertex* GetVertices() { return verticesData; }
	int GetID() { return m_Id; }
	char* GetSource() { return m_srcModel; }
};

