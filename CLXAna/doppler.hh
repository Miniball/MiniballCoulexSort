#ifndef doppler_hh
#define doppler_hh

#include "TRandom.h"
#include "TRandom3.h"
#include "TCutG.h"
#include "TGaxis.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMath.h"
#include "TObject.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

// Experimental definitions
//#ifndef ExpDefs_hh
//# include "ExpDefs.hh"
//#endif

/// The doppler class has a whole suite of functions for performing the
/// Doppler correction, including: calculating kinematics, energy loss,
/// particle gating, gamma-ray angles, particle angles, etc.

class doppler : public TObject {

	public:

	void	ExpDefs( int Zb_, float Ab_, int Zt_, float At_, float Eb_, float Ex_, float thick_,
						float depth_, float cddist_, float cdoffset_, float deadlayer_,
						 float contaminant_, float spededist_, TCutG *Bcut_, TCutG *Tcut_ );
	int		Cut( float PEn, float anno, int quad );
	int		Cut_2p( float PEn1, float anno1, int quad1, float PEn2, float anno2, int quad2 );
	bool	CutG_en2hit( float BEn, float TEn );
	int		GetZb();
	int		GetZt();
	float	GetAb();
	float	GetAt();
	float	GetCDOffset();
	float	GetCDDeadLayer();
	float	GetSpedeDist();
	float	GetPTh( float anno);
	float	GetPPhi( int quad, int seg );
	float	GetPPhi( int quad, int seg, float offset );
	float	GetTTh( float Banno, float BEn );
	float	GetBTh( float Tanno );
	float	GetQPhi( int quad, int seg );
	float	GetTEn( float BEn, float Banno );
	float	GetBEn( float TEn, float Tanno );
	float	GetELoss( float Ei, float dist, int opt, string combo );
	float	GammaAng( float PTh, float PPhi, float GTh, float GPhi );
	float	Beta( float Ek, float m );
	float	DC( float PEn, float PTh, float PPhi, float GTh, float GPhi, float A );
	float	DC_elec( float een, float PEn, float PTh, float PPhi, float GTh, float GPhi, float A );
	bool	stoppingpowers( bool BT, bool TT, bool BA, bool TA, bool BC, bool TC );
	bool	stoppingpowers( string opt );

	static string	convertInt( int number );
	static string	convertFloat( float number );
	
	inline float u_mass() { return 931494.0038; };
	
	private:
	static TRandom3	rand;
	TGraph *gSP[6];
	static string	gElName[110];
	static float	gates[64][3];
	int		Zb, Zt;
	float	Ab, At;
	float	Eb, Ex, thick, depth;
	float	cddist, cdoffset;
	float	deadlayer;
	float	contaminant;
	float	spededist;
	float	bg_frac;
	TCutG	*Bcut, *Tcut;
	
	ClassDef(doppler,1);

};
#endif
