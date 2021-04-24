#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

namespace systemEvent {
	class EventDispatcher;
	class WindowEventCollection;
}

namespace application {
	
	#define MAJOR_GLFW_VERSION 4
	#define MINOR_GLFW_VERSION 3
	
	class Window {
	private:
		friend class systemEvent::EventDispatcher;
	public:
		class BadWindow : public std::runtime_error {
		public:
			explicit BadWindow(const std::string &msg) : std::runtime_error("BadWindow::" + msg) {}
		};
	public:
		explicit Window(const std::string &titleName, int width, int heigth);
		Window() {}
		~Window();
	public:
		void activateContextWindow() { glfwMakeContextCurrent(m_Window); }
		GLFWwindow* getWindow() const { return m_Window; }
		explicit operator bool() const noexcept { return m_Window == nullptr ? false : true; }

	// callback function
	public:
		static void callbackMousePosition(GLFWwindow *window, double xpos, double ypos);
	private:
		void mousePosEventCall(double xpos, double ypos);
	private:
		static void initGLFW();
	private:
		std::string m_WindowTitle;
		GLFWwindow *m_Window = nullptr;
		systemEvent::WindowEventCollection *m_WindowCollectionEvent = nullptr;
	};
	
}


#endif //WINDOW_H