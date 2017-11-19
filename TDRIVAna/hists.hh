#ifndef hists_hh
#define hists_hh

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"
#include "TObject.h"

#include <iostream>
#include <string>
#include <map>
using namespace std;

// Headers for doppler
#ifndef doppler_hh
# include "doppler.hh"
#endif

#define CDBINS 17
#define GEBINS 50
#define PHIBINS 61

#ifdef PHICAL
# define PHI_STEP_WIDTH 1 
# define PHI_NSTEPS 21 // try to use an odd number
#endif

using namespace std;

class hists {

	// Declare histos : 
	// B = Beam detection 
	// T = Target detection 
	// 2hB = both detected - Beam part
	// 2hT = both detected - Target part
	// p=prompt r=random

	public:

	// Undoppler-corrected gamma spectra
	TH1F *p, *r, *pmr;
	TH1F *pB, *rB, *pBmrB;
	TH1F *pT, *rT, *pTmrT;
	TH1F *p_1B, *r_1B, *p_1T, *r_1T, *p_2h, *r_2h;

	// Coincidence matrices
	TH2F *gg;
	TH2F *gg_dcT, *gg_dcB;
	TH1F *gg_td;
	TH1F *gcor_size;

	// Calibrate phi rotation of cd
#ifdef PHICAL
	TH1F *phical_dcB[PHI_NSTEPS], *phical_dcT[PHI_NSTEPS];
#endif
	 
	// Background subtracted, Doppler corrected gamma spectra
	TH1F *B_dcB, *B_dcT, *B_1hdcB, *B_1hdcT;
	TH1F *T_dcT, *T_dcB, *T_1hdcT, *T_1hdcB;
	TH1F *gam_dcB, *gam_dcT, *T_2hdcB, *T_2hdcT;
   
	// Prompt/Random Particle spectra
	TH2F *part, *part1h, *part2h, *partQ[4], *part_ann;
	TH2F *Bh, *Th, *B1h, *T1h, *B2h, *T2h;
	TH2F *Bhhigh, *Bhlow, *Thhigh, *Thlow;
	TH1F *target_ev, *target_1pev, *target_2pev;
#ifdef TWOPART
	TH2F *BT[16], *TB[16];
#endif

	// Second hit - simulated etc.
	TH2F *Bsim, *Tsim;
	TH2F *en2hit;
	TH1F *sum2hit;

	// Testing
	TH1F *multp, *multr;
	TH1F *GeReject, *GePass, *GeRatio;
	TH2F *GeAng;
	//TH3F *GeSiAng;

	// gamma - particle time difference
	TH1F *tdiff;

	// particle - particle time difference
	TH1F *tppdiff;
	TH2F *tpp;
	TH1F *tQQ[2];

	// R(t) Function histograms
	TH1F *rthist[24][7], *rtgam[24][7];
	TH1F *rthist_th[3][7], *rtgam_th[3][7];
	float deltaPhi[3][7];
	float groupTheta[3];
	float groupThWidth;

    // Particle gamma angular correlations
//	TH2S *gamma_particle_ang[24];
//    TH1S *coreid;

	// Variables to be set in g_clx.C via Set_xxx functions
	float ppwin;
	int maxrecoil;
	int minrecoil;

	// Array of cd angles for histogram bins
	double cd_angles[CDBINS];
	double ge_angles[GEBINS];
	double phi_angles[PHIBINS];
	double cdphi_angles[49];

	// Doppler instance
	doppler dc;

	// functions
	void Initialise( doppler dc_, double core_theta[24], double core_phi[24] );
	void Set_ppwin( float user_ppwin );
	void Set_maxrecoil( int user_maxrecoil );
	void Set_minrecoil( int user_minrecoil );

	// fill functions
	void Fill1h( float GEn, float GTh, float GPh, int cid, vector <float> GCor_GEn, vector <float> GCor_GTh,
					vector <float> GCor_GPh, vector <int> GCor_GCluID, vector <float> GCor_Gtd,
					float PEn, Int_t Pann, Int_t Psec, Int_t Pquad, float weight=1.0 );
	void Fill2h( float GEn, float GTh, float GPh, int cid, vector <float> GCor_GEn, vector <float> GCor_GTh,
					vector <float> GCor_GPh, vector <int> GCor_GCluID, vector <float> GCor_Gtd,
					vector <float> PEn, vector<int> Pann, vector<int> Psec, vector<int> Pquad, vector<int> Pptr,
					vector <float> td, float weight=1.0 );
	void FillGam1h( float GEn, float GTh, float GPh, int cid, float PEn, Int_t Pann,
					Int_t Psec, Int_t Pquad, Int_t cut, float weight=1.0 );
	void FillGam2h( float GEn, float GTh, float GPh, int cid, vector <float> PEn, vector<int> Pann,
					vector<int> Psec, vector<int> Pquad, Int_t Bptr, Int_t Tptr, float weight=1.0 );
	void FillGamGam1h( float GEn, float GTh, float GPh, vector <float> GCor_GEn, vector <float> GCor_GTh,
					  vector <float> GCor_GPh, vector <int> GCor_GCluID, vector <float> GCor_Gtd,
					  float PEn, Int_t Pann, Int_t Psec, Int_t Pquad, Int_t cut, float weight=1.0 );
	void FillGamGam2h( float GEn, float GTh, float GPh, vector <float> GCor_GEn, vector <float> GCor_GTh,
					  vector <float> GCor_GPh, vector <int> GCor_GCluID, vector <float> GCor_Gtd, vector <float> PEn,
					  vector<int> Pann, vector<int> Psec, vector<int> Pquad, Int_t Bptr, Int_t Tptr, float weight=1.0 );
	void FillPar1h( float PEn, Int_t Pann, Int_t Psec, Int_t Pquad, Int_t cut, float weight=1.0 );
	void FillPar2h( vector <float> PEn, vector<int> Pann, vector<int> Psec, vector<int> Pquad, Int_t Bptr,
						Int_t Tptr, float weight=1.0 );
	void RtFunc( float GEn, float GTh, float GPh, int cid, float BEn, int Bann, int Bsec, int Bquad );
	void PhiCalHists( float GEn, float GTh, float GPh, float PEn, Int_t Pann,
						Int_t Psec, Int_t Pquad, Int_t cut, float weight=1.0 );
	void AddSpectra( float bg_frac );
		
	private:

	//ClassDef(hists,1);

};
#endif
