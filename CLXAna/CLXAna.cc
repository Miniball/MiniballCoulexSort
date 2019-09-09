// CLXAna: The main routine that calls g_clx
// Liam Gaffney (liam.gaffney@cern.ch) - 01/05/2017

#include "CLXAna.hh"

#ifndef __CINT__

void PrintInput() {
	
	cout << "Zb = " << Zb << endl;
	cout << "Ab = " << Ab << endl;
	cout << "Zt = " << Zt << endl;
	cout << "At = " << At << endl;
	cout << "Eb = " << Eb << " keV/u" << endl;
	cout << "Ex = " << Ex << " keV" << endl;
	cout << "thick = " << thick << " mg/cm2" << endl;
	cout << "depth = " << depth << " mg/cm2" << endl;
	cout << "cddist = " << cddist << " mm" << endl;
	cout << "cdoffset = " << cdoffset << " degrees" << endl;
	cout << "deadlayer = " << deadlayer << " mm" << endl;
	cout << "contaminant = " << contaminant << " mg/cm2" << endl;
	cout << "spededist = " << spededist << " mm" << endl;
	cout << "bg_frac = " << bg_frac << endl;
	cout << "srim = " << srim << endl;
	cout << "cutfile = " << cutfilename << endl;
	if( usekin ) cout << "Using two-body kinematics for particle velocity" << endl;
	else cout << "Using detected particle energy for velocity calculation" << endl;
	
	cout << "\nOutputfile = " << outputfilename << endl << endl;

	return;
	
}

int main( int argc, char *argv[] ) {

	CommandLineInterface* interface = new CommandLineInterface();

	interface->Add("-i", "Input file list", &inputfilenames );
	interface->Add("-o", "Output file name", &outputfilename );
	interface->Add("-c", "Configuration file", &configfilename );
	interface->Add("-cut", "Cutfile [cutfile.root:Bcut:Tcut]", &cutfilename );
	interface->Add("-Zb", "Zb", &Zb );
	interface->Add("-Ab", "Ab", &Ab );
	interface->Add("-Zt", "Zt", &Zt );
	interface->Add("-At", "At", &At );
	interface->Add("-Eb", "Eb", &Eb );
	interface->Add("-Ex", "Ex", &Ex );
	interface->Add("-thick", "Target thickness (mg/cm^2)", &thick );
	interface->Add("-depth", "Depth of interation in target (mg/cm^2)", &depth );
	interface->Add("-cddist", "Relative distance of CD and target (mm)", &cddist );
	interface->Add("-cdoffset", "Rotation of CD detector about phi from vertical (deg)", &cdoffset );
	interface->Add("-deadlayer", "Thickness of Si dead layer (mm)", &deadlayer );
	interface->Add("-contaminant", "Thickness of contaminant layer on target (mg/cm^2)", &contaminant );
	interface->Add("-spededist", "Relative distance of SPEDE and target (mm)", &spededist );
	interface->Add("-bg_frac", "Ratio of prompt and random for background subtraction", &bg_frac );
	interface->Add("-srim", "Directory containing the SRIM files", &srim );
	interface->Add("-usekin", "Use two-body kinematics for particle velocity?", &usekin );

	interface->CheckFlags( argc, argv );

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

	// Get the cut file
	if( cutfilename.size() > 0 ) {

		size_t sep1 = cutfilename.find_first_of( ":" );
		size_t sep2 = cutfilename.find_last_of( ":" );

		if( sep1 <= 1 || sep2 <= 1 || sep1 > cutfilename.size() || sep2 > cutfilename.size() ) {

			cout << "Format for the cutfile should be <cutfile.root>:<Bcut>:<Tcut>\n";
			cout << "where <Bcut> and <Tcut> are the TCutG names of the beam and\n";
			cout << "target cuts in the root file, respectively." << endl;

			return 0;

		}

		string str_file = cutfilename.substr( 0, sep1 );
		string str_bcut = cutfilename.substr( sep1+1, sep2-sep1-1 );
		string str_tcut = cutfilename.substr( sep2+1, cutfilename.size()-sep2-1 );

		TFile *fcut = new TFile( str_file.c_str() );

		if( fcut->IsZombie() ) {

			cout << "Didn't open " << str_file << " correctly\n";
			cout << "Does it exist?\n";

			return 0;

		}

		if( !fcut->GetListOfKeys()->Contains( str_bcut.c_str() ) ) {

			cout << "Didn't find beam cut, " << str_bcut << ", in " << str_file << endl;
			return 0;

		}

		if( !fcut->GetListOfKeys()->Contains( str_tcut.c_str() ) ) {

			cout << "Didn't find target cut, " << str_tcut << ", in " << str_file << endl;
			return 0;

		}

		x.Bcut = (TCutG*)fcut->Get( str_bcut.c_str() );
		x.Tcut = (TCutG*)fcut->Get( str_tcut.c_str() );

	}

	// if not cut file given, make empty cuts
	else {

		x.Bcut = new TCutG();
		x.Tcut = new TCutG();

	}

	// Test if we're using a config file (we overwrite the values if so)
	if( configfilename.size() > 0 ) {

		TEnv *config = new TEnv( configfilename.c_str() );

		Zb = config->GetValue( "Zb", -1 );
		Ab = config->GetValue( "Ab", -1.0 );
		Zt = config->GetValue( "Zt", -1 );
		At = config->GetValue( "At", -1.0 );
		Eb = config->GetValue( "Eb", -1.0 );
		Ex = config->GetValue( "Ex", -1.0 );
		thick = config->GetValue( "thick", -1.0 );
		depth = config->GetValue( "depth", -1.0 );
		cddist = config->GetValue( "cddist", -1.0 );
		cdoffset = config->GetValue( "cdoffset", 242.6 );
		deadlayer = config->GetValue( "deadlayer", 0.0007 );
		contaminant = config->GetValue( "contaminant", -1.0 );
		spededist = config->GetValue( "spededist", 23.6 );
		bg_frac = config->GetValue( "bg_frac", -1.0 );
		srim = config->GetValue( "srim", "./srim" );
		usekin = config->GetValue( "usekin", false );

	}

	// Parameters are already read from the command line if not overwritten by config file
	if( Zb > 0 && Zt > 0 && Ab > 0 && At > 0 && Eb > 0 && Ex > 0 &&
				thick > 0 && depth > 0 && cddist > 0 ) {

		// Setup the experimental parameters
		//x.SetExpDefs( Zb, Ab, Zt, At, Eb, Ex, thick, depth, cddist, cdoffset );
		x.Zb = Zb;
		x.Ab = Ab;
		x.Zt = Zt;
		x.At = At;
		x.Eb = Eb;
		x.Ex = Ex;
		x.thick = thick;
		x.depth = depth;
		x.cddist = cddist;
		x.cdoffset = cdoffset;
		x.deadlayer = deadlayer;
		x.contaminant = contaminant;
		x.spededist = spededist;
		x.bg_frac = bg_frac;
		x.srim = srim;
		x.usekin = usekin;
		
		cout << "Input parameters:" << endl;
		PrintInput();

	}

	// In case something is missing, print out what we have and quit
	else {

		cout << "Some input is missing, please check:" << endl;
		PrintInput();
		cout << "Exiting..." << endl;

		return 0;
	}
	
	// Run sort
	cout << "Begin g_clx loop." << endl;
	x.Loop( outputfilename );
	
	cout << "Finished." << endl;
	cout << "\a" << endl;
	
	return 0;

}
#endif

