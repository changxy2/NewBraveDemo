// ======================================================================================
// File         : Subject.h
// Author       : Zhang yuzhong 
// Last Change  : 12/6/2010
// Description  : (信号插槽设计模式)插槽类
//				  
// ======================================================================================

#include "game_pch.h"
#include "Slot.h"

inline
Slot0::Slot0(void)
{
}

inline
Slot0::~Slot0(void)
{
}


template<typename ClassType> inline
MemberSlot0<ClassType>::MemberSlot0() :
	Slot0(),
	m_pInstance(NULL),
	m_pfnCallback(NULL)
{

}

template<typename ClassType> inline
MemberSlot0<ClassType>::MemberSlot0(ClassType* pInstance, 
									void (ClassType::*pfnCallback)()):
	Slot0(),
	m_pInstance(pInstance),
	m_pfnCallback(pfnCallback)
{
	NIASSERT(pInstance != NULL);
	NIASSERT(pfnCallback != NULL);
}
template<typename ClassType> inline
MemberSlot0<ClassType>::Initialize(ClassType* pInstance, 
								   void (ClassType::*pfnCallback)())
{
	NIASSERT(pInstance);
	NIASSERT(pfnCallback);
	m_pInstance = pInstance;
	m_pfnCallback = pfnCallback;
}

template<typename ClassType> inline
MemberSlot0<ClassType>::~MemberSlot0()
{
	m_pInstance = NULL;
	m_pfnCallback = NULL;
}

template<typename ClassType> inline
MemberSlot0<ClassType>::ReceiveSignal() const
{
	NIASSERT(m_pInstance);
	NIASSERT(m_pfnCallback);
	(m_pInstance->*m_pfnCallback)();
}

template<typename Arg1Type> inline
Slot1<Arg1Type>::Slot1()
{
}

template<typename Arg1Type> inline
Slot1<Arg1Type>::~Slot1()
{
}


template <typename ClassType, typename Arg1Type> inline
MemberSlot1<ClassType, Arg1Type>::MemberSlot1() :
	Slot1<Arg1Type>(),
	m_pInstance(NULL),
	m_pfnCallback(NULL)
{

}

template <typename ClassType, typename Arg1Type> inline
MemberSlot1<ClassType, Arg1Type>::MemberSlot1(ClassType* pInstance, 
											  void (ClassType::*pfnCallback)(Arg1Type)):
	Slot1<Arg1Type>(),
	m_pInstance(pInstance),
	m_pfnCallback(pfnCallback)
{
	NIASSERT(m_pInstance);
	NIASSERT(m_pfnCallback);
}

template <typename ClassType, typename Arg1Type> inline
MemberSlot1<ClassType, Arg1Type>::Initialize(ClassType* pInstance, 
											 void (ClassType::*pfnCallback)(Arg1Type))
{
	NIASSERT(pInstance);
	NIASSERT(pfnCallback);
	m_pInstance = pInstance;
	m_pfnCallback = pfnCallback;
}

template <typename ClassType, typename Arg1Type> inline
MemberSlot1<ClassType, Arg1Type>::~MemberSlot1()
{
	m_pInstance = NULL;
	m_pfnCallback = NULL;
}

template <typename ClassType, typename Arg1Type> inline
MemberSlot1<ClassType, Arg1Type>::ReceiveSignal(Arg1Type Arg1) const
{
	NIASSERT(m_pInstance);
	NIASSERT(m_pfnCallback);
	(m_pInstance->*m_pfnCallback)(Arg1);
}
