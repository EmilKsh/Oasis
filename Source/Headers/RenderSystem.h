#ifndef RENDER_SYSTEM
#define RENDER_SYSTEM

#include"ft2build.h"
#include FT_FREETYPE_H
#include"GraphicalObject.h"
#include"Camera.h"
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
		void UpdateWindow();
		void ProcessInput(GLFWwindow* window);
		void RenderTheQueue();
		void RenderPointCloud(const vector<float>& positions, const vector<float>& colors, const int& count, Shader& shader, const float& pointSize);
		void DrawCircle(const float x, const float y, const float r, const int numberOfSides = 8, Colors color = Colors::White);
		void DrawPoint(const float x, const float y, const float pointSize = 0.01f, Colors color = Colors::White);
		void AddToQueue(GraphicalObj* Obj);
		void CalcDeltaTime();
		bool CheckWindowClosureStatus();
		void Terminate();
		glm::vec2 ToScreenSpaceFitHorizontal(double x, double y, const float wolrdSize[2]);
		glm::vec2 ToScreenSpaceFitVertical(double x, double y, const float wolrdSize[2]);
		void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

		float GetDeltaTime();
		glm::vec2 GetCursorPos() { return glm::vec2(LastMPx, LastMPy); };
		
	public:
		float aspecRatio;
		float totalTime{ 0.f };
		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame
		float currentFrame = 0.0f;
		int WindowSize[2] = { 1280, 720 };
		float LastMPx{ WindowSize[0] / 2.0f }, LastMPy{ WindowSize[1] / 2.0f }, DeltaMPx{ 0.0f }, DeltaMPy{ 0.0f };
		float pitch{ 0.0f }, roll{ 0.0f }, yaw{ -90.0f };
		bool left_mouse_button, right_mouse_button, firstMouse{ true };


		GLFWwindow* window{ nullptr };
		char* windowName{};
		vector<GraphicalObj*> RenderQueue{};
		glm::vec3 direction{ 0.f };
		Camera defaultCam;
		Light pointLight;
		GLuint VAO, VBO, posVBO, colVBO, textVAO, textVBO;
		Shader defaultShader, pointShader;
		Shader textShader;

		std::map<char, Character> Characters;

		static void cursor_pos_callBack(GLFWwindow* window, double xpos, double ypos);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_clicked(GLFWwindow* window, int button, int action, int mod);

	};
}
#endif