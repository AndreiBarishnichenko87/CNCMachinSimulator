#include "eventDespatcher.h"
#include "windowEvents/windowEventCollection.h"
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
	
	WindowEventCollection* EventDispatcher::addWindowCollection(application::Window &window) {
		if (window.m_WindowCollectionEvent == nullptr) {
			m_ProducersEvent.push_back(new WindowEventCollection(EventDispatcher::instance()));
			window.m_WindowCollectionEvent = m_ProducersEvent.back();	
		}
		return window.m_WindowCollectionEvent;
	}
	
	bool EventDispatcher::bindHandler(application::Window &window, MouseMoveHandler *handler) {
		
		if ((window.m_WindowCollectionEvent = addWindowCollection(window)) == nullptr)
			return false;
		window.m_WindowCollectionEvent->connectEventHandler(handler);

		return true;
	}
}
