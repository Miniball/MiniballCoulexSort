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
#ifndef __MBEVTS_HH__
# include "mbevts.hh"
#endif

/// Main class for gamma-particle coinidence analysis

class tr_ana : public TObject {
	public :
	TTree          *fChain;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrent; //!current Tree number in a TChain
	
	// Declaration of leaf types
	//trevts          *trevts;
	UInt_t          fUniqueID;
	UInt_t          fBits;
	float				pen;		///< reconstructed particle energy in keV
	float				pen_de;	///< reconstructed particle dE in keV
	float				pen_e;	///< reconstructed particle E_rest in keV
	int					quad;		///< quadrant of the T-REX (0 = Top; 1 = Left; 2 = Bottom; 3 = Right)
	int					nf;			///< front strip number of CD quadrant
	int					nb;			///< back strip number of CD quadrant
	int					sector;		///< sector of T-REX (0 = FCD; 1 = FBarrel; 2 = BBarrel; 3 = BCD)
	vector <float>		pcor_pen;
	vector <float>		pcor_pen_de;
	vector <float>		pcor_pen_e;
	vector <int>		pcor_quad;
	vector <int>		pcor_nf;
	vector <int>		pcor_nb;
	vector <int>		pcor_sector;
	vector <double>		pcor_ptd;
	vector <float>		gen;		///< gamma-ray energy in keV
	vector <int>		cid;		///< Miniball core id (0-23)
	vector <int>		sid;		///< Miniball segment id (0 is core 1-6 is segments)
	vector <int>		cluid;		///< Miniball cluster id (0-7)
	vector <float>		tha;		///< gamma-ray theta angle in radians
	vector <float>		pha;		///< gamma-ray phi angle in radians
	vector <float>		td;		///< particle-gamma time difference
	vector <int>		coin;		///< particle-gamma coincidence flag (0 = prompt; 1 = random; 2 = delayed)
	Double_t        	time;
	Double_t        	t1t;
	Double_t        	sst;
	int            		laser;		///< laser on/off flag: 1 = on, 0 = off
	int					pr_hits;	///< number of prompt hits
	int					rndm_hits;	///< number of random hits
	int					del_hits;	///< number of delayed hits
	vector <int>		pr_ptr;		///< pointer to prompt hits in particle vector/array
	vector <int>		rndm_ptr;	///< pointer to random hits in particle vector/array
	vector <int>		del_ptr;	///< pointer to delayed hits in particle vector/array
	int           		file;

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
	
	// List of branches
	TBranch        *b_trevts_fUniqueID;   //!
	TBranch        *b_trevts_fBits;   //!
	TBranch        *b_trevts_pen;   //!
	TBranch        *b_trevts_pen_de;   //!
	TBranch        *b_trevts_pen_e;   //!
	TBranch        *b_trevts_quad;   //!
	TBranch        *b_trevts_nf;   //!
	TBranch        *b_trevts_nb;   //!
	TBranch        *b_trevts_sector;   //!
	TBranch        *b_trevts_time;   //!
	TBranch        *b_trevts_t1t;   //!
	TBranch        *b_trevts_sst;   //!
	TBranch        *b_trevts_laser;   //!
	TBranch        *b_trevts_pcor_pen;   //!
	TBranch        *b_trevts_pcor_pen_de;   //!
	TBranch        *b_trevts_pcor_pen_e;   //!
	TBranch        *b_trevts_pcor_quad;   //!
	TBranch        *b_trevts_pcor_nf;   //!
	TBranch        *b_trevts_pcor_nb;   //!
	TBranch        *b_trevts_pcor_sector;   //!
	TBranch        *b_trevts_pcor_ptd;   //!
	TBranch        *b_trevts_gen;   //!
	TBranch        *b_trevts_cid;   //!
	TBranch        *b_trevts_sid;   //!
	TBranch        *b_trevts_cluid;   //!
	TBranch        *b_trevts_tha;   //!
	TBranch        *b_trevts_pha;   //!
	TBranch        *b_trevts_td;   //!
	TBranch        *b_trevts_coin;   //!
	TBranch        *b_trevts_pr_hits;   //!
	TBranch        *b_trevts_rndm_hits;   //!
	TBranch        *b_trevts_del_hits;   //!
	TBranch        *b_trevts_pr_ptr;   //!
	TBranch        *b_trevts_rndm_ptr;   //!
	TBranch        *b_trevts_del_ptr;   //!

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
	if (!tree) return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);
	
	fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_trevts_fUniqueID);
	fChain->SetBranchAddress("fBits", &fBits, &b_trevts_fBits);
	fChain->SetBranchAddress("pen", &pen, &b_trevts_pen);
	fChain->SetBranchAddress("pen_de", &pen_de, &b_trevts_pen_de);
	fChain->SetBranchAddress("pen_e", &pen_e, &b_trevts_pen_e);
	fChain->SetBranchAddress("quad", &quad, &b_trevts_quad);
	fChain->SetBranchAddress("nf", &nf, &b_trevts_nf);
	fChain->SetBranchAddress("nb", &nb, &b_trevts_nb);
	fChain->SetBranchAddress("sector", &sector, &b_trevts_sector);
	fChain->SetBranchAddress("time", &time, &b_trevts_time);
	fChain->SetBranchAddress("t1t", &t1t, &b_trevts_t1t);
	fChain->SetBranchAddress("sst", &sst, &b_trevts_sst);
	fChain->SetBranchAddress("laser", &laser, &b_trevts_laser);
	fChain->SetBranchAddress("pcor_pen", &pcor_pen, &b_trevts_pcor_pen);
	fChain->SetBranchAddress("pcor_pen_de", &pcor_pen_de, &b_trevts_pcor_pen_de);
	fChain->SetBranchAddress("pcor_pen_e", &pcor_pen_e, &b_trevts_pcor_pen_e);
	fChain->SetBranchAddress("pcor_quad", &pcor_quad, &b_trevts_pcor_quad);
	fChain->SetBranchAddress("pcor_nf", &pcor_nf, &b_trevts_pcor_nf);
	fChain->SetBranchAddress("pcor_nb", &pcor_nb, &b_trevts_pcor_nb);
	fChain->SetBranchAddress("pcor_sector", &pcor_sector, &b_trevts_pcor_sector);
	fChain->SetBranchAddress("pcor_ptd", &pcor_ptd, &b_trevts_pcor_ptd);
	fChain->SetBranchAddress("gen", &gen, &b_trevts_gen);
	fChain->SetBranchAddress("cid", &cid, &b_trevts_cid);
	fChain->SetBranchAddress("sid", &sid, &b_trevts_sid);
	fChain->SetBranchAddress("cluid", &cluid, &b_trevts_cluid);
	fChain->SetBranchAddress("tha", &tha, &b_trevts_tha);
	fChain->SetBranchAddress("pha", &pha, &b_trevts_pha);
	fChain->SetBranchAddress("td", &td, &b_trevts_td);
	fChain->SetBranchAddress("coin", &coin, &b_trevts_coin);
	fChain->SetBranchAddress("pr_hits", &pr_hits, &b_trevts_pr_hits);
	fChain->SetBranchAddress("rndm_hits", &rndm_hits, &b_trevts_rndm_hits);
	fChain->SetBranchAddress("del_hits", &del_hits, &b_trevts_del_hits);
	fChain->SetBranchAddress("pr_ptr", &pr_ptr, &b_trevts_pr_ptr);
	fChain->SetBranchAddress("rndm_ptr", &rndm_ptr, &b_trevts_rndm_ptr);
	fChain->SetBranchAddress("del_ptr", &del_ptr, &b_trevts_del_ptr);
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
