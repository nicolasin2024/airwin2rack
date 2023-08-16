/* ========================================
 *  StarChild2 - StarChild2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __StarChild2_H
#include "StarChild2.h"
#endif
namespace airwin2rack::StarChild2 {

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new StarChild2(audioMaster);}

StarChild2::StarChild2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 0.7;
	C = 0.2;
	
	int count;
	
	for(count = 0; count < 6; count++) {lastRefL[count] = 0.0; lastRefR[count] = 0.0;}
	cycle = 0;
		
	for(count = 0; count < 44101; count++) {d[count] = 0.0;}
	dCount = 0;
	
	for(count = 0; count < 11; count++) {wearL[count] = 0.0; wearR[count] = 0.0; factor[count] = 0.0;}
	
	wearLPrev = 0.0; wearRPrev = 0.0;
	p[0] = 1;
	p[1] = 11; p[2] = 13; p[3] = 17; p[4] = 19; p[5] = 23; p[6] = 29; p[7] = 31; p[8] = 37; p[9] = 41;
	p[10] = 43; p[11] = 47; p[12] = 53; p[13] = 59; p[14] = 61; p[15] = 67; p[16] = 71; p[17] = 73; p[18] = 79; p[19] = 83; p[20] = 89;
	p[21] = 97; p[22] = 101; p[23] = 103; p[24] = 107; p[25] = 109; p[26] = 113; p[27] = 127; p[28] = 131; p[29] = 137; p[30] = 139;
	p[31] = 149; p[32] = 151; p[33] = 157; p[34] = 163; p[35] = 167; p[36] = 173; p[37] = 179; p[38] = 181; p[39] = 191; p[40] = 193;
	p[41] = 197; p[42] = 199; p[43] = 211; p[44] = 223; p[45] = 227; p[46] = 229; p[47] = 233; p[48] = 239; p[49] = 241; p[50] = 251;
	p[51] = 257; p[52] = 263; p[53] = 269; p[54] = 271; p[55] = 277; p[56] = 281; p[57] = 283; p[58] = 293; p[59] = 307; p[60] = 311;
	p[61] = 313; p[62] = 317; p[63] = 331; p[64] = 337; p[65] = 347; p[66] = 349; p[67] = 353; p[68] = 359; p[69] = 367; p[70] = 373;
	p[71] = 379; p[72] = 383; p[73] = 389; p[74] = 397; p[75] = 401; p[76] = 409; p[77] = 419; p[78] = 421; p[79] = 431; p[80] = 433;
	p[81] = 439; p[82] = 443; p[83] = 449; p[84] = 457; p[85] = 461; p[86] = 463; p[87] = 467; p[88] = 479; p[89] = 487; p[90] = 491;
	p[91] = 499; p[92] = 503; p[93] = 509; p[94] = 521; p[95] = 523; p[96] = 541; p[97] = 547; p[98] = 557; p[99] = 563; p[100] = 569;
	p[101] = 571; p[102] = 577; p[103] = 587; p[104] = 593; p[105] = 599; p[106] = 601; p[107] = 607; p[108] = 613; p[109] = 617; p[110] = 619;
	p[111] = 631; p[112] = 641; p[113] = 643; p[114] = 647; p[115] = 653; p[116] = 659; p[117] = 661; p[118] = 673; p[119] = 677; p[120] = 683;
	p[121] = 691; p[122] = 701; p[123] = 709; p[124] = 719; p[125] = 727; p[126] = 733; p[127] = 739; p[128] = 743; p[129] = 751; p[130] = 757;
	p[131] = 761; p[132] = 769; p[133] = 773; p[134] = 787; p[135] = 797; p[136] = 809; p[137] = 811; p[138] = 821; p[139] = 823; p[140] = 827;
	p[141] = 829; p[142] = 839; p[143] = 853; p[144] = 857; p[145] = 859; p[146] = 863; p[147] = 877; p[148] = 881; p[149] = 883; p[150] = 887;
	p[151] = 907; p[152] = 911; p[153] = 919; p[154] = 929; p[155] = 937; p[156] = 941; p[157] = 947; p[158] = 953; p[159] = 967; p[160] = 971;
	p[161] = 977; p[162] = 983; p[163] = 991; p[164] = 997; p[165] = 998; p[166] = 999;
	
	int assign;
	for(count = 0; count < 165; count++)
	{
		t[count] = p[count];
		//these get assigned again but we'll give them real values in case of trouble. They are 32 bit unsigned ints
		assign = p[count] % 10;
		//give us the 1, 3, 7 or 9 on the end
		
		switch (assign){
			case 1: outL[count] = 0.0; outR[count] = p[count]; break;
			case 3: outL[count] = p[count] * 0.25; outR[count] = p[count] * 0.75; break;
			case 7: outL[count] = p[count] * 0.75; outR[count] = p[count] * 0.25; break;
			case 9: outL[count] = p[count]; outR[count] = 0.0; break;
		}
		//this gives us a set of stereo offsets that are always the same. When building our delay outputs,
		//we multiply our -1 to 1 float values by this and add the result to a simple signed int.
		//The result gives us a coarser and coarser output the fewer taps we have,
		//and we divide the result by count*count to drop the volume down again.
	}
	pitchCounter = 2;
	increment = 1;
	dutyCycle = 1;
	
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

StarChild2::~StarChild2() {}
VstInt32 StarChild2::getVendorVersion () {return 1000;}
void StarChild2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void StarChild2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

void StarChild2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: break; // unknown parameter, shouldn't happen!
    }
}

float StarChild2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void StarChild2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Sustain", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Grain", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void StarChild2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void StarChild2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 StarChild2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool StarChild2::getEffectName(char* name) {
    vst_strncpy(name, "StarChild2", kVstMaxProductStrLen); return true;
}

VstPlugCategory StarChild2::getPlugCategory() {return kPlugCategEffect;}

bool StarChild2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows StarChild2", kVstMaxProductStrLen); return true;
}

bool StarChild2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
bool StarChild2::parameterTextToValue(VstInt32 index, const char *text, float &value) {
    switch(index) {
    case kParamA: { auto b = string2float(text, value); return b; break; }
    case kParamB: { auto b = string2float(text, value); return b; break; }
    case kParamC: { auto b = string2float(text, value); return b; break; }

    }
    return false;
}
bool StarChild2::canConvertParameterTextToValue(VstInt32 index) {
    switch(index) {
        case kParamA: return true;
        case kParamB: return true;
        case kParamC: return true;

    }
    return false;
}
} // end namespace
