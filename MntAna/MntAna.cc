// TreeAna: The main routine that calls g_clx
// Liam Gaffney (liam.gaffney@cern.ch) - 01/05/2017

// Experimental definitions
#ifndef g_clx_hh
# include "g_clx.hh"
#endif

#include <ctime>
#include <iostream>
using namespace std;

#ifndef __CINT__
int main( int argc, char *argv[] ) {

	string arg_str;
	string outputfilename;
	vector<string> inputfilenames;
	int j = 0;

	// If not arguments, run default
	if( argc == 1 ) {

		time_t rawtime;
		struct tm *timeinfo;
		char buf[80];

		time( &rawtime );
		timeinfo = localtime( &rawtime );

		strftime( buf, 80, "%Y%m%d_%H%M", timeinfo );

		outputfilename = buf;
		outputfilename = "Hists_" + outputfilename + ".root";

		g_clx x;
		x.Loop( outputfilename );

	}

	// Else process command line arguments
	else {

		for( int i = 1; i < argc; i++ ) {

			arg_str.assign( argv[i] );

			// Help
			if( arg_str == "-h" ) {

				cout << argv[0] << "-i <inputfiles> -o <outputfile>" << endl;

			}

			// Input files
			else if( arg_str == "-i" ) {

				if( i >= argc ) {

					cerr << "No input filename given" << endl;
					return 0;

				}

				cout << "List of input files..." << endl;

				while( i+j+1 < argc ) {

					arg_str.assign( argv[i+j+1] );

					if( arg_str[0] == '-' ) break;

					inputfilenames.push_back( arg_str );
					cout << inputfilenames[j] << endl;

					j++;

				}

			} // "-i"

			// Output file
			else if( arg_str == "-o" ) {

				if( i >= argc ) {

					cerr << "No output filename given" << endl;
					return 0;

				}

				else outputfilename = argv[i+1];

				cout << "Output file: " << outputfilename << endl;


			} // "-o"

		} // i

	} // process command line arguments

	// Test if output file is there
	if( outputfilename.size() <= 0 ) {

		cout << "Did you specify your output file correctly?" << endl;
		return 0;

	}

	// Test if input files are there
	if( inputfilenames.size() <= 0 ) {

		cout << "Did you specify your input files correctly?" << endl;
		return 0;

	}

	// Make chain for g_clx
	TChain *chain = new TChain( "g_clx", "" );
	for( unsigned int i = 0; i < inputfilenames.size(); i++ ) {

		chain->Add( inputfilenames[i].c_str() );	

	}

	// Initiate g_clx
	g_clx x( chain );
	x.Loop( outputfilename );

	return 0;

}
#endif

