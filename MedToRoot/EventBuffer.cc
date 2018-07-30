#include "EventBuffer.hh"

using namespace std;

ClassImp(GlobalSettings)
ClassImp(BuiltEvent)
ClassImp(EventBuffer)

EventBuffer::EventBuffer() {

	//cout<<__PRETTY_FUNCTION__<<endl;
	Sorted = false;
	Settings = NULL;
	SubEventNumber = NULL;

}

EventBuffer::EventBuffer(GlobalSettings* settings)
  : Settings(settings) {

	cout<<__PRETTY_FUNCTION__<<endl;
	Sorted = false;
	SubEventNumber = NULL;

}

EventBuffer::~EventBuffer() {

	//cout<<__PRETTY_FUNCTION__<<endl;

}

// set event number of each built event
void EventBuffer::EventNumber(long long eventNumber) {

	for( size_t i = 0; i < BuiltEvents.size(); i++ ) {

		BuiltEvents[i].EventNumber(eventNumber);

	}

}

size_t EventBuffer::AddParticle(unsigned short moduleNumber, AdcSubEvent* adcSubEvent, long long time, bool LaserOn, bool FieldUp, bool FieldDown) {

	//check whether this particle is coincident with one of the others
	//  cout<<"CAEN"<< endl;;
	for( size_t event = 0; event < BuiltEvents.size(); event++ ) {

		if( BuiltEvents[event].Coincident(Settings, time) ) {

			BuiltEvents[event].AddParticle( moduleNumber, adcSubEvent, time, LaserOn, FieldUp, FieldDown );
			return BuiltEvents.size();

		}

	}

	BuiltEvents.push_back( BuiltEvent(moduleNumber, adcSubEvent, time, LaserOn, FieldUp, FieldDown) );

	return BuiltEvents.size();

}

size_t EventBuffer::AddParticle(unsigned short moduleNumber, AdcSubEvent* adcSubEvent, bool LaserOn, bool FieldUp, bool FieldDown) {

	//check whether this particle is coincident with one of the others
	for(size_t event = 0; event < BuiltEvents.size(); event++) {

		if( BuiltEvents[event].Coincident(Settings, adcSubEvent->Timestamp()) ) {

			BuiltEvents[event].AddParticle( moduleNumber, adcSubEvent, adcSubEvent->Timestamp(), LaserOn, FieldUp, FieldDown );
			return BuiltEvents.size();

		}

	}

	BuiltEvents.push_back( BuiltEvent(moduleNumber, adcSubEvent, adcSubEvent->Timestamp(), LaserOn, FieldUp, FieldDown) );

	return BuiltEvents.size();

}

size_t EventBuffer::AddGamma(unsigned short moduleNumber, DgfSubEvent* dgfSubEvent) {

	unsigned short multiplicity;

	// loop over all channels
	for( size_t channel = 0; channel < dgfSubEvent->GetNumberOfChannels(); channel++ ) {

		// check whether channel has a hit and whether energy is greated than zero
		if( (dgfSubEvent->GetHitPattern() & 1<<channel) != 0 && dgfSubEvent->GetEnergy(channel) > 0 ) {

			// to count how often this gamma was filled into the event
			multiplicity = 1;
			for( size_t event = 0; event < BuiltEvents.size(); event++ ) {

				if( BuiltEvents[event].Coincident(Settings, dgfSubEvent->GetLongFastTriggerTime(channel)) ) {

					BuiltEvents[event].AddGamma(moduleNumber, channel, dgfSubEvent, multiplicity);
					multiplicity++;

				}

			}

			// this means the loop ended without finding a coincident particle => add a new event
			if( multiplicity == 1 ) {

				BuiltEvents.push_back( BuiltEvent(moduleNumber, channel, dgfSubEvent) );

	    	}

		}

	}

	return BuiltEvents.size();

}

void EventBuffer::AddTiming( long long EbisTime, long long T1Time, long long SuperCycleTime ) {

	for( size_t i = 0; i < BuiltEvents.size(); i++ ) {

		BuiltEvents[i].AddTiming(EbisTime, T1Time, SuperCycleTime);

	}

}

//sort sub event according to event time
void EventBuffer::Sort() {

	size_t i, j, k;

	// if no events => no sorting
	if( BuiltEvents.size() == 0 ) {

		if( Settings->VerboseLevel() > 3 ) {

			cout << "no events, BuiltEvents.size() = " << BuiltEvents.size() << endl;

		}

		return;
	}

	// simply use the subevent number for sorting, the actual sorting is done later!
	if( SubEventNumber != NULL ) {

      	if( Settings->VerboseLevel() > 3 ) {

			cout << "deleting old SubEventNumber " << SubEventNumber << endl;

		}

		delete[] SubEventNumber;

    }

	SubEventNumber = new unsigned long long[BuiltEvents.size()];

	if( Settings->VerboseLevel() > 3 ) {

      cout << "created new SubEventNumber " << SubEventNumber << endl;

    }

 	// very simple sorting algorithm
	SubEventNumber[0] = 0;
	for( i = 1; i < BuiltEvents.size(); i++ ) {

		for( j = 0; j < i; j++ ) {

			if( BuiltEvents[i].GetTime() < BuiltEvents[SubEventNumber[j]].GetTime() ) {

				// move other sub event number one up
				for( k = i; k > j; k-- ) {

					SubEventNumber[k] = SubEventNumber[k-1];
					if( Settings->VerboseLevel() > 4 ) {

						cout << "i = " << i << ", j = " << j << ", k = " << k << ", SubEventNumber[";
						cout << k << "] = " << SubEventNumber[k] << ", SubEventNumber[" << k-1;
						cout << "] = " << SubEventNumber[k-1] << endl;

					}

				}

				break;

			}

		}

		// fill new sub event number in this spot 
		SubEventNumber[j] = i;

		if( Settings->VerboseLevel() > 4 ) {

			cout << "i = " << i << ", j = " << j << ", SubEventNumber[";
			cout << j << "] = " << SubEventNumber[j] << endl;

		}

	}

	if( Settings->VerboseLevel() > 3 ) {

		cout << "After sorting " << BuiltEvents.size();
		cout << " got following order (first line = index, second line = time):" << endl;

		for( i = 0; i < BuiltEvents.size(); i++ ) {

			cout << setw(20) << SubEventNumber[i] << " ";

		}

		cout << endl;

		for( i = 0; i < BuiltEvents.size(); i++ ) {

			cout << setw(20) << BuiltEvents[SubEventNumber[i]].GetTime() << " ";

		}

		cout<<endl;

	}

	Sorted = true;

}

BuiltEvent* EventBuffer::GetSortedEvent( size_t Index ) {

	if( Sorted ) {

		if( BuiltEvents.size() < Index ) {

			cerr << "Index " << Index << " is out of range 0 - " << BuiltEvents.size() << endl;
			return NULL;

		}

		// set sub event number of the built event
		BuiltEvents[SubEventNumber[Index]].SubEventNumber(Index);

		// return the sub event
		return &BuiltEvents[SubEventNumber[Index]];

	}

	else {

		cerr << "Sorry you first have to sort the buffer to get an sorted event!" << endl;
		return NULL;

	}
 
}
