#ifndef __MBEVTS_HH__
#define __MBEVTS_HH__

#include "TObject.h"

#include <iostream>
using namespace std;

class mbevts : public TObject {
//class mbevts {

	public:
	
	// original gamma-ray
	float gen;
	int cid;
	int sid;
	int cluid;
	float tha;
	float pha;
	
	// list of correlated gammas
	vector <float> gcor_gen;
	vector <int> gcor_cid;
	vector <int> gcor_sid;
	vector <int> gcor_cluid;
	vector <float> gcor_tha;
	vector <float> gcor_pha;
	vector <float> gcor_gtd;
	
	// particles
	vector <float> pen;
	vector <float> time;
	vector <double> sst;
	vector <float> td;
	vector <int> ann;
	vector <int> sec;
	vector <int> det;
	vector <int> coin;
	int laser;
	int pr_hits;
	int rndm_hits;
	int del_hits;

	// pointers for particles
	vector <int> pr_ptr;
	vector <int> rndm_ptr;
	vector <int> del_ptr;
	int file; 

	// setup functions
	void Initialize();
	void SetGen(double en);
	void SetTheta(double theta);
	void SetPhi(double phi);
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
	void SetPart(float en, int a, int s, float t, double ss, float ctd, int co, int quad, int laserflag);
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
