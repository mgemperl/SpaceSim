#include "MessageDispatcher.h"
#include "EntityManager.h"
#include "Clock.h"
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
	// If there is no delay, discharge the message; otherwise, set 
	// its discharge time and put it in the message queue.
	if (message.delay <= 0)
	{
		DischargeMessage(message);
	}
	else
	{
		message.dischargeTime = Clock::GameTimeSeconds() + message.delay;
		m_messageQueue.insert(message);
	}
}

void MessageDispatcher::Update(double elapsedTime)
{
	double time = Clock::GameTimeSeconds();
	std::set<Telegram>::const_iterator front = m_messageQueue.begin();

	// Discharge all messages whose delays have elapsed
	while ((front != m_messageQueue.end()) && (time >= front->dischargeTime))
	{
		DischargeMessage(*front);
		m_messageQueue.erase(front);
		front = m_messageQueue.begin();
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
	m_messageQueue.clear();
}

void MessageDispatcher::Initialize()
{
	m_messageQueue.clear();
}

}