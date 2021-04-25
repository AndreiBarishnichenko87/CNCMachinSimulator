#include "eventDespatcher.h"
#include "event.h"
#include "../window/window.h"

namespace systemEvent {

	EventDispatcher* EventDispatcher::instance() {
		static EventDispatcher dispatcher;
		return &dispatcher;
	}
	
	void EventDispatcher::handleAllEvents() {
		if (m_EventQueue.empty())
			return;
		while(!m_EventQueue.empty()) {
			m_EventQueue.front()->call();
			m_EventQueue.pop();
		}
	}

	void EventDispatcher::removeEventHandlerStore(application::Window &window) {
		removeHandlerStore(m_ListWindowHandlersStore, window);
	}
	
}
