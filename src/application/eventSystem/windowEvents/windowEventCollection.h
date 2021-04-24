#ifndef WINDOW_EVENT_COLLECTION_H
#define WINDOW_EVENT_COLLECTION_H

#include <list>
#include <memory>
#include "mouseEvent.h"

namespace systemEvent {
	
	class EventDispatcher;
	
	class WindowEventCollection {
	public:
		void mouseMoveHandle(double x, double y);
	public:
		bool empty();
		void addEventHandler(const std::shared_ptr<MouseMoveHandler> &handler);
		void deleteEventHandler(const std::shared_ptr<MouseMoveHandler> &handler);
	private:
		std::list<std::shared_ptr<MouseMoveHandler> > m_ListMouseMoveHandler;
	};
	
}

#endif //WINDOW_EVENT_COLLECTION_H