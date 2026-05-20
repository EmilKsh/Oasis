#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <GLFW/glfw3.h>
#include <array>
#include <glm/glm.hpp>

namespace render_system
{
	class InputManager
	{
	public:
		void Attach(GLFWwindow* window);
		void BeginFrame();

		bool IsKeyDown(int key) const;
		bool IsMouseButtonDown(int button) const;
		glm::vec2 GetCursorPos() const { return cursorPos; }
		glm::vec2 GetCursorDelta() const { return cursorDelta; }

		static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	private:
		void OnCursorPosition(double xpos, double ypos);
		void OnKey(int key, int action);
		void OnMouseButton(int button, int action);

		std::array<bool, GLFW_KEY_LAST + 1> keys{};
		std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> mouseButtons{};
		glm::vec2 cursorPos{};
		glm::vec2 cursorDelta{};
		bool firstCursorEvent{ true };
	};
}

#endif
