#pragma once
#include"GraphicalObject.h"
#include"Camera.h"
#include"Light.h"

namespace render_system {
	class RenderSystem
	{
	public:
		RenderSystem(int WindowSize[2]);
		int GLFWInit();
		int FreeTypeInit();
		void process_input(GLFWwindow* window);
		int RenderTheQueue();
		void AddToQueue(GraphicalObj* Obj);
		void CalcDeltaTime();
		float getDeltaTime();
		bool CheckWindowClosureStatus();
		void Terminate();

	private:
		float totalTime{ 0.f };
		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame
		float currentFrame = 0.0f;
		int WindowSize[2] = { 1280, 720 };
		float LastMPx{ WindowSize[0] / 2.0f }, LastMPy{ WindowSize[1] / 2.0f }, DeltaMPx{ 0.0f }, DeltaMPy{ 0.0f };
		float pitch{ 0.0f }, roll{ 0.0f }, yaw{ -90.0f };
		glm::vec3 direction;
		bool left_mouse_button, right_mouse_button, firstMouse{ true };


		GLFWwindow* window{ nullptr };
		vector<GraphicalObj*> RenderQueue{};
		Colors color;

		Camera defaultCam;
		Light pointLight;

		static void cursor_pos_callBack(GLFWwindow* window, double xpos, double ypos);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_clicked(GLFWwindow* window, int button, int action, int mod);

	};
}
