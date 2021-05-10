#include "window.h"

#include "../eventSystem/eventDespatcher.h"
#include "../eventSystem/windowHandlersStore.h"

namespace application {
	
	Window::~Window() {
		glfwDestroyWindow(m_Window);
		systemEvent::EventDispatcher::instance()->removeEventHandlerStore(*this);
	}
	
	std::pair<int, int> Window::getWindowSize() const {
		// pair int_width int_heigth
		std::pair<int, int> sizeWindow; 
		glfwGetWindowSize(m_Window, &std::get<0>(sizeWindow), &std::get<1>(sizeWindow));
		return sizeWindow;
	}
	
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
		: m_WindowTitle(titleName)
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
		glfwSetMouseButtonCallback(m_Window, callbackMouseButton);
		glfwSetScrollCallback(m_Window, callbackMouseScroll);
		glfwSetKeyCallback(m_Window, callbackKeyboard);
		glfwSetWindowSizeCallback(m_Window, callbackWindoowSize);
		glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GLFW_TRUE);
	}
	
	
	void Window::callbackMousePosition(GLFWwindow *window, double xpos, double ypos) {
		Window *myWindowHandler =  reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if(myWindowHandler->m_WindowHandlerStore != nullptr) {
			myWindowHandler->mousePosEventCall(xpos, ypos);
		}
	}
	void Window::mousePosEventCall(double xpos, double ypos) {
		m_WindowHandlerStore->mouseMoveHandle(xpos, ypos);
	}
	
	void Window::callbackMouseButton(GLFWwindow *window, int button, int action, int mods) {
		Window *myWindowHandler =  reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if(myWindowHandler->m_WindowHandlerStore != nullptr) {
			myWindowHandler->mouseButtonEventCall(button, action, mods);
		}
	}
	void Window::mouseButtonEventCall(int button, int action, int mods) {
		m_WindowHandlerStore->mouseButtonHandle(button, action, mods);
	}
	
	void Window::callbackMouseScroll(GLFWwindow *window, double xoffset, double yoffset) {
		Window *myWindowHandler =  reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if(myWindowHandler->m_WindowHandlerStore != nullptr) {
			myWindowHandler->mouseScrollEventCall(yoffset);
		}
	}
	void Window::mouseScrollEventCall(double yoffset) {
		m_WindowHandlerStore->mouseScrollHandle(yoffset);
	}
	
	void Window::callbackKeyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
		Window *myWindowHandler =  reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if(myWindowHandler->m_WindowHandlerStore != nullptr) {
			myWindowHandler->keyboardEventCall(key, scancode, action, mods);
		}
	}
	void Window::keyboardEventCall(int key, int scancode, int action, int mods) {
		m_WindowHandlerStore->keyboardHandle(key, scancode, action, mods);
	}
	
	void Window::callbackWindoowSize(GLFWwindow *window, int width, int height) {
		Window *myWindowHandler =  reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if(myWindowHandler->m_WindowHandlerStore != nullptr) {
			myWindowHandler->windowSizeEventCall(width, height);
		}
	}
	
	void Window::windowSizeEventCall(int width, int height) {
		m_WindowHandlerStore->windowSizeHandle(width, height);
	}
	
	
	
}