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

/// A class to peform the addback for Miniball
/// Actually, it constructs gamma-rays from DGF
/// data and the addback is an option...

class AddBack {
	
public:
	
	// Functions to fill gamma-ray vectors
	void MakeGammaRays( bool addback );
	
	AddBack( BuiltEvent *evt );
	virtual ~AddBack();
	
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
	inline vector<float> GetGenArray(){ return gen_array; };
	inline vector<long long> GetGtdArray(){ return gtd_array; };
	inline vector<unsigned short> GetCluArray(){ return clu_array; };
	inline vector<unsigned short> GetCidArray(){ return cid_array; };
	inline vector<unsigned short> GetSidArray(){ return sid_array; };
	inline vector<float> GetSenArray(){ return sen_array; };
	
	// Clean up at the start of every event
	void ClearEvt();
	
	// Initialsie histograms
	void InitialiseHistograms();
	
private:
	
	// Event
	BuiltEvent *event;
	
	// Calibration
	Calibration *Cal;
	
	// Output file
	TFile *outfile;

	// Temporary variables
	int dgf_num;
	int dgf_num2;
	int dgf_ch;
	int dgf_ch2;
	int dgf_en;
	int dgf_en2;
	long long dgf_t;
	long long dgf_t2;
	
	// Gamma-ray energies
	float GammaEnergy;
	float GammaEnergy2;
	
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
	bool ab_evt;
	unsigned short ab_mul;
	bool veto_gamma;
	unsigned int gSeg;
	vector< unsigned int > dead_segments;
	
	// Histograms
	TH1F *E_gam, *E_gam_seg[8][3][7], *E_gam_seg_cal[8][3][7];
	TH1F *E_BeamDump[2], *T_BeamDump[2], *tdiff_BD;
	TH2F *bd_bd;
	TH1F *hABmult;


	//ClassDef( AddBack, 1 )
	
};

#endif

#ifdef __ADDBACK_CXX
AddBack::AddBack( BuiltEvent *evt ) {
	
	if( evt != 0 ) event = evt;
	
}

AddBack::~AddBack() {
	
	cout << "Destrcutor\n";
	
}

void AddBack::InitialiseHistograms() {
	
	// Total gamma - no Doppler correction
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

}
#endif

