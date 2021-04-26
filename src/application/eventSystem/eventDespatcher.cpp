#include "eventDespatcher.h"
#include "event.h"
#include "../window/window.h"

namespace systemEvent {
	
	template<typename EvStore, typename T, typename H>
	void EventDispatcher::addHandler(std::list<EvStore*> &container, T &eventGenerator, std::shared_ptr<H> handler) {
		if (eventGenerator.getHandlerStore() == nullptr) {
			container.push_back(new EvStore);
			eventGenerator.setHandlerStore(container.back());
		}
		eventGenerator.getHandlerStore()->addEventHandler(handler);
	}
	template void EventDispatcher::addHandler<WindowEventHandlersStore, application::Window, MouseMoveHandler>(std::list<WindowEventHandlersStore*> &container, application::Window &eventGenerator, std::shared_ptr<MouseMoveHandler> handler);
	template void EventDispatcher::addHandler<WindowEventHandlersStore, application::Window, MouseButtonHandler>(std::list<WindowEventHandlersStore*> &container, application::Window &eventGenerator, std::shared_ptr<MouseButtonHandler> handler);
	template void EventDispatcher::addHandler<WindowEventHandlersStore, application::Window, MouseScrollHandler>(std::list<WindowEventHandlersStore*> &container, application::Window &eventGenerator, std::shared_ptr<MouseScrollHandler> handler);
	
	template<typename EvStore, typename T, typename H>
	void EventDispatcher::deleteHandler(std::list<EvStore*> &container, T &eventGenerator, std::shared_ptr<H> handler) {
		if(eventGenerator.getHandlerStore() == nullptr)
			return;
		eventGenerator.getHandlerStore()->deleteEventHandler(handler);
		if (eventGenerator.getHandlerStore()->empty())
			removeHandlerStore(container, eventGenerator);
	}
	template void EventDispatcher::deleteHandler<WindowEventHandlersStore, application::Window, MouseMoveHandler>(std::list<WindowEventHandlersStore*> &container, application::Window &eventGenerator, std::shared_ptr<MouseMoveHandler> handler);
	template void EventDispatcher::deleteHandler<WindowEventHandlersStore, application::Window, MouseButtonHandler>(std::list<WindowEventHandlersStore*> &container, application::Window &eventGenerator, std::shared_ptr<MouseButtonHandler> handler);
	template void EventDispatcher::deleteHandler<WindowEventHandlersStore, application::Window, MouseScrollHandler>(std::list<WindowEventHandlersStore*> &container, application::Window &eventGenerator, std::shared_ptr<MouseScrollHandler> handler);
	
	template<typename EvStore, typename T>
	void EventDispatcher::removeHandlerStore(std::list<EvStore*> &container, T &eventGenerator) {
		EvStore* eventStore = eventGenerator.getHandlerStore();
		auto iter = std::find(container.begin(), container.end(), eventStore);
		if (iter != container.end()) {
			delete *iter;
			container.erase(iter);
			eventGenerator.setHandlerStore(nullptr);
		}
	}
	template void EventDispatcher::removeHandlerStore<WindowEventHandlersStore, application::Window>(std::list<WindowEventHandlersStore*> &container, application::Window &eventGenerator);
	
	
	EventDispatcher* EventDispatcher::instance() {
		static EventDispatcher dispatcher;
		return &dispatcher;
	}
	
	void EventDispatcher::handleAllEvents() {
		if (m_EventQueue.empty())
			return;
		while(!m_EventQueue.empty()) {
			m_EventQueue.front()->call();
			m_EventQueue.pop();
		}
	}

	void EventDispatcher::removeEventHandlerStore(application::Window &window) {
		removeHandlerStore(m_ListWindowHandlersStore, window);
	}

}
