#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H
#include "../event.h"

namespace systemEvent {

	class MouseMoveHandler {
	public:
		bool operator==(const MouseMoveHandler &handler) const { return is_equal(handler); }
		bool operator!=(const MouseMoveHandler &handler) const { return !(*this == handler); }
	private:
		virtual is_equal(const MouseMoveHandler &handler) const = 0;
	public:
		virtual void handle(double xpos, double ypos) const = 0;
	};
	
	class MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(double xpos, double ypos, MouseMoveHandler *handler)
			: Event(EventType::MouseMove), m_Xpos(xpos), m_Ypos(ypos),
				m_Handler(handler)
		{ }
	public:
		void call() const { m_Handler->handle(m_Xpos, m_Ypos); }
	private:
		double m_Xpos, m_Ypos;
		MouseMoveHandler *m_Handler;
	};

}

#endif //MOUSE_EVENT_H