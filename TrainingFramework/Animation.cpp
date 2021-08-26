#include "Animation.h"
#include "stdafx.h"
#include "Camera.h"

Animation::Animation(const char* filePath)
{
	isGun = false;
	isJump = false;
	f_speed = 0;
	d_anim_cursor = 0;
	i_current_frame_index = 0;
	FILE* fp;
	fopen_s(&fp, filePath, "r");
	int cnt, total;
	fscanf_s(fp, "%d %d\n", &total, &cnt);
	i_frame_count = cnt;
	i_total_frame = total;
	m_animation.resize(total);
	for (int i = 0; i < total; i++) {
		int x, y, w, h;
		fscanf_s(fp, "%d,%d,%d,%d\n", &x, &y, &w, &h);
		m_animation[i] = Vector4(x, y, w, h);
	}
	fclose(fp);
}
Animation::Animation(Animation* anim) {
	isGun = false;
	isJump = false;
	f_speed = anim->f_speed;
	d_anim_cursor = 0;
	i_current_frame_index = 0;
	i_frame_count = anim->i_frame_count;
	i_total_frame = anim->i_total_frame;
	m_animation.resize(i_total_frame);
	for (int i = 0; i < i_total_frame; i++) m_animation[i] = anim->m_animation[i];
}

Animation::~Animation()
{
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
	i_current_frame_index = 0;
	d_anim_cursor = 0;
}
Vector4 Animation::getTexture()
{
	Vector4 frame = m_animation[0];
	return frame;
}
void Animation::setFire(int fire) {
	m_fire_pos = fire;
	if (fire != -1) isGun = true;
}

void Animation::play(GLuint* vbo, Vector2 Tsize, Vector2 origin, float deltaTime, bool revert) {
	if (isJump) {
		int lop = 1;
		int piece = 30 / i_frame_count;
		piece++;
		lop *= piece;
		d_anim_cursor += 1;
		if (d_anim_cursor > lop) {
			i_current_frame_index = (i_current_frame_index + 1) % i_frame_count;
			d_anim_cursor = 0;
		}
	}
	else {
		d_anim_cursor += deltaTime;
		if (d_anim_cursor > f_speed) {
			i_current_frame_index = (i_current_frame_index + 1) % i_frame_count;
			if (i_current_frame_index < 3 && m_fire_pos == -2) i_current_frame_index = 3 % i_frame_count;
			if (i_current_frame_index == m_fire_pos) Camera::GetInstance()->is_shoot = true;
			d_anim_cursor = 0;
		}
	}
	if (revert) {
		if (i_current_frame_index < i_frame_count) i_current_frame_index += i_frame_count;
	}
	Vector4 frame = m_animation[i_current_frame_index];
	float x = frame.x, y = frame.y, w = frame.z, h = frame.w;
	if (revert && w > 200) {
		origin.x += 96;
	}
	if (h == 150) {
		origin.y += 50;
		if (revert) origin.x += 10;
	}
	Vertex verticesData[4];
	Vector3 delta = Vector3(origin.x - w / 2, origin.y - h / 2, 0.0);
	verticesData[0].pos = Vector3(-(float)w / 2, -(float)h / 2, 0.0f) - delta;
	verticesData[1].pos = Vector3((float)w / 2, -(float)h / 2, 0.0f) - delta;
	verticesData[2].pos = Vector3(-(float)w / 2, (float)h / 2, 0.0f) - delta;
	verticesData[3].pos = Vector3((float)w / 2, (float)h / 2, 0.0f) - delta;

	x /= Tsize.x;
	y /= Tsize.y;
	w /= Tsize.x;
	h /= Tsize.y;

	verticesData[0].uv = Vector2(x, y + h);
	verticesData[1].uv = Vector2(x + w, y + h);
	verticesData[2].uv = Vector2(x, y);
	verticesData[3].uv = Vector2(x + w, y);


	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Animation::playDead(GLuint * vbo, Vector2 Tsize, Vector2 origin, float deltaTime)
{
	if (Camera::GetInstance()->is_dead) i_current_frame_index = i_frame_count - 1;
	else {
		d_anim_cursor += deltaTime;
		if (d_anim_cursor > f_speed) {
			++i_current_frame_index;
			if (i_current_frame_index == i_frame_count - 1) {
				Camera::GetInstance()->is_dead = true;
			}
			d_anim_cursor = 0;
			if (i_current_frame_index >= i_frame_count - 1) {
				i_current_frame_index = 0;
			}
		}
	}

	Vector4 frame = m_animation[i_current_frame_index];
	float x = frame.x, y = frame.y, w = frame.z, h = frame.w;
	Vertex verticesData[4];
	Vector3 delta = Vector3(origin.x - w / 2, origin.y - h / 2, 0.0);
	verticesData[0].pos = Vector3(-(float)w / 2, -(float)h / 2, 0.0f) - delta;
	verticesData[1].pos = Vector3((float)w / 2, -(float)h / 2, 0.0f) - delta;
	verticesData[2].pos = Vector3(-(float)w / 2, (float)h / 2, 0.0f) - delta;
	verticesData[3].pos = Vector3((float)w / 2, (float)h / 2, 0.0f) - delta;

	x /= Tsize.x;
	y /= Tsize.y;
	w /= Tsize.x;
	h /= Tsize.y;

	verticesData[0].uv = Vector2(x, y + h);
	verticesData[1].uv = Vector2(x + w, y + h);
	verticesData[2].uv = Vector2(x, y);
	verticesData[3].uv = Vector2(x + w, y);


	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
