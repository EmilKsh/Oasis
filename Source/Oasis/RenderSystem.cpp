#include "RenderSystem.h"

//---------------------------- Variables and Objects declaration


int WindowSize[2] = { 1280,720 };

bool left_mouse_button, right_mouse_button, firstMouse{true};
float g_xpos, g_ypos, wc_x, wc_y, Scale_x = 0.05f, Scale_y = Scale_x;
float LastMPx{ WindowSize[0] / 2.0f }, LastMPy{ WindowSize[1] / 2.0f }, DeltaMPx{ 0.0f }, DeltaMPy{ 0.0f };
float pitch{ 0.0f }, roll{ 0.0f }, yaw{ -90.0f };
glm::vec3 direction;

struct KeyFlags
{
	bool UpKeyPressed{ false };
	bool LeftKeyPressed{ false };
	bool RightKeyPressed{ false };
	bool DownKeyPressed{ false };
};


KeyFlags flags;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//--------------------------------------------------- CallBacks 
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void RenderSystem::process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	const float cameraSpeed = 3.0f*getDeltaTime(); // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += cameraUp * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= cameraUp * cameraSpeed;
}

void cursor_pos_callBack(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		LastMPx = xpos;
		LastMPy = ypos;
		firstMouse = false;
	}

	DeltaMPx = xpos - LastMPx;
	DeltaMPy = LastMPy - ypos;
	LastMPx = xpos;
	LastMPy = ypos;

	const float sensitivity = 0.1f;
	DeltaMPx *= sensitivity;
	DeltaMPy *= sensitivity;

	pitch += DeltaMPy;
	yaw += DeltaMPx;

	if (pitch > 180.0f)
		pitch = 180.0f;
	else if (pitch < -180)
		pitch = -180;
	if (yaw > 180.0f)
		yaw = 180.0f;
	else if (yaw < -180)
		yaw = -180;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (!(flags.UpKeyPressed && flags.RightKeyPressed && flags.LeftKeyPressed && flags.DownKeyPressed))
	{
		if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		{
			flags.UpKeyPressed = true;
		}
		else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			flags.LeftKeyPressed = true;
		}
		else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		{
			flags.RightKeyPressed = true;
		}
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		{
			flags.DownKeyPressed = true;
		}
	}
	else
	{
		if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		{
			flags.UpKeyPressed = false;
		}
		else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		{
			flags.LeftKeyPressed = false;
		}
		else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		{
			flags.RightKeyPressed = false;
		}
		else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		{
			flags.DownKeyPressed = false;
		}
	}
}

void mouse_clicked(GLFWwindow* window, int button, int action, int mod)
{
	left_mouse_button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
	right_mouse_button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
}


RenderSystem::RenderSystem(int window_size[2]) : WindowSize{ window_size[0], window_size[1] }
{
	
	glfwInit();

	// setting window hints aka OpenGL version and profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// setting up the window and error handling
	window = glfwCreateWindow(WindowSize[0], WindowSize[1], "Pathfinder", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "window failed to Initialize";
		//return -1;
	}

	// setting the window as OpendGl's current context
	glfwMakeContextCurrent(window);

	//Turning VSync Off! :/
	glfwSwapInterval(0);

	// glad loading error handling
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		//return -1;
	}

	// CallBacks -----------------------------------------------
	// updating viewport size if window size is changed CallBack
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callBack);
	glfwSetMouseButtonCallback(window, mouse_clicked);
	glfwSetKeyCallback(window, key_callback);


}

int RenderSystem::GLFWInit() 
{
	// GLFW initialization ----------------------------------------
	glfwInit();

	// setting window hints aka OpenGL version and profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// setting up the window and error handling
	window = glfwCreateWindow(WindowSize[0], WindowSize[1], "Pathfinder", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "window failed to Initialize";
		return -1;
	}

	// setting the window as OpendGl's current context
	glfwMakeContextCurrent(window);

	//Turning VSync Off! :/
	glfwSwapInterval(0);

	// glad loading error handling
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// CallBacks -----------------------------------------------
	// updating viewport size if window size is changed CallBack
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callBack);
	glfwSetMouseButtonCallback(window, mouse_clicked);
	glfwSetKeyCallback(window, key_callback);
}

int RenderSystem::FreeTypeInit() 
{
	// FreeType Init -------------------------------------------
	//FT_Library ftlib;

	//if (FT_Init_FreeType(&ftlib))
	//{
	//	cout << "ERROR::FreeType: FreeType library failed to initialize." << endl;
	//	return -1;
	//}

	//FT_Face ftface;

	//if (FT_New_Face(ftlib, "fonts/arial.ttf", 0, &ftface))
	//{
	//	cout << "ERROR::FreeType: Failed to load font." << endl;
	//	return -1;
	//}
	return -1;
}

int RenderSystem::RenderTheQueue()
{
	glEnable(GL_DEPTH_TEST);
	
	while (!glfwWindowShouldClose(window))
	{
		// GLFW background setup

		process_input(window);
		//Camera

		glfwSwapBuffers(window);
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if (!RenderQueue.empty())
		{
			for (GraphicalObj* GObj : RenderQueue)
			{
				CalcDeltaTime();

				// Manual Camera Translation
				defaultCam.setTransform(cameraPos, cameraFront, cameraUp);
				glm::mat4 view;
				view = defaultCam.getView();
				GObj->getShader()->set4mat("view", view);
				GObj->DrawShape(color.White);
			}
		}
	}

	// Unbinding and closing all glfw windows and clearing opbjects
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glfwTerminate();
	return 0;
}

void RenderSystem::AddToQueue(GraphicalObj* Obj) 
{
	RenderQueue.push_back(Obj);
}

void RenderSystem::CalcDeltaTime() 
{
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

float RenderSystem::getDeltaTime() 
{
	return deltaTime;
}