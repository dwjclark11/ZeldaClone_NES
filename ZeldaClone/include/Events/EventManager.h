#pragma once
#include "Event.h"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>
#include <cassert>
#include <functional>

// Interface class
class IEventCallback
{
  private:
	virtual void Call( Event& event ) = 0;

  public:
	virtual ~IEventCallback() = default;

	void Execute( Event& event ) { Call( event ); }
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback
{
  private:
	using CallbackFunction = void ( TOwner::* )( TEvent& );
	TOwner* ownerInstance;
	CallbackFunction callbackFunction;

	virtual void Call( Event& event ) override
	{
		std::invoke( callbackFunction, ownerInstance, static_cast<TEvent&>( event ) );
	}

  public:
	EventCallback( TOwner* ownerInstance, CallbackFunction callbackFunction )
		: ownerInstance{ ownerInstance }
		, callbackFunction{ callbackFunction }
	{
	}

	virtual ~EventCallback() override = default;
};

using Handlers = std::vector<std::unique_ptr<IEventCallback>>;

class EventManager
{
  private:
	// The key is the type index, the value is a pointer to event handlers
	std::unordered_map<std::type_index, std::unique_ptr<Handlers>> subscribers;

  public:
	EventManager() = default;
	~EventManager() = default;

	// Clears the subscriber list
	void Reset() { subscribers.clear(); }

	/*
		==========================================================================================================
			Subscribe to an event type <T>
			In our implementation, a listener subscribes to an event
			Example : eventManager->SubsribeToEvent<CollisionEvent>(&Game::OnCollision); // callback
		==========================================================================================================
	*/
	template <typename TEvent, typename TOwner>
	void SubscribeToEvent( TOwner* ownerInstance, void ( TOwner::*callbackFunction )( TEvent& ) )
	{
		auto subscriberItr = subscribers.find( typeid( TEvent ) );
		if ( subscriberItr == subscribers.end() )
		{
			auto pHandlers = std::make_unique<Handlers>();
			auto [ it, result ] = subscribers.emplace( typeid( TEvent ), std::move( pHandlers ) );
			assert( result && "Not working!" );
			subscriberItr = it;
		}

		auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>( ownerInstance, callbackFunction );
		subscriberItr->second->push_back( std::move( subscriber ) );
	}

	/*
		==========================================================================================================
			Emit an event type <T>
			In our implementation, a listener emits an event
			we go ahead an execute all the listener callback functions
			Example : eventManager->EmitEvent<CollisionEvent>(player, enemy)
		==========================================================================================================
	*/
	template <typename TEvent, typename... TArgs>
	void EmitEvent( TArgs&&... args )
	{
		auto handlerItr = subscribers.find( typeid( TEvent ) );
		if ( handlerItr == subscribers.end() )
			return;

		for ( const auto& handler : *handlerItr->second )
		{
			TEvent event{ std::forward<TArgs>( args )... };
			handler->Execute( event );
		}
	}
};
