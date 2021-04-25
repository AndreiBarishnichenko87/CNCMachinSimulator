#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

namespace systemEvent {
	class EventDispatcher;
	class WindowEventHandlersStore;
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
		const std::string& getWindowName() const { return m_WindowTitle; }
		void activateContextWindow() { glfwMakeContextCurrent(m_Window); }
		GLFWwindow* getWindow() const { return m_Window; }
		explicit operator bool() const noexcept { return m_Window == nullptr ? false : true; }

	// callback function
	public:
		static void initGLFW();
		static void callbackMousePosition(GLFWwindow *window, double xpos, double ypos);
		static void callbackMouseButton(GLFWwindow *window, int button, int action, int mods);
		static void callbackMouseScroll(GLFWwindow *window, double xoffset, double yoffset);
	private:
		void mousePosEventCall(double xpos, double ypos);
		void mouseButtonEventCall(int button, int action, int mods);
		void mouseScrollEventCall(double yoffset);
	private:
		systemEvent::WindowEventHandlersStore* getHandlerStore() { return m_WindowHandlerStore; }
		void setHandlerStore(systemEvent::WindowEventHandlersStore *windowHandlStore) { m_WindowHandlerStore = windowHandlStore; }
	private:
		std::string m_WindowTitle;
		GLFWwindow *m_Window = nullptr;
		systemEvent::WindowEventHandlersStore *m_WindowHandlerStore = nullptr;
	};
	
}


#endif //WINDOW_H