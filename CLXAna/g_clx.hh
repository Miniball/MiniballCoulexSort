//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Feb 18 15:39:48 2013 by ROOT version 5.34/05
// from TTree g_clx/g_clx
// found on file: 62Mn_109Ag.root
//////////////////////////////////////////////////////////

#ifndef g_clx_hh
#define g_clx_hh

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
# include "../mbevts/mbevts.hh"
#endif

// Fixed size dimensions of array or collections stored in the TTree if any.

class g_clx : public TObject {
	public :
	TTree          *fChain;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrent; //!current Tree number in a TChain
	
	// Declaration of leaf types
	//mbevts          *mbevts;
	UInt_t          fUniqueID;
	UInt_t          fBits;
	float           gen;
	int             cid;
	int             sid;
	int             cluid;
	float           tha;
	float           pha;
	vector <float>  gcor_gen;
	vector <int>    gcor_cid;
	vector <int>    gcor_sid;
	vector <int>    gcor_cluid;
	vector <float>  gcor_tha;
	vector <float>  gcor_pha;
	vector <float>  gcor_gtd;
	vector <float>  pen;
	vector <double> time;
	vector <double> sst;
	vector <float>  td;
	vector <int>    ann;
	vector <int>    sec;
	vector <int>    det;
	vector <int>    coin;
	int             laser;
	int             pr_hits;
	int             rndm_hits;
	int             del_hits;
	vector <int>    pr_ptr;
	vector <int>    rndm_ptr;
	vector <int>    del_ptr;
	int             file;

	float			GammaEnergy;
	int				Zb, Zt;
	float			Ab, At;
	float			Eb, Ex, thick, depth;
	float			cddist, cdoffset;
	float			deadlayer;
	float			contaminant;
	float			spededist;
	float			bg_frac;
	TCutG			*Bcut, *Tcut;
	
	// List of branches
	TBranch        *b_mbevts_fUniqueID;   //!
	TBranch        *b_mbevts_fBits;   //!
	TBranch        *b_mbevts_gen;   //!
	TBranch        *b_mbevts_cid;   //!
	TBranch        *b_mbevts_sid;   //!
	TBranch        *b_mbevts_cluid;   //!
	TBranch        *b_mbevts_tha;   //!
	TBranch        *b_mbevts_pha;   //!
	TBranch        *b_mbevts_gcor_gen;   //!
	TBranch        *b_mbevts_gcor_cid;   //!
	TBranch        *b_mbevts_gcor_sid;   //!
	TBranch        *b_mbevts_gcor_cluid;   //!
	TBranch        *b_mbevts_gcor_tha;   //!
	TBranch        *b_mbevts_gcor_pha;   //!
	TBranch        *b_mbevts_gcor_gtd;   //!
	TBranch        *b_mbevts_pen;   //!
	TBranch        *b_mbevts_time;   //!
	TBranch        *b_mbevts_sst;   //!
	TBranch        *b_mbevts_td;   //!
	TBranch        *b_mbevts_ann;   //!
	TBranch        *b_mbevts_sec;   //!
	TBranch        *b_mbevts_det;   //!
	TBranch        *b_mbevts_coin;   //!
	TBranch        *b_mbevts_laser;   //!
	TBranch        *b_mbevts_pr_hits;   //!
	TBranch        *b_mbevts_rndm_hits;   //!
	TBranch        *b_mbevts_del_hits;   //!
	TBranch        *b_mbevts_pr_ptr;   //!
	TBranch        *b_mbevts_rndm_ptr;   //!
	TBranch        *b_mbevts_del_ptr;   //!
	TBranch        *b_mbevts_file;   //!
	
	g_clx( TTree *tree = 0 );
	virtual ~g_clx();
	virtual Int_t		GetEntry( Long64_t entry );
	virtual Long64_t	LoadTree( Long64_t entry );
	virtual void		Init( TTree *tree );
	virtual void		Loop( string outputfilename );
	virtual Bool_t		Notify();
	virtual void		Show( Long64_t entry = -1 );
	
	ClassDef(g_clx,1);
	
};

#endif

#ifdef g_clx_cxx
g_clx::g_clx(TTree *tree) : fChain(0) {
	// if parameter tree is not specified (or zero), probably crash
	
	if( tree != 0 )	Init(tree);
	
}

g_clx::~g_clx() {
	if (!fChain) return;
	delete fChain->GetCurrentFile();
}

Int_t g_clx::GetEntry(Long64_t entry) {
	// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}

Long64_t g_clx::LoadTree(Long64_t entry) {
	// Set the environment to read one entry
	if (!fChain) return -5;
	Long64_t centry = fChain->LoadTree(entry);
	if (centry < 0) return centry;
	if (fChain->GetTreeNumber() != fCurrent) {
		fCurrent = fChain->GetTreeNumber();
		Notify();
	}
	return centry;
}

void g_clx::Init(TTree *tree) {
	// The Init() function is called when the selector needs to initialize
	// a new tree or chain. Typically here the branch addresses and branch
	// pointers of the tree will be set.
	// It is normally not necessary to make changes to the generated
	// code, but the routine can be extended by the user if needed.
	// Init() will be called many times when running on PROOF
	// (once per file to be processed).
	
	// Set branch addresses and branch pointers
	if (!tree) return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);
	
	fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_mbevts_fUniqueID);
	fChain->SetBranchAddress("fBits", &fBits, &b_mbevts_fBits);
	fChain->SetBranchAddress("gen", &gen, &b_mbevts_gen);
	fChain->SetBranchAddress("cid", &cid, &b_mbevts_cid);
	fChain->SetBranchAddress("sid", &sid, &b_mbevts_sid);
	fChain->SetBranchAddress("cluid", &cluid, &b_mbevts_cluid);
	fChain->SetBranchAddress("tha", &tha, &b_mbevts_tha);
	fChain->SetBranchAddress("pha", &pha, &b_mbevts_pha);
	fChain->SetBranchAddress("gcor_gen", &gcor_gen, &b_mbevts_gcor_gen);
	fChain->SetBranchAddress("gcor_cid", &gcor_cid, &b_mbevts_gcor_cid);
	fChain->SetBranchAddress("gcor_sid", &gcor_sid, &b_mbevts_gcor_sid);
	fChain->SetBranchAddress("gcor_cluid", &gcor_cluid, &b_mbevts_gcor_cluid);
	fChain->SetBranchAddress("gcor_tha", &gcor_tha, &b_mbevts_gcor_tha);
	fChain->SetBranchAddress("gcor_pha", &gcor_pha, &b_mbevts_gcor_pha);
	fChain->SetBranchAddress("gcor_gtd", &gcor_gtd, &b_mbevts_gcor_gtd);
	fChain->SetBranchAddress("pen", &pen, &b_mbevts_pen);
	fChain->SetBranchAddress("time", &time, &b_mbevts_time);
	fChain->SetBranchAddress("sst", &sst, &b_mbevts_sst);
	fChain->SetBranchAddress("td", &td, &b_mbevts_td);
	fChain->SetBranchAddress("ann", &ann, &b_mbevts_ann);
	fChain->SetBranchAddress("sec", &sec, &b_mbevts_sec);
	fChain->SetBranchAddress("det", &det, &b_mbevts_det);
	fChain->SetBranchAddress("coin", &coin, &b_mbevts_coin);
	fChain->SetBranchAddress("laser", &laser, &b_mbevts_laser);
	fChain->SetBranchAddress("pr_hits", &pr_hits, &b_mbevts_pr_hits);
	fChain->SetBranchAddress("rndm_hits", &rndm_hits, &b_mbevts_rndm_hits);
	fChain->SetBranchAddress("del_hits", &del_hits, &b_mbevts_del_hits);
	fChain->SetBranchAddress("pr_ptr", &pr_ptr, &b_mbevts_pr_ptr);
	fChain->SetBranchAddress("rndm_ptr", &rndm_ptr, &b_mbevts_rndm_ptr);
	fChain->SetBranchAddress("del_ptr", &del_ptr, &b_mbevts_del_ptr);
	fChain->SetBranchAddress("file", &file, &b_mbevts_file);
	Notify();
}

Bool_t g_clx::Notify() {
	// The Notify() function is called when a new file is opened. This
	// can be either for a new TTree in a TChain or when when a new TTree
	// is started when using PROOF. It is normally not necessary to make changes
	// to the generated code, but the routine can be extended by the
	// user if needed. The return value is currently not used.
	
	return kTRUE;
}

void g_clx::Show(Long64_t entry) {
	/* Print contents of entry.	*/
	/* If entry is not specified, print current entry */
	if (!fChain) return;
	fChain->Show(entry);
}

#endif // #ifdef g_clx_cxx
