#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <vector>

class Grid
{
public:
	static Grid* Instance();

	std::vector<float> createGridVertices(float size, int divisions);

    void gridSetUp();

    void gridRender();

private:
    GLuint GridVAO, GridVBO;
    std::vector<float> gridVertices;
};

