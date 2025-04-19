#pragma once

#include<glad/glad.h>


class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint mVAO, mVBO, mEBO;
	GLsizei indexCount;


};
