/* ========================================
 *  ClipSoftly - ClipSoftly.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ClipSoftly_H
#include "ClipSoftly.h"
#endif
namespace airwin2rack::ClipSoftly {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ClipSoftly(audioMaster);}

ClipSoftly::ClipSoftly(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	lastSampleL = 0.0;
	lastSampleR = 0.0;
	for (int x = 0; x < 16; x++) {intermediateL[x] = 0.0; intermediateR[x] = 0.0;}
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	//this is reset: values being initialized only once. Startup values, whatever they are.
	
    _canDo.insert("plugAsChannelInsert"); // plug-in can be used as a channel insert effect.
    _canDo.insert("plugAsSend"); // plug-in can be used as a send effect.
    _canDo.insert("x2in2out"); 
    setNumInputs(kNumInputs);
    setNumOutputs(kNumOutputs);
    setUniqueID(kUniqueId);
    canProcessReplacing();     // supports output replacing
    canDoubleReplacing();      // supports double precision processing
	programsAreChunks(true);
    vst_strncpy (_programName, "Default", kVstMaxProgNameLen); // default program name
}

ClipSoftly::~ClipSoftly() {}
VstInt32 ClipSoftly::getVendorVersion () {return 1000;}
void ClipSoftly::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ClipSoftly::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

void ClipSoftly::setParameter(VstInt32 index, float value) {
}

float ClipSoftly::getParameter(VstInt32 index) {
	return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ClipSoftly::getParameterName(VstInt32 index, char *text) {
}

void ClipSoftly::getParameterDisplay(VstInt32 index, char *text) {
}

void ClipSoftly::getParameterLabel(VstInt32 index, char *text) {
}

VstInt32 ClipSoftly::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ClipSoftly::getEffectName(char* name) {
    vst_strncpy(name, "ClipSoftly", kVstMaxProductStrLen); return true;
}

VstPlugCategory ClipSoftly::getPlugCategory() {return kPlugCategEffect;}

bool ClipSoftly::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ClipSoftly", kVstMaxProductStrLen); return true;
}

bool ClipSoftly::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool ClipSoftly::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {

    }
    return false;
}
bool ClipSoftly::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {

    }
    return false;
}
} // end namespace
