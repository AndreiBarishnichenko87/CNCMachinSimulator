#ifndef MOUSE_BUTTON_EVENT_H
#define MOUSE_BUTTON_EVENT_H

#include <memory>

#include "../event.h"
#include "mouseButtonEventHandler.h"

namespace systemEvent {

	class MouseButtonEvent : public Event {
	public:
		MouseButtonEvent(int button, int action, int mods, MouseButtonHandler *handler)
			: Event(EventType::MouseButton), m_Button(button), m_Action(action), m_Mods(mods),
				m_Handler(handler)
		{ }
	public:
		void call() const override { m_Handler->handle(m_Button, m_Action, m_Mods); }
	private:
		int m_Button, m_Action, m_Mods;
		MouseButtonHandler *m_Handler;
	};
	
}

#endif //MOUSE_BUTTON_EVENT_H