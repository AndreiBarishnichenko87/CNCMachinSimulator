#include "windowEventCollection.h"
#include "../eventDespatcher.h"

#include <algorithm>
#include <functional>

namespace systemEvent {

	void WindowEventCollection::mouseMoveHandle(double x, double y) {
		
		if(m_ListMouseMoveHandler.empty())
			return;
		
		for( auto& i : m_ListMouseMoveHandler ) {
			EventDispatcher::instance()->addEvent(std::shared_ptr<Event>(new MouseMoveEvent(x, y, i.get())));
		}
	}

	void WindowEventCollection::addEventHandler(const std::shared_ptr<MouseMoveHandler> &handler) {
		auto result = m_ListMouseMoveHandler.begin();
		while (result != m_ListMouseMoveHandler.end()) {
			if (*handler == *(*result))
				return;
			++result;
		}
		if (result == m_ListMouseMoveHandler.end())
			m_ListMouseMoveHandler.push_back(handler);
	}
	
	void WindowEventCollection::deleteEventHandler(const std::shared_ptr<MouseMoveHandler> &handler) {
		auto iter = std::find_if(m_ListMouseMoveHandler.begin(), m_ListMouseMoveHandler.end(), 
					[&handler](std::shared_ptr<MouseMoveHandler> &p) { return (*p == *handler); } );
		if (iter != m_ListMouseMoveHandler.end()) {
			m_ListMouseMoveHandler.erase(iter);
		}
	}
	
	bool WindowEventCollection::empty() {
		unsigned int empty = 0;
		empty += !(m_ListMouseMoveHandler.empty());
		
		return !empty;
	}
	
}
