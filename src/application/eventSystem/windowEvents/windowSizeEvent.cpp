#include "windowSizeEvent.h"

namespace systemEvent {

	std::shared_ptr<WindowSizeHandler> makeWindowSizeHandler(void(*function)(int, int), EventHandlingMode handlingMode) {
		
		class Adapter : public WindowSizeHandler {
		public:
			using myFunPtr = void(*)(int, int);
		public:
			Adapter(myFunPtr function, EventHandlingMode handlingMode) : WindowSizeHandler(EventType::WindowSize, handlingMode), m_FunPtr(function) { }
		public:
			void handle(int width, int height) const override {
				(*m_FunPtr)(width, height);
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
		
		return std::shared_ptr<WindowSizeHandler>(new Adapter(function, handlingMode));
	}
}
