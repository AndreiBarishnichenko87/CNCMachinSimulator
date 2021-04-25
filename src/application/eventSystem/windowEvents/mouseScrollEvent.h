#ifndef MOUSE_SCROLL_EVENT_H
#define MOUSE_SCROLL_EVENT_H

#include <memory>

#include "../event.h"
#include "mouseScrollEventHandler.h"

namespace systemEvent {

	class MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(double yoffset, MouseScrollHandler *handler)
			: Event(EventType::MouseScroll), m_Yoffset(yoffset),
				m_Handler(handler)
		{ }
	public:
		void call() const override { m_Handler->handle(m_Yoffset); }
	private:
		double m_Yoffset;
		MouseScrollHandler *m_Handler;
	};
	
}

#endif //MOUSE_SCROLL_EVENT_H