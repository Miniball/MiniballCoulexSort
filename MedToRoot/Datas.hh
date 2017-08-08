#ifndef __DATAS_HH
#define __DATAS_HH

#include <iostream>

#include "TObject.h"

#include "SubEvents.hh"

class AdcData : public TObject {
	
public:
	AdcData();
	AdcData(unsigned short, long long, AdcSubEvent*, bool, bool, bool);
	~AdcData();
	
	inline long long Time() { return fTime; }
	inline unsigned short ModuleNumber() { return fModuleNumber; }
	inline bool LaserOn() { return fLaserOn; }
	inline bool FieldUp() { return fFieldUp; }
	inline bool FieldDown() { return fFieldDown; }
	inline AdcSubEvent* SubEvent() { return &fAdcSubEvent; }
	
protected:
	unsigned short fModuleNumber;
	unsigned long long fTime;
	
	bool fLaserOn;
	bool fFieldUp;
	bool fFieldDown;
	
	AdcSubEvent fAdcSubEvent;
	
	ClassDef(AdcData, 3);
	
};

class DgfData : public TObject {
	
public:
	DgfData();
	DgfData(unsigned short, unsigned short, DgfSubEvent*, unsigned short = 1);
	~DgfData();
	
	inline unsigned short ModuleNumber() { return fModuleNumber; }
	inline unsigned short Channel() { return fChannel; }
	inline unsigned short Multiplicity() { return fMultiplicity; }
	inline unsigned short Energy() { return fEnergy; }
	inline unsigned short* UserValues() { return fUserValues; }
	inline long long Time() { return fLongFastTriggerTime; }
	
protected:
	unsigned short fModuleNumber;
	unsigned short fChannel;
	unsigned short fMultiplicity;
	
	unsigned long long fEventTime;
	unsigned short fEnergy;
	unsigned short fFastTriggerTime;
	unsigned long long fLongFastTriggerTime;
	unsigned short fUserValues[NumberOfUserValues];
	
	ClassDef(DgfData, 2);
	
};

#endif
