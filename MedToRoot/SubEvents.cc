#include "SubEvents.hh"

ClassImp(DgfSubEvent)
ClassImp(AdcSubEvent)
ClassImp(PatternUnitSubEvent)

//DgfSubEvent
DgfSubEvent::DgfSubEvent()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  for(unsigned short i = 0; i < NumberOfChannels; i++)
    {
      Energy[i] = 0;
      FastTriggerTime[i] = 0;
      LongFastTriggerTime[i] = 0;
      for(unsigned short j = 0; j < NumberOfUserValues; j++)
	UserValues[i][j] = 0;
    }
}

DgfSubEvent::~DgfSubEvent()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  //cout<<"start deleting "<<this->GetName()<<endl;

//  if(Energy != NULL)
//    {
//      delete[] Energy;
//      Energy = NULL;
//    }
//
//  if(FastTriggerTime != NULL)
//    {
//      delete[] FastTriggerTime;
//      FastTriggerTime = NULL;
//    }
//
//  if(LongFastTriggerTime != NULL)
//    {
//      delete[] LongFastTriggerTime;
//      LongFastTriggerTime = NULL;
//    }
//
//  if(UserValues != NULL)
//    {
//      delete[] UserValues;
//      UserValues = NULL;
//    }
}

ostream& operator <<(ostream &os,const DgfSubEvent &obj) {
	
	os << "Hitpattern = " << obj.HitPattern << endl;
	os << "EventTime = " << obj.EventTime << endl;
	os << "Energy = " << obj.Energy << endl;
	os << "FastTriggerTime = " << obj.FastTriggerTime << endl;
	os << "LongFastTriggerTime = " << obj.LongFastTriggerTime << endl;

	for( unsigned short i = 0; i < NumberOfChannels; i++ ) {

		cout << "Channel " << i << ": Energy = " << obj.Energy[i] << ", FastTriggerTime = ";
		cout << obj.FastTriggerTime[i] << ", LongFastTriggerTime = " << obj.LongFastTriggerTime << endl;

	}
	
	return os;
	
}

void DgfSubEvent::ClearEvt() {
	
	HitPattern = 0;
	EventTime = -1;
	
	for( unsigned short i = 0; i < NumberOfChannels; i++ ) {

		Energy[i] = 0;
		FastTriggerTime[i] = 0;
		LongFastTriggerTime[i] = 0;
		
		for( unsigned short j = 0; j < NumberOfUserValues; j++ )
			UserValues[i][j] = 0;
		
	}
	
}

AdcSubEvent::~AdcSubEvent() {
	
	fAdcChannel.clear();
	fAdcValue.clear();
	
}

PatternUnitSubEvent::~PatternUnitSubEvent() {
	
	Values.clear();

}

ScalerSubEvent::~ScalerSubEvent() {
	
	Channels.clear();
	Values.clear();

}

DgfScalerSubEvent::~DgfScalerSubEvent() {
	
	fLiveTime.clear();
	fFastPeak.clear();

}

BraggChamberSubEvent::~BraggChamberSubEvent() {
	
	fData[0].clear();
	fData[1].clear();

}
