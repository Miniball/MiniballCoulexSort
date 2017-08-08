#ifndef EVENT_HH
#define EVENT_HH

#include <iostream>
#include <fstream>

#include "TTree.h"

#include "GlobalSettings.hh"
#include "Datas.hh"

using namespace std;

class BuiltEvent : public TObject {

public:
	BuiltEvent();
	//create a new event
	BuiltEvent( unsigned short, AdcSubEvent*, long long, bool, bool, bool );
	BuiltEvent( unsigned short, unsigned short, DgfSubEvent* );
	~BuiltEvent();
	
	//other functions
	void Clear();
	
	//bool Check();
	
	//add particles or gammas to existing event
	void AddParticle( unsigned short, AdcSubEvent*, long long, bool, bool, bool );
	void AddGamma( unsigned short, unsigned short, DgfSubEvent*, unsigned short );
	
	inline void AddTiming( long long EbisTime, long long T1Time, long long SuperCycleTime ) {
		ebisTime = EbisTime;
		t1Time = T1Time;
		superCycleTime = SuperCycleTime;
	};
	
	inline void EventNumber(long long EventNumber) { eventNumber = EventNumber; };
	inline void SubEventNumber(unsigned short SubEventNumber) { subEventNumber = SubEventNumber; };
	
	//functions to access members
	inline size_t NumberOfAdcs() { return adcData.size(); };
	inline size_t NumberOfDgfs() { return dgfData.size(); };
	inline long long EbisTime() { return ebisTime; };
	inline long long T1Time() { return t1Time; };
	inline long long SuperCycleTime() { return superCycleTime; };
	inline long long EventNumber() { return eventNumber; };
	inline unsigned short SubEventNumber() { return subEventNumber; };
	inline AdcData* Adc(size_t Index) { return &(adcData.at(Index)); };
	inline DgfData* Dgf(size_t Index) { return &(dgfData.at(Index)); };
	bool Coincident( GlobalSettings*, long long );
	long long GetTime();
	
protected:
	//data
	long long ebisTime;
	long long t1Time;
	long long superCycleTime;
	
	long long eventNumber;
	unsigned short subEventNumber;
	
	vector <AdcData> adcData;
	vector <DgfData> dgfData;
	
	ClassDef(BuiltEvent, 3)
	
};

#endif
