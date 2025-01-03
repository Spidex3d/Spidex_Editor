#pragma once
#include <glad\glad.h>
#include "BaseModel.h"
#include <string>
class DefaultModels : public BaseModel {

public:
	

	
	static DefaultModels* Instance();

	//void TriangleModel(int idx, const std::string& name);
	void TriangleModel();


	void RenderImGui() const override;
	void DrawTriangle();


private:
	GLuint VAO, VBO;

};

