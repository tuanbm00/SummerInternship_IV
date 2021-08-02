#include "stdafx.h"
#include "CubeTextures.h"
#include "../Utilities/utilities.h"

CubeTextures::CubeTextures(int ID, std::vector<std::string>Faces)
{
	m_Id = ID;
	faces = Faces;
}

CubeTextures::~CubeTextures() {

}

void CubeTextures::Init() {
}

void CubeTextures::BufferTexture() {
	//TEXTURE
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		char* data = LoadTGA(faces[i].c_str(), &width, &height, &nrChannels);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			delete[] data;
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			delete[] data;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, 0x812f);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

}
