/* ========================================
 *  Isolator2 - Isolator2.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_Isolator2_H
#define __Gain_Isolator2_H

#ifndef __audioeffect__
#include "../airwin2rackbase.h"
#endif

#include <set>
#include <string>
#include <math.h>

namespace airwin2rack::Isolator2 {
enum {
	kParamA = 0,
	kParamB = 1,
	kParamC = 2,
	kParamD = 3,
  kNumParameters = 4
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'isom';    //Change this to what the AU identity is!

class Isolator2 : 
    public AudioEffectX 
{
public:
    Isolator2(audioMasterCallback audioMaster);
    ~Isolator2();
    virtual bool getEffectName(char* name);                       // The plug-in name
    virtual VstPlugCategory getPlugCategory();                    // The general category for the plug-in
    virtual bool getProductString(char* text);                    // This is a unique plug-in string provided by Steinberg
    virtual bool getVendorString(char* text);                     // Vendor info
    virtual VstInt32 getVendorVersion();                          // Version number
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);
    virtual void getProgramName(char *name);                      // read the name from the host
    virtual void setProgramName(char *name);                      // changes the name of the preset displayed in the host
    virtual float getParameter(VstInt32 index);                   // get the parameter value at the specified index
    virtual void setParameter(VstInt32 index, float value);       // set the parameter at index to value
    virtual void getParameterLabel(VstInt32 index, char *text);  // label for the parameter (eg dB)
    virtual void getParameterName(VstInt32 index, char *text);    // name of the parameter
    virtual void getParameterDisplay(VstInt32 index, char *text); // text description of the current value    
    // Added by the perl as inverses
    virtual bool parameterTextToValue(VstInt32 index, const char *text, float &value);
    virtual bool canConvertParameterTextToValue(VstInt32 index);
    virtual VstInt32 canDo(char *text);
private:
    char _programName[kVstMaxProgNameLen + 1];
    std::set< std::string > _canDo;

	enum {
		biq_freq,
		biq_reso,
		biq_a0,
		biq_a1,
		biq_a2,
		biq_b1,
		biq_b2,
		biq_aA0,
		biq_aA1,
		biq_aA2,
		biq_bA1,
		biq_bA2,
		biq_aB0,
		biq_aB1,
		biq_aB2,
		biq_bB1,
		biq_bB2,
		biq_sL1,
		biq_sL2,
		biq_sR1,
		biq_sR2,
		biq_total
	}; //coefficient interpolating biquad filter, stereo
	double biquadA[biq_total];
	double biquadB[biq_total];
	double biquadC[biq_total];
	double biquadD[biq_total];
	double biquadE[biq_total];
	double biquadF[biq_total];
	double biquadG[biq_total];
	double highA;
	double highB;
	double lowA;
	double lowB;
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
    float B;
    float C;
    float D;
};

#endif
} // end namespace
