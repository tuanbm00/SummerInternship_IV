#pragma once
#include <fstream>
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Animation.h"
#include <vector>

class Model
{
private:
	int m_NumberOfVertices = 0;
	int m_NumberOfIndices = 0;
	
	std::vector<Animation*> m_anim;;
	int m_Id;
	Vector2 origin;
	
public:
	float m_textureW, m_textureH, m_posX, m_posY, m_spriteW, m_spriteH;
	GLuint vboId;
	bool b_IsAnimation;
	Model();
	Model(Model * model);
	~Model();
	void InitSprite(float spriteX, float SpriteY, float SpriteW, float SpriteH, float TextureW, float TextureH);
	void setOrigin(Vector2 ori);
	void addAnimation(Animation* anm);
	void updateAnimation(float deltaTime, int type);
	void updateDead(float deltaTime);
	int GetNumberofIndices() {
		return m_NumberOfIndices;
	}
	int GetID() { return m_Id; }
	Vector2 getOrigin() {
		return origin;
	}
	Animation * getAnimation(int type) {
		return m_anim[type];
	}
	void resetGun();
	void resetTexture();
	std::vector<Animation*> getAnim() {
		return m_anim;
	}
};

