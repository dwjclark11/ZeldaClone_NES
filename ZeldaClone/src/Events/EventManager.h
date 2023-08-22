#pragma once
#include "../Logger/Logger.h"
#include "Event.h"
#include <map>
#include <typeindex>
#include <memory>
#include <list>

// Interface class 
class IEventCallback
{
private:
	virtual void Call(Event& event) = 0;

public:
	virtual ~IEventCallback() = default;
	
	void Execute(Event& event)
	{
		Call(event);
	}
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback
{
private:
	typedef void (TOwner::* CallbackFunction)(TEvent&);

	TOwner* ownerInstance;
	CallbackFunction callbackFunction;

	virtual void Call(Event& event) override
	{
		std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(event));
	}

public:
	EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction)
		: ownerInstance{ownerInstance}, callbackFunction{callbackFunction}
	{
		//this->ownerInstance = ownerInstance;
		//this->callbackFunction = callbackFunction;
	}

	virtual ~EventCallback() override = default;
};
typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventManager
{
private:
	// The key is the type index, the value is a pointer to event handlers
	std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
	EventManager()
	{

	}
	~EventManager()
	{

	}

	// Clears the subscriber list
	void Reset()
	{
		subscribers.clear();
	}

	/*
		==========================================================================================================
			Subscribe to an event type <T>
			In our implementation, a listener subscribes to an event
			Example : eventManager->SubsribeToEvent<CollisionEvent>(&Game::OnCollision); // callback
		==========================================================================================================
	*/
	template <typename TEvent, typename TOwner>
	void SubscribeToEvent(TOwner* ownerInstance, void(TOwner::* callbackFunction)(TEvent&))
	{
		if (!subscribers[typeid(TEvent)])
		{
			subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
		}
		auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
		subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
	}

	/*
		==========================================================================================================
			Emit an event type <T>
			In our implementation, a listener emits an event
			we go ahead an execute all the listener callback functions
			Example : eventManager->EmitEvent<CollisionEvent>(player, enemy)
		==========================================================================================================
	*/
	template <typename TEvent, typename ...TArgs>
	void EmitEvent(TArgs&& ...args)
	{
		auto handlers = subscribers[typeid(TEvent)].get();
		if (!handlers)
			return;
		
		for (const auto& handler : *handlers)
		{
			TEvent event(std::forward<TArgs>(args)...);
			handler->Execute(event);
		}
	}
};
