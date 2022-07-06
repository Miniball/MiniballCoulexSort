#ifndef hists_cxx
#define hists_cxx

#define GBINS 2000	// number of bins in gamma spectra
#define GMAX 2000	// maximum energy in gamma spectra
#define PBINS 1200	// number of bins in particle spectra
#define PMAX 12000	// maximum energy in particle spectra
#define TBINS 242	// number of bins in tdiff spectra
#define TMAX 1525	// maximum time in tdiff spectra

#ifndef hist_hh
# include "hists.hh"
#endif

void hists::Initialise( doppler dc_ ) {
	
	/// Initialise all of the histograms that you want to fill
	/// If you add a new one, be careful not to break your memory limit.
	/// This causes unknown behaviour and may not give any errors.
	/// In the best case, you have a segmentation violation and you cannot
	/// work out where it comes from... Try deleting some histograms.
	/// Whatever you do, make sure you declare them in hists.hh file.

	dc = dc_;

	cout << "Initialising histograms...\n";

	string hname, htitle;

	for( int k = 0; k < 17; k++ ) cd_angles[k] = dc.GetPTh(k-0.5,0)*TMath::RadToDeg();
	for( int k = 17; k < 32; k++ ) cd_angles[k] = 0.1+((32.5-k)*dc.GetPTh(15.5,0)*TMath::RadToDeg()+90.*(k-16.5))/16.;
	for( int k = 32; k < 48; k++ ) cd_angles[k] = 90+((k-31.5)*(dc.GetPTh(15.5,3)*TMath::RadToDeg()-90.))/16.+0.1;
	for( int k = 48; k < 65; k++ ) cd_angles[k] = dc.GetPTh(63.5-k,3)*TMath::RadToDeg();

	gp = new TH1F("gp","Prompt gammas;Energy [keV];Counts per 1 keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	gr = new TH1F("gr","Random gammas;Energy [keV];Counts per 1 keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	ggp = new TH2F("ggp","Prompt gammas;Energy [keV];Energy [keV];Counts per 1 keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	ggr = new TH2F("ggr","Random gammas;Energy [keV];Energy [keV];Counts per 1 keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));

	// matrices
	pcor_size = new TH1F("pcor_size","pcor_size;Number of correlated particles;frequency",100,-0.5,99.5);

	// Prompt/Random Particle spectra
	// Total energy
	part = new TH2F("part","Detected particle events, singles;Lab angle [deg];Total Energy [keV]",64,cd_angles,PBINS,0,PMAX);
	part_p = new TH2F("part_p","Detected particle events in prompt coincidence with gamma-ray;Lab angle [deg];Total Energy [keV]",64,cd_angles,PBINS,0,PMAX);
	part_r = new TH2F("part_r","Detected particle events in random coincidence with gamma-ray;Lab angle [deg];Total Energy [keV]",64,cd_angles,PBINS,0,PMAX);
	for( int i = 0; i < 4; i++ ){
		hname = "part_Q" + std::to_string(i);
		part_Q[i] = new TH2F( hname.data(),"Detected particle events, singles;Lab angle [deg];Total Energy [keV]",64,cd_angles,PBINS,0,PMAX);
		hname = "part_p_Q" + std::to_string(i);
		part_p_Q[i] = new TH2F( hname.data(),"Detected particle events in prompt coincidence with gamma-ray;Lab angle [deg];Total Energy [keV]",64,cd_angles,PBINS,0,PMAX);
		hname = "part_r_Q" + std::to_string(i);
		part_r_Q[i] = new TH2F( hname.data(),"Detected particle events in random coincidence with gamma-ray;Lab angle [deg];Total Energy [keV]",64,cd_angles,PBINS,0,PMAX);
	}

	// ∆E
	part_dE = new TH2F("part_dE","Detected particle events, singles;Lab angle [deg];#Delta E [keV]",64,cd_angles,PBINS,0,PMAX);
	part_dE_p = new TH2F("part_dE_p","Detected particle events in prompt coincidence with gamma-ray;Lab angle [deg];#Delta E [keV]",64,cd_angles,PBINS,0,PMAX);
	part_dE_r = new TH2F("part_dE_r","Detected particle events in random coincidence with gamma-ray;Lab angle [deg];#Delta E [keV]",64,cd_angles,PBINS,0,PMAX);
	for( int i = 0; i < 4; i++ ){
		hname = "part_dE_Q" + std::to_string(i);
		part_dE_Q[i] = new TH2F( hname.data(),"Detected particle events, singles;Lab angle [deg];#Delta E [keV]",64,cd_angles,PBINS,0,PMAX);
		hname = "part_dE_p_Q" + std::to_string(i);
		part_dE_p_Q[i] = new TH2F( hname.data(),"Detected particle events in prompt coincidence with gamma-ray;Lab angle [deg];#Delta E [keV]",64,cd_angles,PBINS,0,PMAX);
		hname = "part_dE_r_Q" + std::to_string(i);
		part_dE_r_Q[i] = new TH2F( hname.data(),"Detected particle events in random coincidence with gamma-ray;Lab angle [deg];#Delta E [keV]",64,cd_angles,PBINS,0,PMAX);
	}

	// Erest
	part_Erest = new TH2F("part_Erest","Detected particle events, singles;Lab angle [deg];Erest [keV]",64,cd_angles,PBINS,0,PMAX);
	part_Erest_p = new TH2F("part_Erest_p","Detected particle events in prompt coincidence with gamma-ray;Lab angle [deg];Erest [keV]",64,cd_angles,PBINS,0,PMAX);
	part_Erest_r = new TH2F("part_Erest_r","Detected particle events in random coincidence with gamma-ray;Lab angle [deg];Erest [keV]",64,cd_angles,PBINS,0,PMAX);
	for( int i = 0; i < 4; i++ ){
		hname = "part_Erest_Q" + std::to_string(i);
		part_Erest_Q[i] = new TH2F( hname.data(),"Detected particle events, singles;Lab angle [deg];Erest [keV]",64,cd_angles,PBINS,0,PMAX);
		hname = "part_Erest_p_Q" + std::to_string(i);
		part_Erest_p_Q[i] = new TH2F( hname.data(),"Detected particle events in prompt coincidence with gamma-ray;Lab angle [deg];Erest [keV]",64,cd_angles,PBINS,0,PMAX);
		hname = "part_Erest_r_Q" + std::to_string(i);
		part_Erest_r_Q[i] = new TH2F( hname.data(),"Detected particle events in random coincidence with gamma-ray;Lab angle [deg];Erest [keV]",64,cd_angles,PBINS,0,PMAX);
	}


	// Testing
	multp = new TH1F("multp","Prompt gamma multiplicity;Number of particles;Number of events",16,-0.5,15.5);
	multr = new TH1F("multr","Random gamma multiplicity;Number of particles;Number of events",16,-0.5,15.5);
	GeAng = new TH2F("GeAng","Detector angles;#theta;#phi",45,0,180,90,0,359);
	GeSiAng = new TH3F("GeSiAng","Ge and Si coincidences angles;Ge #theta;Si #theta;#phi",45,0,180,30,0,60,270,-720,360);


	return;

}

void hists::FillSingles( trevts *evt ) {

	// Fill some particle spectra
	float PTh = dc.GetPTh( evt->GetNf(), evt->GetSector() );
	part->Fill( PTh * TMath::RadToDeg(), evt->GetPen() );
	part_Q[evt->GetQuad()]->Fill( PTh * TMath::RadToDeg(), evt->GetPen() );
	part_dE->Fill( PTh * TMath::RadToDeg(), evt->GetPde() );
	part_dE_Q[evt->GetQuad()]->Fill( PTh * TMath::RadToDeg(), evt->GetPde() );
	part_Erest->Fill( PTh * TMath::RadToDeg(), evt->GetPerest() );
	part_Erest_Q[evt->GetQuad()]->Fill( PTh * TMath::RadToDeg(), evt->GetPerest() );

	return;

}

void hists::FillPrompt( trevts *evt ) {

	// Fill gammas, but only if it's the first particle event of
	// the correlation, or we double count them
	if( evt->GetFirst() ) {
		
		//---- TODO: Here we need to add Doppler corrected gamma-ray spectra
		//---- TODO: But first we need the kinematic reconstruction implementing in doppler class

		// All gammas
		for( unsigned int i = 0; i < evt->GetNrPrompt(); ++i ){
		
			// Prompt only
			gp->Fill( evt->GetGen( evt->GetPromptPtr(i) ) );
			
			// Second gamma
			for( unsigned int j = i+1; j < evt->GetNrPrompt(); ++j ){
				
				ggp->Fill( evt->GetGen( evt->GetPromptPtr(i) ),
						   evt->GetGen( evt->GetPromptPtr(j) ) );
						
				ggp->Fill( evt->GetGen( evt->GetPromptPtr(j) ),
						   evt->GetGen( evt->GetPromptPtr(i) ) );
						
			} // j++
			
		} // i++
	
	} // first
	
	
	
	// Fill some particle spectra
	float PTh = dc.GetPTh( evt->GetNf(), evt->GetSector() );
	part_p->Fill( PTh * TMath::RadToDeg(), evt->GetPen() );
	part_p_Q[evt->GetQuad()]->Fill( PTh * TMath::RadToDeg(), evt->GetPen() );
	part_dE_p->Fill( PTh * TMath::RadToDeg(), evt->GetPde() );
	part_dE_p_Q[evt->GetQuad()]->Fill( PTh * TMath::RadToDeg(), evt->GetPde() );
	part_Erest_p->Fill( PTh * TMath::RadToDeg(), evt->GetPerest() );
	part_Erest_p_Q[evt->GetQuad()]->Fill( PTh * TMath::RadToDeg(), evt->GetPerest() );

	
	//---- TODO: Here we can loop over the gammas, apply Doppler correction then gate on them
	//---- TODO: to produce gated particle spectra. Similarly vice versa with particle gates.
	
		
	return;

}

void hists::FillRandom( trevts *evt ) {

	// Fill gammas, but only if it's the first particle event of
	// the correlation, or we double count them
	if( evt->GetFirst() ) {
		
		// All gammas
		for( unsigned int i = 0; i < evt->GetNrRandom(); ++i ){
			
			// Random only
			gr->Fill( evt->GetGen( evt->GetRandomPtr(i) ) );
			
			// Second gamma
			for( unsigned int j = i+1; j < evt->GetNrRandom(); ++j ){
				
				ggr->Fill( evt->GetGen( evt->GetRandomPtr(i) ),
						   evt->GetGen( evt->GetRandomPtr(j) ) );
					
				ggr->Fill( evt->GetGen( evt->GetRandomPtr(j) ),
						   evt->GetGen( evt->GetRandomPtr(i) ) );
					
			} // j++
			
		} // i++
		
	} // first

	
	// Fill some particle spectra
	float PTh = dc.GetPTh( evt->GetNf(), evt->GetSector() );
	part_r->Fill( PTh * TMath::RadToDeg(), evt->GetPen() );
	part_r_Q[evt->GetQuad()]->Fill( PTh * TMath::RadToDeg(), evt->GetPen() );
	part_dE_r->Fill( PTh * TMath::RadToDeg(), evt->GetPde() );
	part_dE_r_Q[evt->GetQuad()]->Fill( PTh * TMath::RadToDeg(), evt->GetPde() );
	part_Erest_r->Fill( PTh * TMath::RadToDeg(), evt->GetPerest() );
	part_Erest_r_Q[evt->GetQuad()]->Fill( PTh * TMath::RadToDeg(), evt->GetPerest() );

	return;

}


#endif
