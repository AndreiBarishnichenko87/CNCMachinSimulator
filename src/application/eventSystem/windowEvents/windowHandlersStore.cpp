#include "../eventDespatcher.h"
#include "windowHandlersStore.h"
#include "mouseMoveEvent.h"
#include "mouseButtonEvent.h"
#include "mouseScrollEvent.h"
#include "keyboardEvent.h"

#include <iostream>
#include <algorithm>

namespace systemEvent {

	void WindowEventHandlersStore::mouseMoveHandle(double x, double y) {
		if(!m_ListMouseMoveHandler.empty()) 
			for( auto& i : m_ListMouseMoveHandler )
				EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new MouseMoveEvent(x, y, i.get())));
	}

	void WindowEventHandlersStore::mouseButtonHandle(int button, int action, int mods) {
		if(!m_ListMouseButtonHandler.empty())
			std::for_each(m_ListMouseButtonHandler.begin(), m_ListMouseButtonHandler.end(), 
			[button, action, mods](std::shared_ptr<MouseButtonHandler> &handler) {
				switch(handler->getType()) {
					case MouseButtonHandler::TypeHandler::MOUSE_BUT_PUSH:
						if(action == 1)
							EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new MouseButtonEvent(button, action, mods, handler.get())));
						break;
					case MouseButtonHandler::TypeHandler::MOUSE_BUT_RELESE:
						if(action == 0)
							EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new MouseButtonEvent(button, action, mods, handler.get())));
						break;
					case MouseButtonHandler::TypeHandler::MOUSE_BUT_GENERAL:
						EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new MouseButtonEvent(button, action, mods, handler.get())));
						break;
				}
			});
	}
	
	void WindowEventHandlersStore::keyboardHandle(int key, int scancode, int action, int mods) {
		if(!m_ListKeyboardHandler.empty())
			std::for_each(m_ListKeyboardHandler.begin(), m_ListKeyboardHandler.end(), 
			[key, scancode, action, mods](std::shared_ptr<KeyboardHandler> &handler) {
				switch(action) {
					case 1:
						if(handler->getType() == KeyboardHandler::TypeHandler::KEY_PUSH)
							EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new KeyboardEvent(key, scancode, mods, handler.get())));
						break;
					case 0:
						if(handler->getType() == KeyboardHandler::TypeHandler::KEY_RELESE)
							EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new KeyboardEvent(key, scancode, mods, handler.get())));
						break;
					case 2:
						if(handler->getType() == KeyboardHandler::TypeHandler::KEY_PUSH)
							EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new KeyboardEvent(key, scancode, mods, handler.get())));
						break;
				}
				
				//switch(handler->getType()) {
				//	case KeyboardHandler::TypeHandler::KEY_PUSH:
				//		if(action == 1)
				//			EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new KeyboardEvent(key, scancode, mods, handler.get())));
				//		break;
				//	case KeyboardHandler::TypeHandler::KEY_RELESE:
				//		if(action == 0)
				//			EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new KeyboardEvent(key, scancode, mods, handler.get())));
				//		break;
				//}
			});
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
	void WindowEventHandlersStore::addEventHandler(const std::shared_ptr<KeyboardHandler> &handler) {
		addHandler(m_ListKeyboardHandler, handler); }
	void WindowEventHandlersStore::deleteEventHandler(const std::shared_ptr<KeyboardHandler> &handler) {
		deleteHandler(m_ListKeyboardHandler, handler); }
	
	bool WindowEventHandlersStore::empty() {
		unsigned int empty = 0;
		empty += !(m_ListMouseMoveHandler.empty());
		empty += !(m_ListMouseButtonHandler.empty());
		empty += !(m_ListMouseScrollHandler.empty());
		empty += !(m_ListKeyboardHandler.empty());
		
		return !empty;
	}
	
}
