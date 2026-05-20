#pragma once
#include<chrono>
#include<thread>
#define _USE_MATH_DEFINES
#include<math.h>
#include"RenderSystem.h"
#include"Model.h"
#include"IOModel.h"
#include"Texture.h"

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
	float timer{ 0.f };
	Colors color;

	// RenderSystem definition also initializes OpenGL so any any OpenGL functions should be used after the init
	RenderSystem RS(window);

	Shader basicShader;
	basicShader.Init("./Source/Shaders/Shader");
	Model suzanneMesh = LoadObjModel("./Meshes/Suzanne.obj");
	GraphicalObj  suzanneGraphicalObj;
	suzanneGraphicalObj.SetShader(basicShader);
	suzanneGraphicalObj.VertexUpdate(&suzanneMesh.vertexBuffer, &suzanneMesh.indices);
	suzanneGraphicalObj.transform(glm::vec3(1.f,1.f,1.f), glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f,0.f,0.f));

	Shader testShader;
	testShader.Init("./Source/Shaders/Shader");
	Texture testTexture;
	testTexture.LoadFromFile("./Textures/TestTexture.jpg");
	GraphicalObj GroundPlane;
	GroundPlane.SetShader(testShader);
	GroundPlane.SetTexture(testTexture);
	GroundPlane.BufferUpdate();
	GroundPlane.transform(glm::vec3(10.f, 10.f, 10.f), glm::vec3(0.f, -1.f, -0.f), glm::vec3(1.f, 0.f, 0.f) * glm::radians(90.f));

	RS.AddToQueue(&GroundPlane);
	RS.AddToQueue(&suzanneGraphicalObj);

	while (RS.CheckWindowClosureStatus())
	{
		suzanneGraphicalObj.transform(glm::vec3(1.f), glm::vec3(0.f), timer * glm::vec3(0.f, 1.f, 0.f) * 20.f);
		RS.RenderTheQueue();
		timer += RS.GetDeltaTime();
	}
	
	return 0;
}
