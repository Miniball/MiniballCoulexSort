#ifndef __ADDBACK_HH
#define __ADDBACK_HH

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>

#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TFile.h"

// Header for input tree
#ifndef EVENT_HH
# include "BuiltEvent.hh"
#endif

// Header for calibration
#ifndef __CALIBRATION_HH
# include "Calibration.hh"
#endif


using namespace std;

/// A class to perform the addback for Miniball
/// Actually, it constructs gamma-rays from DGF
/// data and the addback is an option...

class AddBack {
	
public:
	
	AddBack();
	virtual ~AddBack();
	
	// Functions to fill gamma-ray vectors
	void MakeGammaRays( bool addback, bool reject );
	void MakeElectrons();
	
	// Function to set Event
	inline void SetEvent( BuiltEvent *evt ){
		event = evt;
		return;
	};
	
	// Function to set SubEvent for SPEDE
	inline void SetSubEvent( AdcSubEvent *subevt ){
		subevent = subevt;
		return;
	};
	
	// Function to set timestamp for SPEDE
	inline void SetTime( long long t ){
		adc_t = t;
		return;
	};
	
	// Function to set module number for SPEDE
	inline void SetModule( unsigned int n ){
		adc_num = n;
		return;
	};
	
	// Function to set calibration
	inline void SetCalibration( Calibration *_Cal ){
		Cal = _Cal;
		return;
	};
	
	// Function to set the output file
	inline void SetOutputFile( TFile *_outfile ){
		outfile = _outfile;
		return;
	};
	
	// Functions to return gamma-ray vectors
	inline unsigned int GetGenSize(){ return gen_array.size(); };
	inline float GetGen( unsigned int i ){ return gen_array.at(i); };
	inline long long GetGtd( unsigned int i ){ return gtd_array.at(i); };
	inline unsigned short GetClu( unsigned int i ){ return clu_array.at(i); };
	inline unsigned short GetCid( unsigned int i ){ return cid_array.at(i); };
	inline unsigned short GetSid( unsigned int i ){ return sid_array.at(i); };
	inline float GetSen( unsigned int i ){ return sen_array.at(i); };
	
	// Clean up at the start of every event
	void ClearEvt();
	
	// Initialise histograms
	void InitialiseHistograms();
	
	// Initialise variables
	void InitialiseVariables();
	
private:
	
	// Event
	BuiltEvent *event;
	
	// ADC Sub Event (for SPEDE or PAD)
	AdcSubEvent *subevent;
	
	// Calibration
	Calibration *Cal;
	
	// Output file
	TFile *outfile;

	// DGF values
	int dgf_num;
	int dgf_num2;
	int dgf_ch;
	int dgf_ch2;
	int dgf_en;
	int dgf_en2;
	long long dgf_t;
	long long dgf_t2;

	// Adc values
	unsigned int adc_num;
	unsigned int adc_ch;
	unsigned int adc_en;
	long long adc_t;
	
	// Gamma-ray and electron energies
	float GammaEnergy;
	float GammaEnergy2;
	float ElectronEnergy;

	// Maximum segment energy determination
	float MaxSegEnergy;
	int MaxSegClu;
	int MaxSegCore;
	int MaxSegId;
	
	// Gamma-ray vectors
	vector<float> gen_array;
	vector<long long> gtd_array;
	vector<unsigned short> clu_array;
	vector<unsigned short> cid_array;
	vector<unsigned short> sid_array;
	vector<float> sen_array;
	
	// Addback and veto flags
	bool ab_evt, reject_evt;
	unsigned short ab_mul;
	vector<bool> ab_array;
	bool veto_gamma;
	unsigned int gSeg;
	vector< unsigned int > dead_segments;
	
	// Histograms
	TH1F *E_gam, *E_gam_seg[8][3][7], *E_gam_seg_cal[8][3][7];
	TH1F *E_BeamDump[2], *T_BeamDump[2], *tdiff_BD;
	TH2F *bd_bd;
	TH1F *hABmult;

	// spede
	TH1F *E_spede, *E_spede_seg[24], *E_spede_seg_cal[24];
	

	//ClassDef( AddBack, 1 )
	
};

#endif

#ifdef __ADDBACK_CXX
AddBack::AddBack() {
	
}

AddBack::~AddBack() {
	
	//cout << "Destructor\n";
	
}

void AddBack::InitialiseVariables() {
	
	// Crap segments list (i.e. those that need to be vetoed)
	// Counting from 0 to 167, i.e. including cores - clu*21 + core*7 + seg
	dead_segments.push_back( 106 ); // clu 5, core 0, seg 1 = 18A1
	dead_segments.push_back( 107 ); // clu 5, core 0, seg 2 = 18A2
	
	return;

}

void AddBack::InitialiseHistograms() {
	
	// Total gamma - addback if option is ticked
	E_gam = new TH1F("E_gam","E_gam",GBINS,GMIN,GMAX);
	E_gam->GetXaxis()->SetTitle("Energy Gamma Rays [Channels]");
	
	// Gamma spectra for every segment - no Doppler correction
	TDirectory *gam_dir = outfile->mkdir("E_gam_seg");
	gam_dir->cd();
	for( unsigned int i = 0; i < 8; i++ ) {
		
		for( unsigned int j = 0; j < 3; j++ ) {
			
			for( unsigned int k = 0; k < 7; k++ ) {
				
				E_gam_seg[i][j][k] = new TH1F(Form("E_gam_%d_%d_%d",i,j,k),Form("E_gam_%d_%d_%d",i,j,k),16384,-0.5,65535.5);
				E_gam_seg[i][j][k]->GetXaxis()->SetTitle("Energy Gamma Rays [Channels]");
				E_gam_seg_cal[i][j][k] = new TH1F(Form("E_gam_%d_%d_%d_cal",i,j,k),Form("E_gam_%d_%d_%d_cal",i,j,k),GBINS,GMIN,GMAX);
				E_gam_seg_cal[i][j][k]->GetXaxis()->SetTitle("Energy Gamma Rays [keV]");
				
			}
			
		}
		
	}
	
	gDirectory->cd("/");

	// Diagnostics
	hABmult = new TH1F("hABmult","hABmult",4,-0.5,3.5);
	hABmult->GetXaxis()->SetTitle("Addback multiplicity");

	// Beam dump
	tdiff_BD = new TH1F( "tdiff_BD", "tdiff_BD", 400, -200, 200);
	bd_bd = new TH2F( "bd_bd", "bd_bd", GBINS,GMIN,GMAX, GBINS,GMIN,GMAX );
	for( unsigned int i = 0; i < 2; i++ ) {
		
		E_BeamDump[i] = new TH1F( Form("E_BeamDump_%d",i), Form("E_BeamDump_%d",i),GBINS,GMIN,GMAX);
		E_BeamDump[i]->GetXaxis()->SetTitle("Energy of the Beam Dump [keV]");
		T_BeamDump[i] = new TH1F( Form("T_BeamDump_%d",i), Form("T_BeamDump_%d",i),7200,0,7200);
		T_BeamDump[i]->GetXaxis()->SetTitle("Time of the Beam Dump [s]");
		
	}

	// Spede
	TDirectory *spede_dir = outfile->mkdir("E_spede_seg");
	spede_dir->cd();
	E_spede = new TH1F("E_spede","E_spede",ELBINS,ELMIN,ELMAX);
	E_spede->GetXaxis()->SetTitle("Energy Electrons [Channels]");
	for( unsigned int i = 0; i < 24; i++ ) {
		
		E_spede_seg[i] = new TH1F(Form("E_spede_%d",i),Form("E_spede_%d",i),16384,-0.5,65535.5);
		E_spede_seg[i]->GetXaxis()->SetTitle("Energy Electrons [Channels]");
		E_spede_seg_cal[i] = new TH1F(Form("E_spede_%d_cal",i),Form("E_spede_%d_cal",i),ELBINS,ELMIN,ELMAX);
		E_spede_seg_cal[i]->GetXaxis()->SetTitle("Energy Electrons [keV]");
		
	}
	
	gDirectory->cd("/");
	
}
#endif

