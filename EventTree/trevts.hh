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
	int quad;		///< quadrant of the T-REX (0 = Top; 1 = Left; 2 = Bottom; 3 = Right)
	int nf;			///< front strip number of CD quadrant
	int nb;			///< back strip number of CD quadrant
	int sector;		///< sector of T-REX (0 = FCD; 1 = FBarrel; 2 = BBarrel; 3 = BCD)
	double time;	///< particle timestamp
	double t1t;		///< T1 timestamp (1 ms before proton impact)
	double sst;		///< supercycle timestamp
	int laser;		///< laser flag
	bool first;		///< is it the first correlated event? Useful for not double-counting gammas
	
	// list of correlated particles
	vector <float> pcor_pen;
	vector <float> pcor_pen_de;
	vector <float> pcor_pen_e;
	vector <int> pcor_quad;
	vector <int> pcor_nf;
	vector <int> pcor_nb;
	vector <int> pcor_sector;
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
	trevts();
	~trevts();
	void Initialise();
	void SetPen( float en, float de, float erest );
	void SetQuad( int q );
	void SetNf( int id );
	void SetNb( int id );
	void SetSector( int id );
	void SetTime( double t );
	void SetT1( double t );
	void SetSS( double t );
	void SetLaser( int l );
	void SetFirst( bool f );
	void SetCorPen( float en, float de, float erest );
	void SetCorQuad( int q );
	void SetCorNf( int id );
	void SetCorNb( int id );
	void SetCorSector( int id );
	void SetCorTd( double t );
	void SetGamma( float en, int c, int s, int clu, float th, float ph, double ptd, int co );
	void SearchCoin();
	void CopyData( trevts src );

	// get functions
	inline int GetCluid( int nr ){ return cluid[nr]; };
	inline int GetCid( int nr ){ return cid[nr]; };
	inline int GetSid( int nr ){ return sid[nr]; };
	inline float GetGen( int nr ){ return gen[nr]; };
	inline float GetTheta( int nr ){ return tha[nr]; };
	inline float GetPhi( int nr ){ return pha[nr]; };
	inline int GetCoinc( int nr ){ return coin[nr]; };
	inline int GetNrGammas(){ return gen.size(); };
	inline int GetNrPrompt(){ return pr_hits; };
	inline int GetNrRandom(){ return rndm_hits; };
	inline int GetNrDelayed(){ return del_hits; };
	inline int GetPromptPtr( int nr ){ return pr_ptr[nr]; };
	inline int GetRandomPtr( int nr ){ return rndm_ptr[nr]; };
	inline int GetDelayedPtr( int nr ){ return del_ptr[nr]; };
	inline float GetPen(){ return pen; };
	inline float GetPde(){ return pen_de; };
	inline float GetPerest(){ return pen_e; };
	inline int GetSector(){ return sector; };
	inline int GetNf(){ return nf; };
	inline int GetNb(){ return nb; };
	inline int GetQuad(){ return quad; };
	inline int GetNrCorr(){ return pcor_pen.size(); };
	inline double GetTime(){ return time; };
	inline double GetT1T(){ return t1t; };
	inline double GetSST(){ return sst; };
	inline int GetLaser(){ return laser; };
	inline bool GetFirst(){ return first; };

	ClassDef( trevts, 1 );

};

#endif
