// ======================================================================================
// File         : Subject.h
// Author       : Zhang yuzhong 
// Last Change  : 12/6/2010
// Description  : (信号插槽设计模式)信号类
//				  
// ======================================================================================

#include "game_pch.h"
#include "Signal.h"
#include "SignalSlotMacros.h"

inline
Signal0::Signal0(void)
{
}

inline
Signal0::~Signal0(void)
{
	UnsubscribeFromAll(m_setSlots);
}

inline
void Signal0::EmitSignal() const
{
	EmitSignalToAll(m_setSlots, ());
}

inline
void Signal0::Subscribe(Slot0* pSlot)
{
	SubscribeToMe(m_setSlots, pSlot);
}

inline
void Signal0::UnSubscribe(Slot0* pSlot)
{
	UnsubscribeToMe(m_setSlots, pSlot);
}

inline
bool Signal0::IsSubscrible(Slot0* pSlot)
{
	return IsElementInGroup(m_setSlots, pSlot);
}

inline
unsigned int Signal0::NumSubscribed() const
{
	return NumElements(m_setSlots);
}

template <typename Arg1Type> inline
Signal1<Arg1Type>::Signal1()
{

}

template <typename Arg1Type> inline
Signal1<Arg1Type>::~Signal1()
{
	UnsubscribeFromAll(m_setSlots);
}


template <typename Arg1Type> inline
void Signal1<Arg1Type>::EmitSignal(Arg1Type Arg1) const
{
	EmitSignalToAll(m_setSlots, (Arg1));
}

template <typename Arg1Type> inline
void Signal1<Arg1Type>::Subscribe(Slot1<Arg1Type>* pSlot)
{
	SubscribeToMe(m_setSlots, pSlot);
}

template <typename Arg1Type> inline
void Signal1<Arg1Type>::Unsubscribe(Slot1<Arg1Type>* pSlot)
{
	UnsubscribeToMe(m_setSlots, pSlot);
}

template <typename Arg1Type> inline
bool Signal1<Arg1Type>::IsSubscribe(Slot1<Arg1Type>* pSlot)
{
	IsElementInGroup(m_setSlots, pSlot);
}

template <typename Arg1Type> inline
unsigned int Signal1<Arg1Type>::NumSubscribed() const
{
	return NumElements(m_setSlots);
}
