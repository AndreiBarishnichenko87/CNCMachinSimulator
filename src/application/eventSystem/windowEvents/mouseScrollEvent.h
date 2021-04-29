#ifndef MOUSE_SCROLL_EVENT_H
#define MOUSE_SCROLL_EVENT_H

#include <memory>
#include "../event.h"

namespace systemEvent {
	
	// EVENT HANDLER
	// -------------
	class MouseScrollHandler : public EventHandler {
	protected:
		MouseScrollHandler(EventType typeEvent, EventHandlingMode handlingMode) : EventHandler(typeEvent, handlingMode) { }
	public:
		virtual void handle(double yoffset) const = 0;
	public:
		virtual ~MouseScrollHandler() {}
	};
	
	// EVENT
	// -----
	class MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(double yoffset, MouseScrollHandler *handler, EventType typeEvent)
			: Event(typeEvent), m_Yoffset(yoffset),
				m_Handler(handler)
		{ }
	public:
		void call() const override { m_Handler->handle(m_Yoffset); }
	private:
		double m_Yoffset;
		MouseScrollHandler *m_Handler;
	};
	
	// MAKE ADAPTER FOR EVENT HANDLER
	// ------------------------------
	std::shared_ptr<MouseScrollHandler> makeMouseScrollHandler(void(*function)(double), EventHandlingMode handlingMode = EventHandlingMode::Immediately);
	
	template<typename T>
	std::shared_ptr<MouseScrollHandler> makeMouseScrollHandler(T &object, void(T::*function)(double), EventHandlingMode handlingMode = EventHandlingMode::Immediately) {
		
		class Adapter : public MouseScrollHandler {
		public:
			typedef void(T::*myFunPtr)(double);
		public:
			Adapter(T &obj, myFunPtr function, EventHandlingMode handlingMode) : MouseScrollHandler(EventType::MouseScroll, handlingMode), m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(double yoffset) const {
				(m_Object->*m_FunPtr)(yoffset);
			}
		private:
			bool is_equal(EventHandler &handler) const override {
				Adapter *adapterPtr = dynamic_cast<Adapter*>(&handler);
				if(adapterPtr == nullptr) {
					return false;
				}
				return (m_Object == adapterPtr->m_Object) && (m_FunPtr == adapterPtr->m_FunPtr) ? true : false;
			}
		private:
			T *m_Object;
			myFunPtr m_FunPtr;
		};
		return std::shared_ptr<MouseScrollHandler>(new Adapter(object, function, handlingMode));
	}
	
}

#endif //MOUSE_SCROLL_EVENT_H