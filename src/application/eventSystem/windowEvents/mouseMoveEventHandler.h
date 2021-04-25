#ifndef MOUSE_MOVE_EVENT_HANDLERS_H
#define MOUSE_MOVE_EVENT_HANDLERS_H

#include <memory>

namespace systemEvent {
	
	class MouseMoveHandler {
	public:
		virtual ~MouseMoveHandler() {}
	public:
		bool operator==(MouseMoveHandler &handler) const { return is_equal(handler); }
		bool operator!=(MouseMoveHandler &handler) const { return !(*this == handler); }
	private:
		virtual bool is_equal(MouseMoveHandler &handler) const = 0;
	public:
		virtual void handle(double xpos, double ypos) const = 0;
	};
	
	
	// Make adapter for mouseMoveHandler base class
	// --------------------------------------------
	std::shared_ptr<MouseMoveHandler> makeMouseMoveHandler(void(*function)(double, double));
	
	template<typename T>
	std::shared_ptr<MouseMoveHandler> makeMouseMoveHandler(T &object, void(T::*function)(double, double)) {
		
		class Adapter : public MouseMoveHandler {
		public:
			typedef void(T::*myFunPtr)(double, double);
		public:
			Adapter(T &obj, myFunPtr function) : m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(double xpos, double ypos) const {
				(m_Object->*m_FunPtr)(xpos, ypos);
			}
		private:
			bool is_equal(MouseMoveHandler &handler) const override {
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
		return std::shared_ptr<MouseMoveHandler>(new Adapter(object, function));
	}

	
}

#endif //MOUSE_MOVE_EVENT_HANDLERS_H