#ifndef EVENT_H
#define EVENT_H

#include <cstdint>

namespace systemEvent {
	
	enum EventType : uint8_t {
		None = 0,
		MouseMove, 
		MouseButtonPush,
		MouseButtonRelese,
		MouseScroll, 
		KeyPush,
		KeyRelese,
		KeyRepeat,
		WindowSize
	};
	
	enum EventHandlingMode : uint8_t { 
		Default = 0, 
		Immediately 
	};
	
	class Event {
	protected:
		explicit Event(EventType typeEvent = EventType::None) : m_TypeEvent(typeEvent) { }
	public:
		virtual void call() const = 0;
		EventType getEventType() const { return m_TypeEvent; }
	public:
		virtual ~Event() { }
	private:
		EventType m_TypeEvent;
	};
	
	class EventHandler {
	protected:
		EventHandler(EventType typeEvent, EventHandlingMode handlingMode) : m_TypeEvent(typeEvent), m_HandlingMode(handlingMode) {}
	private:
		virtual bool is_equal(EventHandler &handler) const = 0;
	public:
		EventHandlingMode getHandlerMode() const { return m_HandlingMode; }
		EventType getEventType() const { return m_TypeEvent; }
		bool operator==(EventHandler &handler) const { return is_equal(handler); }
		bool operator!=(EventHandler &handler) const { return !(*this == handler); }
	public:
		virtual ~EventHandler() { }
	private:
		EventType m_TypeEvent;
		EventHandlingMode m_HandlingMode;
	};

}

#endif //EVENT_H