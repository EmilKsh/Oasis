#include "RenderSystem.h"
#include <filesystem>
#include <vector>

namespace render_system
{
	namespace
	{
		bool IsOasisRoot(const std::filesystem::path& path)
		{
			return std::filesystem::exists(path / "Source" / "Shaders")
				&& std::filesystem::exists(path / "Fonts");
		}

		std::filesystem::path FindOasisRoot()
		{
			const std::filesystem::path sourcePath = std::filesystem::absolute(__FILE__);
			for (std::filesystem::path path = sourcePath.parent_path(); !path.empty(); path = path.parent_path())
			{
				if (IsOasisRoot(path))
					return path;

				if (path == path.root_path())
					break;
			}

			const std::filesystem::path cwd = std::filesystem::current_path();
			const std::vector<std::filesystem::path> candidates{
				cwd,
				cwd / "Oasis",
				cwd / "..",
				cwd / ".." / "Oasis",
				cwd / ".." / "..",
				cwd / ".." / ".." / "Oasis",
				cwd / ".." / ".." / "..",
				cwd / ".." / ".." / ".." / "Oasis"
			};

			for (const std::filesystem::path& candidate : candidates)
			{
				const std::filesystem::path normalized = std::filesystem::weakly_canonical(candidate);
				if (IsOasisRoot(normalized))
					return normalized;
			}

			return cwd;
		}

		std::string ToPathString(const std::filesystem::path& path)
		{
			return path.lexically_normal().string();
		}
	}

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

		const float cameraSpeed = 100.0f * GetDeltaTime(); // adjust accordingly
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
		const float mouseX = static_cast<float>(xpos);
		const float mouseY = static_cast<float>(ypos);

		if (self->firstMouse)
		{
			self->LastMPx = mouseX;
			self->LastMPy = mouseY;
			self->firstMouse = false;
		}

		self->DeltaMPx = mouseX - self->LastMPx;
		self->DeltaMPy = self->LastMPy - mouseY;
		self->LastMPx = mouseX;
		self->LastMPy = mouseY;

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


	RenderSystem::RenderSystem(int window_size[2], char* windowName) : WindowSize{ window_size[0], window_size[1] }, windowName{ windowName }
	{
		LastMPx = WindowSize[0] / 2.0f;
		LastMPy = WindowSize[1] / 2.0f;

		aspecRatio = float(window_size[0]) / window_size[1];

		if (GLFWInit() != 0)
		{
			return;
		}

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &posVBO);
		glGenBuffers(1, &colVBO);
		glGenVertexArrays(1, &VAO);
		defaultShader.Init("../Oasis/Source/Shaders/SimpleShader");
		pointShader.Init("../Oasis/Source/Shaders/PointShader");

		const std::filesystem::path oasisRoot = FindOasisRoot();
		if (!defaultShader.Init(ToPathString(oasisRoot / "Source" / "Shaders" / "SimpleShader")))
		{
			return;
		}

		if (FreeTypeInit() != 0)
		{
			return;
		}

		initialized = true;
		BeginFrame();
	}

	RenderSystem::~RenderSystem() 
	{
		Terminate();
	}

	int RenderSystem::GLFWInit()
	{
		// GLFW initialization ----------------------------------------
		if (!glfwInit())
		{
			printf("GLFW initialization failed.\n");
			return -1;
		}

		// setting window hints aka OpenGL version and profile
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// setting up the window and error handling
		window = glfwCreateWindow(WindowSize[0], WindowSize[1], windowName, NULL, NULL);
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
			glfwDestroyWindow(window);
			window = nullptr;
			glfwTerminate();
			return -1;
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

	bool RenderSystem::CheckWindowClosureStatus() {
		if (!initialized || window == nullptr)
			return false;

		return !glfwWindowShouldClose(window);
	}

	void RenderSystem::BeginFrame()
	{
		if (!initialized || window == nullptr)
			return;

		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderSystem::EndFrame()
	{
		if (!initialized || window == nullptr)
			return;

		ProcessInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void RenderSystem::UpdateWindow()
	{
		EndFrame();
		BeginFrame();
	}

	void RenderSystem::RenderTheQueue()
	{
		if (!initialized || window == nullptr)
			return;

		BeginFrame();
			
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
		EndFrame();
	}

	void RenderSystem::AddToQueue(GraphicalObj* Obj)
	{
		RenderQueue.push_back(Obj);
	}

	void RenderSystem::RenderPointCloud(const vector<float>& positions, const vector<float>& colors, const int& count, Shader& shader, const float& pointSize)
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, colVBO);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);

		shader.use();

		glPointSize(pointSize);
		glDrawArrays(GL_POINTS, 0, count);

		glBindVertexArray(0);
	}

	void RenderSystem::CalcDeltaTime()
	{
		currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	float RenderSystem::GetDeltaTime()
	{
		return deltaTime;
	}

	void RenderSystem::Terminate()
	{
		if (window == nullptr)
			return;

		// Unbinding and closing all glfw windows and clearing opbjects
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &textVBO);
		glDeleteVertexArrays(1, &textVAO);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glfwDestroyWindow(window);
		window = nullptr;
		initialized = false;
		glfwTerminate();
	}

	void RenderSystem::DrawCircle(const float x, const float y, const float r, const int numberOfSides, Colors color)
	{
		const int& numberOfVertices = numberOfSides + 2;
		circleVertices.clear();
		circleVertices.reserve(static_cast<size_t>(numberOfVertices) * 3);

		circleVertices.push_back(x);
		circleVertices.push_back(y * aspecRatio);
		circleVertices.push_back(0);

		for (size_t i = 0; i < numberOfVertices - 1; i++)
		{
			const float& angle = i * 2 * glm::pi<float>() / numberOfSides;
			circleVertices.push_back(x + sin(angle) * r); // x
			circleVertices.push_back(y + cos(angle) * r * aspecRatio); // y
			circleVertices.push_back(0);
		}

		defaultShader.use();
		defaultShader.set3fv("myColor", ColorValues[color]);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(GLfloat), circleVertices.data(), GL_DYNAMIC_DRAW);
		
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

	glm::vec2 RenderSystem::ToScreenSpaceFitHorizontal(double x, double y, const float worldSize[2])
	{
		const float& scale = WindowSize[0] / worldSize[0];

		return glm::vec2(x / worldSize[0], y * scale / WindowSize[1]);
	}

	glm::vec2 RenderSystem::ToScreenSpaceFitVertical(double x, double y, const float worldSize[2])
	{
		const float& scale = WindowSize[1] / worldSize[1];

		return glm::vec2(x * scale / WindowSize[0], y / worldSize[1]);
	}

	int RenderSystem::FreeTypeInit()
	{
		const std::filesystem::path oasisRoot = FindOasisRoot();

		// Setting up text shader
		if (!textShader.Init(ToPathString(oasisRoot / "Source" / "Shaders" / "TextShader")))
		{
			return -1;
		}

		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WindowSize[0]), 0.0f, static_cast<float>(WindowSize[1]));
		textShader.use();
		glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// FreeType Init -------------------------------------------
		FT_Library ft;

		if (FT_Init_FreeType(&ft))
		{
			printf("ERROR::FreeType: FreeType library failed to initialize.\n");
			return -1;
		}

		FT_Face face;

		const std::string fontPath = ToPathString(oasisRoot / "Fonts" / "SourceCodePro-Regular.ttf");
		if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
		{
			printf("ERROR::FreeType: Failed to load font: %s\n", fontPath.c_str());
			FT_Done_FreeType(ft);
			return -1;
		}
		
		FT_Set_Pixel_Sizes(face, 0, 48); // choose a pixel size you want

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		glGenVertexArrays(1, &textVAO);
		glGenBuffers(1, &textVBO);
		glBindVertexArray(textVAO);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return 0;
	}

	void RenderSystem::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
	{
		// activate corresponding render state	
		textShader.use();

		glUniform3f(glGetUniformLocation(textShader.ID, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(textVAO);

		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;
			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, textVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
