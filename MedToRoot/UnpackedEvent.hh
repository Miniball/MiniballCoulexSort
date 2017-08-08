#ifndef __UNPACKED_EVENT_HH
#define __UNPACKED_EVENT_HH

#define CAEN7X5_NBOFCHAN  32
#define CAEN_V7X5_SH_TYPE_OF_WORD 0x18
#define CAEN_V7X5_M_TYPE_OF_WORD 0x7
#define CAEN_V7X5_D_HDR 0x2
#define CAEN_V7X5_D_VALID_DATUM 0x0
#define CAEN_V7X5_D_EOB 0x4
#define CAEN_V7X5_SH_CRATENR 0x10
#define CAEN_V7X5_M_CRATENR 0xff 
#define CAEN_V7X5_SH_WC 0x8
#define CAEN_V7X5_M_WC  0x3f
#define CAEN_V7X5_M_MSERIAL 0xff
#define CAEN_V7X5_SH_CHN 0x10 
#define CAEN_V7X5_M_CHN 0x3f
#define CAEN_V7X5_B_OVERFLOW 0x1000
#define CAEN_V7X5_B_UNDERTHRESH 0x2000
#define CAEN_V7X5_M_ADCDATA 0xfff
#define CAEN_V7X5_M_EVENTCOUNTER 0xffffff

#define SIS3600_D_HDR     (0x1 << 15)
#define SIS3600_MSERIAL   (0xff)

#define SCALER_SH_CRATENR 0x10
#define SCALER_M_CRATENR 0xff
#define SCALER_M_SERIAL 0xffff

#define DGF_SCALER_END_OF_BUFFER 0x5252
#define DGF_SCALER_MAGIC_WORD 0x2525

#define BRAGG_CHAMBER_HEADER        (0x1 << 15)
#define BRAGG_CHAMBER_MODULE_NUMBER  0xFF
#define BRAGG_CHAMBER_MODULE_ID      0x7F
#define BRAGG_CHAMBER_SH_ModuleId   8

#define MESYTEC_MADC_NBOFCHAN  32
#define MESYTEC_MADC_MODULE_ID 0xff
#define MESYTEC_MADC_OUTPUT_FORMAT 0x8000 //should be zero
#define MESYTEC_MADC_ADC_RESOLUTION 0x7000 //0=2k,1=4k,2=4k hires,3=8k,4=8k hires
#define MESYTEC_MADC_WORD_COUNT 0xfff
#define MESYTEC_MADC_CHANNEL_NUMBER 0x1f
#define MESYTEC_MADC_VALUE 0x1fff
#define MESYTEC_MADC_OUT_OF_RANGE 0x4000
#define MESYTEC_MADC_EXTENDED_TIMESTAMP 0x80
#define MESYTEC_MADC_EXTENDED_TIMESTAMP_SHIFT 30
#define MESYTEC_MADC_EXTENDED_TIMESTAMP_DATA 0xffff
#define MESYTEC_MADC_END_OF_EVENT 0xc0000000
#define MESYTEC_MADC_TIMESTAMP 0x3fffffff


#include <iostream>
#include <iomanip>
#include "stdlib.h"

#include "TObject.h"

#include "mbsio.h"
#include "xiadefs.h"

#include "GlobalSettings.hh"
#include "Modules.hh"
#include "SubEvents.hh"

class UnpackedEvent : public TObject
{
public:
  
  UnpackedEvent();
  UnpackedEvent(GlobalSettings*);
  ~UnpackedEvent();

  //processing & decoding of mbs events
  int ProcessEvent(const MBSDataIO*);
  bool ExtractSubevents(const MBSDataIO*);
  bool DecodeDgf(int, int, char*);
  bool DecodeAdc(int, int, char*);
  bool DecodeMesytecAdc(int, int, char*);
  bool DecodeSIS3600PatternUnit(int, int, char*);
  bool DecodeBraggChamber(int, int, char*);
  bool DecodeScaler(int, int, char*);
  bool DecodeDgfScaler(int, int, char*);

  //setting Modules

  void SetDgfModule(size_t Index, DgfModule NewDgfModule)
  {
    if(Index <= Settings->NumberOfDgfModules())
      {
	DgfModules[Index] = NewDgfModule;
      }
    else
      {
	cerr<<__PRETTY_FUNCTION__<<": Error, index "<<Index<<" out of bounds (0 - "<<Settings->NumberOfDgfModules()<<")"<<endl;
      }
  };

  void SetAdcModule(size_t Index, AdcModule NewAdcModule)
  {
    if(Index <= Settings->NumberOfAdcModules())
      {
	AdcModules[Index] = NewAdcModule;
      }
    else
      {
	cerr<<__PRETTY_FUNCTION__<<": Error, index "<<Index<<" out of bounds (0 - "<<Settings->NumberOfAdcModules()<<")"<<endl;
      }
  };

  void SetPatternUnit(size_t Index, PatternUnit NewPatternUnit)
  {
    if(Index <= Settings->NumberOfPatternUnits())
      {
	PatternUnits[Index] = NewPatternUnit;
      }
    else
      {
	cerr<<__PRETTY_FUNCTION__<<": Error, index "<<Index<<" out of bounds (0 - "<<Settings->NumberOfPatternUnits()<<")"<<endl;
      }
  };

  //clearing all modules
  void ClearEvt()
  {
    if(Settings->VerboseLevel() > 3)
      {
	cout<<"clearing "<<Settings->NumberOfDgfModules()<<" dgfs, "<<Settings->NumberOfAdcModules()<<" adcs and "<<Settings->NumberOfPatternUnits()<<" pattern units"<<endl;
      }
    
    int i;
    for(i = 0; i < Settings->NumberOfDgfModules(); i++)
      DgfModules[i].ClearEvt();
    for(i = 0; i < Settings->NumberOfAdcModules(); i++)
      AdcModules[i].ClearEvt();
    for(i = 0; i < Settings->NumberOfPatternUnits(); i++)
      PatternUnits[i].ClearEvt();

    fScalerData = false;

    Scaler.ClearEvt();
    fDgfScaler.ClearEvt();
    fBraggChamber.ClearEvt();

    if(Settings->VerboseLevel() > 3)
      {
	cout<<"done"<<endl;
      }
  };

  //verify data
  bool Verify();

  //accessing modules
  long long GetEventNumber()
  {
    return EventNumber;
  }

  int GetNumberOfDgfModules()
  {
    return Settings->NumberOfDgfModules();
  };

  int GetNumberOfAdcModules()
  {
    return Settings->NumberOfAdcModules();
  };

  int GetNumberOfPatternUnits()
  {
    return Settings->NumberOfPatternUnits();
  };

  DgfModule* GetDgfModule(size_t Index)
  {
    if(Index <= Settings->NumberOfDgfModules())
      {
	return &DgfModules[Index];
      }
    else
      {
	cerr<<__PRETTY_FUNCTION__<<": Error, index "<<Index<<" out of bounds (0 - "<<Settings->NumberOfDgfModules()<<")"<<endl;
	return NULL;
      }
  };

  AdcModule* GetAdcModule(size_t Index)
  {
    if(Index <= Settings->NumberOfAdcModules())
      {
	return &AdcModules[Index];
      }
    else
      {
	cerr<<__PRETTY_FUNCTION__<<": Error, index "<<Index<<" out of bounds (0 - "<<Settings->NumberOfAdcModules()<<")"<<endl;
	return NULL;
      }
  };

  PatternUnit* GetPatternUnit(size_t Index)
  {
    if(Index <= Settings->NumberOfPatternUnits())
      {
	return &PatternUnits[Index];
      }
    else
      {
	cerr<<__PRETTY_FUNCTION__<<": Error, index "<<Index<<" out of bounds (0 - "<<Settings->NumberOfPatternUnits()<<")"<<endl;
	return NULL;
      }
  };

  DgfModule* GetTimestampModule(size_t Index)
  {
    if(Index <= Settings->NumberOfTimestampModules())
      {
	return &DgfModules[Settings->TimestampModule(Index)];
      }
    else
      {
	cerr<<__PRETTY_FUNCTION__<<": Error, index "<<Index<<" out of bounds (0 - "<<Settings->NumberOfTimestampModules()<<")"<<endl;
	return NULL;
      }
  };

  unsigned short GetTimestamps(vector<unsigned short>&);
  unsigned short GetTimestamps(unsigned short);

  DgfModule* GetEbisT1AndSuperCycleModule()
  {
    return &DgfModules[Settings->EbisT1AndSuperCycleModule()];
  };

  bool ScalerData()
  {
    return fScalerData;
  };

  SISScaler* GetScaler()
  {
    return &Scaler;
  };

  DgfScaler* GetDgfScaler()
  {
    return &fDgfScaler;
  };

  BraggChamber* GetBraggChamber()
  {
    return &fBraggChamber;
  };

  void Statistics();

private:
  void CorrectMesytecAdcTimestamps();

protected:
  //global Settings
  GlobalSettings* Settings;

  //statistics
  unsigned long long EventNumber;
  unsigned long long WrongHitPattern;
  unsigned long long WrongAdcHeader;
  unsigned long long AdcOverflow;
  unsigned long long AdcUnderflow;

  //data
  vector<DgfModule> DgfModules;
  vector<AdcModule> AdcModules;
  vector<PatternUnit> PatternUnits;
  SISScaler Scaler;
  DgfScaler fDgfScaler;
  BraggChamber fBraggChamber;

  bool fScalerData;

  //variable to store buffer time of the first buffer (to correct mesytec adc time)
  unsigned int fBufferTime;

  ClassDef(UnpackedEvent, 1)
};

#endif
