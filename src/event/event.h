#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include <algorithm>

namespace event {

	template<class ...TParams>
	class AbstractEventHandler {
		typedef AbstractEventHandler<TParams...> MyType;
	public:
		virtual void call(TParams... params ) = 0;
		virtual ~AbstractEventHandler() {}
	public:
		bool operator==(const MyType &other) const {
			return isEquals(other);
		}
		bool operator!=(const MyType &other) const {
			return !(*this == other);
		}
	protected:
		virtual bool isEquals(const MyType &other) const = 0;
	protected:
		AbstractEventHandler() {}
	};

	template<typename ...TParams>
	class IEvent {
	protected:
		typedef AbstractEventHandler<TParams...> TEventHandler;
	public:
		bool operator+=(TEventHandler &eventHandler) {
			return addHandler(eventHandler);
		}
		bool operator-=(TEventHandler &eventHandler) {
			return removeHandler(eventHandler);
		}
	protected:
		IEvent() {}
		virtual ~IEvent() {}
		virtual bool addHandler(TEventHandler &eventHandler) = 0;
		virtual bool removeHandler(TEventHandler &eventHandler) = 0;
	};

	template<class ...TParams> 
	class TEvent : public IEvent<TParams...> {
		typedef AbstractEventHandler<TParams...> TEventHandler;
		typedef typename std::list<TEventHandler*>::const_iterator TEventHandlerIt;
	private:
		std::list<TEventHandler*> m_handlers;
	private:
		inline TEventHandlerIt findEventHandler(TEventHandler &eventHandler) const {
			return std::find_if(m_handlers.cbegin(), m_handlers.cend(), [&eventHandler](const TEventHandler *oneHandler) 
				{return (*oneHandler == eventHandler); } );
		}
	protected:
		virtual bool addHandler(TEventHandler &eventHandler) override {
			if(findEventHandler(eventHandler) == m_handlers.end()) {
				m_handlers.push_back(&eventHandler);
				return true;
			}
			return false;
		}
		virtual bool removeHandler(TEventHandler &eventHandler) override {
			auto it = findEventHandler(eventHandler);
			if(it != m_handlers.end()){
				TEventHandler *removedEventHandler = *it;
				m_handlers.erase(it);
				delete removedEventHandler;
				return true;
			}
			return false;
		}
	public:
		TEvent() : IEvent<TParams...>() { }
		~TEvent() {
			for(TEventHandler *oneHandler : m_handlers)
				delete oneHandler;
			m_handlers.clear();
		}
	public:
		void operator() (TParams... params) {
			for(TEventHandler *oneHandler :m_handlers)
				oneHandler->call(params...);
		}
	};

	template<class TObject, class ...TParams>
	class MethodEventHandler : public AbstractEventHandler<TParams...> {
		typedef void (TObject::*TMethod)(TParams...);
		typedef MethodEventHandler<TObject, TParams...> MyType;
	private:
		TObject& m_object;
		TMethod m_method;
	protected:
		virtual bool isEquals(const AbstractEventHandler<TParams...> &other) const override {
			const MyType* _other = dynamic_cast<const MyType*>(&other);
			return ((_other != nullptr) && (&m_object == &(_other->m_object)) && (m_method == _other->m_method));
		}
	public:
		MethodEventHandler(TObject& object, TMethod method)
			: AbstractEventHandler<TParams...>(), m_object(object), m_method(method) {
			assert(m_method != nullptr);
		}
		virtual void call(TParams... params) override final {
			(m_object.*m_method)(params...);
		}
	};
	
	template<class TObject, class ...TParams>
	AbstractEventHandler<TParams...> &createMethodEventHandler(TObject& object, void(TObject::*method)(TParams...)) {
		return *new MethodEventHandler<TObject, TParams...> (object, method);
	}

}