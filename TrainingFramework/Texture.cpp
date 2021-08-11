#include "stdafx.h"
#include "Texture.h"
#include "../Utilities/utilities.h"

Texture::Texture(int ID, char* srcTexture)
{
	m_Id = ID;
	strcpy(m_srcTexture, srcTexture);
}

Texture::~Texture() {

}

void Texture::Init() {
	
}

void Texture::BufferTexture() {
	int iWidth, iHeight, iBpp;
	char* imageData = LoadTGA(m_srcTexture, &iWidth, &iHeight, &iBpp);
//	printf("loaded image %dx%d, %d, %s\n", iWidth, iHeight, iBpp, m_srcTexture);
	if (iBpp == 24)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		delete[] imageData;
	}
	else if (iBpp == 32)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		delete[] imageData;
	}
	else printf("load failed!\n");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

}

char* Texture::GetSource() {
	return m_srcTexture;
}
