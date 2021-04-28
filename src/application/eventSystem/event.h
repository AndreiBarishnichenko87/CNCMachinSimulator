#ifndef EVENT_H
#define EVENT_H

#include <cstdint>

namespace systemEvent {
	
	enum EventType : uint8_t {
		None = 0,
		MouseMove, 
		MouseButton,
		MouseScroll, 
		KeyboardPush
	};

	class Event {
	protected:
		Event(EventType typeEvent = EventType::None) : m_TypeEvent(typeEvent) { }
	public:
		virtual ~Event() { }
	public:
		virtual void call() const = 0;
		EventType getTypeEvent() const { return m_TypeEvent; }
	private:
		EventType m_TypeEvent;
	};


}

#endif //EVENT_H