#ifndef MOUSE_EVENT_HANDLERS_H
#define MOUSE_EVENT_HANDLERS_H

#include <memory>

namespace systemEvent {
	
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
	
	
	// Make adapter for mouseMoveHandler base class
	// --------------------------------------------
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

	
}

#endif //MOUSE_EVENT_HANDLERS_H