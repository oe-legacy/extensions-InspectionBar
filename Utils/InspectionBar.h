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
    void GetFloat(void *value, RWValue<float>* rwv);
    void SetFloat(const void *value, RWValue<float>* rwv);
    
public:
    InspectionBar(string title, ValueList vl);

    void AddFields(AntTweakBar& m);
    static void TW_CALL AntSetCallback(const void *value, void *clientdata);
    static void TW_CALL AntGetCallback(void *value, void *clientdata);
};

    }
}

#endif // _OE_INSPECTION_BAR_H_
    
