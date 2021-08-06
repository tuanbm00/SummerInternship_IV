#pragma once
#include "Vertex.h"
#include "../Utilities/Math.h"

enum AnimationType {
	Idle,
	RunFW,
	RunBW,
	Jump
};

class Animation
{
public:
	Animation(const char* filePath);
	~Animation();

	void play(GLuint * vbo, Vector2 Tsize, Vector2 origin, float deltaTime);

	void setAnimationSpeed(float newSpeed);
private:
	double d_anim_cursor;
	int i_current_frame_index;
	int i_frame_count;
	float f_speed;
	Vector4* m_animation;
};

