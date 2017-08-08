#ifndef __GLOBAL_SETTINGS_HH
#define __GLOBAL_SETTINGS_HH

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include "TSystem.h"
#include "TEnv.h"
#include "TObject.h"
#include "TMath.h"

#include "CommandLineInterface.hh"

using namespace std;
using namespace TMath;

class GlobalSettings : public TObject
{
public:
  GlobalSettings(){;};//default ctor
  GlobalSettings(int, char* []);
  ~GlobalSettings();

  friend ostream& operator <<(ostream &,const GlobalSettings &);

  inline unsigned short FirstMiniballDgf()
  {
    return fFirstMiniballDgf;
  };

  inline unsigned short LastMiniballDgf()
  {
    return fLastMiniballDgf;
  };

  inline unsigned short BeamdumpDgf()
  {
    return fBeamdumpDgf;
  };

  inline unsigned short FirstAdc()
  {
    return fFirstAdc;
  };

  inline unsigned short LastAdc()
  {
    return fLastAdc;
  };

  inline unsigned short BraggChamber()
  {
    return fBraggChamber;
  };

  inline unsigned short Scaler()
  {
    return fScaler;
  };

  inline unsigned short PatternUnit()
  {
    return fPatternUnit;
  };

  inline unsigned short FirstDgfScaler()
  {
    return fFirstDgfScaler;
  };

  inline unsigned short LastDgfScaler()
  {
    return fLastDgfScaler;
  };

  inline unsigned short NumberOfDgfModules()
  {
    return fNumberOfDgfModules;
  };

  inline unsigned short NumberOfAdcModules()
  {
    return fNumberOfAdcModules;
  };

  inline unsigned short NumberOfPatternUnits()
  {
    return fNumberOfPatternUnits;
  };

  inline unsigned short NumberOfDgfChannels()
  {
    return fNumberOfDgfChannels;
  };

  inline unsigned short NumberOfTimestampModules()
  {
    return fNumberOfTimestampModules;
  };

  inline unsigned short TimestampModule(unsigned short Index)
  {
    if(Index <= fNumberOfTimestampModules)
      {
	return fTimestampModule[Index]-fMarabouDgfModuleOffset;
      }
    else
      {
	return 0xffff;
      }
  };

  inline double TimestampModuleOffset(unsigned short Index)
  {
    if(Index <= fNumberOfTimestampModules)
      {
	return fTimestampModuleOffset[Index];
      }
    else
      {
	return -1.;
      }
  };

  inline unsigned short TimestampChannel()
  {
    return fTimestampChannel;
  };

  inline unsigned short NumberOfAdcsPerTimestampModule()
  {
    return fNumberOfAdcsPerTimestampModule;
  };

  inline unsigned short EbisT1AndSuperCycleModule()
  {
    return fEbisT1AndSuperCycleModule-fMarabouDgfModuleOffset;
  };

  inline unsigned short EbisChannel()
  {
    return fEbisChannel;
  };

  inline unsigned short T1Channel()
  {
    return fT1Channel;
  };

  inline unsigned short SuperCycleChannel()
  {
    return fSuperCycleChannel;
  };

  inline unsigned short MarabouDgfModuleOffset()
  {
    return fMarabouDgfModuleOffset;
  };

  inline unsigned short MarabouAdcModuleOffset()
  {
    return fMarabouAdcModuleOffset;
  };

  inline unsigned short MarabouPatternUnitOffset()
  {
    return fMarabouPatternUnitOffset;
  };

  inline long long LastEvent()
  {
    return fLastEvent;
  };

  inline int VerboseLevel()
  {
    return fVerboseLevel;
  }

  inline const char* MedFile()
  {
    return fMedFile.c_str();
  };
  
  inline const char* OnBeamFile()
  {
    return fOnBeamFile.c_str();
  };

  inline const char* OnBeamBackgroundFile()
  {
    return fOnBeamBackgroundFile.c_str();
  };

  inline const char* OffBeamFile()
  {
    return fOffBeamFile.c_str();
  };

  inline const char* ScalerFile()
  {
    return fScalerFile.c_str();
  };

  bool IsTimestampModule(unsigned short ModuleNumber)
  {
    for(unsigned short i = 0;i < fNumberOfTimestampModules; i++)
      {
	if(ModuleNumber == fTimestampModule[i]-fMarabouDgfModuleOffset)
	  return true;
      }

    if(ModuleNumber == fEbisT1AndSuperCycleModule-fMarabouDgfModuleOffset)
      return true;;

    return false;
  };

  inline bool Coincident(long long time)
  {
    if(fLowestCoincidence < time && time < fHighestCoincidence)
      {
	return true;
      }
    return false;
  };

  unsigned short RealTimeIndex()
  {
    return fRealTimeIndex;
  };

  unsigned short RunTimeIndex()
  {
    return fRunTimeIndex;
  };

  unsigned short GSLTTimeIndex()
  {
    return fGSLTTimeIndex;
  };

  unsigned short NumberOfEventsIndex()
  {
    return fNumberOfEventsIndex;
  };

  unsigned short ChannelIndicesOffset(unsigned short channel)
  {
    if(channel >= fNumberOfDgfChannels)
      {
	cerr<<"Error, channel "<<channel<<" out of range (0 - "<<fNumberOfDgfChannels<<")"<<endl;

	return 0xffff;
      }

    return fChannelIndicesOffset[channel];
  };

  unsigned short LiveTimeIndex()
  {
    return fLiveTimeIndex;
  };

  unsigned short FastPeakIndex()
  {
    return fFastPeakIndex;
  };

  unsigned short BraggTraceLength()
  {
    return fBraggTraceLength;
  };

  bool SourceRun()
  {
    return fSourceRun;
  };

  bool IncludeScaler()
  {
    return fIncludeScaler;
  };

  bool MesytecAdc()
  {
    return fMesytecAdc;
  };
  
  bool SPEDEChamb()
  {
    return fSPEDEChamb;
  };

  unsigned short NofCaenAdc()
  {
    return fNofCaenAdc;
  };

  unsigned short TypeOfSetup()
  {
    return fTypeOfSetup;
  };
  
  
  unsigned short EbisWindowLowerEdge()
  {
    return fEbisWindowLowerEdge;
  };

  unsigned short EbisWindowUpperEdge()
  {
    return fEbisWindowUpperEdge;
  };
  long long DgfInitDelay(){
    return fDgfInitDelay;
  }

  bool Verify(); //performs some consistency checks on the settings

protected:
  void ReadSettings();
  void ReadUnpackerSettings();

protected:
  string fMedFile;
  string fOnBeamFile;
  string fOnBeamBackgroundFile;
  string fOffBeamFile;
  string fScalerFile;
  string fSettingsFile;

  TEnv* fSettings;

  int fVerboseLevel;
  long long fLastEvent;

  //Module numbers
  unsigned short fFirstMiniballDgf;
  unsigned short fLastMiniballDgf;
      
  unsigned short fBeamdumpDgf;
      
  unsigned short fFirstAdc;
  unsigned short fLastAdc;
      
  unsigned short fBraggChamber;
  unsigned short fScaler;
  unsigned short fPatternUnit;
  unsigned short fFirstDgfScaler;
  unsigned short fLastDgfScaler;

  double fCoincidenceWindowWidth;
  double fReferencePointOfCoincidenceWindow;

  long long fLowestCoincidence;
  long long fHighestCoincidence;

  unsigned short fNumberOfDgfModules;
  unsigned short fNumberOfAdcModules;
  unsigned short fNumberOfPatternUnits;

  unsigned short fNumberOfDgfChannels;

  unsigned short fNumberOfAdcsPerTimestampModule;

  unsigned short fNumberOfTimestampModules;
  unsigned short* fTimestampModule; //!fNumberOfTimestampModules
  double* fTimestampModuleOffset;//!fNumberOfTimestampModules

  unsigned short fTimestampChannel;

  unsigned short fEbisT1AndSuperCycleModule;
  unsigned short fEbisChannel;
  unsigned short fT1Channel;
  unsigned short fSuperCycleChannel;

  unsigned short fMarabouDgfModuleOffset;
  unsigned short fMarabouAdcModuleOffset;
  unsigned short fMarabouPatternUnitOffset;

  //dgf scaler parameter indices
  unsigned short fRealTimeIndex;
  unsigned short fRunTimeIndex;
  unsigned short fGSLTTimeIndex;
  unsigned short fNumberOfEventsIndex;
  unsigned short* fChannelIndicesOffset;
  unsigned short fLiveTimeIndex;
  unsigned short fFastPeakIndex;

  unsigned short fBraggTraceLength;

  bool fSourceRun;
  bool fIncludeScaler;
  bool fMesytecAdc;
  bool fSPEDEChamb;
  unsigned short fNofCaenAdc;
  unsigned short fTypeOfSetup;
  unsigned short fEbisWindowLowerEdge;
  unsigned short fEbisWindowUpperEdge;

  long long fDgfInitDelay;
  ClassDef(GlobalSettings, 1)
};

#endif
