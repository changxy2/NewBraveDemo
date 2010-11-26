// ======================================================================================
// File         : AIModel.h
// Author       : Wu Jie 
// Last Change  : 10/19/2010 | 15:19:53 PM | Tuesday,October
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "ID.h"
#include <egf/BuiltinModelHelper.h>
#include <egf/BehaviorDescriptor.h>
#include <efd/ParameterList.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// DISABLE { 
// Determine// Determine the PropertyID values by inspecting the exported flat model.
// static const egf::PropertyID kPropertyID_prop1_DirectBind = ???;
// static const egf::PropertyID kPropertyID_prop2_Accessor= ???;
// static const egf::PropertyID kPropertyID_prop3_AssociativeArray = ???;
// } DISABLE end 

///////////////////////////////////////////////////////////////////////////////
// classes defines
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// class 
// 
// Purpose: 
// 
///////////////////////////////////////////////////////////////////////////////

class AIModel : public egf::IBuiltinModelImpl
{
    // Built-in models are efd::IBase derived objects so they need to be assigned a unique ClassID
    // and implement reference counting.
    EE_DECLARE_CLASS1(AIModel, game::CLASS_ID_AIModel, egf::IBuiltinModelImpl);
    EE_DECLARE_CONCRETE_REFCOUNT;

    // Properties can be implemented using the various EE_BUILTINMODELPROPERTY* macros.
    // See egf/BuiltinModelHelper.h for details on these macros.
    // These macros bind application member variables to the Model's Properties.
    EE_DECLARE_BUILTINMODEL_PROPERTIES
        // DISABLE { 
        // // Here we directly bind the property description to the address of m_prop1_DirectBindInt:
        // EE_BUILTINMODELPROPERTY(
        //                         MyBuiltinModel,
        //                         kPropertyID_prop1_DirectBind, 
        //                         efd::UInt32, 
        //                         MyBuiltinModel, 
        //                         m_prop1_DirectBindInt)

        // // In this example we bind the property using a pair of application Get/Set accessor methods
        // EE_BUILTINMODELPROPERTY_ACCESSOR(
        //                                  MyBuiltinModel, 
        //                                  kPropertyID_prop2_Accessor, 
        //                                  efd::UInt32, 
        //                                  MyBuiltinModel, 
        //                                  GetProp2, 
        //                                  SetProp2)

        // // In this example we bind an associative array property descriptor directly to m_array:
        // EE_BUILTINMODELASSOCPROPERTY(
        //                              MyBuiltinModel, 
        //                              kPropertyID_prop3_AssociativeArray, 
        //                              Float32, 
        //                              MyBuiltinModel, 
        //                              m_array)
        // } DISABLE end 
    EE_END_BUILTINMODEL_PROPERTIES

    ///////////////////////////////////////////////////////////////////////////////
    // public
    ///////////////////////////////////////////////////////////////////////////////

public:

    AIModel (); 

    void OnTick ( efd::Float64 _deltaTime );

    ///////////////////////////////////////////////////////////////////////////////
    // protected
    ///////////////////////////////////////////////////////////////////////////////

protected:

    virtual bool Dispatch ( const egf::BehaviorDescriptor* pBehaviorToInvoke, efd::ParameterList* pParams ) {
        // if your built-in model implements behaviors they are dispatched via this method.
        // Built-in behaviors are called via IBuiltinModel::Dispatch.
        return false;
    }

    // DISABLE { 
    // // Application getter/setter...
    // efd::UInt32 GetProp2() const
    // {
    //     EE_LOG ( kMyLogModule, 
    //              kLVL3, 
    //              ("Getting prop2_Accessor value %d from entity %s", m_prop2_Accessor, m_pOwningEntity->GetEntityID().ToString().c_str());
    //            )
    //     return m_prop2_Accessor;
    // }

    // void SetProp2(const efd::UInt32& newValue)
    // {
    //     EE_ASSERT(newValue >= 0 && newValue <= 1000000);
    //     m_prop2_Accessor = newValue;
    //     // When setting a Property via Entity::SetPropertyValue, the dirty bit is automatically set.
    //     // If the application calls SetProp2 to modify the member directly, it may want to 
    //     // call Dirty itself.
    // }

    // efd::UInt32 m_prop1_DirectBindInt;
    // efd::UInt32 m_prop2_Accessor;
    // efd::map<efd::utf8string, efd::Float32> m_array;
    // } DISABLE end 

}; // end class AIModel 
