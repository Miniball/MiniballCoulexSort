#ifndef __MBEVTS_HH__
#define __MBEVTS_HH__

#include "TObject.h"

#include <iostream>
using namespace std;

class mbevts : public TObject {
//class mbevts {

	public:
	
	// original gamma-ray
	float gen;	///< gamma-ray energy in keV
	int cid;	///< Miniball core id (0-23)
	int sid;	///< Miniball segment id (0 is core 1-6 is segments)
	int cluid;	///< Miniball cluster id (0-7)
	float tha;	///< gamma-ray theta angle in radians
	float pha;	///< gamma-ray phi angle in radians
	
	// list of correlated gammas
	vector <float> gcor_gen;
	vector <int> gcor_cid;
	vector <int> gcor_sid;
	vector <int> gcor_cluid;
	vector <float> gcor_tha;
	vector <float> gcor_pha;
	vector <float> gcor_gtd;
	
	// particles
	vector <float> pen;		///< particle energy
	vector <double> time;	///< particle timestamp
	vector <double> t1t;	///< T1 timestamp (1 ms before proton impact)
	vector <double> sst;	///< supercycle timestamp
	vector <float> td;		///< particle-gamma time difference
	vector <int> ann;		///< front strip number of CD (0 is outer strip, 15 is inner strip)
	vector <int> sec;		///< back strip number of CD (0-11 going clockwise)
	vector <int> det;		///< CD quadrant (0-3)
	vector <int> coin;		///< particle-gamma coincidence flag (0 = prompt; 1 = random; 2 = delayed)
	int laser;				///< laser flag
	int pr_hits;			///< number of prompt hits
	int rndm_hits;			///< number of random hits
	int del_hits;			///< number of delayed hits

	// pointers for particles
	vector <int> pr_ptr;	///< pointer to prompt hits in particle vector
	vector <int> rndm_ptr;	///< pointer to random hits in particle vector
	vector <int> del_ptr;	///< pointer to delayed hits in particle vector
	int file;

	// setup functions
	void Initialize();
	void SetGen(float en);
	void SetTheta(float theta);
	void SetPhi(float phi);
	void SetCluid(int id);
	void SetCid(int id);
	void SetSid(int id);
	void SetCorGamGen(float en);
	void SetCorGamTheta(float theta);
	void SetCorGamPhi(float phi);
	void SetCorGamCluid(int id);
	void SetCorGamCid(int id);
	void SetCorGamSid(int id);
	void SetCorGamGtd(float td);
	void SetPart(float en, int a, int s, double t, double ss, double t1, float ctd, int co, int quad, int laserflag);
	void SearchCoin();
	void CopyData(mbevts* src);

	// get functions
	int GetCluid();
	int GetCid();
	int GetSid();
	float GetGen();
	float GetTheta();
	float GetPhi();
	float GetPen(int nr);
	int GetNrParts();
	int GetSec(int nr);
	int GetAnn(int nr);
	
	ClassDef(mbevts,3);

};

#endif
