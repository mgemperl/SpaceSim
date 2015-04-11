#pragma once

#include "Data.h"
#include <string>

namespace SpaceSimNS
{

template<class entity_type>
class State;

template <class entity_type>
class StateMachine
{
public:

	StateMachine(entity_type* owner)
	{
		m_pEntity = owner;
		m_pCurrentState = NULL;
		m_pPrevState = NULL;
		m_pGlobalState = NULL;
	}

	~StateMachine()
	{
		if (m_pCurrentState != NULL)
		{
			delete m_pCurrentState;
		}

		if (m_pPrevState != NULL)
		{
			delete m_pPrevState;
		}

		if (m_pGlobalState != NULL)
		{
			delete m_pGlobalState;
		}
	}

	// To initialize state machine
	void SetState(State<entity_type>* state)
	{
		if (m_pCurrentState != NULL)
		{
			delete m_pCurrentState;
		}

		m_pCurrentState = state;
	}
	void SetGlobalState(State<entity_type>* globalState)
	{
		if (m_pGlobalState != NULL)
		{
			delete m_pCurrentState;
		}

		m_pGlobalState = globalState;
	}
	void SetPrevState(State<entity_type>* prevState)
	{
		if (m_pPrevState != NULL)
		{
			delete m_pCurrentState;
		}

		m_pPrevState = prevState;
	}

	State<entity_type>* CurrentState() const
	{
		return m_pCurrentState;
	}
	State<entity_type>* PrevState() const
	{
		return m_pPrevState;
	}
	State<entity_type>* GlobalState() const
	{
		return m_pGlobalState;
	}

	bool IsInState(StateType state) const
	{
		return m_pCurrentState->GetState() == state;
	}

	void Update() const
	{
		if (m_pGlobalState != NULL)
		{
			m_pGlobalState->Execute();
		}

		if (m_pCurrentState != NULL)
		{
			m_pCurrentState->Execute();
		}
	}

	void ChangeState(State<entity_type>* pNeoState)
	{
		if (pNeoState != NULL)
		{
			if (m_pCurrentState != NULL)
			{
				m_pCurrentState->Exit();
				delete m_pPrevState;
				m_pPrevState = m_pCurrentState;
			}

			m_pCurrentState = pNeoState;
			m_pCurrentState->Enter();
		}
	}

	void RevertToPrevState()
	{
		if (m_pPrevState != NULL)
		{
			State<entity_type> pTemp = m_pCurrentState;
			
			m_pCurrentState->Exit();
			m_pCurrentState = m_pPrevState;
			m_pCurrentState->Enter();
			m_pPrevState = pTemp;
		}
	}

private:

	entity_type* m_pEntity;

	State<entity_type>* m_pCurrentState;
	State<entity_type>* m_pPrevState;
	State<entity_type>* m_pGlobalState;


};

}
