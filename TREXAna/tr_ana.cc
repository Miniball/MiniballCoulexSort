// Sorting for Miniball Transfer data and building histograms
// Cuts are done in doppler.hh using function, doppler::Cut()
// This is really just a placeholder and example...
// It needs expanding drastically if it is to be useful
// Liam Gaffney (liam.gaffney@liverpool.ac.uk) - 12/02/2021

#define tr_ana_cxx

#ifndef hist_hh
# include "hists.hh"
#endif
#ifndef tr_ana_hh
# include "tr_ana.hh"
#endif

void tr_ana::Loop( string outputfilename ) {
	
	if( fChain == 0 ) return;

	// Output file name	
	TFile *out = new TFile( outputfilename.c_str(), "RECREATE" );

	// Create doppler instance and set experimental parameters
	doppler dc;
	dc.ExpDefs( Zb, Ab, Zt, At, Eb, Ex, thick, depth, cddist, cdoffset,
					deadlayer, contaminant, spededist, srim, usekin );

	// Create stopping power curves from the srim output files
	// Comment out to use the default parameters in doppler.hh
	// stoppingpowers( BT, TT, BA, TA )
	if( !dc.stoppingpowers( false, false, false, false ) ) return;

	// Test if it's an electron or gamma
	bool electron;

	// Include errors on histograms (required for correct bg subtraction)
	TH1::SetDefaultSumw2();

	// Declare the histograms here and initialise!
	hists h;
	h.Initialise( dc );

	// Loop over events
	cout << "Looping over events...\n";
	Int_t nbytes = 0, nbs = 0;
	Int_t skipFactor = 1;
	for( Long64_t jentry=0; jentry<fChain->GetEntries()/skipFactor; jentry++ ) {	

		Long64_t ientry = LoadTree(jentry);
		if( ientry < 0 ) break;

		nbs = GetEntry(jentry);   nbytes += nbs;

		if( jentry%5000==0 ){
			cout << " " << jentry << "/" << fChain->GetEntries() << "  (" << jentry*100./fChain->GetEntries() << "%)    \r";
			cout << flush;
		}

		// Gamma multiplicity
		if( evt->GetFirst() ) {
		
			h.multp->Fill( evt->GetNrPrompt() );
			h.multr->Fill( evt->GetNrRandom() );
		
			// Loop over gamma counter
			for( unsigned int i = 0; i < evt->GetNrGammas(); i++ ){

				// Germanium angles
				h.GeAng->Fill( evt->GetTheta(i)*TMath::RadToDeg(), evt->GetPhi(i)*TMath::RadToDeg() );

				// Germanium angles vs. Silicon angles
				h.GeSiAng->Fill( evt->GetTheta(i)*TMath::RadToDeg(), dc.GetPTh(evt->GetNf(),evt->GetSector())*TMath::RadToDeg(), (evt->GetPhi(i)-dc.GetPPhi(evt->GetQuad(),evt->GetNb(),evt->GetSector()))*TMath::RadToDeg() );
				
			} // END - Loop over gamma counter

		}


		// Check size of particle multiplicity
		h.pcor_size->Fill( evt->GetNrCorr() );

		
		// Fill singles particle spectra, with gamma or no gamma.
		h.FillSingles( evt );

		// Fill conditioned spectra
		// Condition on gamma detection
		if( evt->GetNrPrompt() > 0 && evt->GetNrRandom() == 0 )
			h.FillPrompt( evt );

		else if( evt->GetNrPrompt() == 0 && evt->GetNrRandom() > 0 )
			h.FillRandom( evt );
		
		else {} // what if there are prompt and random gammas?

	} // for (Long64_t jentry=0; jentry<fChain->GetEntries();jentry++)
	

	out->Write();

}
