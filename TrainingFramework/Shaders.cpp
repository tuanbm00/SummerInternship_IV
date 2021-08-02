#include "stdafx.h"
#include "Shaders.h"

Shaders::Shaders(int ID, char* fileVertexShader, char* fileFragmentShader, int depth_Test, int cull_Face, int Blend)
{
	m_Id = ID;
	strcpy(m_fileVS, fileVertexShader);
	strcpy(m_fileFS, fileFragmentShader);
	m_Depth_Test = depth_Test;
	m_Cull_Face = cull_Face;
	m_Blend = Blend;
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

int Shaders::Init()
{

	vertexShader = esLoadShader(GL_VERTEX_SHADER, m_fileVS);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, m_fileFS);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	m_aPosition = glGetAttribLocation(program, "a_Position");
	m_aColor = glGetAttribLocation(program, "a_color");
	m_aUV = glGetAttribLocation(program, "a_uv");
	m_uWVP = glGetUniformLocation(program, "u_WVP");
	m_uDistanceToCamera = glGetUniformLocation(program, "u_DistanceToCamera");
	m_uTime = glGetUniformLocation(program, "u_Time");

	//Cube
	m_aCPosition = glGetAttribLocation(program, "a_CPosition");
	m_uCWVP = glGetUniformLocation(program, "u_CWVP");
	m_uCubeTexture = glGetUniformLocation(program, "u_samplerCubeMap");

	//Texture
	m_uTextures[0] = glGetUniformLocation(program, "u_Texture1");
	m_uTextures[1] = glGetUniformLocation(program, "u_Texture2");
	m_uTextures[2] = glGetUniformLocation(program, "u_Texture3");
	m_uTextures[3] = glGetUniformLocation(program, "u_BlendTexture");
	m_uTextures[4] = glGetUniformLocation(program, "u_Heightmap");
	
	return 0;
}
