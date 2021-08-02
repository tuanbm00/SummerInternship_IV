#pragma once
#include <iostream>
#include <vector>
class CubeTextures
{
private:
	int m_Id;

public:
	CubeTextures(int ID, std::vector<std::string>Faces);
	~CubeTextures();
	std::vector<std::string> faces;
	void Init();
	void BufferTexture();
	int GetID() { return m_Id; }
};

