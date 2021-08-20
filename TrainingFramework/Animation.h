#pragma once
#include "Vertex.h"
#include <vector>
#include "../Utilities/Math.h"

enum AnimationType {
	base,
	Idle,
	Run,
	Jump,
	RunJump,
	Falling,
	Gun0,
	Gun1,
	Gun2,
	Gun3,
	Wound,
	dead
};

class Animation
{
public:
	Animation(const char* filePath);
	Animation(Animation * anim);
	~Animation();

	void play(GLuint* vbo, Vector2 Tsize, Vector2 origin, float deltaTime, bool revert = false);
	void playDead(GLuint* vbo, Vector2 Tsize, Vector2 origin, float deltaTime);

	void setAnimationSpeed(float newSpeed);
	void setID(int id);
	int GetID(){
		return m_ID;
	}
	void resetAnimation();
	Vector4 getTexture();
	void setFire(int fire);
	bool isGun;
	bool isJump;
	int i_frame_count;
	int i_total_frame;
	std::vector<Vector4> m_animation;
private:
	double d_anim_cursor;
	int i_current_frame_index;
	float f_speed;
	int m_ID;
	int m_fire_pos;
};

