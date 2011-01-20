#include "InspectionBar.h"
#include <Logging/Logger.h>

#include <Display/AntTweakBar.h>
#include <Utils/Convert.h>



namespace OpenEngine {
    namespace Utils {
using namespace OpenEngine::Display;

InspectionBar::InspectionBar(string title, ValueList vl) 
  : ITweakBar(title),
    values(vl)
{
    
}

void InspectionBar::AddFields(AntTweakBar& m) {
    TwBar* twBar = GetBar();

    for (ValueList::iterator itr = values.begin();
         itr != values.end();
         itr++) {

        IValue* val = *itr;
               
        if (RWValue<Quaternion<float> > *qv = dynamic_cast<RWValue<Quaternion<float> >* >(val)) {
            Callback<RWValue<Quaternion<float> > > *cb
                = new Callback<RWValue<Quaternion<float> > >(*this,
                                                                &InspectionBar::GetQuaternion,
                                                                &InspectionBar::SetQuaternion,
                                                                qv);

            TwAddVarCB(twBar, 
                       qv->name.c_str(),
                       TW_TYPE_QUAT4F,
                       &InspectionBar::AntSetCallback,
                       &InspectionBar::AntGetCallback,
                       cb,
                       "");
        } else if (RWValue<Vector<3,float> > *vv = dynamic_cast<RWValue<Vector<3,float> >* > (val)) {

            Callback<RWValue<Vector<3,float> > > *cb
                = new Callback<RWValue<Vector<3, float> > >(*this,
                                                            &InspectionBar::GetVector3f,
                                                            &InspectionBar::SetVector3f,
                                                            vv);


            TwAddVarCB(twBar, 
                       vv->name.c_str(),
                       vv->isColor?TW_TYPE_COLOR3F:m.antVec3fType,
                       &InspectionBar::AntSetCallback,
                       &InspectionBar::AntGetCallback,
                       cb,
                       "");

            
        } else if (RWValue<Vector<4,float> > *vv = dynamic_cast<RWValue<Vector<4,float> >* > (val)) {

            Callback<RWValue<Vector<4,float> > > *cb
                = new Callback<RWValue<Vector<4, float> > >(*this,
                                                            &InspectionBar::GetVector4f,
                                                            &InspectionBar::SetVector4f,
                                                            vv);


            TwAddVarCB(twBar, 
                       vv->name.c_str(),
                       vv->isColor?TW_TYPE_COLOR4F:m.antVec4fType,
                       &InspectionBar::AntSetCallback,
                       &InspectionBar::AntGetCallback,
                       cb,
                       "");            
        } else if (RWValue<float> *fv = dynamic_cast<RWValue<float> *>(val)) {
            Callback<RWValue<float> > *cb
                = new Callback<RWValue<float> >(*this,
                                                &InspectionBar::GetFloat,
                                                &InspectionBar::SetFloat,
                                                fv);
            string opts = "";
            if (fv->properties.count(MIN)) {
                opts = opts +  string(" min=") + Convert::ToString( fv->properties[MIN]);
            }
            if (fv->properties.count(MAX)) {
                opts = opts +  string(" max=") + Convert::ToString( fv->properties[MAX]);
            }
            if (fv->properties.count(STEP)) {
                opts = opts +  string(" step=") + Convert::ToString( fv->properties[STEP]);
            }
            TwAddVarCB(twBar, 
                       fv->name.c_str(),
                       TW_TYPE_FLOAT,
                       &InspectionBar::AntSetCallback,
                       &InspectionBar::AntGetCallback,
                       cb,
                       opts.c_str());
        } else if (RWValue<unsigned int> *iv = dynamic_cast<RWValue<unsigned int> *>(val)) {
            Callback<RWValue<unsigned int> > *cb
                = new Callback<RWValue<unsigned int> >(*this,
                                                       &InspectionBar::GetUInt,
                                                       &InspectionBar::SetUInt,
                                                       iv);
            string opts = "";
            if (iv->properties.count(MIN)) {
                opts = opts +  string(" min=") + Convert::ToString( iv->properties[MIN]);
            }
            if (iv->properties.count(MAX)) {
                opts = opts +  string(" max=") + Convert::ToString( iv->properties[MAX]);
            }
            if (iv->properties.count(STEP)) {
                opts = opts +  string(" step=") + Convert::ToString( iv->properties[STEP]);
            }
            TwAddVarCB(twBar, 
                       iv->name.c_str(),
                       TW_TYPE_UINT32,
                       &InspectionBar::AntSetCallback,
                       &InspectionBar::AntGetCallback,
                       cb,
                       opts.c_str());
        } else if (RWValue<bool> *bv = dynamic_cast<RWValue<bool>*>(val)) {
            Callback<RWValue<bool> > *cb
                = new Callback<RWValue<bool> >(*this,
                                               &InspectionBar::GetBool,
                                               &InspectionBar::SetBool,
                                               bv);
            string opts = "";
            TwAddVarCB(twBar,
                       bv->name.c_str(),
                       TW_TYPE_BOOLCPP,
                       &InspectionBar::AntSetCallback,
                       &InspectionBar::AntGetCallback,
                       cb,
                       opts.c_str());
        } else if (RWValue<string> *sv = dynamic_cast<RWValue<string>*>(val)) {
            Callback<RWValue<string> > *cb
                = new Callback<RWValue<string> >(*this,
                                                 &InspectionBar::GetString,
                                                 &InspectionBar::SetString,
                                                 sv);
            string opts = "";
            TwAddVarCB(twBar,
                       sv->name.c_str(),
                       TW_TYPE_STDSTRING,
                       &InspectionBar::AntSetCallback,
                       &InspectionBar::AntGetCallback,
                       cb,
                       opts.c_str());


        } else if (ActionValue *av = dynamic_cast<ActionValue*>(val)) {            
            TwAddButton(twBar, val->name.c_str(), &InspectionBar::AntButtonCallback,av,"");
        } else {

            string* hesten = new string("Unknown value"); 
            TwAddVarRO(twBar, val->name.c_str(), TW_TYPE_STDSTRING, hesten, "");
        }
    }
}

void InspectionBar::GetQuaternion(void *value, RWValue<Quaternion<float> > *rwv) {
    Quaternion<float> q = rwv->Get();
    float *val = (float*)value;
    val[0] = q[0];
    val[1] = q[1];
    val[2] = q[2];
    val[3] = q[3];
}

void InspectionBar::SetQuaternion(const void *value, RWValue<Quaternion<float> > *rwv) {
    Quaternion<float> q;
    float *val = (float*)value;
    q[0] = val[0];
    q[1] = val[1];
    q[2] = val[2];
    q[3] = val[3];
    rwv->Set(q);
}

void InspectionBar::GetVector3f(void *value, RWValue<Vector<3,float> > *rwv) {
    Vector<3,float> v = rwv->Get();
    AntTweakBar::antVec3f *val = (AntTweakBar::antVec3f*)value;
    val->x = v[0];
    val->y = v[1];
    val->z = v[2];
}

void InspectionBar::SetVector3f(const void *value, RWValue<Vector<3,float> > *rwv) {
    Vector<3,float> v;
    AntTweakBar::antVec3f *val = (AntTweakBar::antVec3f*)value;
    v[0] = val->x;
    v[1] = val->y;
    v[2] = val->z;
    rwv->Set(v);
}
void InspectionBar::GetVector4f(void *value, RWValue<Vector<4,float> > *rwv) {
    Vector<4,float> v = rwv->Get();
    AntTweakBar::antVec4f *val = (AntTweakBar::antVec4f*)value;
    val->x = v[0];
    val->y = v[1];
    val->z = v[2];
    val->w = v[3];
}

void InspectionBar::SetVector4f(const void *value, RWValue<Vector<4,float> > *rwv) {
    Vector<4,float> v;
    AntTweakBar::antVec4f *val = (AntTweakBar::antVec4f*)value;
    v[0] = val->x;
    v[1] = val->y;
    v[2] = val->z;
    v[3] = val->w;
    rwv->Set(v);
}

void TW_CALL InspectionBar::AntSetCallback(const void *value, void *clientdata) {
    ICallback* cb = (ICallback*)clientdata;
    cb->Set(value);
}
void TW_CALL InspectionBar::AntGetCallback(void *value, void *clientdata) {
    ICallback* cb = (ICallback*)clientdata;
    cb->Get(value);
}
void TW_CALL InspectionBar::AntButtonCallback(void *clientdata) {
    ActionValue *av = (ActionValue*)clientdata;
    av->Call();
}

    }
}
