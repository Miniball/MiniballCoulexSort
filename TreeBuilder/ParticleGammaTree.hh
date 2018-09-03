#ifndef ParticleGammaTree_hh
#define ParticleGammaTree_hh

#include <iostream>
#include <iomanip>
#include <string>

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2S.h"
#include "TMath.h"
using namespace std;

// Command line interface
#ifndef __COMMAND_LINE_INTERFACE
# include "CommandLineInterface.hh"
#endif

// Header for input tree
#ifndef EVENT_HH
# include "BuiltEvent.hh"
#endif

// Header for calibration
#ifndef __CALIBRATION_HH
# include "Calibration.hh"
#endif

// Header for calibration
#ifndef __MBGEOMETRY_HH__
# include "MBGeometry.hh"
#endif

// Headers for outut trees
#ifndef __MBEVTS_HH__
# include "mbevts.hh"
#endif
#ifndef __TREVTS_HH__
# include "trevts.hh"
#endif

//ClassImp(BuiltEvent)
//ClassImp(AdcData)
//ClassImp(DgfData)

/// Main class for building the particle-gamma coincidence trees

class ParticleGammaTree : public TObject {

public:
	
	/// Constructor
	ParticleGammaTree( TChain *tree, BuiltEvent *evt );
	
	/// Destructor
	~ParticleGammaTree();
	
	/// Main function, called after constructing the GammaTree class
	virtual void Loop( string outputfilename );

	/// Calibration
	Calibration *Cal;
	
	/// Input tree
	TChain *fTree;
	
	/// BuiltEvent pointer
	BuiltEvent *event;
	
	/// Setup sorting flags
	virtual void SetupFlags( bool _singles, bool _gamgam, bool _addback, bool _crex, bool _trex,
							 bool _cdpad, bool _ionch, bool _spede, bool _verbose );

	
	// Variables
	float gamma_theta[8][3][7];	///< Theta angle of the MB segments
	float gamma_phi[8][3][7];	///< Phi angle of the MB segments
	double dtAdc[4];			///< time offsets of each Adc with respect to DGF time (ticks)
	
	double tMinPrompt;	///< minimum edge of prompt window (particle-gamma)
	double tMaxPrompt;	///< maximum edge of prompt window (particle-gamma)
	double tMinRandom;	///< minimum edge of random window (particle-gamma)
	double tMaxRandom;	///< maximum edge of random window (particle-gamma)
	double tMinDelayed;	///< minimum edge of delayed window (particle-gamma)
	double tMaxDelayed;	///< maximum edge of delayed window (particle-gamma)
 
	double tMinPromptElectron;	///< minimum edge of prompt window (particle-electron)
	double tMaxPromptElectron;	///< maximum edge of prompt window (particle-electron)
	double tMinRandomElectron;	///< minimum edge of random window (particle-electron)
	double tMaxRandomElectron;	///< maximum edge of random window (particle-electron)
 
	double WeightPR; ///< ratio of prompt and random windows
	
	/// Initalise variables
	virtual void InitialiseVariables();
	
private:
	
	// Sorting flags
	bool singles;
	bool gamgam;
	bool addback;
	bool crex;
	bool trex;
	bool cdpad;
	bool ionch;
	bool spede;
	bool verbose;

	ClassDef( ParticleGammaTree, 1 );
	
};
#endif

#ifdef ParticleGammaTree_cxx
ParticleGammaTree::ParticleGammaTree( TChain *tree, BuiltEvent *evt ) : fTree(0) {
	
	if( tree != 0 )	fTree = tree;
	if( evt != 0 ) event = evt;
	
}

ParticleGammaTree::~ParticleGammaTree() {
	
	if( !fTree ) return;
	delete fTree->GetCurrentFile();
	
}

void ParticleGammaTree::InitialiseVariables() {
	
	// Prompt and random time windows (to be read by calibration eventually!)
	tMinPrompt = -12.;			tMaxPrompt = 6.;			// 18 ticks
	tMinRandom = 8.;			tMaxRandom = 35.;			// 27 ticks
	tMinDelayed = -31.;			tMaxDelayed = -13.;			// 18 ticks
 
	tMinPromptElectron = -6.;	tMaxPromptElectron = 6.;	// 12 ticks
	tMinRandomElectron = 7.;	tMaxRandomElectron = 33.;	// = (39/18)*12 = 26
 
	WeightPR = TMath::Abs( tMinPrompt - tMaxPrompt );
	WeightPR /= TMath::Abs( tMinRandom - tMaxRandom );
	
	cout << "WeightPR: " << WeightPR << endl;
	

	// How many ticks need to align the prompt peak for each adc?
	for( unsigned int i = 0; i < 4; i++ ) dtAdc[i] = Cal->AdcTime(i);
	
	return;
	
}

void ParticleGammaTree::SetupFlags( bool _singles, bool _gamgam, bool _addback, bool _crex, bool _trex,
								   bool _cdpad, bool _ionch, bool _spede, bool _verbose ){

	// Set the private variables, the flags
	singles = _singles;
	gamgam = _gamgam;
	addback = _addback;
	crex = _crex;
	trex = _trex;
	cdpad = _cdpad;
	ionch = _ionch;
	spede = _spede;
	verbose = _verbose;
	
	return;
	
}

#endif