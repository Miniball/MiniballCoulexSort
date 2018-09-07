#ifndef __PARTICLEFINDER_HH
#define __PARTICLEFINDER_HH

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

/// A class to search for particle events depending on the
/// configuration of the setup (trex, cdpad, spede, ionch)

class ParticleFinder {
	
public:
	
	ParticleFinder( BuiltEvent *evt );
	virtual ~ParticleFinder();
	
	// Functions to fill particle event vectors
	void FindParticles();
	
	// Particle reconstruction
	unsigned int ReconstructHeavyIons();
	
	// Function to set SubEvent
	inline void SetSubEvent( AdcSubEvent *subevt ){
		subevent = subevt;
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
	
	// Function to set Laser status
	inline void SetLaser( bool l ){
		laser_status = l;
		return;
	};
	
	// Function to set timestamp
	inline void SetTime( long long t ){
		adc_t = t;
		return;
	};
	
	// Function to set module number
	inline void SetModule( unsigned int n ){
		adc_num = n;
		return;
	};

	// Functions to return max particle energies and ids
	inline float GetMaxEf(){ return maxfrontenergy; };
	inline float GetMaxEb(){ return maxbackenergy; };
	inline unsigned short GetMaxNf(){ return maxfrontid; };
	inline unsigned short GetMaxNb(){ return maxbackid; };
	
	// Functions to return reconstructed particle data
	inline unsigned int ReconstructedSize(){ return PEn.size(); };
	inline float GetPEn( unsigned int i ){ return PEn.at(i); };
	inline unsigned int GetQuad( unsigned int i){ return Quad.at(i); };
	inline unsigned int GetNf( unsigned int i){ return Nf.at(i); };
	inline unsigned int GetNb( unsigned int i){ return Nb.at(i); };
	inline long long GetTime( unsigned int i){ return time.at(i); };
	inline bool GetLaser( unsigned int i){ return laser.at(i); };
	
	// Clean up at the start of every event
	void ClearEvt();
	
	// Clean up the event vectors for each adc
	void NextAdc();
	
	// Initialsie histograms
	void InitialiseHistograms();
	
private:
	
	// Built Event
	BuiltEvent *event;
	
	// ADC Sub Event
	AdcSubEvent *subevent;
	
	// Calibration
	Calibration *Cal;
	
	// Output file
	TFile *outfile;

	// Particle event vectors
	vector<float> frontenergy;
	vector<float> backenergy;
	vector<unsigned short> frontid;
	vector<unsigned short> backid;
	
	// Reconstructed Particle arrays
	vector<float> PEn;
	vector<unsigned int> Quad;	// 0: Top, 1: Bottom, 2: Left and 3; Right
	vector<unsigned int> Nf;	// Rings for CDs, Total Energy for Pad or Strips for Barrel
	vector<unsigned int> Nb;	// Strips for CDs and Total Energy for Barrel
	vector<long long> time;
	vector<bool> laser;
	
	// Maximum energy and id
	float maxfrontenergy;
	float maxbackenergy;
	unsigned short maxfrontid;
	unsigned short maxbackid;
	
	// Adc values
	float PartEnergy;
	unsigned int adc_num;
	unsigned int adc_ch;
	unsigned int adc_en;
	long long adc_t;
	bool laser_status;
	
	// Histograms
	// diagnostics
	TH1F *cd_debug;

	// particles
	TH1F *E_part_ch[4][32];
	TH1F *E_part_ch_cal[4][32];
	
	// particle
	TH2F *CD_front_energy[4], *CD_front_energy_cal[4];
	TH2F *CD_back_energy[4], *CD_back_energy_cal[4];
	TH2F *E_f_b[4];
	

	//ClassDef( ParticleFinder, 1 )
	
};

#endif

#ifdef __PARTICLEFINDER_CXX
ParticleFinder::ParticleFinder( BuiltEvent *evt ) {
	
	if( evt != 0 ) event = evt;
	
}

ParticleFinder::~ParticleFinder() {
	
	//cout << "Destructor\n";
	
}

void ParticleFinder::InitialiseHistograms() {
	
	// Particle spectra for every segment
	TDirectory *part_dir = outfile->mkdir("E_part");
	part_dir->cd();
	for( unsigned int i = 0; i < 4; i++ ) {
		
		for( unsigned int j = 0; j < 32; j++ ) {
			
			E_part_ch[i][j] = new TH1F(Form("E_part_%d_%d",i,j),Form("E_part_%d_%d",i,j),4096,-0.5,4095.5);
			E_part_ch[i][j]->GetXaxis()->SetTitle("Energy Particles [Channels]");
			E_part_ch_cal[i][j] = new TH1F(Form("E_part_%d_%d_cal",i,j),Form("E_part_%d_%d_cal",i,j),PBINS,PMIN,PMAX);
			E_part_ch_cal[i][j]->GetXaxis()->SetTitle("Energy Particles [MeV]");
			
		}
		
	}
	
	gDirectory->cd("/");
	
	// particle-spectra
	TDirectory *cd_dir = outfile->mkdir("CD_spec");
	cd_dir->cd();
	
	for( unsigned int i = 0; i < 4; i++ ) {
		
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
		
		E_f_b[i] = new TH2F(Form("E_f_b_%d",i),Form("E_f_b_%d",i),PBINS,PMIN,PMAX,PBINS,PMIN,PMAX);
		E_f_b[i]->GetXaxis()->SetTitle("Front Energy particle [MeV]");
		E_f_b[i]->GetYaxis()->SetTitle("Back Energy particle [MeV]");
		
	}
	
	gDirectory->cd("/");
	
	// Debugging the particle reconstruction
	cd_debug = new TH1F("cd_debug","cd_debug",8,-0.5,7.5);
	cd_debug->GetXaxis()->SetTitle("CD debugging");

	return;
	
}
#endif

