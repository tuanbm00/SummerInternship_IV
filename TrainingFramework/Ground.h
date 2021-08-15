#pragma once
#include"GLES2/gl2.h"
#include "Vertex.h"
#include "Texture.h"
#include "Shaders.h"
#include<vector>
class Ground
{
public:
	Ground() {
		m_numberOfIndices = 0;
		m_numberOfVertex = 0;
	}
	void Init();
	void Draw();
	void InitWVP();
	void UpdateWVP();
	void addVertex(float spriteX, float spriteY, float spriteW, float spriteH, float textureW, float textureH, Vector2 origin);
	int addNewV(Vertex v);
	void setTexture(Texture * tex);
	void setShader(Shaders * shader);
private:

	Matrix m_WorldMatrix;
	Matrix m_WVP;
	Texture * m_texture;
	Shaders * m_shader;
	GLuint vboID, iboID;
	int m_numberOfVertex;
	int m_numberOfIndices;
	std::vector<Vertex> vertices;
	std::vector<int> indices;
};

