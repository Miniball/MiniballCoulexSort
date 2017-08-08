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

ostream& operator <<(ostream &os,const DgfSubEvent &obj)
{
  os<<"Hitpattern = "<<obj.HitPattern<<endl
    <<"EventTime = "<<obj.EventTime<<endl
    <<"Energy = "<<obj.Energy<<endl
    <<"FastTriggerTime = "<<obj.FastTriggerTime<<endl
    <<"LongFastTriggerTime = "<<obj.LongFastTriggerTime<<endl;
  for(unsigned short i = 0; i < NumberOfChannels; i++)
    {
      cout<<"Channel "<<i<<": Energy = "<<obj.Energy[i]<<", FastTriggerTime = "<<obj.FastTriggerTime[i]<<", LongFastTriggerTime = "<<obj.LongFastTriggerTime<<endl;
    }
  //unsigned short** UserValues;//!NumberOfChannels 6

  return os;
}

void DgfSubEvent::Clear()
{
  HitPattern = 0;
  EventTime = -1;

  for(unsigned short i = 0; i < NumberOfChannels; i++)
    {
      if(Energy != NULL)
	{
	  Energy[i] = 0;
	}
      if(FastTriggerTime != NULL)
	{
	  FastTriggerTime[i] = 0;
	}
      if(LongFastTriggerTime != NULL)
	{
	  LongFastTriggerTime[i] = 0;
	}
      if(UserValues != NULL)
	{
	  for(unsigned short j = 0; j < NumberOfUserValues; j++)
	    {
	      UserValues[i][j] = 0;
	    }
	}
    }//for(i=0;i<NumberOfChannels;i++)
}

//AdcSubEvent

AdcSubEvent::~AdcSubEvent()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  fAdcChannel.clear();
  fAdcValue.clear();
}

//PatternUnitSubEvent

PatternUnitSubEvent::~PatternUnitSubEvent()
{
  Values.clear();
}

//ScalerSubEvent

ScalerSubEvent::~ScalerSubEvent()
{
  Channels.clear();
  Values.clear();
}

DgfScalerSubEvent::~DgfScalerSubEvent()
{
  fLiveTime.clear();
  fFastPeak.clear();
}

BraggChamberSubEvent::~BraggChamberSubEvent()
{
  fData[0].clear();
  fData[1].clear();
}
