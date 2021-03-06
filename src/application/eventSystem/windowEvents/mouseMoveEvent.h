#ifndef MOUSE_MOVE_EVENT_H
#define MOUSE_MOVE_EVENT_H

#include <memory>
#include "../event.h"

namespace systemEvent {
	
	// EVENT HANDLER
	// -------------
	class MouseMoveHandler : public EventHandler {
	public:
		MouseMoveHandler(EventType typeEvent, EventHandlingMode handlingMode) : EventHandler(typeEvent, handlingMode) { }
	public:
		virtual void handle(double xpos, double ypos) const = 0;
	public:
		virtual ~MouseMoveHandler() {}
	};
	
	// EVENT 
	// -----
	class MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(double xpos, double ypos, MouseMoveHandler *handler, EventType typeEvent)
			: Event(typeEvent), m_Xpos(xpos), m_Ypos(ypos),
				m_Handler(handler)
		{ }
	public:
		void call() const override { m_Handler->handle(m_Xpos, m_Ypos); }
	private:
		double m_Xpos, m_Ypos;
		MouseMoveHandler *m_Handler;
	};
	
	// MAKE ADAPTER FOR EVENT HANDLER
	// ------------------------------
	std::shared_ptr<MouseMoveHandler> makeMouseMoveHandler(void(*function)(double, double), EventHandlingMode handlingMode = EventHandlingMode::Immediately);
	
	template<typename T>
	std::shared_ptr<MouseMoveHandler> makeMouseMoveHandler(T &object, void(T::*function)(double, double), EventHandlingMode handlingMode = EventHandlingMode::Immediately) {
		
		class Adapter : public MouseMoveHandler {
		public:
			typedef void(T::*myFunPtr)(double, double);
		public:
			Adapter(T &obj, myFunPtr function, EventHandlingMode handlingMode) : MouseMoveHandler(EventType::MouseScroll, handlingMode), m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(double xpos, double ypos) const {
				(m_Object->*m_FunPtr)(xpos, ypos);
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
		return std::shared_ptr<MouseMoveHandler>(new Adapter(object, function, handlingMode));
	}
	
	
}

#endif //MOUSE_MOVE_EVENT_H