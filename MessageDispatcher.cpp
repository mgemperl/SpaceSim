#include "MessageDispatcher.h"
#include "EntityManager.h"
#include "Data.h"

namespace SpaceSimNS
{

MessageDispatcher* MessageDispatcher::Instance()
{
	static MessageDispatcher instance;
	return &instance;
}

void MessageDispatcher::DispatchTelegram(Telegram& message)
{
	message.dispatchTick = message.delay + *m_pGameTick;
	m_MessageQueue.insert(message);
}

void MessageDispatcher::DispatchMessages()
{
	std::set<Telegram>::const_iterator front = m_MessageQueue.begin();
		
	while ((front != m_MessageQueue.end()) && (front->dispatchTick <= *m_pGameTick))
	{
		DischargeMessage(*front);
		m_MessageQueue.erase(front);
		front = m_MessageQueue.begin();
	}
}

void MessageDispatcher::DischargeMessage(const Telegram& message)
{
	Entity* receiver = EntityManager::Instance()->GetEntityFromID(message.receiverID);
	
	if (receiver != NULL)
	{
		receiver->HandleMessage(message);
	}
}

void MessageDispatcher::ClearMessages()
{
	m_MessageQueue.clear();
}

void MessageDispatcher::Initialize()
{
}

}