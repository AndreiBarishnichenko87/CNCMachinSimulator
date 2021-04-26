#ifndef MOUSE_BUTTON_EVENT_H
#define MOUSE_BUTTON_EVENT_H

#include <memory>
#include "../event.h"

namespace systemEvent {
	
	// EVENT HANDLER
	// -------------
	class MouseButtonHandler {
	public:
		virtual ~MouseButtonHandler() {}
	public:
		bool operator==(MouseButtonHandler &handler) const { return is_equal(handler); }
		bool operator!=(MouseButtonHandler &handler) const { return !(*this == handler); }
	private:
		virtual bool is_equal(MouseButtonHandler &handler) const = 0;
	public:
		virtual void handle(int button, int action, int mods) const = 0;
	};
	
	
	// MAKE ADAPTER FOR EVENT HANDLER
	// ------------------------------
	std::shared_ptr<MouseButtonHandler> makeMouseButtonHandler(void(*function)(int, int, int));
	
	template<typename T>
	std::shared_ptr<MouseButtonHandler> makeMouseButtonHandler(T &object, void(T::*function)(int, int, int)) {
		
		class Adapter : public MouseButtonHandler {
		public:
			typedef void(T::*myFunPtr)(int, int, int);
		public:
			Adapter(T &obj, myFunPtr function) : m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(int button, int action, int mods) const {
				(m_Object->*m_FunPtr)(button, action, mods);
			}
		private:
			bool is_equal(MouseButtonHandler &handler) const override {
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
		return std::shared_ptr<MouseButtonHandler>(new Adapter(object, function));
	}
	
	// EVENT 
	// -----
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