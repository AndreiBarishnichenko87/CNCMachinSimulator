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
	private:
		EventDispatcher() {}
		EventDispatcher(const EventDispatcher&) = delete;
		EventDispatcher& operator=(const EventDispatcher&) = delete;
	private:
		WindowEventCollection* addWindowCollection(application::Window &window);
	public:
		bool bindHandler(application::Window &window, MouseMoveHandler *handler);
	public:
		void addEvent(Event* event) { m_EventQueue.push(event); }
		void popFrontEvent() { return m_EventQueue.pop(); }
		Event* getFrontEvent() { return m_EventQueue.empty() ? nullptr : m_EventQueue.front(); }
	public:
		bool isEmpty() const { return m_EventQueue.empty(); }
		int countUnprocessEvent() const { return m_EventQueue.size(); }
		void handleAllEvents();
	private:
		std::list<WindowEventCollection*> m_ProducersEvent;
		std::queue<Event*> m_EventQueue;
	public:
		static EventDispatcher* instance();
	};

}

#endif //EVENT_DESPATCHER_H