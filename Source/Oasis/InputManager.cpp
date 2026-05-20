#include "InputManager.h"

namespace render_system
{
	namespace
	{
		InputManager* GetInputManager(GLFWwindow* window)
		{
			return static_cast<InputManager*>(glfwGetWindowUserPointer(window));
		}
	}

	void InputManager::Attach(GLFWwindow* window)
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, CursorPositionCallback);
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
	}

	void InputManager::BeginFrame()
	{
		cursorDelta = glm::vec2(0.0f);
	}

	bool InputManager::IsKeyDown(int key) const
	{
		if (key < 0 || key >= static_cast<int>(keys.size()))
			return false;

		return keys[static_cast<size_t>(key)];
	}

	bool InputManager::IsMouseButtonDown(int button) const
	{
		if (button < 0 || button >= static_cast<int>(mouseButtons.size()))
			return false;

		return mouseButtons[static_cast<size_t>(button)];
	}

	void InputManager::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (InputManager* input = GetInputManager(window))
			input->OnCursorPosition(xpos, ypos);
	}

	void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (InputManager* input = GetInputManager(window))
			input->OnKey(key, action);
	}

	void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (InputManager* input = GetInputManager(window))
			input->OnMouseButton(button, action);
	}

	void InputManager::OnCursorPosition(double xpos, double ypos)
	{
		const glm::vec2 nextPos(static_cast<float>(xpos), static_cast<float>(ypos));
		if (firstCursorEvent)
		{
			cursorPos = nextPos;
			firstCursorEvent = false;
			return;
		}

		cursorDelta += nextPos - cursorPos;
		cursorPos = nextPos;
	}

	void InputManager::OnKey(int key, int action)
	{
		if (key < 0 || key >= static_cast<int>(keys.size()))
			return;

		if (action == GLFW_PRESS)
			keys[static_cast<size_t>(key)] = true;
		else if (action == GLFW_RELEASE)
			keys[static_cast<size_t>(key)] = false;
	}

	void InputManager::OnMouseButton(int button, int action)
	{
		if (button < 0 || button >= static_cast<int>(mouseButtons.size()))
			return;

		if (action == GLFW_PRESS)
			mouseButtons[static_cast<size_t>(button)] = true;
		else if (action == GLFW_RELEASE)
			mouseButtons[static_cast<size_t>(button)] = false;
	}
}
