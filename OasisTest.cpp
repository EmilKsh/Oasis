#pragma once
#include<chrono>
#include<thread>
#define _USE_MATH_DEFINES
#include<math.h>
#include"RenderSystem.h"
#include"Model.h"

//#include<ft2build.h>
//#include FT_FREETYPE_H

using std::cout, std::endl, std::vector, std::array;
using namespace std::this_thread;
using namespace std::chrono;


// Cube Coordinates
vector<float> vertices = {
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

Model parseObj(string filePath = "");

//-------------------------------------------------------- Main 
int main()
{
	int window[2]{ 1080, 1080 };
	Colors color;

	// RenderSystem definition also initializes OpenGL so any any OpenGL functions should be used after the init
	RenderSystem RS(window);

	Shader TestShader;
	TestShader.AddTexture("./Textures/TestTexture.jpg");

	Model suzanneMesh = parseObj("./Meshes/Suzanne.obj");
	GraphicalObj  suzanneGraphicalObj;
	suzanneGraphicalObj.SetShader(&TestShader);
	suzanneGraphicalObj.VertexUpdate(&suzanneMesh.vertexBuffer, &suzanneMesh.indices);
	suzanneGraphicalObj.BufferUpdate();
	suzanneGraphicalObj.transform(glm::vec3(1.f,1.f,1.f), glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f,0.f,0.f));

	GraphicalObj GroundPlane;
	GroundPlane.SetShader(&TestShader);
	GroundPlane.BufferUpdate();
	GroundPlane.transform(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f, 0.f, 0.f) * glm::radians(90.f));

	RS.AddToQueue(&GroundPlane);
	//RS.AddToQueue(&suzanneGraphicalObj);
	RS.RenderTheQueue();

	return 0;
}