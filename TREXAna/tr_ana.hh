////////////////////////////////////////////////////////////
// Header for the tr_ana class to do the TREXAna for Miniball
// Liam Gaffney (liam.gaffney@liverpool.ac.uk) - 12/02/2022
////////////////////////////////////////////////////////////

#ifndef tr_ana_hh
#define tr_ana_hh

#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TFile.h>
#include <TGraph.h>
#include <TF1.h>
#include <TCutG.h>
#include <TMath.h>
#include <TGaxis.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

// Header file for the classes stored in the TTree if any.
#include <TObject.h>

// Experimental definitions
//#ifndef ExpDefs_hh
//# include "ExpDefs.hh"
//#endif

// Headers for tree
#ifndef __TREVTS_HH__
# include "trevts.hh"
#endif

/// Main class for gamma-particle coinidence analysis

class tr_ana : public TObject {
	public :
	TTree          *fChain;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrent; //!current Tree number in a TChain
	
	// Declaration of leaf types
	trevts          *evt;

	float			GammaEnergy;
	int				Zb, Zt;
	float			Ab, At;
	float			Eb, Ex, thick, depth;
	float			zoffset;		///< offset of the target with respect to the origin in mm (beam direction positive)
	float			cddist;			///< relative distance between the CD and target in mm
	float			cdoffset;		///< phi rotation of CD wrt to (det=0;nb=0) at vertical
	float			deadlayer;		///< deadlayer thickness in mm
	float			contaminant;	///< contaminant layer thickness in mg/cm^2
	float			spededist;		///< SPEDE to target distance in mm
	float			bg_frac;		///< ratio of prompt and random background subtraction (negative)
	TCutG			*Bcut;			///< Graphical cut for beam-like particles
	TCutG			*Tcut;			///< Graphical cut for target-like particles
	string			srim;			///< Directory containing the srim files
	bool			usekin;			///< Flag to use two-body kinematics for particle velocity
	
	/// Constructor: if parameter tree is not specified (or zero), probably crash
	tr_ana( TTree *tree = 0 );

	/// Destructor
	virtual ~tr_ana();

	/// Read contents of entry.
	virtual Int_t		GetEntry( Long64_t entry );

	/// Set the environment to read one entry
	virtual Long64_t	LoadTree( Long64_t entry );

	/// The Init() function is called when the selector needs to initialize
	/// a new tree or chain. Typically here the branch addresses and branch
	/// pointers of the tree will be set.
	/// It is normally not necessary to make changes to the generated
	/// code, but the routine can be extended by the user if needed.
	/// Init() will be called many times when running on PROOF
	/// (once per file to be processed).
	virtual void		Init( TTree *tree );

	/// This is the main logic routine for sorting the data.
	/// It shouldn't really be played with, except there are still some
	/// harcoded values that don't really make sense anymore.
	/// The ones that are useful will be moved to the config file/input
	/// and the ones that aren't will be deleted.
	/// You will also find the Miniball cluster angles hardcoded here too.
	/// They will eventually be moved to the TreeBuilder stage.
	virtual void		Loop( string outputfilename );

	/// The Notify() function is called when a new file is opened. This
	/// can be either for a new TTree in a TChain or when when a new TTree
	/// is started when using PROOF. It is normally not necessary to make changes
	/// to the generated code, but the routine can be extended by the
	/// user if needed. The return value is currently not used.
	virtual Bool_t		Notify();

	/// Print contents of entry.
	/// If entry is not specified, print current entry
	virtual void		Show( Long64_t entry = -1 );
	
	ClassDef(tr_ana,1);
	
};

#endif

#ifdef tr_ana_cxx
tr_ana::tr_ana( TTree *tree ) : fChain(0) {
	
	if( tree != 0 )	Init(tree);
	
}

tr_ana::~tr_ana() {
	
	if( !fChain ) return;
	delete fChain->GetCurrentFile();
	
}

Int_t tr_ana::GetEntry( Long64_t entry ) {

	if( !fChain ) return 0;
	return fChain->GetEntry(entry);

}

Long64_t tr_ana::LoadTree( Long64_t entry ) {

	if( !fChain ) return -5;
	Long64_t centry = fChain->LoadTree(entry);
	if( centry < 0 ) return centry;
	if( fChain->GetTreeNumber() != fCurrent ) {
		fCurrent = fChain->GetTreeNumber();
		Notify();
	}
	
	return centry;

}

void tr_ana::Init( TTree *tree ) {
	
	// Set branch addresses and branch pointers
	evt = 0;
	if (!tree) return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);
	
	fChain->SetBranchAddress( "trevts", &evt );
	Notify();
	
}

Bool_t tr_ana::Notify() {
	
	return kTRUE;
}

void tr_ana::Show( Long64_t entry ) {
	
	if (!fChain) return;
	fChain->Show(entry);
	
}

#endif // #ifdef tr_ana_cxx
