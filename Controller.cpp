#include "Controller.h"
#include <stdlib.h>

namespace SpaceSimNS
{

template <typename T>
Controller<T>::Controller()
{
	m_pEntity = NULL;
}

template <typename T>
Controller<T>::~Controller()
{
}

template <typename T>
void Controller<T>::Unassign()
{
	m_pEntity = NULL;
}

template <typename T>
void Controller<T>::Assign(T* pEntity)
{
	m_pEntity = pEntity;
}

}