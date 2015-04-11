#pragma once

#include "Data.h"

namespace SpaceSimNS
{

template <class entity_type>
class State
{
public:
	State(entity_type* pEntity) : m_pEntity{ pEntity } {}
	virtual ~State() {}

	virtual void Enter() = 0;

	virtual void Execute() = 0;

	virtual StateType GetState() const = 0;

	virtual void Exit() = 0;

	virtual LRESULT HandleUserInput(
		UINT message, WPARAM wParam, LPARAM lParam) 
	{
		return S_OK;
	}

protected:

	entity_type* m_pEntity;

};

}