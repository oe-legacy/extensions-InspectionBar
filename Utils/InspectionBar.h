// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_INSPECTION_BAR_H_
#define _OE_INSPECTION_BAR_H_

#include <Display/ITweakBar.h>

#include <Utils/IInspector.h>

#include <list>
#include <Math/Quaternion.h>

namespace OpenEngine {
    namespace Utils {

using namespace OpenEngine::Math;

using namespace OpenEngine::Display;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Utils::Inspection;

/**
 * Short description.
 *
 * @class InspectionBar InspectionBar.h ts/city/InspectionBar.h
 */
class InspectionBar : public ITweakBar {
private:

    class ICallback {
    public:
        virtual void Get(void *) =0;
        virtual void Set(const void *) =0;
    };

    template <class T>
    class Callback : public ICallback {
        InspectionBar& instance;
        void (InspectionBar::*getFun)(void*, T*);
        void (InspectionBar::*setFun)(const void*, T*);
        T* arg;
    public:
        Callback(InspectionBar& ins,
                 void (InspectionBar::*gFun)(void*, T*),
                 void (InspectionBar::*sFun)(const void*, T*),
                 T* arg)
            : instance(ins),
              getFun(gFun),
              setFun(sFun),
              arg(arg) {}
        void Get(void *v) {
            (instance.*getFun)(v,arg);
        }
        void Set(const void *v) {
            (instance.*setFun)(v,arg);
        }
    };
    


    ValueList values;
    void GetQuaternion(void *value, RWValue<Quaternion<float> >* rwv);
    void SetQuaternion(const void *value, RWValue<Quaternion<float> >* rwv);

    void GetVector3f(void *value, RWValue<Vector<3,float> >* rwv);
    void SetVector3f(const void *value, RWValue<Vector<3,float> >* rwv);

    void GetVector4f(void *value, RWValue<Vector<4,float> >* rwv);
    void SetVector4f(const void *value, RWValue<Vector<4,float> >* rwv);

#define INSPECTIONBAR_GETSET(_type, _name)                        \
    void Get##_name (void *value, RWValue<_type>* rwv) {          \
    _type *val = (_type*)value;                                   \
    *(val) = rwv->Get();                                          \
    }                                                             \
    void Set##_name(const void *value, RWValue<_type>* rwv) {     \
    _type *val = (_type*)value;                                   \
    rwv->Set(*val);                                               \
    }                                                             \


    INSPECTIONBAR_GETSET(unsigned int, UInt) 
    INSPECTIONBAR_GETSET(bool, Bool) 
    INSPECTIONBAR_GETSET(float, Float)

    void SetString(const void *value, RWValue<string>* rwv) {
        const std::string *srcPtr = static_cast<const std::string *>(value);
        rwv->Set(*srcPtr);

    }

    void GetString(void *value, RWValue<string>* rwv) {
        std::string *destPtr = static_cast<std::string *>(value);
        TwCopyStdStringToLibrary(*destPtr, rwv->Get()); 
        // the use of TwCopyStdStringToLibrary is required here

    }


    // void GetUInt(void *value, RWValue<unsigned int>* rwv) {
    //     unsigned int *val = (unsigned int*)value;
    //     val[0] = rwv->Get();
        
    // }
    // void SetUInt(const void *value, RWValue<unsigned int>* rwv) {
    //     unsigned int *val = (unsigned int*)value;
    //     rwv->Set(val[0]);
    // }
    
    
public:
    InspectionBar(string title, ValueList vl);

    void AddFields(AntTweakBar& m);
    static void TW_CALL AntSetCallback(const void *value, void *clientdata);
    static void TW_CALL AntGetCallback(void *value, void *clientdata);
    static void TW_CALL AntButtonCallback(void *clientdata);
};

    }
}

#endif // _OE_INSPECTION_BAR_H_
    
