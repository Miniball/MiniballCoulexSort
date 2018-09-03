#ifndef __TREVTS_HH__
#define __TREVTS_HH__

#include "TObject.h"

#include <iostream>
using namespace std;

class trevts : public TObject {
//class trevts {

	public:
	
	// original particle
	float pen;		///< reconstructed particle energy in keV
	float pen_de;	///< reconstructed particle dE in keV
	float pen_e;	///< reconstructed particle E_rest in keV
	int det;		///< quadrant of the T-REX (0-3 = FCD; 4-7 = FBarrel; 8-11 = BBarrel; 12-15 = BCD)
	int ann;		///< annular strip number of CD quadrant
	int sec;		///< sector strip number of CD quadrant
	double time;	///< particle timestamp
	double t1t;		///< T1 timestamp (1 ms before proton impact)
	double sst;		///< supercycle timestamp
	int laser;		///< laser flag
	
	// list of correlated particles
	vector <float> pcor_pen;
	vector <float> pcor_pen_de;
	vector <float> pcor_pen_e;
	vector <int> pcor_det;
	vector <int> pcor_ann;
	vector <int> pcor_sec;
	vector <double> pcor_ptd;
	
	// gamma-rays
	vector <float> gen;		///< gamma-ray energy in keV
	vector <int> cid;		///< Miniball core id (0-23)
	vector <int> sid;		///< Miniball segment id (0 is core 1-6 is segments)
	vector <int> cluid;		///< Miniball cluster id (0-7)
	vector <float> tha;		///< gamma-ray theta angle in radians
	vector <float> pha;		///< gamma-ray phi angle in radians
	vector <float> td;		///< particle-gamma time difference
	vector <int> coin;		///< particle-gamma coincidence flag (0 = prompt; 1 = random; 2 = delayed)
	int pr_hits;			///< number of prompt hits
	int rndm_hits;			///< number of random hits
	int del_hits;			///< number of delayed hits

	// pointers for gamma-rays
	vector <int> pr_ptr;	///< pointer to prompt hits in gamma-ray vector
	vector <int> rndm_ptr;	///< pointer to random hits in gamma-ray vector
	vector <int> del_ptr;	///< pointer to delayed hits in gamma-ray vector

	// setup functions
	void Initialize();
	void SetPen( float en, float de, float erest );
	void SetQuad( int q );
	void SetAnn( int id );
	void SetSec( int id );
	void SetTime( double t );
	void SetT1( double t );
	void SetSS( double t );
	void SetLaser( int l );
	void SetCorPen( float en, float de, float erest );
	void SetCorQuad( int q );
	void SetCorAnn( int id );
	void SetCorSec( int id );
	void SetCorTd( double t );
	void SetGamma( float en, int c, int s, int clu, float th, float ph, double ptd, int co );
	void SearchCoin();
	void CopyData( trevts* src );

	// get functions
	int GetCluid( int nr );
	int GetCid( int nr );
	int GetSid( int nr );
	float GetGen( int nr );
	float GetTheta( int nr );
	float GetPhi( int nr );
	int GetNrGammas();
	float GetPen();
	float GetPde();
	float GetPerest();
	int GetSec();
	int GetAnn();
	int GetQuad();
	
	ClassDef( trevts, 1 );

};

#endif
