#pragma once
class Texture
{
private:
	char m_srcTexture[250];
	int m_Id;

public:
	Texture(int ID, char* srcTexture);
	~Texture();

	void Init();
	void BufferTexture();
	int GetID() { return m_Id; }
	char* GetSource();
};
