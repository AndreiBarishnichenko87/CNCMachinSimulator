#ifndef KEYBOARD_EVENT_H
#define KEYBOARD_EVENT_H

#include <memory>
#include "../event.h"

namespace systemEvent {
	
	// EVENT HANDLER
	// -------------
	class KeyboardHandler : public EventHandler {
	protected:
		KeyboardHandler(EventType typeEvent, EventHandlingMode handlingMode) : EventHandler(typeEvent, handlingMode) { }
	public:
		virtual void handle(int key, int scancode, int mods) const = 0;
	public:
		virtual ~KeyboardHandler() {}
	};
	
	// EVENTS 
	// ------
	class KeyboardEvent : public Event {
	public:
		KeyboardEvent(int key, int scancode, int mods, KeyboardHandler *handler, EventType typeEvent)
			: Event(typeEvent), m_Key(key), m_Scancode(scancode), m_Mods(mods),
				m_Handler(handler)
		{ }
	public:
		void call() const override { m_Handler->handle(m_Key, m_Scancode, m_Mods); }
	private:
		int m_Key, m_Scancode, m_Mods;
		KeyboardHandler *m_Handler;
	};
	
	// MAKE ADAPTER FOR EVENT HANDLER
	// ------------------------------
	std::shared_ptr<KeyboardHandler> makeKeyboardPushHandler(void(*function)(int, int, int), EventHandlingMode handlingMode = EventHandlingMode::Immediately);
	std::shared_ptr<KeyboardHandler> makeKeyboardReleseHandler(void(*function)(int, int, int), EventHandlingMode handlingMode = EventHandlingMode::Immediately);

	template<typename T>
	std::shared_ptr<KeyboardHandler> makeKeyboardPushHandler(T &object, void(T::*function)(int, int, int), EventHandlingMode handlingMode = EventHandlingMode::Immediately) {
		
		class Adapter : public KeyboardHandler {
		public:
			typedef void(T::*myFunPtr)(int, int, int);
		public:
			Adapter(T &obj, myFunPtr function, EventHandlingMode handlingMode) : KeyboardHandler(EventType::KeyPush, handlingMode), m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(int key, int scancode, int mods) const {
				(m_Object->*m_FunPtr)(key, scancode, mods);
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
		return std::shared_ptr<KeyboardHandler>(new Adapter(object, function, handlingMode));
	}

	template<typename T>
	std::shared_ptr<KeyboardHandler> makeKeyboardReleseHandler(T &object, void(T::*function)(int, int, int), EventHandlingMode handlingMode = EventHandlingMode::Immediately) {
		
		class Adapter : public KeyboardHandler {
		public:
			typedef void(T::*myFunPtr)(int, int, int);
		public:
			Adapter(T &obj, myFunPtr function, EventHandlingMode handlingMode) : KeyboardHandler(EventType::KeyRelese, handlingMode), m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(int key, int scancode, int mods) const {
				(m_Object->*m_FunPtr)(key, scancode, mods);
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
		return std::shared_ptr<KeyboardHandler>(new Adapter(object, function, handlingMode));
	}
	
}

#endif //KEYBOARD_EVENT_H