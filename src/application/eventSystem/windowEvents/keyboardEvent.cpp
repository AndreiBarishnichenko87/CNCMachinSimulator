#include "keyboardEvent.h"

namespace systemEvent {

	std::shared_ptr<KeyboardHandler> makeKeyboardPushHandler(void(*function)(int, int, int), EventHandlingMode handlingMode) {
		
		class Adapter : public KeyboardHandler {
		public:
			using myFunPtr = void(*)(int, int, int);
		public:
			Adapter(myFunPtr function, EventHandlingMode handlingMode) : KeyboardHandler(EventType::KeyPush, handlingMode), m_FunPtr(function) { }
		public:
			void handle(int key, int scancode, int mods) const override {
				(*m_FunPtr)(key, scancode, mods);
			}
		private:
			bool is_equal(EventHandler &handler) const override {
				Adapter *adapterPtr = dynamic_cast<Adapter*>(&handler);
				if(adapterPtr == nullptr) {
					return false;
				}
				return (m_FunPtr == adapterPtr->m_FunPtr) ? true : false;
			}
		private:
			myFunPtr m_FunPtr;
		};
		
		return std::shared_ptr<KeyboardHandler>(new Adapter(function, handlingMode));
	}
	
	std::shared_ptr<KeyboardHandler> makeKeyboardReleseHandler(void(*function)(int, int, int), EventHandlingMode handlingMode) {
		
		class Adapter : public KeyboardHandler {
		public:
			using myFunPtr = void(*)(int, int, int);
		public:
			Adapter(myFunPtr function, EventHandlingMode handlingMode) : KeyboardHandler(EventType::KeyRelese, handlingMode), m_FunPtr(function) { }
		public:
			void handle(int key, int scancode, int mods) const override {
				(*m_FunPtr)(key, scancode, mods);
			}
		private:
			bool is_equal(EventHandler &handler) const override {
				Adapter *adapterPtr = dynamic_cast<Adapter*>(&handler);
				if(adapterPtr == nullptr) {
					return false;
				}
				return (m_FunPtr == adapterPtr->m_FunPtr) ? true : false;
			}
		private:
			myFunPtr m_FunPtr;
		};
		
		return std::shared_ptr<KeyboardHandler>(new Adapter(function, handlingMode));
	}
	
}
