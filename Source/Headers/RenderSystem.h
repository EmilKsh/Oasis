#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM

#include"ft2build.h"
#include FT_FREETYPE_H
#include"GraphicalObject.h"
#include"Camera.h"
#include"InputManager.h"
#include"Light.h"


namespace render_system {
	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	class RenderSystem
	{
	public:
		RenderSystem(int WindowSize[2], char* windowName = (char*)"Viewport");
		~RenderSystem();
		int GLFWInit();
		int FreeTypeInit();
		void BeginFrame();
		void EndFrame();
		void UpdateWindow();
		void ProcessInput(GLFWwindow* window);
		void RenderTheQueue();
		void RenderPointCloud(const vector<float>& positions, const vector<float>& colors, const int& count, Shader& shader, const float& pointSize);
		void DrawCircle(const float x, const float y, const float r, const int numberOfSides = 8, Colors color = Colors::White);
		void DrawPoint(const float x, const float y, const float pointSize = 0.01f, Colors color = Colors::White);
		void DrawLine(const float x1, const float y1, const float x2, const float y2, glm::vec3 color, const float width = 1.0f);
		void DrawFilledRect(const float x, const float y, const float width, const float height, glm::vec3 color);
		void AddToQueue(GraphicalObj* Obj);
		void CalcDeltaTime();
		bool CheckWindowClosureStatus();
		void Terminate();
		glm::vec2 ToScreenSpaceFitHorizontal(double x, double y, const float wolrdSize[2]);
		glm::vec2 ToScreenSpaceFitVertical(double x, double y, const float wolrdSize[2]);
		void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

		float GetDeltaTime();
		glm::vec2 GetCursorPos() const { return input.GetCursorPos(); };
		InputManager& GetInput() { return input; }
		const InputManager& GetInput() const { return input; }
		
	public:
		float aspecRatio;
		float totalTime{ 0.f };
		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame
		float currentFrame = 0.0f;
		int WindowSize[2] = { 1280, 720 };
		bool initialized{ false };


		GLFWwindow* window{ nullptr };
		char* windowName{};
		InputManager input{};
		vector<GraphicalObj*> RenderQueue{};
		glm::vec3 cameraPos{ 0.0f, 0.0f, 3.0f };
		glm::vec3 cameraFront{ 0.0f, 0.0f, -1.0f };
		glm::vec3 cameraUp{ 0.0f, 1.0f, 0.0f };
		glm::vec3 direction{ 0.f };
		float pitch{ 0.0f };
		float yaw{ -90.0f };
		Camera defaultCam;
		Light pointLight;
		GLuint VAO{}, VBO{}, posVBO{}, colVBO{}, textVAO{}, textVBO{};
		Shader defaultShader, pointShader;
		Shader textShader;
		vector<GLfloat> circleVertices{};

		std::map<char, Character> Characters;

		void UpdateCameraFromInput();
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	};
}
#endif
