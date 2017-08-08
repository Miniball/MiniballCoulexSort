#include "BuiltEvent.hh"

ClassImp(AdcData)
ClassImp(DgfData)
ClassImp(BuiltEvent)

BuiltEvent::BuiltEvent() {

	//cout<<__PRETTY_FUNCTION__<<endl;

}

BuiltEvent::BuiltEvent(unsigned short moduleNumber, AdcSubEvent* adcSubEvent, long long time, bool LaserOn, bool FieldUp, bool FieldDown) {

	//cout<<__PRETTY_FUNCTION__<<endl;
	adcData.push_back(AdcData(moduleNumber, time, adcSubEvent, LaserOn, FieldUp, FieldDown));

}

BuiltEvent::BuiltEvent(unsigned short moduleNumber, unsigned short channel, DgfSubEvent* dgfSubEvent) {

	//cout<<__PRETTY_FUNCTION__<<endl;
	dgfData.push_back(DgfData(moduleNumber, channel, dgfSubEvent));

}

BuiltEvent::~BuiltEvent() {

	//cout<<__PRETTY_FUNCTION__<<endl;
	adcData.clear();
	dgfData.clear();

}

void BuiltEvent::Clear() {

	adcData.clear();
	dgfData.clear();

}

void BuiltEvent::AddParticle(unsigned short moduleNumber, AdcSubEvent* adcSubEvent, long long time, bool LaserOn, bool FieldUp, bool FieldDown) {

	adcData.push_back(AdcData(moduleNumber, time, adcSubEvent, LaserOn, FieldUp, FieldDown));

}

void BuiltEvent::AddGamma(unsigned short moduleNumber, unsigned short channel, DgfSubEvent* dgfSubEvent, unsigned short multiplicity) {

	dgfData.push_back(DgfData(moduleNumber, channel, dgfSubEvent, multiplicity));

}

bool BuiltEvent::Coincident(GlobalSettings* Settings, long long time) {
	
	size_t i;
	
	// if there are particles in this event, check whether they are in coincidence
	if( adcData.size() > 0 ) {

		//loop over all adc events
		for( i = 0; i < adcData.size(); i++ ) {
			
			if( Settings->Coincident( time-adcData[i].Time() ) )
				return true;
			
		}

		return false;

	}
	
	else {
		
      // loop over all dgf events
      for( i = 0; i < dgfData.size(); i++ ) {

		  if(Settings->Coincident(time-dgfData[i].Time()))
			  return true;

	  }

		return false;

	}

}

long long BuiltEvent::GetTime() {
	
	if( adcData.size() > 0 ) {
  
		return adcData[0].Time();
  
	}
	
	else if( dgfData.size() > 0 ) {
		
		return dgfData[0].Time();
		
	}
	
	else {
  
		cerr<<__PRETTY_FUNCTION__<<": Error, neither particle nor gamma stored in this event!"<<endl;
		return -1;
		
	}
	
}
