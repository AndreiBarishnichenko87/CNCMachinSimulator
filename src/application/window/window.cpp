#include "window.h"

#include <iostream>

#include "../eventSystem/eventDespatcher.h"

namespace application {
	
	void Window::initGLFW() {
		static bool glfwIsInit = false;
		if (!glfwIsInit) {
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_GLFW_VERSION);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_GLFW_VERSION);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwIsInit = true;
		}
	}

	Window::Window(const std::string &titleName, int width, int heigth)
		: m_WindowTitle(titleName), m_DispatcherEvent(systemEvent::EventDispatcher::instance())
	{
		Window::initGLFW();
		m_Window = glfwCreateWindow(width, heigth, m_WindowTitle.c_str(), NULL, NULL);
		if (m_Window == NULL)
		{
			glfwTerminate();
			throw BadWindow(std::string("window has not been created"));
		}
		
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, reinterpret_cast<void*>(this));
		
		// set callback function handler
		glfwSetCursorPosCallback(m_Window, callbackMousePosition);
	}

	void Window::callbackMousePosition(GLFWwindow *window, double xpos, double ypos) {
		Window *myWindowHandler =  reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	}
}