// Building of particle-gamma coincidence trees for Coulex or Transfer
// analysis at Miniball. This code uses the output of MedToRoot
// Liam Gaffney (liam.gaffney@cern.ch) - 31/08/2018

#include "TreeBuilder.hh"
#define PBINS 800
#define PRANGE 800
#define PMIN -1.0*PRANGE/PBINS
#define PMAX PRANGE+PMIN

#define GBINS 4000
#define GRANGE 4000
#define GMIN -1.0*GRANGE/GBINS
#define GMAX GRANGE+GMIN
#define GAMMA_ARRAY 250

#define ELBINS 2000
#define ELRANGE 2000
#define ELMIN -1.0*ELRANGE/ELBINS
#define ELMAX ELRANGE+ELMIN
#define ELECTRON_ARRAY 250

#ifndef __CINT__

int main(int argc, char* argv[]) {

	// ------------------------------------------------------------------------ //
	// Read command line
	// ------------------------------------------------------------------------ //
	interface->Add("-i", "inputfiles", &InputFiles );
	interface->Add("-o", "outputfile", &OutputFile );
	interface->Add("-c", "calibrationfile", &CalibrationFile );
	interface->Add("-s", "singles", &singles );
	interface->Add("-gg", "gamma-gamma", &gamgam );
	interface->Add("-addback", "addback", &addback );
	interface->Add("-crex", "CREX", &crex );
	interface->Add("-trex", "TREX", &trex );
	interface->Add("-spede", "SPEDE", &spede );
	interface->Add("-cdpad", "CD-PAD", &cdpad );
	interface->Add("-ionch", "IonChamber", &ionch );
	interface->Add("-vl", "verbose", &verbose );

	interface->CheckFlags(argc, argv);

	if( InputFiles.size() == 0 || OutputFile.size() == 0 ) {

		cerr << "You have to provide at least one input file and the output file!" << endl;
		exit(1);

	}

	if( CalibrationFile.size() == 0 ) {

		cout << "No Calibration File given, you have to provide one!" << endl;
		exit(1);

	}

	cout << "input file(s):" << endl;
	for( unsigned int i=0; i<InputFiles.size(); i++ ) {

		cout << InputFiles[i] << endl;
	
	}
	
	cout << "calibration file: " << CalibrationFile << endl;
	cout << "output file: " << OutputFile << endl;
	
	// ------------------------------------------------------------------------ //


	// ------------------------------------------------------------------------ //
	// Initialise events and setup the input tree
	// ------------------------------------------------------------------------ //
	TChain* tr;
	tr = new TChain( "tr" );
	for( unsigned int i = 0; i < InputFiles.size(); i++ ) {
	
		tr->Add( InputFiles[i] );
		
	}

	if( tr == NULL ) {

		cout << "could not find tree tr in file " << endl;

		for( unsigned int i=0; i < InputFiles.size(); i++ ) {

			cout << InputFiles[i] << endl;

		}
		
		return 3;

	}

	BuiltEvent* event = new BuiltEvent;
	tr->SetBranchAddress( "Event", &event );
	
	// ------------------------------------------------------------------------ //
	
	
	// ------------------------------------------------------------------------ //
	// Initialise the output tree
	// ------------------------------------------------------------------------ //

	// If T-REX data, build a tree for transfer analysis
	if( trex ) {
		
		TransferTree *pg_ana = new TransferTree( tr );
		
	}
	
	// else we have Coulex or other gamma-ray data...
	else {
		
		GammaTree *pg_ana = new GammaTree( tr );
		
	}
	
	// Make calibration
	Calibration *Cal = new Calibration( CalibrationFile );
	//if( verbose ) Cal->PrintCalibration();
	pg_ana.Cal = Cal;

	// ------------------------------------------------------------------------ //
	
	
	// ------------------------------------------------------------------------ //
	// Setup Miniball angles
	// ------------------------------------------------------------------------ //
	
	MBGeometry mbg;
	for( int i = 0; i < 8; i++ ) { // loop over clusters
		
		mbg.SetupCluster( Cal->ClusterTheta(i), Cal->ClusterPhi(i),
						 Cal->ClusterAlpha(i), Cal->ClusterR(i), Cal->ZOffset());
		
		for( unsigned int j = 0; j < 3; j++ ) { // loop over cores
			
			pgana.gamma_theta[i][j][0] = mbg.GetCoreTheta(j) * TMath::DegToRad();
			pgana.gamma_phi[i][j][0] = mbg.GetCorePhi(j) * TMath::DegToRad();
			
			for( int k = 0; k < 6; k++ ) { // loop over segments
				
				pgana.gamma_theta[i][j][k+1] = mbg.GetSegTheta(j,k) * TMath::DegToRad();
				pgana.gamma_phi[i][j][k+1] = mbg.GetSegPhi(j,k) * TMath::DegToRad();
				
			}
			
		}
		
	}
	
	
	// ------------------------------------------------------------------------ //

	
	// ------------------------------------------------------------------------ //
	// Call the particle-gamma analysis routine, clean up and finish
	// ------------------------------------------------------------------------ //

	pg_ana.Loop( Outpufile );
	
	delete tr;

	cout << "Finished." << endl;
	return 0;
	
}

#endif
