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
	Int_t skipFactor =1;
	for( Long64_t jentry=0; jentry<fChain->GetEntries()/skipFactor; jentry++ ) {	

		Long64_t ientry = LoadTree(jentry);
		
		if( ientry < 0 ) break;

		nbs = fChain->GetEntry(jentry);   nbytes += nbs;

		if( jentry%5000==0 ){
			cout << " " << jentry << "/" << fChain->GetEntries() << "  (" << jentry*100./fChain->GetEntries() << "%)    \r";
			cout << flush;
		}

		// Paricle multiplicity
		if( pr_hits != 0 ) h.multp->Fill( pr_hits );
		else if( rndm_hits != 0 )  h.multr->Fill( rndm_hits );


		// Loop over gamma counter
		for( unsigned int i = 0; i < gen.size(); i++ ){

			// Germanium angles
			h.GeAng->Fill( tha[i]*TMath::RadToDeg(), pha[i]*TMath::RadToDeg() );
			h.GeAng_clu[cluid[i]]->Fill( tha[i]*TMath::RadToDeg(), pha[i]*TMath::RadToDeg() );

			// Germanium angles vs. Silicon angles
			h.GeSiAng->Fill( tha[i]*TMath::RadToDeg(), dc.GetPTh(nf,sector)*TMath::RadToDeg(), (pha[i]-dc.GetPPhi(quad,nb,sector))*TMath::RadToDeg() );
			h.GeSiAng_clu[cluid[i]]->Fill( tha[i]*TMath::RadToDeg(), dc.GetPTh(nf,sector)*TMath::RadToDeg(), (pha[i]-dc.GetPPhi(quad,nb,sector))*TMath::RadToDeg() );
			
		} // END - Loop over particle counter

		// Check size of particle multiplicity
		h.pcor_size->Fill( pcor_pen.size() );

		// Fill conditioned spectra

		// Condition on gamma detection
		if( pr_hits==0 && rndm_hits==0 )
			h.FillSingles();

		else if( pr_hits>0 && rndm_hits==0 )
			h.FillPrompt();

		else if( pr_hits==0 && rndm_hits>0 )
			h.FillRandom();
		
		else {} // what if there are prompt and random gammas?

	} // for (Long64_t jentry=0; jentry<fChain->GetEntries();jentry++)
	

	out->Write();

}
