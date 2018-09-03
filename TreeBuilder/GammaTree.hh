#ifndef TreeBuilder_hh
#define TreeBuilder_hh

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
# include "../mbevts/mbevts.hh"
#endif
#ifndef __TREVTS_HH__
# include "../trevts/trevts.hh"
#endif

/// Main class for building the particle-gamma coincidence trees

class GammaTree : public TObject {

public:
	
	/// Constructor
	GammaTree( TTree *tree );
	
	/// Destructor
	~GammaTree();
	
	/// Main function, called after constructing the GammaTree class
	virtual void Loop( string outputfilename );

	/// Calibration
	Calibration *Cal;
	
	/// Input tree
	TTree *fTree;
	
	// Variables
	float gamma_theta[8][3][7];	///< Theta angle of the MB segments
	float gamma_phi[8][3][7];	///< Phi angle of the MB segments
	
	
private:
	
	
};
#endif

#ifdef g_clx_cxx
GammaTree::GammaTree( TTree *tree ) : fTree(0) {
	
	if( tree != 0 )	fTree = tree;
	
}

GammaTree::~GammaTree() {
	
	if( !fTree ) return;
	delete fTree->GetCurrentFile();
	
}
#endif