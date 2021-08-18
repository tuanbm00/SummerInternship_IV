#include "stdafx.h"
#include "TextManager.h"

void TextManager::Initialize() {
	glGenBuffers(1, &m_VBO);

	if (FT_Init_FreeType(&m_ft))
	{
		printf("Could not init freetype library\n");
		return;
	}
	if (FT_New_Face(m_ft, "../Resources/Fonts/arial.ttf", 0, &m_face)) {
		printf("Could not open font arial.ttf\n");
		return;
	}
	FT_Set_Pixel_Sizes(m_face, 0, 48);
	m_glyphSlot = m_face->glyph;

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_TextShader.SetFileVS("../Resources/Shaders/TextShaderVS.vs");
	m_TextShader.SetFileFS("../Resources/Shaders/TextShaderFS.fs");
	m_TextShader.Init();
}
void TextManager::RenderString(const char* text, Vector4 color, float x, float y, float scaleX, float scaleY) {
	// use glProgram, bind texture & pass color uniform here
	glUseProgram(m_TextShader.program);
	glUniform4f(glGetUniformLocation(m_TextShader.program, "u_color"), color.x, color.y, color.z, color.w);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	float sx = 1.0f / Globals::screenWidth * scaleX;
	float sy = 1.0f / Globals::screenHeight * scaleY;
	x = -1.0f + 2.0f * x / Globals::screenWidth;
	y = -1.0f + 2.0f * y / Globals::screenHeight;
	for (const char* p = text; *p; p++)
	{
		if (FT_Load_Char(m_face, *p, FT_LOAD_RENDER))
		{
			printf("ERROR::FREETYTPE: Failed to load Glyph\n");
			continue;
		}
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			m_glyphSlot->bitmap.width,
			m_glyphSlot->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			m_glyphSlot->bitmap.buffer
		);
		float x2 = x + m_glyphSlot->bitmap_left * sx;
		float y2 = -y - m_glyphSlot->bitmap_top * sy;
		float w = m_glyphSlot->bitmap.width * sx;
		float h = m_glyphSlot->bitmap.rows * sy;
		GLfloat box[4][4] = {
		{x2, -y2 , 0, 0},
		{x2 + w, -y2 , 1, 0},
		{x2, -y2 - h, 0, 1},
		{x2 + w, -y2 - h, 1, 1},
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (m_glyphSlot->advance.x >> 6) * sx;
		y += (m_glyphSlot->advance.y >> 6) * sy;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

TextManager::TextManager()
{
}

TextManager::~TextManager()
{
}