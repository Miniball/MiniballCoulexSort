#ifndef hists_cxx
#define hists_cxx

#define GBINS 4000
#define GRANGE 4000
#define GMIN -1.0*GRANGE/GBINS
#define GMAX GRANGE+GMIN

#define PBINS 800
#define PRANGE 800
#define PMIN -1.0*PRANGE/PBINS
#define PMAX PRANGE+PMIN

#define TBINS 242	// number of bins in tdiff spectra
#define TRANGE 800
#define TMIN -1.0*TRANGE/TBINS
#define TMAX TRANGE+TMIN

#ifndef hist_hh
# include "hists.hh"
#endif

void hists::Initialise( doppler dc_ ) {

	cout << "Initialising histograms...\n";

	dc = dc_;

	string hname, htitle;

	for( int k = 0; k < 17; k++ ) cd_angles[k] = dc.GetPTh(15.5-k)*TMath::RadToDeg(); 
	for( int k = 0; k < 91; k++ ) ge_angles[k] = 45. + k*1.0; 
	for( int k = 0; k < 181; k++ ) phi_angles[k] = -180. + k*2.0; 

	p = new TH1F("p","Prompt gammas;Energy [keV];Counts per 1 keV",GBINS, GMIN, GMAX );
	r = new TH1F("r","Random gammas;Energy [keV];Counts per 1 keV",GBINS, GMIN, GMAX );
	pmr = new TH1F("pmr",hname.c_str(),GBINS, GMIN, GMAX );
	pB = new TH1F("pB","Prompt gammas gated on beam",GBINS, GMIN, GMAX );
	rB = new TH1F("rB","Random gammas gated on beam",GBINS, GMIN, GMAX );
	pT = new TH1F("pT","Prompt gammas gated on target",GBINS, GMIN, GMAX );
	rT = new TH1F("rT","Random gammas gated on target",GBINS, GMIN, GMAX );
	pBmrB = new TH1F("pBmrB","Prompt gated gammas on beam minus random gated on beam",GBINS, GMIN, GMAX );
	pTmrT = new TH1F("pTmrT","Prompt gated gammas on target minus random gated on target",GBINS, GMIN, GMAX );
	p_1B = new TH1F("p_1B","#gamma-rays in prompt coincidence with 1-beam particle",GBINS, GMIN, GMAX );
	r_1B = new TH1F("r_1B","#gamma-rays in random coincidence with 1-beam particle",GBINS, GMIN, GMAX );
	p_1T = new TH1F("p_1T","#gamma-rays in prompt coincidence with 1-target particle",GBINS, GMIN, GMAX );
	r_1T = new TH1F("r_1T","#gamma-rays in random coincidence with 1-target particle",GBINS, GMIN, GMAX );
	p_2h = new TH1F("p_2h","#gamma-rays in prompt coincidence with 2-particles",GBINS, GMIN, GMAX );
	r_2h = new TH1F("r_2h","#gamma-rays in random coincidence with 2-particles",GBINS, GMIN, GMAX );

	// matrices
	gg = new TH2F("gg","Gamma-gamma matrix;Energy [keV];Energy [keV];Counts",GBINS, GMIN, GMAX ,GBINS, GMIN, GMAX );
	gg_dcB = new TH2F("gg_dcB","Gamma-gamma matrix, DC for beam;Energy [keV];Energy [keV];Counts",GBINS, GMIN, GMAX ,GBINS, GMIN, GMAX );
	gg_dcT = new TH2F("gg_dcT","Gamma-gamma matrix, DC for target;Energy [keV];Energy [keV];Counts",GBINS, GMIN, GMAX ,GBINS, GMIN, GMAX );
	gg_td = new TH1F("gg_td","#gamma-#gamma time difference;#tau (t_{#gamma} - t_{#gamma}) [ns];Counts per 12.5ns",TBINS,TMAX-25*TBINS,TMAX); // Total
	gcor_size = new TH1F("gcor_size","gcor_size;Number of correlated gammas/electrons;frequency",100,-0.5,99.5);
	
	/* Doppler corrected spectra per detector */
#ifdef PHICAL
	for(int i=0; i<PHI_NSTEPS; i++){
		hname = "phical_dcB" + dc.convertInt(i+1);
		phical_dcB[i] = new TH1F(hname.c_str(),"Beam gated prompt, Doppler corrected #gamma-rays",GBINS, GMIN, GMAX );	
		hname = "phical_dcT" + dc.convertInt(i+1);
		phical_dcT[i] = new TH1F(hname.c_str(),"Beam gated prompt, Doppler corrected #gamma-rays",GBINS, GMIN, GMAX );	
	}
#endif

	// Background subtracted, Doppler corrected spectra 
	B_dcB = new TH1F("B_dcB","Beam gated gamma rays, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );
	B_dcT = new TH1F("B_dcT","Beam gated gamma rays, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );
	B_1hdcB = new TH1F("B_1hdcB","Beam gated gamma rays, 1-p, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );
	B_1hdcT = new TH1F("B_1hdcT","Beam gated gamma rays, 1-p, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );
	gam_dcB = new TH1F("gam_dcB","Total statistics for gamma rays, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );
	
	T_dcT = new TH1F("T_dcT","Target gated, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );
	T_dcB = new TH1F("T_dcB","Target gated, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );
	T_1hdcT = new TH1F("T_1hdcT","Target gated, 1-p, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );
	T_1hdcB = new TH1F("T_1hdcB","Target gated, 1-p, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );
	gam_dcT = new TH1F("gam_dcT","Total statistics for gamma rays, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );
	T_2hdcT = new TH1F("T_2hdcT","T_2hit;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );
	T_2hdcB = new TH1F("T_2hdcB","T_2hit DC for beam;Energy [keV];Counts per 1keV",GBINS, GMIN, GMAX );

	for( int i = 0; i < 4; i++ ) { // per CD quadrant
	
		for( int j = 0; j < 24; j++ ) { // per Ge detector
		
			hname = "T_dcB_x" + dc.convertInt(i) + "_" + dc.convertInt(j);
			htitle = "Target gated (Q" + dc.convertInt(i) + ", Ge" + dc.convertInt(j) + "), background subtracted gamma rays, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV";
			T_dcB_x[(int)(24*i+j)] = new TH1F(hname.c_str(),htitle.c_str(),GBINS, GMIN, GMAX );	
		
			hname = "T_dcT_x" + dc.convertInt(i) + "_" + dc.convertInt(j);
			htitle = "Target gated (Q" + dc.convertInt(i) + ", Ge" + dc.convertInt(j) + "), background subtracted gamma rays, Doppler corrected for target recoil;Energy [keV];Counts per 1keV";
			T_dcT_x[(int)(24*i+j)] = new TH1F(hname.c_str(),htitle.c_str(),GBINS, GMIN, GMAX );	
		
			hname = "B_dcB_x" + dc.convertInt(i) + "_" + dc.convertInt(j);
			htitle = "Beam gated (Q" + dc.convertInt(i) + ", Ge" + dc.convertInt(j) + "), background subtracted gamma rays, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV";
			B_dcB_x[(int)(24*i+j)] = new TH1F(hname.c_str(),htitle.c_str(),GBINS, GMIN, GMAX );	
		
			hname = "B_dcT_x" + dc.convertInt(i) + "_" + dc.convertInt(j);
			htitle = "Beam gated (Q" + dc.convertInt(i) + ", Ge" + dc.convertInt(j) + "), background subtracted gamma rays, Doppler corrected for target recoil;Energy [keV];Counts per 1keV";
			B_dcT_x[(int)(24*i+j)] = new TH1F(hname.c_str(),htitle.c_str(),GBINS, GMIN, GMAX );	
		
		}

	}
	
   
	// Prompt/Random Particle 1 hit
	part = new TH2F("part","Detected particle events;Lab angle [deg];Energy [MeV]",16,cd_angles, PBINS, PMIN, PMAX );
	part1h = new TH2F("part1h","Detected particle events;Lab angle [deg];Energy [MeV]",16,cd_angles, PBINS, PMIN, PMAX );
	part2h = new TH2F("part2h","Detected particle events;Lab angle [deg];Energy [MeV]",16,cd_angles, PBINS, PMIN, PMAX );
	part_ann = new TH2F("part_ann","Detected particle events;Annular strip number;Energy [MeV]",16,-0.5,15.5, PBINS, PMIN, PMAX );
	Bh = new TH2F("Bh","Beam-like events;Lab angle [deg];Energy [MeV]",16,cd_angles, PBINS, PMIN, PMAX );
	Th = new TH2F("Th","Target-like events;Lab angle [deg];Energy [MeV]",16,cd_angles, PBINS, PMIN, PMAX );
	B1h = new TH2F("B1h","Beam-like 1-particle events;Lab angle [deg];Energy [MeV]",16,cd_angles, PBINS, PMIN, PMAX );
	T1h = new TH2F("T1h","Target-like 1-particle events;Lab angle [deg];Energy [MeV]",16,cd_angles, PBINS, PMIN, PMAX );
	B2h = new TH2F("B2h","Beam-like 2-particle events;Lab angle [deg];Energy [MeV]",16,cd_angles, PBINS, PMIN, PMAX );
	T2h = new TH2F("T2h","Target-like 2-particle events;Lab angle [deg];Energy [MeV]",16,cd_angles, PBINS, PMIN, PMAX );
	target_ev = new TH1F("target_ev","Number of deconstructed recoil events per degree;Lab angle [deg];Number of events per degree",16,cd_angles);
	target_1pev = new TH1F("target_1pev","Number of recoil events, without coincidence, per degree;Lab angle [deg];Number of events per degree",16,cd_angles);
	target_2pev = new TH1F("target_2pev","Number of recoil events, coincident with a projectile, per degree;Lab angle [deg];Number of events per degree",16,cd_angles);
	for(int i=0; i<4; i++){
		hname = "partQ" + dc.convertInt(i+1);
		partQ[i] = new TH2F(hname.c_str(),"Random subtracted 1-particle events;Lab angle [deg];Energy [MeV]",16,cd_angles, PBINS, PMIN, PMAX );
	}
	
#ifdef TWOPART
	for(int i=0; i<16; i++){
		hname = "B" + dc.convertInt(i);
		BT[i] = new TH2F(hname.c_str(),hname.c_str(),16,cd_angles, PBINS, PMIN, PMAX );
		hname = "T" + dc.convertInt(i);
		TB[i] = new TH2F(hname.c_str(),hname.c_str(),16,cd_angles, PBINS, PMIN, PMAX );		
	}
#endif

	// Second hit - simulated etc.
	Bsim = new TH2F("Bsim","Simulated beam particle after target detection;Lab angle [deg];Energy [MeV]",90,0,180,PBINS,-PMAX,PMAX);
	Tsim = new TH2F("Tsim","Simulated target particle after beam detection;Lab angle [deg];Energy [MeV]",90,0,180,PBINS,-PMAX,PMAX);
	en2hit = new TH2F("en2hit","Energy of beam vs. Energy of target;Beam energy [MeV];Target energy [MeV]", PBINS, PMIN, PMAX , PBINS, PMIN, PMAX );
	sum2hit = new TH1F("sum2hit","Sum energy of beam target;Sum energy [MeV];Counts", PBINS, PMIN, PMAX );

	// Testing
	multp = new TH1F("multp","Prompt Particle multiplicity;Number of particles;Number of events",8,0.5,8.5); 
	multr = new TH1F("multr","Random Particle multiplicity;Number of particles;Number of events",8,0.5,8.5); 
	GeReject = new TH1F("GeReject","Events with Ge angle equal to zero;Cluster Number",9,-0.5,8.5); 
	GePass = new TH1F("GePass","Events with Ge angle greater than zero;Cluster Number",9,-0.5,8.5); 
	GeRatio = new TH1F("GeRatio","Ratio of Ge events with angle less than or equal to zero to those with real angles;Cluster Number",9,-0.5,8.5); 

	// Ge and Si angular combinations
	GeAng = new TH2F( "GeAng", "Detector angles;#theta;#phi", 45, 0, 180, 90, 0, 359 );
	GeSiAng = new TH3F( "GeSiAng", "Ge and Si coincidences angles;Ge #theta;Si #theta;#phi",
		90, ge_angles, 16, cd_angles, 180, phi_angles);

	// gamma - particle time difference
	tdiff = new TH1F("tdiff","Particle-#gamma time difference;#tau (t_{p} - t_{#gamma}) [ns];Counts per 12.5ns",TBINS,TMAX-25*TBINS,TMAX); // Total

	// particle - particle time difference
	tppdiff = new TH1F("tppdiff","Particle-particle time difference;t_{p_{1}} - t_{p_{2}} [ns];Counts per 25ns",12000/25,-6000,6000);
	tpp = new TH2F("tpp","Particle-gamma time difference matrix;t_{#gamma} - t_{p_{1}} [ns];t_{#gamma} - t_{p_{2}} [ns]",TBINS,TMAX-25*TBINS,TMAX,TBINS,TMAX-25*TBINS,TMAX);
	for(int i=0; i<2; i++){
		hname = "tQ" + dc.convertInt(i+1) + "Q" + dc.convertInt(i+3);
		htitle = "Q" + dc.convertInt(i+1) + "-Q" + dc.convertInt(i+3) + " time difference;t_{p}(Q" + dc.convertInt(i+1) + ") - t_{p}(Q" + dc.convertInt(i+3) + ") [ns];Counts per 25ns;Counts per 25ns";		
		tQQ[i] = new TH1F(hname.c_str(),htitle.c_str(),12000/25,-6000,6000);
	}			

	// Default values for g_clx definitions
	ppwin = 300.;
	maxrecoil = 16;
	minrecoil = 0;

	return;

}

void hists::Set_ppwin(float user_ppwin) {
	ppwin = user_ppwin;
}

void hists::Set_maxrecoil(int user_maxrecoil) {
	maxrecoil = user_maxrecoil;
}

void hists::Set_minrecoil(int user_minrecoil) {
	minrecoil = user_minrecoil;
}

void hists::Fill1h( float GEn, float GTh, float GPh, int cid, vector <float> GCor_GEn, vector <float> GCor_GTh,
					vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd,
					float PEn, Int_t Pann, Int_t Psec, Int_t Pquad, float weight ) {

	if( weight > 0 ) p->Fill( GEn );
	else r->Fill( GEn );

	int cut = dc.Cut( PEn, Pann, Pquad );
	
	FillPar1h( PEn, Pann, Psec, Pquad, cut, weight );

	if( cut >= 0 ) {
	
		FillGam1h( GEn, GTh, GPh, cid, PEn, Pann, Psec, Pquad, cut, weight );
		FillGamGam1h( GEn, GTh, GPh, GCor_GEn, GCor_GTh, GCor_GPh, GCor_CluID, GCor_Gtd,
						PEn, Pann, Psec, Pquad, cut, weight );
		PhiCalHists( GEn, GTh, GPh, PEn, Pann, Psec, Pquad, cut, weight );

	}
	
	return;

}

void hists::Fill2h( float GEn, float GTh, float GPh, int cid, vector <float> GCor_GEn, vector <float> GCor_GTh,
					vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd,
					vector<float> PEn, vector<int> Pann, vector<int> Psec, vector<int> Pquad, vector<int> Pptr, vector<float> td, float weight ) {

	float Bptr, Tptr;

	float time_diff = TMath::Abs(td[Pptr[0]]-td[Pptr[1]]);
	int quad_diff = TMath::Abs(Pquad[Pptr[0]]-Pquad[Pptr[1]]);
	int cut2 = dc.Cut_2p(PEn[Pptr[0]],Pann[Pptr[0]],Pquad[Pptr[0]],PEn[Pptr[1]],Pann[Pptr[1]],Pquad[Pptr[1]]);
	bool cutg_0 = dc.CutG_en2hit(PEn[Pptr[1]]/1000.,PEn[Pptr[0]]/1000.);
	bool cutg_1 = dc.CutG_en2hit(PEn[Pptr[0]]/1000.,PEn[Pptr[1]]/1000.);
	int cut_0 = dc.Cut(PEn[Pptr[0]],Pann[Pptr[0]],Pquad[Pptr[0]]);
	int cut_1 = dc.Cut(PEn[Pptr[1]],Pann[Pptr[1]],Pquad[Pptr[1]]);

	if( quad_diff == 2 && time_diff <= ppwin && cut2 == 0 && cutg_0 ) { // target is [0]

		Bptr = Pptr[1];
		Tptr = Pptr[0];
		FillGam2h( GEn, GTh, GPh, cid, PEn, Pann, Psec, Pquad, Bptr, Tptr, weight );
		FillPar2h( PEn, Pann, Psec, Pquad, Bptr, Tptr, weight );
		
	}

	else if( quad_diff == 2 && time_diff <= ppwin && cut2 == 1 && cutg_1 ) { // target is [1]

		Bptr = Pptr[0];
		Tptr = Pptr[1];
		FillGam2h( GEn, GTh, GPh, cid, PEn, Pann, Psec, Pquad, Bptr, Tptr, weight );
		FillPar2h( PEn, Pann, Psec, Pquad, Bptr, Tptr, weight );
		
	}

	else if( cut_0 == 0 ) { // Pptr[0] is a 1p recoil
	
		FillGam1h( GEn, GTh, GPh, cid, PEn[Pptr[0]], Pann[Pptr[0]], Psec[Pptr[0]], Pquad[Pptr[0]], cut_0, weight );
		FillPar1h( PEn[Pptr[0]], Pann[Pptr[0]], Psec[Pptr[0]], Pquad[Pptr[0]], cut_0, weight );
		
	}
	
	else if( cut_1 == 0 ) { // Pptr[1] is a 1p recoil
	
		FillGam1h( GEn, GTh, GPh, cid, PEn[Pptr[1]], Pann[Pptr[1]], Psec[Pptr[1]], Pquad[Pptr[1]], cut_1, weight );
		FillPar1h( PEn[Pptr[1]], Pann[Pptr[1]], Psec[Pptr[1]], Pquad[Pptr[1]], cut_1, weight );
		
	}
	
	return;

}

void hists::FillGam1h( float GEn, float GTh, float GPh, int cid, float PEn, Int_t Pann,
						Int_t Psec, Int_t Pquad, Int_t cut, float weight ) {


	float BEn, TEn, BTh, TTh, BPh, TPh;

	// Target
	if( cut == 0 ) {
	
		if( weight > 0 ) p_1T->Fill(GEn);
		else r_1T->Fill(GEn);
		
		TEn = PEn;
		//TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
		TTh = dc.GetPTh(Pann);
		TPh = dc.GetPPhi(Pquad,Psec);
		
		BEn = dc.GetBEn(PEn,Pann);
		BTh = dc.GetBTh(Pann);
		BPh = dc.GetQPhi(Pquad,Psec);

		T_dcB_x[(int)(Pquad*24+cid)]->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb() ), weight);	
		T_dcT_x[(int)(Pquad*24+cid)]->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt() ), weight);
	
		if( Pann >= minrecoil && Pann <= maxrecoil ) {

			T_1hdcB->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb() ), weight);
			T_1hdcT->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt() ), weight);
			
		}

	}
	
	// Projectile
	else if( cut > 0 ){
	
		if( weight > 0 ) p_1B->Fill(GEn);
		else r_1B->Fill(GEn);
		
		BEn = PEn;
		BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"TS");
		BTh = dc.GetPTh(Pann);
		BPh = dc.GetPPhi(Pquad,Psec);
		
		TEn = dc.GetTEn(PEn,Pann);
		TTh = dc.GetTTh(Pann,PEn);
		TPh = dc.GetQPhi(Pquad,Psec);

		B_dcB_x[(int)(Pquad*24+cid)]->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb() ), weight);	
		B_dcT_x[(int)(Pquad*24+cid)]->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt() ), weight);

		B_1hdcB->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb() ), weight);
		B_1hdcT->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt() ), weight);
		
	}

	return;

}

void hists::FillGam2h( float GEn, float GTh, float GPh, int cid, vector<float> PEn, vector<int> Pann,
						vector<int> Psec, vector<int> Pquad, Int_t Bptr, Int_t Tptr, float weight ) {

	if( weight > 0 ) p_2h->Fill(GEn);
	else r_2h->Fill(GEn);
	
	int Bann = Pann[Bptr];
	int Tann = Pann[Tptr];
	float BEn = PEn[Bptr];
	BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"TS");
	float TEn = PEn[Tptr];
	TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
	float BTh = dc.GetPTh(Bann);
	float TTh = dc.GetPTh(Tann);
	float BPh = dc.GetPPhi(Pquad[Bptr],Psec[Bptr]);
	float TPh = dc.GetPPhi(Pquad[Tptr],Psec[Tptr]);

	T_dcB_x[(int)(Pquad[Tptr]*24+cid)]->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb() ), weight);	
	T_dcT_x[(int)(Pquad[Tptr]*24+cid)]->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt() ), weight);

	B_dcB_x[(int)(Pquad[Bptr]*24+cid)]->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb() ), weight);	
	B_dcT_x[(int)(Pquad[Bptr]*24+cid)]->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt() ), weight);

	if( Tann >= minrecoil && Tann <= maxrecoil ) {

		T_2hdcB->Fill(GEn*dc.DC(BEn,BTh,BPh,GTh,GPh, dc.GetAb() ), weight);
		T_2hdcT->Fill(GEn*dc.DC(TEn,TTh,TPh,GTh,GPh, dc.GetAt() ), weight);
		
	}

	return;

}

void hists::FillGamGam1h( float GEn, float GTh, float GPh, vector <float> GCor_GEn, vector <float> GCor_GTh,
						 vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd,
						 float PEn, Int_t Pann, Int_t Psec, Int_t Pquad, int cut, float weight ) {
	
	float BEn = 0, TEn = 0, BTh = 0, TTh = 0, BPh = 0, TPh = 0;
	
	// Target
	if( cut == 0 ) {
		
		if( Pann < minrecoil || Pann > maxrecoil ) return;
		
		TEn = PEn;
		//TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
		TTh = dc.GetPTh(Pann);
		TPh = dc.GetPPhi(Pquad,Psec);
		
		BEn = dc.GetBEn(PEn,Pann);
		BTh = dc.GetBTh(Pann);
		BPh = dc.GetQPhi(Pquad,Psec);

	}
	
	// Projectile
	else if( cut > 0 ){
		
		BEn = PEn;
		BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"TS");
		BTh = dc.GetPTh(Pann);
		BPh = dc.GetPPhi(Pquad,Psec);
		
		TEn = dc.GetTEn(PEn,Pann);
		TTh = dc.GetTTh(Pann,PEn);
		TPh = dc.GetQPhi(Pquad,Psec);
		
	}
	
	for( unsigned int i = 0; i < GCor_GEn.size(); i++ ) {
		
		if( GCor_CluID.at(i) != 8 ) { // gamma
			
			gg_td->Fill( GCor_Gtd.at(i)*25. );
			
			if( GCor_Gtd.at(i)*25. > -200. && GCor_Gtd.at(i)*25. < 210. ) {
				
				gg->Fill(GEn, GCor_GEn.at(i), weight);
				gg_dcT->Fill( GEn*dc.DC(TEn,TTh,TPh,GTh,GPh, dc.GetAt() ),
							  GCor_GEn.at(i)*dc.DC(TEn,TTh,TPh,GCor_GTh.at(i),GCor_GPh.at(i), dc.GetAt() ),
							  weight );
				gg_dcB->Fill( GEn*dc.DC(BEn,BTh,BPh,GTh,GPh, dc.GetAb() ),
							  GCor_GEn.at(i)*dc.DC(BEn,BTh,BPh,GCor_GTh.at(i),GCor_GPh.at(i), dc.GetAb() ),
							  weight );
				
			}
			
		}
		
	}
	
	return;
	
}

void hists::FillGamGam2h( float GEn, float GTh, float GPh, vector <float> GCor_GEn, vector <float> GCor_GTh,
						 vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd,
						 vector<float> PEn, vector<int> Pann, vector<int> Psec, vector<int> Pquad,
						 Int_t Bptr, Int_t Tptr, float weight ) {
	
	int Bann = Pann[Bptr];
	int Tann = Pann[Tptr];
	float BEn = PEn[Bptr];
	BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"TS");
	float TEn = PEn[Tptr];
	TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
	float BTh = dc.GetPTh(Bann);
	float TTh = dc.GetPTh(Tann);
	float BPh = dc.GetPPhi(Pquad[Bptr],Psec[Bptr]);
	float TPh = dc.GetPPhi(Pquad[Tptr],Psec[Tptr]);
	
	for( unsigned int i = 0; i < GCor_GEn.size(); i++ ) {
		
		if( GCor_CluID.at(i) != 8 ) { // gamma
			
			gg_td->Fill( GCor_Gtd.at(i)*25. );
			
			if( GCor_Gtd.at(i)*25. > -200. && GCor_Gtd.at(i)*25. < 210. ) {
				
				gg->Fill(GEn, GCor_GEn.at(i), weight); 
				gg_dcT->Fill( GEn*dc.DC(TEn,TTh,TPh,GTh,GPh, dc.GetAt() ),
							  GCor_GEn.at(i)*dc.DC(TEn,TTh,TPh,GCor_GTh.at(i),GCor_GPh.at(i), dc.GetAt() ),
							  weight );
				gg_dcB->Fill( GEn*dc.DC(BEn,BTh,BPh,GTh,GPh, dc.GetAb() ),
							  GCor_GEn.at(i)*dc.DC(BEn,BTh,BPh,GCor_GTh.at(i),GCor_GPh.at(i), dc.GetAb() ),
							  weight );
				
			}
			
		}
		
	}
	
	return;
	

	
}

void hists::PhiCalHists( float GEn, float GTh, float GPh, float PEn, Int_t Pann,
						Int_t Psec, Int_t Pquad, Int_t cut, float weight ) {

	// Calibration of the phi offset of the CD detector
#ifdef PHICAL

	float BEn = 0, TEn = 0, BTh = 0, TTh = 0, BPh = 0, TPh = 0;

	// Target
	if( cut == 0 ) {
	
		TEn = PEn;
		TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
		TTh = dc.GetPTh(Pann);
		TPh = dc.GetPPhi(Pquad,Psec);
		
		BEn = dc.GetBEn(PEn,Pann);
		BTh = dc.GetBTh(Pann);
		BPh = dc.GetQPhi(Pquad,Psec);

	}
	
	// Projectile
	else if( cut > 0 ){
	
		BEn = PEn;
		BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"TS");
		BTh = dc.GetPTh(Pann);
		BPh = dc.GetPPhi(Pquad,Psec);
		
		TEn = dc.GetTEn(PEn,Pann);
		TTh = dc.GetTTh(Pann,PEn);
		TPh = dc.GetQPhi(Pquad,Psec);
		
	}
	
	else return;
	
	// Move BPh and TPh
	BPh -= (float)(int)((PHI_NSTEPS-1)/2) * PHI_STEP_WIDTH * TMath::DegToRad();
	TPh -= (float)(int)((PHI_NSTEPS-1)/2) * PHI_STEP_WIDTH * TMath::DegToRad();

	// Make steps either side of the offset
	for( int j = 0; j < PHI_NSTEPS; j++ ) {

		BPh += (float)j * PHI_STEP_WIDTH * TMath::DegToRad();
		TPh += (float)j * PHI_STEP_WIDTH * TMath::DegToRad();
		phical_dcT[j]->Fill( GEn*dc.DC(TEn,TTh,TPh,GTh,GPh, dc.GetAt() ), weight );
		phical_dcB[j]->Fill( GEn*dc.DC(BEn,BTh,BPh,GTh,GPh, dc.GetAb() ), weight );

	}

#endif

	return;
	
}

void hists::FillPar1h( float PEn, Int_t Pann, Int_t Psec, Int_t Pquad, Int_t cut, float weight ) {
	
	float PTh = dc.GetPTh(Pann)*TMath::RadToDeg();	
	float BTh, TTh, BEn, TEn;

	part1h->Fill(PTh, PEn/1000.);
	partQ[Pquad]->Fill(PTh, PEn/1000.);
	part_ann->Fill(Pann, PEn/1000.);
	
	if( cut == 0 ) {
	
		BTh = dc.GetBTh(Pann)*TMath::RadToDeg();
		BEn = dc.GetBEn(PEn,Pann);
		BEn -= dc.GetELoss(BEn,dc.GetCDDeadLayer(),0,"TS");

		Th->Fill(PTh,PEn/1000.);
		T1h->Fill(PTh,PEn/1000.);
		Bsim->Fill(BTh,BEn/1000.);

	}
	
	else if( cut > 0 ) {
	
		TTh = dc.GetTTh(Pann,PEn)*TMath::RadToDeg();
		TEn = dc.GetTEn(PEn,Pann);
		TEn -= dc.GetELoss(TEn,dc.GetCDDeadLayer(),0,"TS");
	
		Bh->Fill(PTh,PEn/1000.);
		B1h->Fill(PTh,PEn/1000.);
		Tsim->Fill(TTh,TEn/1000.);		
	
	}
	
	return;

}

void hists::FillPar2h( vector<float> PEn, vector<int> Pann, vector<int> Psec, vector<int> Pquad, Int_t Bptr, Int_t Tptr, float weight ) {

	int Bann = Pann[Bptr];
	int Tann = Pann[Tptr];
	int Bsec = Psec[Bptr];
	int Tsec = Psec[Tptr];
	int Bquad = Pquad[Bptr];
	int Tquad = Pquad[Tptr];
	float BEn = PEn[Bptr];
	float TEn = PEn[Tptr];
	float BTh = dc.GetPTh(Bann)*TMath::RadToDeg();
	float TTh = dc.GetPTh(Tann)*TMath::RadToDeg();
	float BPh = dc.GetPPhi(Bquad,Bsec)*TMath::RadToDeg();
	float TPh = dc.GetPPhi(Tquad,Tsec)*TMath::RadToDeg();

	Bh->Fill(BTh, BEn/1000.);
	Th->Fill(TTh, TEn/1000.);
	B2h->Fill(BTh, BEn/1000.);
	T2h->Fill(TTh, TEn/1000.);
	partQ[Bquad]->Fill(BTh, BEn/1000.);
	partQ[Tquad]->Fill(TTh, TEn/1000.);
	part_ann->Fill(Bann, BEn/1000.);
	part_ann->Fill(Tann, TEn/1000.);
	en2hit->Fill(BEn/1000.,TEn/1000.);
	sum2hit->Fill((BEn+TEn)/1000.);	
#ifdef TWOPART
	BT[Bann]->Fill(TTh,TEn/1000.);
	TB[Tann]->Fill(BTh,BEn/1000.);
#endif

	return;

}

void hists::AddSpectra(float bg_frac) {

	pmr->Add(p,r,1.0,bg_frac);
	
	pB->Add(p_1B,p_2h);
	rB->Add(r_1B,r_2h);
	pBmrB->Add(pB,rB,1.0,bg_frac);
	
	pT->Add(p_1T,p_2h);
	rT->Add(r_1T,r_2h);
	pTmrT->Add(pT,rT,1.0,bg_frac);
	
	part2h->Add(B2h,T2h);
	part->Add(part1h,part2h);
	
	B_dcB->Add(B_1hdcB,T_2hdcB);
	B_dcT->Add(B_1hdcT,T_2hdcT);
	T_dcB->Add(T_1hdcB,T_2hdcB);
	T_dcT->Add(T_1hdcT,T_2hdcT);
	gam_dcB->Add(B_dcB,T_1hdcB);
	gam_dcT->Add(T_dcT,B_1hdcT);
	
	GeRatio->Divide(GeReject,GePass);
    
	target_1pev->Add((TH1F*)T1h->ProjectionX());
	target_2pev->Add((TH1F*)T2h->ProjectionX());
	target_ev->Add(target_1pev,target_2pev);
	for(int bin=1; bin <= target_ev->GetXaxis()->GetNbins(); bin++)
		target_ev->SetBinContent(bin,target_ev->GetBinContent(bin)/target_ev->GetBinWidth(bin));
	for(int bin=1; bin <= target_1pev->GetXaxis()->GetNbins(); bin++)
		target_1pev->SetBinContent(bin,target_1pev->GetBinContent(bin)/target_1pev->GetBinWidth(bin));
	for(int bin=1; bin <= target_2pev->GetXaxis()->GetNbins(); bin++)
		target_2pev->SetBinContent(bin,target_2pev->GetBinContent(bin)/target_2pev->GetBinWidth(bin));

}
#endif
