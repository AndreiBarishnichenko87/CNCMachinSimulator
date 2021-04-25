#ifndef MOUSE_MOVE_EVENT_H
#define MOUSE_MOVE_EVENT_H

#include <memory>

#include "../event.h"
#include "mouseMoveEventHandler.h"

namespace systemEvent {

	class MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(double xpos, double ypos, MouseMoveHandler *handler)
			: Event(EventType::MouseMove), m_Xpos(xpos), m_Ypos(ypos),
				m_Handler(handler)
		{ }
	public:
		void call() const override { m_Handler->handle(m_Xpos, m_Ypos); }
	private:
		double m_Xpos, m_Ypos;
		MouseMoveHandler *m_Handler;
	};
	
}

#endif //MOUSE_MOVE_EVENT_H