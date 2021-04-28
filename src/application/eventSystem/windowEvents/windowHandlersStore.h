#ifndef WINDOW_HANDLERS_STORE_H
#define WINDOW_HANDLERS_STORE_H

#include <list>
#include <memory>
#include <algorithm>

namespace systemEvent {
	
	class MouseMoveHandler;
	class MouseButtonHandler;
	class MouseScrollHandler;
	class KeyboardHandler;
	
	class WindowEventHandlersStore {
	public:
		void mouseMoveHandle(double x, double y);
		void mouseButtonHandle(int button, int action, int mods);
		void mouseScrollHandle(double yoffset);
		void keyboardHandle(int key, int scancode, int action, int mods);
		
	private:
		// test template addEventHandler 
		template<typename T>
		void addHandler(std::list<std::shared_ptr<T> > &container, const std::shared_ptr<T> &handler) {
			if(container.empty()) {
				container.push_back(handler);
			} else {
				auto iter = std::find_if(container.begin(), container.end(), 
						[&handler](std::shared_ptr<T> &p) { return (*p == *handler); } );
				if (iter == container.end()) {
					container.push_back(handler);
				}				
			}
		}
		
		template<typename T>
		void deleteHandler(std::list<std::shared_ptr<T> > &container, const std::shared_ptr<T> &handler) {
			if(container.empty()) {
				return;
			}
			auto iter = std::find_if(container.begin(), container.end(), 
					[&handler](std::shared_ptr<T> &p) { return (*p == *handler); } );
			if (iter != container.end()) {
				container.erase(iter);
			}
		}
	public:
		bool empty();

		void addEventHandler(const std::shared_ptr<MouseMoveHandler> &handler);
		void deleteEventHandler(const std::shared_ptr<MouseMoveHandler> &handler);
		
		void addEventHandler(const std::shared_ptr<MouseButtonHandler> &handler);
		void deleteEventHandler(const std::shared_ptr<MouseButtonHandler> &handler);
		
		void addEventHandler(const std::shared_ptr<MouseScrollHandler> &handler);
		void deleteEventHandler(const std::shared_ptr<MouseScrollHandler> &handler);
		
		void addEventHandler(const std::shared_ptr<KeyboardHandler> &handler);
		void deleteEventHandler(const std::shared_ptr<KeyboardHandler> &handler);
	private:
		std::list<std::shared_ptr<MouseMoveHandler> > m_ListMouseMoveHandler;
		std::list<std::shared_ptr<MouseButtonHandler> > m_ListMouseButtonHandler;
		std::list<std::shared_ptr<MouseScrollHandler> > m_ListMouseScrollHandler;
		std::list<std::shared_ptr<KeyboardHandler> > m_ListKeyboardHandler;
	};
	
}

#endif //WINDOW_HANDLERS_STORE_H