#include "mouseMoveEvent.h"

namespace systemEvent {

	std::shared_ptr<MouseMoveHandler> makeMouseMoveHandler(void(*function)(double, double), EventHandlingMode handlingMode) {
		
		class Adapter : public MouseMoveHandler {
		public:
			using myFunPtr = void(*)(double, double);
		public:
			Adapter(myFunPtr function, EventHandlingMode handlingMode) : MouseMoveHandler(EventType::MouseScroll, handlingMode), m_FunPtr(function) { }
		public:
			void handle(double xpos, double ypos) const override {
				(*m_FunPtr)(xpos, ypos);
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
		
		return std::shared_ptr<MouseMoveHandler>(new Adapter(function, handlingMode));
	}
	
}
