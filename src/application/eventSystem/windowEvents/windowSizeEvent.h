#ifndef WINDOW_SIZE_EVENT_H
#define WINDOW_SIZE_EVENT_H

#include <memory>
#include "../event.h"

namespace systemEvent {
	
	// EVENT HANDLER
	// -------------
	class WindowSizeHandler : public EventHandler {
	protected:
		WindowSizeHandler(EventType typeEvent, EventHandlingMode handlingMode) : EventHandler(typeEvent, handlingMode) { }
	public:
		virtual void handle(int width, int height) const = 0;
	public:
		virtual ~WindowSizeHandler() {}
	};
	
	// EVENTS 
	// ------
	class WindowSizeEvent : public Event {
	public:
		WindowSizeEvent(int width, int height, WindowSizeHandler *handler, EventType typeEvent)
			: Event(typeEvent), m_Width(width), m_Heigth(height),
				m_Handler(handler)
		{ }
	public:
		void call() const override { m_Handler->handle(m_Width, m_Heigth); }
	private:
		int m_Width, m_Heigth;
		WindowSizeHandler *m_Handler;
	};
	
	// MAKE ADAPTER FOR EVENT HANDLER
	// ------------------------------
	std::shared_ptr<WindowSizeHandler> makeWindowSizeHandler(void(*function)(int, int), EventHandlingMode handlingMode = EventHandlingMode::Immediately);

	template<typename T>
	std::shared_ptr<WindowSizeHandler> makeWindowSizeHandler(T &object, void(T::*function)(int, int), EventHandlingMode handlingMode = EventHandlingMode::Immediately) {
		
		class Adapter : public WindowSizeHandler {
		public:
			typedef void(T::*myFunPtr)(int, int);
		public:
			Adapter(T &obj, myFunPtr function, EventHandlingMode handlingMode) : WindowSizeHandler(EventType::WindowSize, handlingMode), m_Object(&obj), m_FunPtr(function) { }
		public:
			virtual void handle(int width, int height) const {
				(m_Object->*m_FunPtr)(width, height);
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
		return std::shared_ptr<WindowSizeHandler>(new Adapter(object, function, handlingMode));
	}

	
}

#endif //WINDOW_SIZE_EVENT_H