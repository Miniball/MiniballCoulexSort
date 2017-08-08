#ifndef EVENT_BUFFER_HH
#define EVENT_BUFFER_HH

#include <iostream>
#include <iomanip>
#include <fstream>

#include "TObject.h"

#include "mbsio.h"

#include "GlobalSettings.hh"
#include "BuiltEvent.hh"
#include "Modules.hh"

class EventBuffer : public TObject
{
public:
  EventBuffer();
  EventBuffer(GlobalSettings*);
  ~EventBuffer();

  //reset
  inline void Clear()
  {
    if(Settings == NULL)
      {
	cout<<"no settings!"<<endl;
	exit(10);
      }

    if(Settings->VerboseLevel() > 3)
      {
	cout<<__PRETTY_FUNCTION__<<endl;
      }
    BuiltEvents.clear();
    Sorted = false;
  };

  //sort according to time
  void Sort();

  //set event number
  void EventNumber(long long eventNumber);

  //add particles, gammas or timing
  size_t AddParticle(unsigned short, AdcSubEvent*, long long, bool, bool, bool);
  size_t AddParticle(unsigned short, AdcSubEvent*, bool, bool, bool);
  size_t AddGamma(unsigned short, DgfSubEvent*);

  void AddTiming(long long, long long, long long);
  
  //get number of built events
  inline size_t NumberOfBuiltEvents()
  {
    return BuiltEvents.size();
  };

  //get sorted events (only if sorted and then set also the sub event number of the built event)
  BuiltEvent* GetSortedEvent(size_t);

protected:
  GlobalSettings* Settings;

  bool Sorted;

  unsigned short* SubEventNumber;//!BuiltEvents.size()
  vector <BuiltEvent> BuiltEvents;

  ClassDef(EventBuffer, 1)
};

#endif
