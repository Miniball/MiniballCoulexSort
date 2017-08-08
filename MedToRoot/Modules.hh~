#ifndef __MODULES_HH
#define __MODULES_HH

#include <iostream>

#include "TObject.h"

#include "SubEvents.hh"

class DgfModule : public TObject
{
public:

  DgfModule();
  ~DgfModule();

  //access functions
  void AddSubEvent(DgfSubEvent NewSubEvent)
  {
    fSubEvents.push_back(NewSubEvent);
  };

  void Clear()
  {
    fType = 999;
    fSubEvents.clear();
  };

  //set functions
  void SetModuleNumber(unsigned short moduleNumber)
  {
    fModuleNumber = moduleNumber;
  };

  void SetType(unsigned short type)
  {
    fType = type;
  };

  //get functions
  unsigned short GetModuleNumber()
  {
    return fModuleNumber;
  };

  unsigned short GetType()
  {
    return fType;
  };

  size_t GetNumberOfSubEvents()
  {
    return fSubEvents.size();
  };

  DgfSubEvent* GetSubEvent(size_t Index)
  {
    return &(fSubEvents.at(Index));
  };

  //int GetTimestamps(int*);

protected:

  unsigned short fModuleNumber;
  unsigned short fType;
  vector <DgfSubEvent> fSubEvents;

  ClassDef(DgfModule, 1)
};

class AdcModule : public TObject
{
public:
  AdcModule();//default ctor
  ~AdcModule();//dtor

  void Clear()
  {
    SubEvents.clear();
  };

  //set functions
  void SetModuleNumber(unsigned short moduleNumber)
  {
    ModuleNumber = moduleNumber;
  };

  void AddSubEvent(AdcSubEvent NewSubEvent)
  {
    SubEvents.push_back(NewSubEvent);
  };
  void DeletaLastSubEvent()
  {
    SubEvents.pop_back();
  };
  
  //get functions
  size_t GetNumberOfSubEvents()
  {
    return SubEvents.size();
  }

  unsigned short GetModuleNumber()
  {
    return ModuleNumber;
  };

  AdcSubEvent* GetSubEvent(size_t Index)
  {
    return &(SubEvents.at(Index));
  };

  AdcSubEvent GetLastSubEvent()
  {
    return SubEvents.back();
  };

protected:

  unsigned short ModuleNumber;
  vector <AdcSubEvent> SubEvents;

  ClassDef(AdcModule, 1)
};

class PatternUnit : public TObject
{
public:
  PatternUnit(){};
  ~PatternUnit();

  void Clear()
  {
    SubEvents.clear();
  };

  //set and access functions
  void SetModuleNumber(unsigned short moduleNumber)
  {
    ModuleNumber = moduleNumber;
  };

  void AddSubEvent(PatternUnitSubEvent SubEvent)
  {
    SubEvents.push_back(SubEvent);
  };

  size_t GetNumberOfSubEvents()
  {
    return SubEvents.size();
  }

  unsigned short GetModuleNumber()
  {
    return ModuleNumber;
  };

  PatternUnitSubEvent* GetSubEvent(size_t Index)
  {
    return &SubEvents.at(Index);
  };

  PatternUnitSubEvent GetLastSubEvent()
  {
    return SubEvents.back();
  };

  unsigned short GetNumberOfTriggers();

  bool LaserOn();
  bool FieldUp();
  bool FieldDown();

protected:
  unsigned short ModuleNumber;
  vector <PatternUnitSubEvent> SubEvents;

  ClassDef(PatternUnit, 1)
};

class SISScaler : public TObject
{
public:
  SISScaler(){};
  ~SISScaler();

  void Clear()
  {
    SubEvents.clear();
  };

  //set and access functions
  void SetModuleNumber(unsigned short moduleNumber)
  {
    ModuleNumber = moduleNumber;
  };

  void AddSubEvent(ScalerSubEvent SubEvent)
  {
    SubEvents.push_back(SubEvent);
  };

  size_t GetNumberOfSubEvents()
  {
    return SubEvents.size();
  }

  unsigned short GetModuleNumber()
  {
    return ModuleNumber;
  };

  ScalerSubEvent* GetSubEvent(size_t Index)
  {
    return &SubEvents.at(Index);
  };

  ScalerSubEvent GetLastSubEvent()
  {
    return SubEvents.back();
  };

protected:
  unsigned short ModuleNumber;
  vector <ScalerSubEvent> SubEvents;

  ClassDef(SISScaler, 1)
};

class DgfScaler : public TObject
{
public:
  DgfScaler(){};
  ~DgfScaler();

  void Clear()
  {
    SubEvents.clear();
  };

  //set and access functions
  void AddSubEvent(DgfScalerSubEvent& SubEvent)
  {
    //cout<<"adding subevent ..."<<flush;
    SubEvents.push_back(SubEvent);
    //cout<<" done"<<endl;
  };

  size_t GetNumberOfSubEvents()
  {
    return SubEvents.size();
  }

  DgfScalerSubEvent* GetSubEvent(size_t Index)
  {
    return &SubEvents.at(Index);
  };

  DgfScalerSubEvent GetLastSubEvent()
  {
    return SubEvents.back();
  };

protected:
  vector <DgfScalerSubEvent> SubEvents;

  ClassDef(DgfScaler, 1)
};


class BraggChamber : public TObject
{
public:
  BraggChamber(){};
  ~BraggChamber();

  void Clear()
  {
    SubEvents.clear();
  };

  //set and access functions
  void AddSubEvent(BraggChamberSubEvent SubEvent)
  {
    SubEvents.push_back(SubEvent);
  };

  size_t GetNumberOfSubEvents()
  {
    return SubEvents.size();
  }

  BraggChamberSubEvent* GetSubEvent(size_t Index)
  {
    return &SubEvents.at(Index);
  };

  BraggChamberSubEvent GetLastSubEvent()
  {
    return SubEvents.back();
  };

protected:
  vector <BraggChamberSubEvent> SubEvents;

  ClassDef(BraggChamber, 1)
};


#endif
