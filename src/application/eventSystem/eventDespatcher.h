#ifndef EVENT_DESPATCHER_H
#define EVENT_DESPATCHER_H

#include <iostream>
#include <typeinfo>

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
		void addHandler(std::list<EvStore*> &container, T &eventGenerator, std::shared_ptr<H> handler) {
			if (eventGenerator.getHandlerStore() == nullptr) {
				container.push_back(new EvStore);
				eventGenerator.setHandlerStore(container.back());
			}
			eventGenerator.getHandlerStore()->addEventHandler(handler);
		}
		
		template<typename EvStore, typename T, typename H>
		void deleteHandler(std::list<EvStore*> &container, T &eventGenerator, std::shared_ptr<H> handler) {
			if(eventGenerator.getHandlerStore() == nullptr)
				return;
			eventGenerator.getHandlerStore()->deleteEventHandler(handler);
			if (eventGenerator.getHandlerStore()->empty())
				removeHandlerStore(container, eventGenerator);
		}
		
		template<typename EvStore, typename T>
		void removeHandlerStore(std::list<EvStore*> &container, T &eventGenerator) {
			EvStore* eventStore = eventGenerator.getHandlerStore();
			auto iter = std::find(container.begin(), container.end(), eventStore);
			if (iter != container.end()) {
				delete *iter;
				container.erase(iter);
				eventGenerator.setHandlerStore(nullptr);
			}
		}
		
		// for debuging
		// ------------
		void add(int &i) { ++i; }
		void sub(int &i) { --i; }
		
		template<typename T>
		int& typeCounterShowDebug() {
			static int i = 0;
			std::cout << typeid(T).name() << " : " << i << std::endl;
			return i;
		}
		// end debuging
		// ------------

	public:
		void removeEventHandlerStore(application::Window &window);
		
		template<typename T>
		void bindHandler(application::Window &window, std::shared_ptr<T> handler) {
			add(typeCounterShowDebug<T>());
			addHandler(m_ListWindowHandlersStore, window, handler); 
		}
		
		template<typename T>
		void unbindHandler(application::Window &window, std::shared_ptr<T> handler) {
			sub(typeCounterShowDebug<T>());
			deleteHandler(m_ListWindowHandlersStore, window, handler);
		}
		
	public:
		void handleAllEvents();
		void addEvent(std::shared_ptr<Event> &&event) { m_EventQueue.push(event); }
	private:
		std::list<WindowEventHandlersStore*> m_ListWindowHandlersStore;
		std::queue<std::shared_ptr<Event> > m_EventQueue;
	};

}

#endif //EVENT_DESPATCHER_H