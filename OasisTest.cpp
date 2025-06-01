#pragma once
#include<chrono>
#include<thread>
#define _USE_MATH_DEFINES
#include<math.h>
#include"RenderSystem.h"
#include"Model.h"
#include"IOModel.h"

//#include<ft2build.h>
//#include FT_FREETYPE_H

using std::cout, std::endl, std::vector, std::array;
using namespace std::this_thread;
using namespace std::chrono;

using model::Model, IOModel::LoadObjModel, render_system::RenderSystem;

//-------------------------------------------------------- Main 
int main()
{
	int window[2]{ 1080, 1080 };
	Colors color;

	// RenderSystem definition also initializes OpenGL so any any OpenGL functions should be used after the init
	RenderSystem RS(window);

	Shader TestShader;
	TestShader.AddTexture("./Textures/TestTexture.jpg");

	Model suzanneMesh = LoadObjModel("./Meshes/Suzanne.obj");
	GraphicalObj  suzanneGraphicalObj;
	suzanneGraphicalObj.SetShader(&TestShader);
	suzanneGraphicalObj.VertexUpdate(&suzanneMesh.vertexBuffer, &suzanneMesh.indices);
	suzanneGraphicalObj.BufferUpdate();
	//suzanneGraphicalObj.transform(glm::vec3(1.f,1.f,1.f), glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f,0.f,0.f));

	/*GraphicalObj cube;
	cube.SetShader(&TestShader);
	cube.VertexUpdate(&vertices);
	cube.transform();*/

	RS.AddToQueue(&suzanneGraphicalObj);
	RS.RenderTheQueue();

	return 0;
}