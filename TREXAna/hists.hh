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

// Headers for trevts
#ifndef __TREVTS_HH__
# include "trevts.hh"
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
	TH2F *ggp, *ggr;

	// Coincidence matrices
	TH1F *pcor_size;
	
	// Prompt/Random Particle spectra
	TH2F *part, *part_p, *part_r;
	TH2F *part_dE, *part_dE_p, *part_dE_r;
	TH2F *part_Erest, *part_Erest_p, *part_Erest_r;
	TH2F *part_Q[4], *part_p_Q[4], *part_r_Q[4];
	TH2F *part_dE_Q[4], *part_dE_p_Q[4], *part_dE_r_Q[4];
	TH2F *part_Erest_Q[4], *part_Erest_p_Q[4], *part_Erest_r_Q[4];

   
	// Testing
	TH1F *multp, *multr;
	TH2F *GeAng;
	TH3F *GeSiAng;

	// Array of cd angles for histogram bins
	double cd_angles[65];

	// Doppler instance
	doppler dc;
	
	// functions
	void Initialise( doppler dc_ );

	// fill functions
	void FillSingles( trevts *evt );
	void FillPrompt( trevts *evt );
	void FillRandom( trevts *evt );

	private:

	//ClassDef(hists,1);

};
#endif
