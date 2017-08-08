#ifndef __SUB_EVENT_HH
#define __SUB_EVENT_HH

#define NumberOfChannels   4
#define NumberOfUserValues 6
#define NumberOfBraggChannels 2

#include <iostream>
#include <vector>
#include <cstdlib>

#include "TObject.h"

using namespace std;

class DgfSubEvent : public TObject {
	
public:
	
	DgfSubEvent();//default ctor
	~DgfSubEvent();//default dtor
	
	friend ostream& operator <<(ostream &,const DgfSubEvent &);
	
	// Reset
	void ClearEvt();
	
	// Set functions
	inline void SetHitPattern(unsigned short hitPattern)
	{
		HitPattern = hitPattern;
	};
	
	inline void SetEventTime(unsigned short RunTime, unsigned short EventTimeHigh, unsigned short EventTimeLow)
	{
		EventTime = EventTimeLow + 65536ll*EventTimeHigh + 65536ll*65536ll*RunTime;
	};
	
	inline void SetFastTriggerTime(unsigned short channelNumber, unsigned short fastTriggerTime)
	{
		if(channelNumber < NumberOfChannels)
			FastTriggerTime[channelNumber] = fastTriggerTime;
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": wrong channel number "<<channelNumber<<" (0-"<<NumberOfChannels-1<<")"<<endl;
		}
	};
	
	inline void SetEnergy(unsigned short channelNumber, unsigned short energy)
	{
		if(channelNumber < NumberOfChannels)
			Energy[channelNumber] = energy;
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": wrong channel number "<<channelNumber<<" (0-"<<NumberOfChannels-1<<")"<<endl;
		}
	};
	
	inline void SetLongFastTriggerTime(unsigned short channelNumber, unsigned short RunTime, unsigned short EventTimeHigh, unsigned short EventTimeLow)
	{
		if(channelNumber < NumberOfChannels)
		{
			//check for overflow
			if(FastTriggerTime[channelNumber] > EventTimeLow)
				LongFastTriggerTime[channelNumber] = FastTriggerTime[channelNumber] + 65536ll*EventTimeHigh + 65536ll*65536ll*RunTime;
			else
				LongFastTriggerTime[channelNumber] = FastTriggerTime[channelNumber] + 65536ll*EventTimeHigh + 65536ll + 65536ll*65536ll*RunTime;
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": wrong channel number "<<channelNumber<<" (0-"<<NumberOfChannels-1<<")"<<endl;
		}
	};
	
	inline void SetUserValues(unsigned short*& q, unsigned short channelNumber)
	{
		if(channelNumber < NumberOfChannels)
		{
			for(unsigned short i = 0; i < NumberOfUserValues; i++)
	  {
		  UserValues[channelNumber][i] = *q++;
	  }
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": wrong channel number "<<channelNumber<<" (0-"<<NumberOfChannels-1<<")"<<endl;
		}
	};
	
	//get functions
	inline unsigned short GetHitPattern() {return HitPattern;};
	
	inline unsigned short GetEventTime() {return EventTime;};
	
	inline unsigned short GetFastTriggerTime(unsigned short channelNumber)
	{
		if(channelNumber < NumberOfChannels)
			return FastTriggerTime[channelNumber];
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": wrong channel number "<<channelNumber<<" (0-"<<NumberOfChannels-1<<")"<<endl;
			return 999;
		}
	};
	
	inline unsigned short GetEnergy(unsigned short channelNumber)
	{
		if(channelNumber < NumberOfChannels)
			return Energy[channelNumber];
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": wrong channel number "<<channelNumber<<" (0-"<<NumberOfChannels-1<<")"<<endl;
			return 999;
		}
	};
	
	inline unsigned short GetNumberOfChannels()
	{
		return NumberOfChannels;
	};
	
	inline long long GetLongFastTriggerTime(unsigned short channelNumber)
	{
		if(channelNumber < NumberOfChannels)
			return LongFastTriggerTime[channelNumber];
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": wrong channel number "<<channelNumber<<" (0-"<<NumberOfChannels-1<<")"<<endl;
			return 999;
		}
	};
	
	inline void GetUserValues(unsigned short channelNumber, unsigned short* userValues)
	{
		if(channelNumber < NumberOfChannels)
		{
			for(unsigned short i = 0; i < NumberOfUserValues; i++)
	  {
		  userValues[i] = UserValues[channelNumber][i];
		  return;
	  }
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": wrong channel number "<<channelNumber<<" (0-"<<NumberOfChannels-1<<")"<<endl;
			return;
		}
	};
	
protected:
	unsigned short HitPattern;
	long long EventTime;
	unsigned short Energy[NumberOfChannels];
	unsigned short FastTriggerTime[NumberOfChannels];
	long long LongFastTriggerTime[NumberOfChannels];
	unsigned short UserValues[NumberOfChannels][NumberOfUserValues];
	
	ClassDef(DgfSubEvent, 1)
};

class AdcSubEvent : public TObject
{
public:
	AdcSubEvent(){};
	~AdcSubEvent();
	
	// Reset
	inline void ClearEvt()
	{
		fAdcChannel.clear();
		fAdcValue.clear();
	};
	
	inline size_t Size()
	{
		if(fAdcChannel.size() == fAdcValue.size())
			return fAdcChannel.size();
		else
		{
			cerr<<"Error, AdcChannel and AdcValue have different sizes ("<<fAdcChannel.size()<<" != "<<fAdcValue.size()<<")"<<endl;
			return 0;
		}
	};
	
	//set functions
	inline void Add(unsigned short adcChannel, unsigned short adcValue)
	{
		fAdcChannel.push_back(adcChannel);
		fAdcValue.push_back(adcValue);
	};
	
	inline void Timestamp(long long timeStamp)
	{
		fTimestamp = timeStamp;
	};
	
	//get functions
	inline vector <unsigned short> AdcChannel()
	{
		return fAdcChannel;
	}
	
	inline vector <unsigned short> AdcValue()
	{
		return fAdcValue;
	}
	
	inline unsigned short AdcChannel(unsigned short Index)
	{
		return fAdcChannel.at(Index);
	};
	
	inline unsigned short AdcValue(unsigned short Index)
	{
		return fAdcValue.at(Index);
	};
	
	inline unsigned short GetLastAdcChannel()
	{
		return fAdcChannel.back();
	};
	
	inline unsigned short GetLastAdcValue()
	{
		return fAdcValue.back();
	};
	
	inline void Get(unsigned short Index, unsigned short& adcChannel, unsigned short& adcValue)
	{
		//check that index is in bounds (if size is valid)
		if(Index < Size())
		{
			adcChannel = fAdcChannel[Index];
			adcValue = fAdcValue[Index];
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": Index "<<Index<<" out of bounds (0-"<<Size()<<")"<<endl;
			adcChannel = 0xffff;
			adcValue = 0;
		}
	};
	
	inline long long Timestamp()
	{
		return fTimestamp;
	};
	
protected:
	long long fTimestamp;
	vector <unsigned short> fAdcChannel;
	vector <unsigned short> fAdcValue;
	
	ClassDef(AdcSubEvent, 1)
};

class PatternUnitSubEvent : public TObject
{
public:
	PatternUnitSubEvent(){;};
	~PatternUnitSubEvent();
	
	inline size_t Size()
	{
		return Values.size();
	};
	
	// Reset
	inline void ClearEvt()
	{
		Values.clear();
	};
	
	inline void Add(unsigned int Value)
	{
		Values.push_back(Value);
	};
	
	inline unsigned int GetValue(unsigned short Index)
	{
		return Values.at(Index);
	};
	
	inline unsigned int GetLastValue()
	{
		return Values.back();
	};
	
protected:
	vector <unsigned int> Values;
	
	ClassDef(PatternUnitSubEvent, 3)
};

class ScalerSubEvent : public TObject
{
public:
	ScalerSubEvent(){;};
	~ScalerSubEvent();
	
	inline size_t Size()
	{
		if(Channels.size() == Values.size())
			return Values.size();
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": Error, size of channels and values is not the same ("<<Channels.size()<<", "<<Values.size()<<")"<<endl;
			exit(1);
		}
	};
	
	// Reset
	inline void ClearEvt()
	{
		Channels.clear();
		Values.clear();
	};
	
	inline void Add(unsigned short Channel, unsigned int Value)
	{
		Channels.push_back(Channel);
		Values.push_back(Value);
	};
	
	inline unsigned short GetChannel(unsigned short Index)
	{
		return Channels.at(Index);
	};
	
	inline unsigned int GetValue(unsigned short Index)
	{
		return Values.at(Index);
	};
	
	inline unsigned short GetLastChannel()
	{
		return Channels.back();
	};
	
	inline unsigned int GetLastValue()
	{
		return Values.back();
	};
	
protected:
	vector <unsigned short> Channels;
	vector <unsigned int> Values;
	
	ClassDef(ScalerSubEvent, 1)
};

class DgfScalerSubEvent : public TObject
{
	//parameter names (if existing):
	//257  REALTIMEA
	//258  REALTIMEB
	//259  REALTIMEC
	//260  RUNTIMEA
	//261  RUNTIMEB
	//262  RUNTIMEC
	//263  GSLTTIMEA
	//264  GSLTTIMEB
	//265  GSLTTIMEC
	//266  NUMEVENTSA
	//267  NUMEVENTSB
	//268  DSPERROR
	//269  SYNCHDONE
	//270  TEMPERATURE
	//271  BUFHEADLEN
	//272  EVENTHEADLEN
	//273  CHANHEADLEN
	//274  U14
	//288  USEROUT
	//304  AOUTBUFFER
	//305  LOUTBUFFER
	//306  AECORR
	//307  LECORR
	//308  ATCORR
	//309  LTCORR
	//310  HARDWAREID
	//311  HARDVARIANT
	//312  FIFOLENGTH
	//313  FIPPIID
	//314  FIPPIVARIANT
	//315  INTRFCID
	//316  INTRFCVARIANT
	//317  SOFTWAREID
	//318  SOFTVARIANT
	
	//319  LIVETIMEA0
	//320  LIVETIMEB0
	//321  LIVETIMEC0
	//322  FASTPEAKSA0
	//323  FASTPEAKSB0
	
	//343  LIVETIMEA1
	//344  LIVETIMEB1
	//345  LIVETIMEC1
	//346  FASTPEAKSA1
	//347  FASTPEAKSB1
	
	//367  LIVETIMEA2
	//368  LIVETIMEB2
	//369  LIVETIMEC2
	//370  FASTPEAKSA2
	//371  FASTPEAKSB2
	
	//391  LIVETIMEA3
	//392  LIVETIMEB3
	//393  LIVETIMEC3
	//394  FASTPEAKSA3
	//395  FASTPEAKSB3
	
public:
	DgfScalerSubEvent()
	{
		fLiveTime.clear();
		fFastPeak.clear();
	};
	
	DgfScalerSubEvent(unsigned short NumberOfDgfChannels)
	: fNumberOfDgfChannels(NumberOfDgfChannels)
	{
		fLiveTime.assign(fNumberOfDgfChannels,0);
		fFastPeak.assign(fNumberOfDgfChannels,0);
	};
	
	~DgfScalerSubEvent();
	
	// Reset
	inline void ClearEvt()
	{
		fModuleNumber = 0xffff;
		fClusterID = 0xffff;
		
		fRealTime = 0;
		fRunTime = 0;
		fGSLTTime = 0;
		fNumberOfEvents = 0;
		fLiveTime.clear();
		fFastPeak.clear();
	};
	
	//set functions
	void ModuleNumber(unsigned short DgfID)
	{
		fModuleNumber = DgfID;
	};
	
	void ClusterID(unsigned short ClusterID)
	{
		fClusterID = ClusterID;
	};
	
	void RealTime(long long time)
	{
		fRealTime = time;
	};
	
	void RunTime(long long time)
	{
		fRunTime = time;
	};
	
	void GSLTTime(long long time)
	{
		fGSLTTime = time;
	};
	
	void NumberOfEvents(unsigned short numberofevents)
	{
		fNumberOfEvents = numberofevents;
	};
	
	void LiveTime(unsigned short i, long long time)
	{
		if(i < fNumberOfDgfChannels)
		{
			fLiveTime[i] = time;
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": Error, trying to write to index "<<i<<" of "<<fNumberOfDgfChannels<<endl;
			return;
		}
	};
	
	void FastPeak(unsigned short i, unsigned int fastpeak)
	{
		if(i < fNumberOfDgfChannels)
		{
			fFastPeak[i] = fastpeak;
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": Error, trying to write to index "<<i<<" of "<<fNumberOfDgfChannels<<endl;
			return;
		}
	};
	
	//get functions
	unsigned short ModuleNumber()
	{
		return fModuleNumber;
	};
	
	unsigned short ClusterID()
	{
		return fClusterID;
	};
	
	long long RealTime()
	{
		return fRealTime;
	};
	
	long long RunTime()
	{
		return fRunTime;
	};
	
	long long GSLTTime()
	{
		return fGSLTTime;
	};
	
	unsigned short NumberOfEvents()
	{
		return fNumberOfEvents;
	};
	
	long long LiveTime(unsigned short i)
	{
		if(i < fNumberOfDgfChannels)
		{
			return fLiveTime[i];
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": Error, trying to read from index "<<i<<" of "<<fNumberOfDgfChannels<<endl;
			return 0;
		}
	};
	
	int FastPeak(unsigned short i)
	{
		if(i < fNumberOfDgfChannels)
		{
			return fFastPeak[i];
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": Error, trying to read from index "<<i<<" of "<<fNumberOfDgfChannels<<endl;
			return 0;
		}
	};
	
protected:
	unsigned short fModuleNumber;
	unsigned short fClusterID;
	
	long long fRealTime;
	long long fRunTime;
	long long fGSLTTime;
	unsigned short fNumberOfEvents;
	vector<long long> fLiveTime;
	vector<int> fFastPeak;
	
	unsigned short fNumberOfDgfChannels;
	
	ClassDef(DgfScalerSubEvent, 1)
};

class BraggChamberSubEvent : public TObject
{
public:
	BraggChamberSubEvent(){;};
	~BraggChamberSubEvent();
	
	// Reset
	inline void ClearEvt()
	{
		for(unsigned short i = 0; i < NumberOfBraggChannels; i++)
			fData[i].clear();
	};
	
	//set functions
	void DataSize(unsigned short Size)
	{
		fDataSize = Size;
	};
	
	void Trigger(unsigned short i, bool Trigger)
	{
		if(i < NumberOfBraggChannels)
		{
			fTrigger[i] = Trigger;
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": channel can only be 0 or 1, not "<<i<<endl;
		}
	};
	
	void Overflow(unsigned short i, bool Overflow)
	{
		if(Overflow)//only set overflow, don't unset it
		{
			if(i < NumberOfBraggChannels)
	  {
		  fOverflow[i] = Overflow;
	  }
			else
	  {
		  cerr<<__PRETTY_FUNCTION__<<": channel can only be 0 or 1, not "<<i<<endl;
	  }
		}
	};
	
	//get functions
	vector <unsigned int>* GetChannel(unsigned short i)
	{
		if(i < NumberOfBraggChannels)
		{
			return &(fData[i]);
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": channel can only be 0 or 1, not "<<i<<endl;
			return NULL;
		}
	};
	
	unsigned short DataSize()
	{
		return fDataSize;
	};
	
	bool Trigger(unsigned short i)
	{
		if(i < NumberOfBraggChannels)
		{
			return fTrigger[i];
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": channel can only be 0 or 1, not "<<i<<endl;
			return false;
		}
	};
	
	bool Overflow(unsigned short i)
	{
		if(i < NumberOfBraggChannels)
		{
			return fOverflow[i];
		}
		else
		{
			cerr<<__PRETTY_FUNCTION__<<": channel can only be 0 or 1, not "<<i<<endl;
			return false;
		}
	};
	
protected:
	unsigned short fDataSize;
	bool fTrigger[NumberOfBraggChannels];
	bool fOverflow[NumberOfBraggChannels];
	vector <unsigned int> fData[NumberOfBraggChannels];
	
	ClassDef(BraggChamberSubEvent, 1)
};

#endif
