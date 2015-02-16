#pragma once

#include <set>


namespace SpaceSimNS
{

struct Telegram;

class MessageDispatcher
{
public:
	typedef unsigned long TICK;

	static MessageDispatcher* Instance();

	void DispatchTelegram(Telegram& message);

	void DispatchMessages();

	void ClearMessages();

	void Initialize();

private:
	std::set<Telegram> m_MessageQueue;
	const TICK* m_pGameTick;

	void DischargeMessage(const Telegram& message);

	MessageDispatcher() {};
	~MessageDispatcher() {};
};

}