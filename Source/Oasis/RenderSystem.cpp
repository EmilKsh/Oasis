#include "RenderSystem.h"

namespace render_system
{
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
	void RenderSystem::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void RenderSystem::ProcessInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		const float cameraSpeed = 100.0f * getDeltaTime(); // adjust accordingly
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

	void RenderSystem::cursor_pos_callBack(GLFWwindow* window, double xpos, double ypos)
	{
		RenderSystem* self = (RenderSystem*)(glfwGetWindowUserPointer(window));

		if (self->firstMouse)
		{
			self->LastMPx = xpos;
			self->LastMPy = ypos;
			self->firstMouse = false;
		}

		self->DeltaMPx = xpos - self->LastMPx;
		self->DeltaMPy = self->LastMPy - ypos;
		self->LastMPx = xpos;
		self->LastMPy = ypos;

		const float sensitivity = 0.1f;
		self->DeltaMPx *= sensitivity;
		self->DeltaMPy *= sensitivity;

		self->pitch += self->DeltaMPy;
		self->yaw += self->DeltaMPx;

		if (self->pitch > 180.0f)
			self->pitch = 180.0f;
		else if (self->pitch < -180)
			self->pitch = -180;
		if (self->yaw > 180.0f)
			self->yaw = 180.0f;
		else if (self->yaw < -180)
			self->yaw = -180;

		self->direction.x = cos(glm::radians(self->yaw)) * cos(glm::radians(self->pitch));
		self->direction.y = sin(glm::radians(self->pitch));
		self->direction.z = sin(glm::radians(self->yaw)) * cos(glm::radians(self->pitch));
		cameraFront = glm::normalize(self->direction);
	}

	void RenderSystem::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

	void RenderSystem::mouse_clicked(GLFWwindow* window, int button, int action, int mod)
	{
		RenderSystem* self = (RenderSystem*)(glfwGetWindowUserPointer(window));

		self->left_mouse_button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
		self->right_mouse_button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
	}


	RenderSystem::RenderSystem(int window_size[2]) : WindowSize{ window_size[0], window_size[1] }
	{
		LastMPx = WindowSize[0] / 2.0f;
		LastMPy = WindowSize[1] / 2.0f;

		aspecRatio = float(window_size[0]) / window_size[1];

		GLFWInit();
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		defaultShader.Init("./Oasis/Source/Shaders/SimpleShader");
	}

	RenderSystem::~RenderSystem() 
	{
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
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
			printf("GLFW initialization failed: window not created");			
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
		}

		glfwSetWindowUserPointer(window, this);
		// CallBacks -----------------------------------------------
		// updating viewport size if window size is changed CallBack

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, cursor_pos_callBack);
		glfwSetMouseButtonCallback(window, mouse_clicked);
		glfwSetKeyCallback(window, key_callback);
		glEnable(GL_DEPTH_TEST);
		
		return 0;
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

	bool RenderSystem::CheckWindowClosureStatus() {
		return !glfwWindowShouldClose(window);
	}

	void RenderSystem::UpdateWindow() 
	{
		ProcessInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderSystem::RenderTheQueue()
	{
		// GLFW background setup

		ProcessInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
		glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
		//RenderQueue[1]->transform(glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.0f, 0.f) * glm::radians(totalTime*1000));
		pointLight.position.x = 3.f * cos(totalTime * 100.f);
		pointLight.position.z = -1.f * sin(totalTime * 100.f);

		if (!RenderQueue.empty())
		{
			for (GraphicalObj* GObj : RenderQueue)
			{
				CalcDeltaTime();

				// Camera
				if (right_mouse_button)
				{
					defaultCam.setTransform(cameraPos, cameraFront, cameraUp);
				}
				const glm::mat4& view = defaultCam.getView();
				const glm::mat4& projection = defaultCam.GetProjection();

				// Draw Object
				Shader* currentShader = GObj->getShader();
				currentShader->use();
				currentShader->set3fv("lightPos", pointLight.position);
				currentShader->setFloat("lightIntensity", pointLight.intensity);
				currentShader->set4mat("view", view);
				currentShader->set4mat("projection", projection);
				currentShader->set4mat("model", GObj->GetTransformMat());
				GObj->DrawShape(Colors::White);
			}
		}
		totalTime += deltaTime;
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

	void RenderSystem::Terminate()
	{
		// Unbinding and closing all glfw windows and clearing opbjects
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glfwTerminate();
	}

	void RenderSystem::DrawCircle(const float x, const float y, const float r, const int numberOfSides, Colors color)
	{
		const int& numberOfVertices = numberOfSides + 2;
		vector<GLfloat> vertices;

		vertices.push_back(x);
		vertices.push_back(y * aspecRatio);
		vertices.push_back(0);

		for (size_t i = 0; i < numberOfVertices - 1; i++)
		{
			const float& angle = i * 2 * glm::pi<float>() / numberOfSides;
			vertices.push_back(x + sin(angle) * r); // x
			vertices.push_back(y + cos(angle) * r * aspecRatio); // y
			vertices.push_back(0);
		}

		defaultShader.use();
		defaultShader.set3fv("myColor", ColorValues[color]);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

		glBindVertexArray(VAO);
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	}


	void RenderSystem::DrawPoint(const float x, const float y, const float pointSize, Colors color)
	{
		vector<GLfloat> vertices;

		vertices.push_back(x + pointSize / 2.f);
		vertices.push_back(y + pointSize * aspecRatio / 2.f);
		vertices.push_back(0);
		vertices.push_back(x + pointSize / 2.f);
		vertices.push_back(y - pointSize * aspecRatio / 2.f);
		vertices.push_back(0);
		vertices.push_back(x - pointSize / 2.f);
		vertices.push_back(y - pointSize * aspecRatio / 2.f);
		vertices.push_back(0);
		vertices.push_back(x - pointSize / 2.f);
		vertices.push_back(y - pointSize * aspecRatio / 2.f);
		vertices.push_back(0);
		vertices.push_back(x - pointSize / 2.f);
		vertices.push_back(y + pointSize * aspecRatio / 2.f);
		vertices.push_back(0);
		vertices.push_back(x + pointSize / 2.f);
		vertices.push_back(y + pointSize * aspecRatio / 2.f);
		vertices.push_back(0);

		vector<int> indexBuffer = {
		0, 1, 3,
		1, 2, 3,
		};

		defaultShader.use();
		defaultShader.set3fv("myColor", ColorValues[color]);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 18);
	}
}