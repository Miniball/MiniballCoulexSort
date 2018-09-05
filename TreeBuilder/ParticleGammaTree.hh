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
	
	/// Main function, called after constructing the GammaTree class
	virtual void Loop( string outputfilename );

	/// Calibration
	Calibration *Cal;
	
	/// Input tree
	TChain *fTree;
	
	/// BuiltEvent pointer
	BuiltEvent *event;
	
	/// Initalise variables
	virtual void InitialiseVariables();
	
	/// Setup histograms
	virtual void SetupHistograms( TFile *outfile );
	
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
	
	unsigned int i,j,k,l;

	
	// Histograms
	// diagnostics
	TH1F *adc, *dgf, *free_dgf, *cd_debug;
	
	// time differences
	TH1F *tdiff_gp, *tdiff_ep, *tdiff_gg;
	TH1F *tdiff_gp_q[4], *tdiff_BD;
	
	// gamma
	TH1F *E_gam;
	TH2F *gg;

	// particles
	TH2F *part;
	TH1F *E_part_ann[4][16], *E_part_ann_cal[4][16];
	TH1F *E_part_sec[4][12], *E_part_sec_cal[4][12];
	TH2F *CD_front_energy[4], *CD_front_energy_cal[4];
	TH2F *CD_back_energy[4], *CD_back_energy_cal[4];
	TH2F *CD_front_back[4][4];

	// spede
	TH1F *E_spede, *E_spede_seg[24], *E_spede_seg_cal[24];
	
	// Ionisation chamber
	TH2F *dEE;
	
	// CD-pad detector, freely triggered
	TH1F *padE_sum, *padE[4], *padE_cal[4];
	TH2F *cd_dEE_sum, *cd_dEE_anti, *cd_dEE[4];
	
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

	// Counters
	unsigned int GammaCtr;
	unsigned int PartCtr;

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
	GammaCtr = 0;
	PartCtr = 0;

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

void ParticleGammaTree::SetupHistograms( TFile *outfile ){
	
	// diagnostics
	adc = new TH1F("adc","adc",80,-0.5,79.5);
	adc->GetXaxis()->SetTitle("Number of ADC's");
	dgf = new TH1F("dgf","dgf",56,-0.5,55.5);
	dgf->GetXaxis()->SetTitle("Number of DGF's");
	free_dgf = new TH1F("free_dgf","free_dgf",56,-0.5,55.5);
	free_dgf->GetXaxis()->SetTitle("Number of free DGF's");
	cd_debug = new TH1F("cd_debug","cd_debug",4,-0.5,3.5);
	cd_debug->GetXaxis()->SetTitle("CD debugging");
	
	// time differences
	tdiff_gp = new TH1F("tdiff_gp","tdiff_gp",201,-100.5,100.5);
	tdiff_gp->GetXaxis()->SetTitle("time diff (particle - gamma) [us]");
	tdiff_ep = new TH1F("tdiff_ep","tdiff_ep",201,-100.5,100.5);
	tdiff_ep->GetXaxis()->SetTitle("time diff (particle - electron) [us]");
	tdiff_gg = new TH1F("tdiff_gg","tdiff_gg",201,-100.5,100.5);
	tdiff_gg->GetXaxis()->SetTitle("time diff between one Dgf and the others [us]");
	for( i = 0; i < 4; i++ )
		tdiff_gp_q[i] = new TH1F(Form("tdiff_gp_%d",i),Form("tdiff_gp_%d",i),201,-100.5,100.5);
	
	// Gamma-gamma - no Doppler correction
	gg = new TH2F("gg","#gamma-#gamma matrix;Energy [keV];Energy[keV]",GBINS,GMIN,GMAX,GBINS,GMIN,GMAX);

	// Particle spectra for every segment
	part = new TH2F("part","part",16,-0.5,15.5,1000,0,1000);
	TDirectory *part_dir = outfile->mkdir("E_part");
	part_dir->cd();
	for( i = 0; i < 4; i++ ) {
		
		for( j = 0; j < 16; j++ ) {
			
			E_part_ann[i][j] = new TH1F(Form("E_part_ann_%d_%d",i,j),Form("E_part_ann_%d_%d",i,j),4096,-0.5,4095.5);
			E_part_ann[i][j]->GetXaxis()->SetTitle("Energy Particles [Channels]");
			E_part_ann_cal[i][j] = new TH1F(Form("E_part_ann_%d_%d_cal",i,j),Form("E_part_ann_%d_%d_cal",i,j),PBINS,PMIN,PMAX);
			E_part_ann_cal[i][j]->GetXaxis()->SetTitle("Energy Particles [MeV]");
			
		}
		
		for( j = 0; j < 12; j++ ) {
			
			E_part_sec[i][j] = new TH1F(Form("E_part_sec_%d_%d",i,j),Form("E_part_sec_%d_%d",i,j),4096,-0.5,4095.);
			E_part_sec[i][j]->GetXaxis()->SetTitle("Energy Particles [Channels]");
			E_part_sec_cal[i][j] = new TH1F(Form("E_part_sec_%d_%d_cal",i,j),Form("E_part_sec_%d_%d_cal",i,j),PBINS,PMIN,PMAX);
			E_part_sec_cal[i][j]->GetXaxis()->SetTitle("Energy Particles [MeV]");
			
		}
		
	}
	
	gDirectory->cd("/");
	
	E_spede = NULL;
	if( spede ) {
		
		// Total electron - no Doppler correction
		E_spede = new TH1F("E_spede","E_spede",ELBINS,ELMIN,ELMAX);
		E_spede->GetXaxis()->SetTitle("Energy Electrons [Channels]");
		
		// Electron spectra for every segment - no Doppler correction
		TDirectory *spede_dir = outfile->mkdir("E_spede_seg");
		spede_dir->cd();
		for( i = 0; i < 24; i++ ) {
			
			E_spede_seg[i] = new TH1F(Form("E_spede_%d",i),Form("E_spede_%d",i),16384,-0.5,65535.5);
			E_spede_seg[i]->GetXaxis()->SetTitle("Energy Electrons [Channels]");
			E_spede_seg_cal[i] = new TH1F(Form("E_spede_%d_cal",i),Form("E_spede_%d_cal",i),ELBINS,ELMIN,ELMAX);
			E_spede_seg_cal[i]->GetXaxis()->SetTitle("Energy Electrons [keV]");
			
		}
		
		gDirectory->cd("/");
		
	}
	
	// particle-spectra
	TDirectory *cd_dir = outfile->mkdir("CD_spec");
	cd_dir->cd();
	vector< string > cd_hname;
	cd_hname.push_back( "CD_front_back_%d_1v1" );
	cd_hname.push_back( "CD_front_back_%d_1vn" );
	cd_hname.push_back( "CD_front_back_%d_nv1" );
	cd_hname.push_back( "CD_front_back_%d_nvn" );
	
	for( i = 0; i < 4; i++ ) {
		
		CD_front_energy[i] = new TH2F(Form("CD_front_energy_%d",i),Form("CD_front_energy_%d",i),16,-0.5,15.5,4096,0,4096);
		CD_front_energy[i]->GetXaxis()->SetTitle("Ring number");
		CD_front_energy[i]->GetYaxis()->SetTitle("Energy particle [adc ch.]");
		CD_back_energy[i] = new TH2F(Form("CD_back_energy_%d",i),Form("CD_back_energy_%d",i),12,-0.5,11.5,4096,0,4096);
		CD_back_energy[i]->GetXaxis()->SetTitle("Strip number");
		CD_back_energy[i]->GetYaxis()->SetTitle("Energy particle [adc ch.]");
		
		CD_front_energy_cal[i] = new TH2F(Form("CD_front_energy_cal_%d",i),Form("CD_front_energy_cal_%d",i),16,-0.5,15.5,PBINS,PMIN,PMAX);
		CD_front_energy_cal[i]->GetXaxis()->SetTitle("Ring number");
		CD_front_energy_cal[i]->GetYaxis()->SetTitle("Energy particle [MeV]");
		CD_back_energy_cal[i] = new TH2F(Form("CD_back_energy_cal_%d",i),Form("CD_back_energy_cal_%d",i),12,-0.5,11.5,PBINS,PMIN,PMAX);
		CD_back_energy_cal[i]->GetXaxis()->SetTitle("Strip number");
		CD_back_energy_cal[i]->GetYaxis()->SetTitle("Energy particle [MeV]");
		
		for( j = 0; j < 4; j++ ) {
			
			CD_front_back[i][j] = new TH2F(Form(cd_hname[j].c_str(),i),Form(cd_hname[j].c_str(),i),PBINS,PMIN,PMAX,PBINS,PMIN,PMAX);
			CD_front_back[i][j]->GetXaxis()->SetTitle("Front Energy particle [MeV]");
			CD_front_back[i][j]->GetYaxis()->SetTitle("Back Energy particle [MeV]");
			
		}
		
	}
	
	gDirectory->cd("/");
	
	// Ionisation chamber
	TDirectory *ic_dir = outfile->mkdir("IC_spec");
	ic_dir->cd();
	dEE = NULL;
	if( ionch ) dEE = new TH2F( "dEE", "ionisation chamber;E_{rest};delta-E", 4096, -0.5, 4095.5, 4096, -0.5, 4095.5 );
	gDirectory->cd("/");
	
	// CD-pad detector, freely triggered
	padE_sum = NULL;
	if( cdpad ) {
		
		TDirectory *pad_dir = outfile->mkdir("PAD_spec");
		pad_dir->cd();
		padE_sum = new TH1F( "padE_sum", "Energy on the PAD detector;Energy [keV];", 4096, -0.5, 4095.5 );
		cd_dEE_sum = new TH2F( "cd_dEE_sum", "dE-E of the CD-PAD;Energy [keV];Energy [keV];", 4096, -2., 16382., 4096, -2., 16382. );
		cd_dEE_anti = new TH2F( "cd_dEE_anti", "dE-E of the CD-PAD not in coincidence with same quadrant;Energy [keV];Energy [keV];", 4096, -2., 16382., 4096, -2., 16382. );
		
		for( i = 0; i < 4; i++ ) {
			
			padE[i] = new TH1F( Form("padE_%d",i), "Energy on the PAD detector;Energy [adc ch.];", 4096, -0.5, 4095.5 );
			padE_cal[i] = new TH1F( Form("padE%d_cal",i), "Energy on the PAD detector;Energy [keV];", ELBINS, ELMIN, ELMAX );
			cd_dEE[i] = new TH2F( Form("cd_dEE%d",i), "dE-E of the CD-PAD;Energy [keV];Energy [keV];", 4096, -2., 16382., 4096, -2., 16382. );
			
		}
		
		gDirectory->cd("/");
		
	}
	// ------------------------------------------------------------------------ //
	
}

#endif