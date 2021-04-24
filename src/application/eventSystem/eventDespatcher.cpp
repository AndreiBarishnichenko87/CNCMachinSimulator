#include <algorithm>

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
	// private method 
	// --------------
	WindowEventCollection* EventDispatcher::getEventCollection(application::Window &window) { 
		return window.m_WindowCollectionEvent; 
	}
	
	WindowEventCollection* EventDispatcher::registerEventCollection(application::Window &window) {
		if (getEventCollection(window) == nullptr) {
			m_ListOfWindowCollection.push_back(new WindowEventCollection);
			window.m_WindowCollectionEvent = m_ListOfWindowCollection.back();	
		}
		return window.m_WindowCollectionEvent;
	}

	// public method
	// -------------
	bool EventDispatcher::bindHandler(application::Window &window, std::shared_ptr<MouseMoveHandler> handler) {	
		if (registerEventCollection(window) == nullptr)
			return false;
		getEventCollection(window)->addEventHandler(handler);
		return true;
	}
	
	void EventDispatcher::unbindHandler(application::Window &window, std::shared_ptr<MouseMoveHandler> handler) {
		getEventCollection(window)->deleteEventHandler(handler);
		if (getEventCollection(window)->empty())
			removeEventCollection(window);
	}
	
	void EventDispatcher::removeEventCollection(application::Window &window) {
		if (getEventCollection(window) == nullptr)
			return;
		auto iter = std::find(m_ListOfWindowCollection.begin(), m_ListOfWindowCollection.end(), window.m_WindowCollectionEvent);
		if (iter != m_ListOfWindowCollection.end()) {
			delete *iter;
			m_ListOfWindowCollection.erase(iter);
			window.m_WindowCollectionEvent = nullptr;
		}
	}
	
}
