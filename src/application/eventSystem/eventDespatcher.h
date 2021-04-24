#ifndef EVENT_DESPATCHER_H
#define EVENT_DESPATCHER_H

#include <list>
#include <queue>
#include <memory>

#include "event.h"

#define RESERVE_QUEUE_EVENT 50

namespace application {
	class Window;
}

namespace systemEvent {
	
	class WindowEventCollection;
	class MouseMoveHandler;
	
	class EventDispatcher {
	public:
		static EventDispatcher* instance();
	private:
		EventDispatcher() {}
		EventDispatcher(const EventDispatcher&) = delete;
		EventDispatcher& operator=(const EventDispatcher&) = delete;
	private:
		WindowEventCollection* registerEventCollection(application::Window &window);
		WindowEventCollection* getEventCollection(application::Window &window);
	
	// method event handler managemant method
	public:
		bool bindHandler(application::Window &window, std::shared_ptr<MouseMoveHandler> handler);
		void unbindHandler(application::Window &window, std::shared_ptr<MouseMoveHandler> handler);
		void removeEventCollection(application::Window &window);
	
	// Event manipulator method
	public:
		void handleAllEvents();
		void addEvent(std::shared_ptr<Event> &&event) { m_EventQueue.push(event); }
		bool isExistEventForProcess() const { return !m_EventQueue.empty(); }
		int countUnprocessEvent() const { return m_EventQueue.size(); }
	private:
		std::list<WindowEventCollection*> m_ListOfWindowCollection;
		std::queue<std::shared_ptr<Event> > m_EventQueue;
	};

}

#endif //EVENT_DESPATCHER_H