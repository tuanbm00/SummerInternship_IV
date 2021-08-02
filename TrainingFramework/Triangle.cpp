#include "stdafx.h"
//#include "Triangle.h"
//#include "Vertex.h"
//
//Triangle::Triangle(float X, float Y, float Z, float M){
//	x = X, y = Y, z = Z, m = M;
//}
//
//Triangle::~Triangle() {
//}
//
//
//int Triangle::Init() {
//	deltaX = x;
//
//	//triangle data (heap)
//	Vertex* verticesData = new Vertex[3];
//	verticesData[0].pos.x = x;  verticesData[0].pos.y = y;  verticesData[0].pos.z = z; verticesData[0].uv.x = 0.5f;  verticesData[0].uv.y = 1.0f; //verticesData[0].color.x = 1.0f;  verticesData[0].color.y = 0.0f;  verticesData[0].color.z = 0.0f; 
//	verticesData[1].pos.x = x-0.4f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z = z;  verticesData[1].uv.x = 0.0f;  verticesData[1].uv.y = 0.0f;//verticesData[1].color.x = 0.0f;  verticesData[1].color.y = 1.0f;  verticesData[1].color.z = 0.0f; verticesData[1].uv.x = 0.0f;  verticesData[1].uv.y = 0.0f;
//	verticesData[2].pos.x = x+0.4f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z = z;  verticesData[2].uv.x = 1.0f;  verticesData[2].uv.y = 0.0f; //verticesData[2].color.x = 0.0f;  verticesData[2].color.y = 0.0f;  verticesData[2].color.z = 1.0f; verticesData[2].uv.x = 1.0f;  verticesData[2].uv.y = 0.0f;
//	
//	//buffer
//	glGenBuffers(1, &vboId);
//	glBindBuffer(GL_ARRAY_BUFFER, vboId);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, verticesData, GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//
//	delete[] verticesData;
//
//	//buffer object
//	int *indices = new int[3];
//	for (int i = 0; i < 3; i++) {
//		indices[i] = i;
//	}
//
//	glGenBuffers(1, &iboId);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 3, indices, GL_STATIC_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	delete[] indices;
//
//	//texture
//	glGenTextures(1, &textureId);
//	glBindTexture(GL_TEXTURE_2D, textureId);
//	int iWidth, iHeight, iBpp;
//	char* imageData = LoadTGA("..//Resources//Textures//Woman1.tga", &iWidth, &iHeight, &iBpp);
//	if (iBpp == 24)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
//		delete[] imageData;
//	}
//	else if (iBpp == 32)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
//		delete[] imageData;
//	}
//	else printf("load failed!\n");
//
//
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//
//
//	//creation of shaders and program 
//	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
//}
//
//void Triangle::Draw() {
//
//	glUseProgram(myShaders.program);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vboId);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
//
//	if (myShaders.positionAttribute != -1)
//	{
//		glEnableVertexAttribArray(myShaders.positionAttribute);
//		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//	}
//
//	//Color
//	//if (myShaders.colorAttribute != -1)
//	//{
//	//	glEnableVertexAttribArray(myShaders.colorAttribute);
//	//	glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
//	//}
//
//	if (myShaders.uvAttribute != -1)
//	{
//		glEnableVertexAttribArray(myShaders.uvAttribute);
//		glVertexAttribPointer(myShaders.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
//	}
//
//	//Setting Texture Uniform
//	glBindTexture(GL_TEXTURE_2D, textureId);
//	glUniform1i(myShaders.textureUniform, 0);
//
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
//	//glDrawArrays(GL_TRIANGLES, 0, 3);
//
//
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//void Triangle::CleanUp()
//{
//	glDeleteBuffers(1, &vboId);
//	glDeleteBuffers(1, &iboId);
//	glDeleteBuffers(1, &textureId);
//	//delete this;
//}
