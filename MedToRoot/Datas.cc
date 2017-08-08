#include "Datas.hh"

ClassImp(AdcData)
ClassImp(DgfData)
ClassImp(AdcSubEvent)

// AdcData
AdcData::AdcData() {
	
	//cout<<__PRETTY_FUNCTION__<<endl;
	
}

AdcData::AdcData(unsigned short moduleNumber, long long time, AdcSubEvent* adcSubEvent, bool LaserOn, bool FieldUp, bool FieldDown)
:fModuleNumber(moduleNumber), fTime(time), fLaserOn(LaserOn), fFieldUp(FieldUp), fFieldDown(FieldDown) {
	
	//  cout<<__PRETTY_FUNCTION__<<endl;
	fAdcSubEvent = *adcSubEvent;
	
}

AdcData::~AdcData() {
	
	//  cout<<__PRETTY_FUNCTION__<<endl;
	//fAdcSubEvent->Clear();
	
}

// DgfData
DgfData::DgfData() {
	
	//cout<<__PRETTY_FUNCTION__<<endl;
	
}

DgfData::DgfData(unsigned short moduleNumber, unsigned short channel, DgfSubEvent* dgfSubEvent, unsigned short multiplicity)
:fModuleNumber(moduleNumber), fChannel(channel), fMultiplicity(multiplicity) {
	
	//cout<<__PRETTY_FUNCTION__<<endl;
	fEventTime = dgfSubEvent->GetEventTime();
	fEnergy = dgfSubEvent->GetEnergy(channel);
	fFastTriggerTime = dgfSubEvent->GetFastTriggerTime(channel);
	fLongFastTriggerTime = dgfSubEvent->GetLongFastTriggerTime(channel);
	dgfSubEvent->GetUserValues(channel, fUserValues);
	
}

DgfData::~DgfData() {
	
	//cout<<__PRETTY_FUNCTION__<<endl;
	
}
