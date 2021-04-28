#ifndef MOUSE_BUTTON_EVENT_H
#define MOUSE_BUTTON_EVENT_H

#include <memory>
#include "../event.h"

namespace systemEvent {
	
	// EVENT HANDLER
	// -------------
	class MouseButtonHandler {
	public:
		enum TypeHandler {
			MOUSE_BUT_GENERAL = 1,
			MOUSE_BUT_PUSH,
			MOUSE_BUT_RELESE
		};
	public:
		virtual ~MouseButtonHandler() {}
	protected:
		MouseButtonHandler(TypeHandler handlerType) : m_Type(handlerType) { }
	public:
		bool operator==(MouseButtonHandler &handler) const { return is_equal(handler); }
		bool operator!=(MouseButtonHandler &handler) const { return !(*this == handler); }
	private:
		virtual bool is_equal(MouseButtonHandler &handler) const = 0;
	public:
		virtual void handle(int button, int action, int mods) const = 0;
		TypeHandler getType() const { return m_Type; }
	private:
		TypeHandler m_Type;
	};
	
	
	// MAKE ADAPTER FOR EVENT HANDLER
	// ------------------------------
	
	// General button
	// --------------
	std::shared_ptr<MouseButtonHandler> makeMouseButtonHandler(void(*function)(int, int, int));
	template<typename T>
	std::shared_ptr<MouseButtonHandler> makeMouseButtonHandler(T &object, void(T::*function)(int, int, int)) {
		
		class Adapter : public MouseButtonHandler {
		public:
			typedef void(T::*myFunPtr)(int, int, int);
		public:
			Adapter(T &obj, myFunPtr function) : MouseButtonHandler(MOUSE_BUT_GENERAL), m_Object(&obj), m_FunPtr(function) { }
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
	
	// Push button 
	// -----------
	std::shared_ptr<MouseButtonHandler> makeMouseButtonPushHandler(void(*function)(int, int));
	template<typename T>
	std::shared_ptr<MouseButtonHandler> makeMouseButtonPushHandler(T &object, void(T::*function)(int, int)) {
		
		class Adapter : public MouseButtonHandler {
		public:
			typedef void(T::*myFunPtr)(int, int);
		public:
			Adapter(T &obj, myFunPtr function) : MouseButtonHandler(MOUSE_BUT_PUSH), m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(int button, int action, int mods) const {
				(m_Object->*m_FunPtr)(button, mods);
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
	
	// Push button 
	// -----------
	std::shared_ptr<MouseButtonHandler> makeMouseButtonReleseHandler(void(*function)(int, int));
	template<typename T>
	std::shared_ptr<MouseButtonHandler> makeMouseButtonReleseHandler(T &object, void(T::*function)(int, int)) {
		
		class Adapter : public MouseButtonHandler {
		public:
			typedef void(T::*myFunPtr)(int, int);
		public:
			Adapter(T &obj, myFunPtr function) : MouseButtonHandler(MOUSE_BUT_RELESE), m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(int button, int action, int mods) const {
				(m_Object->*m_FunPtr)(button, mods);
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