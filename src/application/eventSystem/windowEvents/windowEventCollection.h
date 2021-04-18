#ifndef WINDOW_EVENT_COLLECTION_H
#define WINDOW_EVENT_COLLECTION_H

#include <list>
#include "mouseEvent.h"

namespace systemEvent {
	
	class EventDispatcher;
	
	class WindowEventCollection {
	public:
		WindowEventCollection(EventDispatcher *dispatcher);
	public:
		void callMouseMoveEvent(double x, double y);
		bool operator==(const WindowEventCollection& windowCollection) const;
		bool operator!=(const WindowEventCollection& windowCollection) const { return !(*this == windowCollection); }
	public:
		void connectEventHandler(MouseMoveHandler *handler);
	private:
		EventDispatcher* m_Dispatcher;
		std::list<MouseMoveHandler*> m_ListMouseMoveHandler;
	};
	
}

#endif //WINDOW_EVENT_COLLECTION_H