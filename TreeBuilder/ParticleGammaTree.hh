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

// Header for addback
#ifndef __ADDBACK_HH
# include "AddBack.hh"
#endif

// Header for particle finder
#ifndef __PARTICLEFINDER_HH
# include "ParticleFinder.hh"
#endif

// Header for calibration
#ifndef __MBGEOMETRY_HH__
# include "MBGeometry.hh"
#endif

// Headers for output trees
#ifndef __MBEVTS_HH__
# include "mbevts.hh"
#endif
#ifndef __TREVTS_HH__
# include "trevts.hh"
#endif

/// Main class for building the particle-gamma coincidence trees

class ParticleGammaTree : public TObject {

public:
	
	/// Constructor
	ParticleGammaTree( TChain *tree, BuiltEvent *evt );
	
	/// Destructor
	~ParticleGammaTree();
	
	// Clean up
	void ClearEvt();
	
	/// Main function, called after constructing the GammaTree class
	virtual void Loop( string outputfilename );
	
	// Fill the output tree for TREX events
	virtual void FillTREXTree();
	
	// Fill the output tree for CLX events
	virtual void FillCLXTree();
	
	// Build particle-gamma coincidences for transfer
	virtual void CLXCoincidences();
	
	// Build gamma-particle coincidences for Coulex
	virtual void TREXCoincidences();
	
	// Build gamma-gamma coincidences matrix (always useful)
	virtual void GammaGammaCoincidences();
	
	// Function to set calibration
	inline void SetCalibration( Calibration *_Cal ){
		Cal = _Cal;
		return;
	};

	/// Initalise variables
	virtual void InitialiseVariables();
	
	/// Setup histograms
	virtual void SetupHistograms();
	
	/// Setup sorting flags
	virtual void SetupFlags( bool _singles, bool _gamgam, bool _addback, bool _reject,
							 bool _segsum, bool _crex, bool _trex, bool _cdpad,
							 bool _ionch, bool _spede, bool _verbose );

	
	// Variables
	float gamma_theta[8][3][7];	///< Theta angle of the MB segments
	float gamma_phi[8][3][7];	///< Phi angle of the MB segments
	
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

	double tMinPromptGamma;	///< minimum edge of prompt window (gamma-gamma)
	double tMaxPromptGamma;	///< maximum edge of prompt window (gamma-gamma)
	double tMinRandomGamma;	///< minimum edge of random window (gamma-gamma)
	double tMaxRandomGamma;	///< maximum edge of random window (gamma-gamma)


	
	// Histograms
	// diagnostics
	TH1F *adc, *dgf, *free_dgf, *cd_debug;
	
	// time differences
	TH1F *tdiff_gp, *tdiff_ep, *tdiff_gg, *tdiff_pgg;
	TH1F *tdiff_gp_q[4], *tdiff_BD;
	
	// gamma
	TH2F *gg, *gg_pr, *gg_ran;

private:
	
	/// Calibration
	Calibration *Cal;
	
	/// Input tree
	TChain *fTree;
	
	/// BuiltEvent pointer
	BuiltEvent *event;
	
	// Sorting flags
	bool singles;
	bool gamgam;
	bool addback;
	bool reject;
	bool segsum;
	bool crex;
	bool trex;
	bool cdpad;
	bool ionch;
	bool spede;
	bool verbose;
	
	// Temporary variables
	int dgf_num;
	int dgf_num2;
	int dgf_ch;
	int dgf_ch2;
	int dgf_en;
	int dgf_en2;
	long long dgf_t;
	long long dgf_t2;
	unsigned int adc_num;
	unsigned int adc_ch;
	unsigned int adc_en;
	unsigned int adc_ch2;
	unsigned int adc_en2;
	long long adc_t;
	float tdiffPG;
	float tdiffGG;
	int coinc_flag;

	// Counters
	unsigned int ParticleCounterQ[4];
	
	// Gamma and particle finders
	AddBack ab;
	ParticleFinder pf;

	// Trees - CLX
	mbevts write_mb_evts, fill_mb_evts;
	vector<mbevts> mb_evts;
	TTree *g_clx;
	
	// Trees - TREX
	trevts write_tr_evts, fill_tr_evts;
	vector<trevts> tr_evts;
	TTree *p_tr;
	
	
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
	
	// Counters
	for( unsigned int i = 0; i < 4; i++ ) ParticleCounterQ[i] = 0.;

	// Prompt and random time windows (to be read by calibration eventually!)
	tMinPrompt = -12.;			tMaxPrompt = 6.;			// 18 ticks
	tMinRandom = 10.;			tMaxRandom = 34.;			// 24 ticks
	tMinDelayed = -31.;			tMaxDelayed = -13.;			// 18 ticks
 
	tMinPromptElectron = -6.;	tMaxPromptElectron = 6.;	// 12 ticks
	tMinRandomElectron = 7.;	tMaxRandomElectron = 33.;	// = (39/18)*12 = 26
 
	tMinPromptGamma = -15.;	tMaxPromptGamma = 15.;	// from tdiff_gg
	tMinRandomGamma = 20.;	tMaxRandomGamma = 35.;	// from tdiff_gg

	WeightPR = TMath::Abs( tMinPrompt - tMaxPrompt );
	WeightPR /= TMath::Abs( tMinRandom - tMaxRandom );


	cout << "WeightPR: " << WeightPR << endl;
	
	// ------------------------------------------------------------------------ //
	// Write to the g_clx/p_tr trees
	// ------------------------------------------------------------------------ //
	g_clx = new TTree( "g_clx", "g_clx" );
	g_clx->Branch( "mbevts", "mbevts", &write_mb_evts );
	
	p_tr = new TTree( "p_tr", "p_tr" );
	p_tr->Branch( "trevts", "trevts", &write_tr_evts );
	
	return;
	
}

void ParticleGammaTree::SetupFlags( bool _singles, bool _gamgam, bool _addback, bool _reject,
								   bool _segsum, bool _crex, bool _trex, bool _cdpad,
								   bool _ionch, bool _spede, bool _verbose ){

	// Set the private variables, the flags
	singles = _singles;
	gamgam = _gamgam;
	addback = _addback;
	reject = _reject;
	segsum = _segsum;
	crex = _crex;
	trex = _trex;
	cdpad = _cdpad;
	ionch = _ionch;
	spede = _spede;
	verbose = _verbose;
	
	return;
	
}

void ParticleGammaTree::SetupHistograms(){
	
	// time differences
	tdiff_gp = new TH1F("tdiff_gp","tdiff_gp",201,-100.5,100.5);
	tdiff_gp->GetXaxis()->SetTitle("time diff (particle - gamma) [25 ns tics]");
	tdiff_ep = new TH1F("tdiff_ep","tdiff_ep",201,-100.5,100.5);
	tdiff_ep->GetXaxis()->SetTitle("time diff (particle - electron) [25 ns tics]");
	tdiff_gg = new TH1F("tdiff_gg","tdiff_gg",201,-100.5,100.5);
	tdiff_gg->GetXaxis()->SetTitle("time diff between gamma rays [25 ns tics]");
	tdiff_pgg = new TH1F("tdiff_pgg","tdiff_pgg, coincident with particle",201,-100.5,100.5);
	tdiff_pgg->GetXaxis()->SetTitle("time diff between gamma rays [25 ns tics]");
	for( unsigned int i = 0; i < 4; i++ )
		tdiff_gp_q[i] = new TH1F(Form("tdiff_gp_%d",i),Form("tdiff_gp_%d",i),201,-100.5,100.5);
	
	// Gamma-gamma - no Doppler correction
	gg = new TH2F("gg","#gamma-#gamma matrix;Energy [keV];Energy[keV]",GBINS,GMIN,GMAX,GBINS,GMIN,GMAX);
	gg_pr = new TH2F("gg_pr","prompt #gamma-#gamma matrix;Energy [keV];Energy[keV]",GBINS,GMIN,GMAX,GBINS,GMIN,GMAX);
	gg_ran = new TH2F("gg_ran","random #gamma-#gamma matrix;Energy [keV];Energy[keV]",GBINS,GMIN,GMAX,GBINS,GMIN,GMAX);

	// ------------------------------------------------------------------------ //
	
}

#endif
