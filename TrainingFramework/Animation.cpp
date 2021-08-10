#include "Animation.h"
#include "stdafx.h"

Animation::Animation(const char* filePath)
{
	f_speed = 0;
	d_anim_cursor = 0;
	i_current_frame_index = 0;
	FILE* fp = fopen(filePath, "r");
	if (fp == NULL) {
		printf("Can not open animation file! %s\n", filePath);
	}
	int cnt;
	fscanf(fp, "%d\n", &cnt);
	i_frame_count = cnt;
	m_animation = new Vector4[i_frame_count];
	for (int i = 0; i < i_frame_count; i++) {
		int x, y, w, h;
		fscanf(fp, "%d,%d,%d,%d\n", &x, &y, &w, &h);
		m_animation[i] = Vector4(x, y, w, h);
	}
	printf("loaded animation %s\n", filePath);
	fclose(fp);
}

Animation::~Animation()
{
	delete[] m_animation;
}

void Animation::play(GLuint* vbo, Vector2 Tsize, Vector2 origin, float deltaTime)
{
	d_anim_cursor += deltaTime;
	if (d_anim_cursor > f_speed) {
		i_current_frame_index = (i_current_frame_index + 1) % i_frame_count;
		d_anim_cursor = 0;
	}
	Vector4 frame = m_animation[i_current_frame_index];
	float x = frame.x, y = frame.y, w = frame.z, h = frame.w;
	Vertex *  verticesData = new Vertex[4];
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
	delete[] verticesData;
}

void Animation::setAnimationSpeed(float newSpeed)
{
	f_speed = newSpeed;
}

void Animation::setID(int id)
{
	m_ID = id;
}
