#pragma once
#include "../Utilities/utilities.h"

class Shaders 
{
private:
	
public:
	int m_Depth_Test, m_Cull_Face, m_Blend;
	Shaders(int ID, char* fileVertexShader, char* fileFragmentShader,int depth_Test = 0, int cull_Face = 0, int Blend = 0);
	~Shaders();

	GLuint program, vertexShader, fragmentShader;
	int m_Id;
	char m_fileVS[250];
	char m_fileFS[250];
	GLint m_aPosition, m_aCPosition;
	GLint m_aColor;
	GLint m_aUV;
	GLint m_uTexture, m_uWVP, m_uCWVP, m_uCubeTexture, m_uDistanceToCamera;
	GLint m_uBlendTexture, m_uHeightmap, m_uTexture1, m_uTexture2, m_uTexture3;
	GLint m_uTextures[5];

	GLint m_uTime;

	int Init();
	

	int GetID() { return m_Id; }
};