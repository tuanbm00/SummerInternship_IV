#include "stdafx.h"
#include "Texture.h"
#include "../Utilities/utilities.h"

Texture::Texture(int ID, char* srcTexture)
{
	m_Id = ID;
	strcpy_s(m_srcTexture,sizeof m_srcTexture, srcTexture);
}

Texture::~Texture() {
	glDeleteTextures(1, &mTextureId);
}

void Texture::Init() {
	int width = 0;
	int height = 0;
	int bpp = 0;
	const char* imageData = LoadTGA(m_srcTexture, &width, &height, &bpp);
	GLenum format = (bpp == 24 ? GL_RGB : GL_RGBA);

	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
	delete[] imageData;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	
}

char* Texture::GetSource() {
	return m_srcTexture;
}
