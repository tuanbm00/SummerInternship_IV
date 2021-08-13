#pragma once
#include "Singleton.h"
#include "Texture.h"
#include "Shaders.h"
#include "Globals.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../Utilities/Math.h"
#include "../Utilities/utilities.h"
#include "Vertex.h"

class TextManager : public Singleton<TextManager>
{
public:
	TextManager();
	~TextManager();
	// shader used for text rendering
	Shaders m_TextShader;
	// pre-compiles
	void Initialize();
	// renders a string of text
	void RenderString(const char* text, Vector4 color, float x, float y, float scaleX, float scaleY);

private:
	FT_Library m_ft;
	FT_Face m_face;
	FT_GlyphSlot m_glyphSlot;
	GLuint m_VBO, m_Texture;
};