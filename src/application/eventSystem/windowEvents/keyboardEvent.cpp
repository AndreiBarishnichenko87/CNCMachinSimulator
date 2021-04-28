#include "keyboardEvent.h"

namespace systemEvent {

	std::shared_ptr<KeyboardHandler> makeKeyboardPushHandler(void(*function)(int, int, int)) {
		
		class Adapter : public KeyboardHandler {
		public:
			using myFunPtr = void(*)(int, int, int);
		public:
			Adapter(myFunPtr function) : KeyboardHandler(TypeHandler::KEY_PUSH), m_FunPtr(function) { }
		public:
			void handle(int key, int scancode, int mods) const override {
				(*m_FunPtr)(key, scancode, mods);
			}
		private:
			bool is_equal(KeyboardHandler &handler) const override {
				Adapter *adapterPtr = dynamic_cast<Adapter*>(&handler);
				if(adapterPtr == nullptr) {
					return false;
				}
				return (m_FunPtr == adapterPtr->m_FunPtr) ? true : false;
			}
		private:
			myFunPtr m_FunPtr;
		};
		
		return std::shared_ptr<KeyboardHandler>(new Adapter(function));
	}
	
	std::shared_ptr<KeyboardHandler> makeKeyboardReleseHandler(void(*function)(int, int, int)) {
		
		class Adapter : public KeyboardHandler {
		public:
			using myFunPtr = void(*)(int, int, int);
		public:
			Adapter(myFunPtr function) : KeyboardHandler(TypeHandler::KEY_RELESE), m_FunPtr(function) { }
		public:
			void handle(int key, int scancode, int mods) const override {
				(*m_FunPtr)(key, scancode, mods);
			}
		private:
			bool is_equal(KeyboardHandler &handler) const override {
				Adapter *adapterPtr = dynamic_cast<Adapter*>(&handler);
				if(adapterPtr == nullptr) {
					return false;
				}
				return (m_FunPtr == adapterPtr->m_FunPtr) ? true : false;
			}
		private:
			myFunPtr m_FunPtr;
		};
		
		return std::shared_ptr<KeyboardHandler>(new Adapter(function));
	}
	
}
