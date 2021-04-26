#include "../eventDespatcher.h"
#include "windowHandlersStore.h"
#include "mouseMoveEvent.h"
#include "mouseButtonEvent.h"
#include "mouseScrollEvent.h"

#include <iostream>

namespace systemEvent {

	void WindowEventHandlersStore::mouseMoveHandle(double x, double y) {
		if(!m_ListMouseMoveHandler.empty()) 
			for( auto& i : m_ListMouseMoveHandler )
				EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new MouseMoveEvent(x, y, i.get())));
	}
	
	void WindowEventHandlersStore::mouseButtonHandle(int button, int action, int mods) {
		if(!m_ListMouseButtonHandler.empty())
			for( auto& i : m_ListMouseButtonHandler )
				EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new MouseButtonEvent(button, action, mods, i.get())));
	}
	
	void WindowEventHandlersStore::mouseScrollHandle(double yoffset) {
		if(!m_ListMouseScrollHandler.empty())
			for( auto& i : m_ListMouseScrollHandler )
				EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new MouseScrollEvent(yoffset, i.get())));
	}

	void WindowEventHandlersStore::addEventHandler(const std::shared_ptr<MouseMoveHandler> &handler) {	
		addHandler(m_ListMouseMoveHandler, handler); }
	void WindowEventHandlersStore::deleteEventHandler(const std::shared_ptr<MouseMoveHandler> &handler) {
		deleteHandler(m_ListMouseMoveHandler, handler);	}
	void WindowEventHandlersStore::addEventHandler(const std::shared_ptr<MouseButtonHandler> &handler) {
		addHandler(m_ListMouseButtonHandler, handler); }
	void WindowEventHandlersStore::deleteEventHandler(const std::shared_ptr<MouseButtonHandler> &handler) {
		deleteHandler(m_ListMouseButtonHandler, handler); }
	void WindowEventHandlersStore::addEventHandler(const std::shared_ptr<MouseScrollHandler> &handler) {
		addHandler(m_ListMouseScrollHandler, handler); }
	void WindowEventHandlersStore::deleteEventHandler(const std::shared_ptr<MouseScrollHandler> &handler) {
		deleteHandler(m_ListMouseScrollHandler, handler); }
	
	
	bool WindowEventHandlersStore::empty() {
		unsigned int empty = 0;
		empty += !(m_ListMouseMoveHandler.empty());
		empty += !(m_ListMouseButtonHandler.empty());
		empty += !(m_ListMouseScrollHandler.empty());
		
		return !empty;
	}
	
}
