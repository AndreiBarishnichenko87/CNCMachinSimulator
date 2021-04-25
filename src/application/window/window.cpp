#include "window.h"

#include "../eventSystem/eventDespatcher.h"
#include "../eventSystem/windowEvents/windowHandlersStore.h"

namespace application {
	
	Window::~Window() {
		glfwDestroyWindow(m_Window);
		systemEvent::EventDispatcher::instance()->removeEventHandlerStore(*this);
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
	}
	
	
	void Window::callbackMousePosition(GLFWwindow *window, double xpos, double ypos) {
		Window *myWindowHandler =  reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if(myWindowHandler->m_WindowHandlerStore != nullptr) {
			myWindowHandler->mousePosEventCall(xpos, ypos);
		}
	}
	
	void Window::callbackMouseButton(GLFWwindow *window, int button, int action, int mods) {
		Window *myWindowHandler =  reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if(myWindowHandler->m_WindowHandlerStore != nullptr) {
			myWindowHandler->mouseButtonEventCall(button, action, mods);
		}
	}
	
	void Window::callbackMouseScroll(GLFWwindow *window, double xoffset, double yoffset) {
		Window *myWindowHandler =  reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if(myWindowHandler->m_WindowHandlerStore != nullptr) {
			myWindowHandler->mouseScrollEventCall(yoffset);
		}
	}
	
	void Window::mouseScrollEventCall(double yoffset) {
		m_WindowHandlerStore->mouseScrollHandle(yoffset);
		systemEvent::EventDispatcher::instance()->handleAllEvents();
	}
	
	void Window::mousePosEventCall(double xpos, double ypos) {
		m_WindowHandlerStore->mouseMoveHandle(xpos, ypos);
		systemEvent::EventDispatcher::instance()->handleAllEvents();
	}
	
	void Window::mouseButtonEventCall(int button, int action, int mods) {
		m_WindowHandlerStore->mouseButtonHandle(button, action, mods);
		systemEvent::EventDispatcher::instance()->handleAllEvents();
	}
	
}