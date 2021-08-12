#include "Animation.h"
#include "stdafx.h"

Animation::Animation(const char* filePath)
{
	isJump = false;
	f_speed = 0;
	d_anim_cursor = 0;
	i_current_frame_index = 0;
	FILE* fp = fopen(filePath, "r");
	int cnt, total;
	fscanf(fp, "%d %d\n", &total, &cnt);
	i_frame_count = cnt;
	m_animation = new Vector4[total];
	for (int i = 0; i < total; i++) {
		int x, y, w, h;
		fscanf(fp, "%d,%d,%d,%d\n", &x, &y, &w, &h);
		m_animation[i] = Vector4(x, y, w, h);
	}
	fclose(fp);
}

Animation::~Animation()
{
	delete[] m_animation;
}

Vector4 Animation::play(float deltaTime, bool revert)
{
	if (isJump) {
		int lop = deltaTime / 0.003f;
		int piece = 25 / i_frame_count;
		lop *= piece;
		d_anim_cursor += 1;
		if (d_anim_cursor > lop) {
			i_current_frame_index = (i_current_frame_index + 1) % i_frame_count;
			d_anim_cursor = 0;
		}
		if (revert) {
			if (i_current_frame_index < i_frame_count) i_current_frame_index += i_frame_count;
		}
		Vector4 frame = m_animation[i_current_frame_index];
		return frame;
	}
	d_anim_cursor += deltaTime;
	if (d_anim_cursor > f_speed) {
		i_current_frame_index = (i_current_frame_index + 1) % i_frame_count;
		d_anim_cursor = 0;
	}
	if (revert) {
		if (i_current_frame_index < i_frame_count) i_current_frame_index += i_frame_count;
	}
	Vector4 frame = m_animation[i_current_frame_index];
	return frame;
}

void Animation::setAnimationSpeed(float newSpeed)
{
	f_speed = newSpeed;
}

void Animation::setID(int id)
{
	m_ID = id;
}
void Animation::resetAnimation()
{
	isJump = true;
	i_current_frame_index = 0;
	d_anim_cursor = 0;
}
