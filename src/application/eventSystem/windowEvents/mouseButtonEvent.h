#ifndef MOUSE_BUTTON_EVENT_H
#define MOUSE_BUTTON_EVENT_H

#include <memory>
#include "../event.h"

namespace systemEvent {
	
	// EVENT HANDLER
	// -------------
	class MouseButtonHandler : public EventHandler {
	public:
		MouseButtonHandler(EventType typeEvent, EventHandlingMode handlingMode) : EventHandler(typeEvent, handlingMode) { }
	public:
		virtual void handle(int button, int action, int mods) const = 0;
	public:
		virtual ~MouseButtonHandler() {}
	};
	
	// EVENT 
	// -----
	class MouseButtonEvent : public Event {
	public:
		MouseButtonEvent(int button, int action, int mods, MouseButtonHandler *handler, EventType typeEvent)
			: Event(typeEvent), m_Button(button), m_Action(action), m_Mods(mods),
				m_Handler(handler)
		{ }
	public:
		void call() const override { m_Handler->handle(m_Button, m_Action, m_Mods); }
	private:
		int m_Button, m_Action, m_Mods;
		MouseButtonHandler *m_Handler;
	};
	
	// MAKE ADAPTER FOR EVENT HANDLER
	// ------------------------------

	
	// Push button 
	// -----------
	std::shared_ptr<MouseButtonHandler> makeMouseButtonPushHandler(void(*function)(int, int), EventHandlingMode handlingMode = EventHandlingMode::Immediately);
	template<typename T>
	std::shared_ptr<MouseButtonHandler> makeMouseButtonPushHandler(T &object, void(T::*function)(int, int), EventHandlingMode handlingMode = EventHandlingMode::Immediately) {
		
		class Adapter : public MouseButtonHandler {
		public:
			typedef void(T::*myFunPtr)(int, int);
		public:
			Adapter(T &obj, myFunPtr function, EventHandlingMode handlingMode) : MouseButtonHandler(EventType::MouseButtonPush, handlingMode), m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(int button, int action, int mods) const {
				(m_Object->*m_FunPtr)(button, mods);
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
		return std::shared_ptr<MouseButtonHandler>(new Adapter(object, function, handlingMode));
	}
	
	// Relese button 
	// -------------
	std::shared_ptr<MouseButtonHandler> makeMouseButtonReleseHandler(void(*function)(int, int), EventHandlingMode handlingMode = EventHandlingMode::Immediately);
	template<typename T>
	std::shared_ptr<MouseButtonHandler> makeMouseButtonReleseHandler(T &object, void(T::*function)(int, int), EventHandlingMode handlingMode = EventHandlingMode::Immediately) {
		
		class Adapter : public MouseButtonHandler {
		public:
			typedef void(T::*myFunPtr)(int, int);
		public:
			Adapter(T &obj, myFunPtr function, EventHandlingMode handlingMode) : MouseButtonHandler(EventType::MouseButtonRelese, handlingMode), m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(int button, int action, int mods) const {
				(m_Object->*m_FunPtr)(button, mods);
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
		return std::shared_ptr<MouseButtonHandler>(new Adapter(object, function, handlingMode));
	}
	
}

#endif //MOUSE_BUTTON_EVENT_H