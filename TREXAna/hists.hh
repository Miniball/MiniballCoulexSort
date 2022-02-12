#ifndef hists_hh
#define hists_hh

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"
#include "TObject.h"

#include <iostream>
#include <string>
using namespace std;

// Headers for doppler
#ifndef doppler_hh
# include "doppler.hh"
#endif

using namespace std;

/// A class for making TREX analysis histograms

/// The hists class is used to define all histograms for the analysis.
/// Crucially it defines a set of functions to be called, rather than
/// filling each histogram individually with every call in tr_ana::Loop.
/// This has the advantage of not repeating fill commands and therefore
/// reducing the potential for copy/paste errors.
/// Please try to use these functions as much as is reasonably possible.

class hists {

	// Declare histos : 
	// B = Beam detection 
	// T = Target detection 
	// 2hB = both detected - Beam part
	// 2hT = both detected - Target part
	// p=prompt r=random

	public:

	// Undoppler-corrected gamma spectra
	TH1F *gp, *gr;
	
	// Coincidence matrices
	TH1F *pcor_size;
   
	// Testing
	TH1F *multp, *multr;
	TH1F *GeReject, *GePass, *GeRatio;
	TH2F *GeAng;
	TH2F *GeAng_clu[8];
	TH3F *GeSiAng, *GeSiAng_clu[8];

	// Array of cd angles for histogram bins
	double cd_angles[65];

	// Doppler instance
	doppler dc;

	// functions
	void Initialise( doppler dc_ );

	// fill functions
	void FillSingles( );
	void FillPrompt( );
	void FillRandom( );

	private:

	//ClassDef(hists,1);

};
#endif
