#include "mouseScrollEvent.h"

namespace systemEvent {

	std::shared_ptr<MouseScrollHandler> makeMouseScrollHandler(void(*function)(double),  EventHandlingMode handlingMode) {
		
		class Adapter : public MouseScrollHandler {
		public:
			using myFunPtr = void(*)(double);
		public:
			Adapter(myFunPtr function, EventHandlingMode handlingMode) : MouseScrollHandler(EventType::MouseScroll, handlingMode), m_FunPtr(function) { }
		public:
			void handle(double yoffset) const override {
				(*m_FunPtr)(yoffset);
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
		
		return std::shared_ptr<MouseScrollHandler>(new Adapter(function, handlingMode));
	}
	
}
