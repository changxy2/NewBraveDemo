// ======================================================================================
// File         : Subject.h
// Author       : Zhang yuzhong 
// Last Change  : 12/6/2010
// Description  : (信号插槽设计模式)信号插槽相关宏定义
//				  
// ======================================================================================

#ifndef SIGNALSLOTMACROS_H
#define SIGNALSLOTMACROS_H

#define UnsubscribeFromAll(TSetName)                    \
    {                                                   \
       TSetName.RemoveAll();							\
    }
//---------------------------------------------------------------------------
#define SubscribeToMe(TSetName, Element)                \
    {                                                   \
        int iIsThere = TSetName.Find(Element);          \
        if (iIsThere == -1)                             \
        {                                               \
            TSetName.Add(Element);                      \
        }                                               \
    }
//---------------------------------------------------------------------------
#define UnsubscribeToMe(TSetName, Element)              \
    {                                                   \
        int iIsThere = TSetName.Find(Element);          \
        if (iIsThere != -1)                             \
        {                                               \
            TSetName.RemoveAt(iIsThere);                \
        }                                               \
    }
//---------------------------------------------------------------------------

#define EmitSignalToAll(TSetName, ArgList)              \
    {                                                   \
        unsigned int uiIdx, uiSize;                     \
        uiSize = TSetName.GetSize();                    \
        for (uiIdx = 0; uiIdx < uiSize; ++uiIdx)        \
        {                                               \
            TSetName.GetAt(uiIdx)->ReceiveSignal ArgList;\
        }                                               \
    }
//---------------------------------------------------------------------------
#define NumElements(TSetName)                           \
    TSetName.GetSize();
//---------------------------------------------------------------------------
#define IsElementInGroup(TSetName, Element)             \
    (TSetName.Find(Element) != -1);
//---------------------------------------------------------------------------

#endif // SIGNALSLOTMACROS_H
