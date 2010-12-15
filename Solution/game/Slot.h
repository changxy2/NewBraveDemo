// ======================================================================================
// File         : Subject.h
// Author       : Zhang yuzhong 
// Last Change  : 12/6/2010
// Description  : (信号插槽设计模式)插槽类
//				  
// ======================================================================================

#pragma once

#include <NiTSet.h>

// 无参数
class Signal0;
class Slot0 : public NiMemObject
{
public:
	Slot0(void);
	virtual ~Slot0(void);
	virtual void ReceiveSignal() const = 0;
};

template<typename ClassType>
class MemberSlot0 : public Slot0
{
public:
	MemberSlot0();
	MemberSlot0(ClassType* pInstance, void (ClassType::*pfnCallback)());
	void Initialize(ClassType* pInstance, void (ClassType::*pfnCallback)());
	virtual ~MemberSlot0();
	virtual void ReceiveSignal() const;
protected:
	ClassType* m_pInstance;
	void (ClassType::*m_pfnCallback)();
};

// 1个参数
class Signal1;
template<typename Arg1Type>
class Slot1 : public NiMemObject
{
public:
	Slot1();
	virtual ~Slot1();
	virtual void ReceiveSignal(Arg1Type Arg1) const = 0;
};

template<typename ClassType, typename Arg1Type>
class MemberSlot1 : public Slot1<Arg1Type>
{
public:
	MemberSlot1();
	MemberSlot1(ClassType* pInstance, void (ClassType::pfnCallback)(Arg1Type));
	void Initialize(ClassType* pInstance, void (ClassType::pfnCallback)(Arg1Type));
	virtual ~MemberSlot1();
	virtual void ReceiveSignal(Arg1Type Arg1) const;
protected:
	ClassType* m_pInstance;
	void (ClassType::m_pfnCallback)(Arg1Type);
};

#include "Slot.inl"
