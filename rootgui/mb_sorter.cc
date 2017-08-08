// Very basic ROOT GUI to perform sorting of Miniball data
//
// Liam Gaffney (liam.gaffney@cern.ch) - 02/05/2017

#include <TGClient.h>
#include <TApplication.h>

#include "MBSorter.hh"

int main( int argc, char **argv ) {
	
	TApplication theApp( "App", &argc, argv );
	new MBSorter();
	theApp.Run();
	
	return 0;
	
}
