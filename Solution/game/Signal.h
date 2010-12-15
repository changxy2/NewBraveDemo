// ======================================================================================
// File         : Subject.h
// Author       : Zhang yuzhong 
// Last Change  : 12/6/2010
// Description  : (信号插槽设计模式)信号类 可以对应多个插槽
//				  
// ======================================================================================

#pragma once

#include <NiTSet.h>
class Slot0;
class Signal0 :	public NiMemObject
{
public:
	Signal(void);
	virtual ~Signal(void);
	void EmitSignal() const;
	void Subscribe(Slot0* pSlot);
	void UnSubscribe(Slot0* pSlot);
	bool IsSubscrible(Slot0* pSlot);
	unsigned int NumSubscribed() const;

protected:
	NiTPrimitiveSet<Slot0*> m_setSlots;
};


template <typename Arg1Type>
class Slot1;

template <typename Arg1Type>
class Signal1 : public NiMemObject
{
public:
	Signal1();
	virtual ~Signal1();
	void EmitSignal(Arg1Type Arg1) const;
	void Subscribe(Slot1<Arg1Type>* pSlot);
	void Unsubscribe(Slot1<Arg1Type>* pSlot);
	bool IsSubscribe(Slot1<Arg1Type>* pSlot);
	unsigned int NumSubscribed() const;
protected:
	NiTPrimitiveSet<Slot1<Arg1Type>*> m_setSlots;
};

#include "Signal.inl"