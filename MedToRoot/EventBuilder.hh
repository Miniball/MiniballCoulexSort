#ifndef EVENT_BUILDER_HH
#define EVENT_BUILDER_HH

#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>

#include "TFile.h"

#include "mbsio.h"

#include "GlobalSettings.hh"
#include "UnpackedEvent.hh"
#include "BuiltEvent.hh"
#include "EventBuffer.hh"

class EventBuilder : public TObject
{
public:
  EventBuilder();
  EventBuilder(GlobalSettings*);
  ~EventBuilder();

  int ProcessEvent(const MBSDataIO * mbs);

  void BuildEvent();

  void Finish();

  void Statistics();

private:
  GlobalSettings* Settings;
  
  TFile* OnBeamFile;
  TTree* OnBeamTree;
  BuiltEvent* OnBeamEvent;

  TFile* OnBeamBackgroundFile;
  TTree* OnBeamBackgroundTree;
  BuiltEvent* OnBeamBackgroundEvent;

  TFile* OffBeamFile;
  TTree* OffBeamTree;
  BuiltEvent* OffBeamEvent;

  TFile* ScalerFile;
  TTree* ScalerTree;
  SISScaler* fScaler;
  DgfScaler* fDgfScaler;
  BraggChamber* fBraggChamber;

  UnpackedEvent* unpackedEvent;

  EventBuffer* eventBuffer;

  //some variables for statistics
  unsigned long long TotalNumberOfBytesCommittedToOnBeamTree;
  unsigned long long TotalNumberOfBytesCommittedToOnBeamBackgroundTree;
  unsigned long long TotalNumberOfBytesCommittedToOffBeamTree;
  unsigned long long TotalNumberOfBytesCommittedToScalerTree;
  unsigned long long NumberOfBytesWrittenToOnBeamFile;
  unsigned long long NumberOfBytesWrittenToOnBeamBackgroundFile;
  unsigned long long NumberOfBytesWrittenToOffBeamFile;
  unsigned long long NumberOfBytesWrittenToScalerFile;
  unsigned long long PatternUnitMismatches;

  //Ebis, T1 and SuperCycle pulses
  unsigned long long TotalNumberOfEbisPulses;
  unsigned long long TotalNumberOfT1Pulses;
  unsigned long long TotalNumberOfSuperCyclePulses;

  unsigned long long EbisTime;

  unsigned long long T1Time;

  unsigned long long SuperCycleTime;

  //laser ON bit
  bool LaserOn;
  
  //Field Up and Down bit
  bool Field;
  bool FieldUp;
  bool FieldDown;

  ClassDef(EventBuilder, 1)
};

#endif
