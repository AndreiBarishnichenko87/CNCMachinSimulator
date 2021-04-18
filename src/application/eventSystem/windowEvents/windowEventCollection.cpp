#include "windowEventCollection.h"
#include "../eventDespatcher.h"

#include <algorithm>
#include <functional>

namespace systemEvent {
	
	WindowEventCollection::WindowEventCollection(EventDispatcher *dispatcher) {
		m_Dispatcher = EventDispatcher::instance();
	}
	
	void WindowEventCollection::callMouseMoveEvent(double x, double y) {
		
		if(!m_ListMouseMoveHandler.empty())
			return;
		
		for( auto& i : m_ListMouseMoveHandler ) {
			m_Dispatcher->addEvent(new MouseMoveEvent(x, y, i));
		}
	}
	
	bool WindowEventCollection::operator==(const WindowEventCollection& windowCollection) const {
		return this == &windowCollection;
	}
	
	void WindowEventCollection::connectEventHandler(MouseMoveHandler *handler) {
		
		auto result = m_ListMouseMoveHandler.begin();
		while (result != m_ListMouseMoveHandler.end()) {
			if (*handler == *(*result))
				break;
			++result;
		}
		
		if (result == m_ListMouseMoveHandler.end())
			m_ListMouseMoveHandler.push_back(handler);
	}
	
}
