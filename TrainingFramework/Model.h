#pragma once
#include <fstream>
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h";
#include "Animation.h"
#include <vector>

class Model
{
private:
	int m_NumberOfVertices = 0;
	int m_NumberOfIndices = 0;
	
	Vertex* verticesData;
	int* indices;
	std::vector<Animation*> m_anim;;
	int m_Id;
	char m_srcModel[250];
	Vector2 origin;
	
public:
	float m_textureW, m_textureH, m_posX, m_posY, m_spriteW, m_spriteH;
	GLuint vboId, iboId;
	bool b_IsAnimation;
	Model();
	Model(int ID, char* srcModel);
	~Model();
	int Init();
	void InitSprite(float spriteX, float SpriteY, float SpriteW, float SpriteH, float TextureW, float TextureH);
	int LoadModel();
	void setOrigin(Vector2 ori);
	void addAnimation(Animation* anm);
	void updateAnimation(float deltaTime, int type);
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
	Vector2 getOrigin() {
		return origin;
	}
	Animation * getAnimation(int type) {
		return m_anim[type];
	}
};

