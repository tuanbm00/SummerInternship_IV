#include "stdafx.h"
#include "Shaders.h"

Shaders::Shaders(int ID, char* fileVertexShader, char* fileFragmentShader, int depth_Test, int cull_Face, int Blend)
{
	m_Id = ID;
	strcpy_s(m_fileVS, sizeof m_fileVS, fileVertexShader);
	strcpy_s(m_fileFS, sizeof m_fileFS, fileFragmentShader);
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

	//Texture
	m_uTextures[0] = glGetUniformLocation(program, "u_Texture1");
	
	
	return 0;
}
