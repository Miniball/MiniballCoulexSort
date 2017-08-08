#include <iostream>

/* med includes */
#include "mbsio.h"

//analysis includes
#include "EventBuilder.hh"

using namespace std;

extern "C" {
	//unsigned int mbs_next_sheader(const MBSDataIO *);
	//unsigned int mbs_next_sdata(const MBSDataIO *);
	const MBSDataIO * mbs_open_file(const char *, const char *, int, FILE *);
	int mbs_close_file(const MBSDataIO *);
	unsigned int mbs_next_event(const MBSDataIO *);
}

int main(int argc, char* argv[]) {
	
	GlobalSettings* Settings;
	
	Settings = new GlobalSettings(argc, argv);
	
	if( Settings == NULL ) {
		
		cerr << "couldn't create Settings!" << endl;
		return 1;
		
	}
	
	//variables needed later on
	long long nofProcessedEvents = 0;
	int status;
	
	// open med file  (works only from file !!!)
	const MBSDataIO* mbs;
	
	printf("opening file %s ...\n", Settings->MedFile());
	
	mbs = mbs_open_file(Settings->MedFile(), "F", 0x4000, NULL);
	
	// if error when opening file
	if( mbs == NULL ) printf("couldn't open file %s -> exit!!!\n", Settings->MedFile());
	
	//EventBuilder instance
	EventBuilder* eventBuilder = new EventBuilder(Settings);
	
	// endless loop over events
	while(mbs != NULL) {
		
		// output every 1e4 readout events
		if(nofProcessedEvents%10000==0)
		{

			cout << "Processing event number " << setw(7) << nofProcessedEvents << "\r";
			cout.flush();
	
		}
		
		// get med buffer and call med-hdsubs
		unsigned int eventType = 0;
		
		eventType = mbs_next_event(mbs);	                          /* next event */
		if(eventType == MBS_ETYPE_EOF)
		{		                  /* end of file? */
			printf("End of file %s\n", Settings->MedFile());
			mbs_close_file(mbs);
			break;
		}
		else if(eventType == MBS_ETYPE_ERROR)
		{	                  /* error? */
			printf("Aborting after error\n");
			break;
		}
		else if(eventType == MBS_ETYPE_ABORT)
		{	                  /* abort? */
			printf("Aborting\n");
			break;
		}
		else {
			status = eventBuilder->ProcessEvent(mbs);
		}
		
		// check #of processed events
		if(nofProcessedEvents == Settings->LastEvent())
		{
			break;
		}
		
		//if stop trigger was found or an illegal trigger => stop
		if(status == 15 || status == 2)
		{
			break;
		}
		
		// increment number of processed events
		nofProcessedEvents++;
		
	} //while(mbs != NULL)
	
	cout << endl;
	
	eventBuilder->Finish();
	
	delete eventBuilder;
	
	return 0;
	
}//end of main


