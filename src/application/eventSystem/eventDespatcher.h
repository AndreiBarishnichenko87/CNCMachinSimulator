#ifndef EVENT_DESPATCHER_H
#define EVENT_DESPATCHER_H

#include <iostream>
#include <typeinfo>

#include <cassert>
#include <list>
#include <queue>
#include <memory>
#include <algorithm>

#include "windowEvents/windowHandlersStore.h"

namespace application {
	class Window;
}

namespace systemEvent {
	
	class Event;
	class WindowEventHandlersStore;
	class MouseMoveHandler;
	class MouseButtonHandler;
	class MouseScrollHandler;
	
	class EventDispatcher {
	public:
		static EventDispatcher* instance();
	private:
		EventDispatcher() {}
		EventDispatcher(const EventDispatcher&) = delete;
		EventDispatcher& operator=(const EventDispatcher&) = delete;
	private:
		template<typename EvStore, typename T, typename H>
		void addHandler(std::list<EvStore*> &container, T &eventGenerator, std::shared_ptr<H> handler);
		template<typename EvStore, typename T, typename H>
		void deleteHandler(std::list<EvStore*> &container, T &eventGenerator, std::shared_ptr<H> handler);
		template<typename EvStore, typename T>
		void removeHandlerStore(std::list<EvStore*> &container, T &eventGenerator);
	public:
		template<typename T>
		friend void bindHandler(application::Window &window, std::shared_ptr<T> handler);
		template<typename T>
		friend void unbindHandler(application::Window &window, std::shared_ptr<T> handler);
	public:
		void removeEventHandlerStore(application::Window &window);	
	public:
		void handleAllEvents();
		void addEvent(std::shared_ptr<Event> &&event) { m_EventQueue.push(event); }
	private:
		std::list<WindowEventHandlersStore*> m_ListWindowHandlersStore;
		std::queue<std::shared_ptr<Event> > m_EventQueue;
	};
	
	// for debuging
	// ------------
	inline void add(int &i) { ++i; }
	inline void sub(int &i) { --i; }
	
	template<typename T>
	int& typeCounterShowDebug() {
		static int i = 0;
		assert(i >= 0 && "eventDispatcher:: handler is removed greater than created");
		std::cout << typeid(T).name() << " : " << i << std::endl;
		return i;
	}
	// end debuging
	// ------------
	
	template<typename T>
	void bindHandler(application::Window &window, std::shared_ptr<T> handler) {
		EventDispatcher *dispatcher = EventDispatcher::instance();
		add(typeCounterShowDebug<T>());
		dispatcher->addHandler(dispatcher->m_ListWindowHandlersStore, window, handler); 
	}
	
	template<typename T>
	void unbindHandler(application::Window &window, std::shared_ptr<T> handler) {
		EventDispatcher *dispatcher = EventDispatcher::instance();
		sub(typeCounterShowDebug<T>());
		dispatcher->deleteHandler(dispatcher->m_ListWindowHandlersStore, window, handler);
	}

}

#endif //EVENT_DESPATCHER_H