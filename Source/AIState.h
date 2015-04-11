#pragma once

#include "State.h"

namespace SpaceSimNS
{

class Message;

template <class entity_type>
class AIState : public State<entity_type>
{
public:

	AIState(entity_type* controller) : State<entity_type>(controller) {}
	virtual ~AIState() {}

	virtual void OnMessage(const Telegram& message) = 0;

};

}