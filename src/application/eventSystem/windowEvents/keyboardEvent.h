#ifndef KEYBOARD_EVENT_H
#define KEYBOARD_EVENT_H

#include <memory>
#include "../event.h"

namespace systemEvent {
	
	// EVENT HANDLER
	// -------------
	class KeyboardHandler {
	public:
		enum TypeHandler {
			KEY_PUSH = 1,
			KEY_RELESE
		};
	protected:
		KeyboardHandler(TypeHandler handlerType) : m_Type(handlerType) { }
	public:
		bool operator==(KeyboardHandler &handler) const { return is_equal(handler); }
		bool operator!=(KeyboardHandler &handler) const { return !(*this == handler); }
	private:
		virtual bool is_equal(KeyboardHandler &handler) const = 0;
	public:
		virtual void handle(int key, int scancode, int mods) const = 0;
		TypeHandler getType() const { return m_Type; }
	private:
		TypeHandler m_Type;
	public:
		virtual ~KeyboardHandler() {}
	};
	
	
	// MAKE ADAPTER FOR EVENT HANDLER
	// ------------------------------
	std::shared_ptr<KeyboardHandler> makeKeyboardPushHandler(void(*function)(int, int, int));
	std::shared_ptr<KeyboardHandler> makeKeyboardReleseHandler(void(*function)(int, int, int));
	
	template<typename T>
	std::shared_ptr<KeyboardHandler> makeKeyboardPushHandler(T &object, void(T::*function)(int, int, int)) {
		
		class Adapter : public KeyboardHandler {
		public:
			typedef void(T::*myFunPtr)(int, int, int);
		public:
			Adapter(T &obj, myFunPtr function) : KeyboardHandler(TypeHandler::KEY_PUSH), m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(int key, int scancode, int mods) const {
				(m_Object->*m_FunPtr)(key, scancode, mods);
			}
		private:
			bool is_equal(KeyboardHandler &handler) const override {
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
		return std::shared_ptr<KeyboardHandler>(new Adapter(object, function));
	}
	
	template<typename T>
	std::shared_ptr<KeyboardHandler> makeKeyboardReleseHandler(T &object, void(T::*function)(int, int, int)) {
		
		class Adapter : public KeyboardHandler {
		public:
			typedef void(T::*myFunPtr)(int, int, int);
		public:
			Adapter(T &obj, myFunPtr function) : KeyboardHandler(TypeHandler::KEY_RELESE), m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(int key, int scancode, int mods) const {
				(m_Object->*m_FunPtr)(key, scancode, mods);
			}
		private:
			bool is_equal(KeyboardHandler &handler) const override {
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
		return std::shared_ptr<KeyboardHandler>(new Adapter(object, function));
	}
	
	// EVENT 
	// -----
	class KeyboardEvent : public Event {
	public:
		KeyboardEvent(int key, int scancode, int mods, KeyboardHandler *handler)
			: Event(EventType::KeyboardPush), m_Key(key), m_Scancode(scancode), m_Mods(mods),
				m_Handler(handler)
		{ }
	public:
		void call() const override { m_Handler->handle(m_Key, m_Scancode, m_Mods); }
	private:
		int m_Key, m_Scancode, m_Mods;
		KeyboardHandler *m_Handler;
	};
}

#endif //KEYBOARD_EVENT_H