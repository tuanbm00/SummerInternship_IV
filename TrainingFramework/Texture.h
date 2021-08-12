#pragma once
#include <GLES2/gl2.h>
class Texture
{
private:
	char m_srcTexture[250];
	int m_Id;

public:
	Texture(int ID, char* srcTexture);
	~Texture();
	GLuint mTextureId;
	void Init();
	int GetID() { return m_Id; }
	char* GetSource();
};
