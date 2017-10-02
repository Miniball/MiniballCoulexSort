#ifndef hists_cxx
#define hists_cxx

#define GBINS 4000	// number of bins in gamma spectra
#define GMAX 4000	// maximum energy in gamma spectra
#define EBINS 3000	// number of bins in electron spectra
#define EMAX 6000	// maximum energy in electron spectra
#define PBINS 800	// number of bins in particle spectra
#define PMAX 800	// maximum energy in particle spectra
#define TBINS 242	// number of bins in tdiff spectra
#define TMAX 1525	// maximum time in tdiff spectra

#ifndef hist_hh
# include "hists.hh"
#endif

void hists::Initialise() {

	cout << "Initialising histograms...\n";

	string hname, htitle;

	for( int k=0; k<17; k++ ) cd_angles[k] = doppler::GetPTh(15.5-k)*TMath::RadToDeg(); 

	p = new TH1F("p","Prompt gammas;Energy [keV];Counts per 1 keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	r = new TH1F("r","Random gammas;Energy [keV];Counts per 1 keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	d = new TH1F("d","Delayed gammas;Energy [keV];Counts per 1 keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	hname = "Prompt minus random;Energy [keV];Counts per " + doppler::convertInt( (int)(GMAX/GBINS) ) + " keV";
	pmr = new TH1F("pmr",hname.c_str(),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	hname = "Delayed minus random;Energy [keV];Counts per " + doppler::convertInt( (int)(GMAX/GBINS) ) + " keV";
	dmr = new TH1F("dmr",hname.c_str(),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	pB = new TH1F("pB","Prompt gammas gated on beam",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	rB = new TH1F("rB","Random gammas gated on beam",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	pT = new TH1F("pT","Prompt gammas gated on target",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	rT = new TH1F("rT","Random gammas gated on target",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	pBmrB = new TH1F("pBmrB","Prompt gated gammas on beam minus random gated on beam",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	pTmrT = new TH1F("pTmrT","Prompt gated gammas on target minus random gated on target",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	p_1B = new TH1F("p_1B","#gamma-rays in prompt coincidence with 1-beam particle",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	r_1B = new TH1F("r_1B","#gamma-rays in random coincidence with 1-beam particle",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	p_1T = new TH1F("p_1T","#gamma-rays in prompt coincidence with 1-target particle",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	r_1T = new TH1F("r_1T","#gamma-rays in random coincidence with 1-target particle",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	p_2h = new TH1F("p_2h","#gamma-rays in prompt coincidence with 2-particles",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	r_2h = new TH1F("r_2h","#gamma-rays in random coincidence with 2-particles",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));

	// matrices
	gg = new TH2F("gg","Total gamma-gamma matrix;Energy [keV];Energy [keV];Counts",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	pp = new TH2F("pp","Gamma-gamma matrix, background subtracted;Energy [keV];Energy [keV];Counts",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	pd = new TH2F("pd","Gamma-gamma matrix, prompt vs. delayed, background subtracted;Prompt Energy [keV];Delayed Energy [keV];Counts",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	dd = new TH2F("dd","Gamma-gamma matrix, delayed vs. delayed, background subtracted;Delayed Energy [keV];Delayed Energy [keV];Counts",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	gg_dcB = new TH2F("gg_dcB","Gamma-gamma matrix, DC for beam;Energy [keV];Energy [keV];Counts",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	gg_dcT = new TH2F("gg_dcT","Gamma-gamma matrix, DC for target;Energy [keV];Energy [keV];Counts",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	gg_td = new TH1F("gg_td","#gamma-#gamma time difference;#tau (t_{#gamma} - t_{#gamma}) [ns];Counts per 12.5ns",TBINS,TMAX-25*TBINS,TMAX); // Total
	gd_td = new TH1F("gd_td","#gamma_d-#gamma_p time difference;#tau (t_{d} - t_{p}) [ns];Counts per 12.5ns",TBINS,TMAX-25*TBINS,TMAX); // Total
	gcor_size = new TH1F("gcor_size","gcor_size;Number of correlated gammas/electrons;frequency",100,-0.5,99.5);
	
	/* Doppler corrected spectra per detector */
#ifdef PHICAL
	for(int i=0; i<PHI_NSTEPS; i++){
		hname = "phical_dcB" + doppler::convertInt(i+1);
		phical_dcB[i] = new TH1F(hname.c_str(),"Beam gated prompt, Doppler corrected #gamma-rays",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));	
		hname = "phical_dcT" + doppler::convertInt(i+1);
		phical_dcT[i] = new TH1F(hname.c_str(),"Beam gated prompt, Doppler corrected #gamma-rays",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));	
	}
#endif
#ifdef CALIB
	for(int i=0; i<8; i++){
		hname = "GeCalib_clu" + doppler::convertInt(i+1);
		GeCalib_clu[i] = new TH1F(hname.c_str(),"Beam gated prompt, Doppler corrected #gamma-rays",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	}
	hname = "GeCalib_clu" + doppler::convertInt(CLUSTER)+ "_dcT_MaxEnergy";
	GeCalib_dcTMaxE = new TH2F(hname.c_str(),"Beam gated prompt, Target Doppler corrected #gamma-rays",GE_THETA_NSTEPS-1,0,GE_THETA_NSTEPS-1,GE_PHI_NSTEPS-1,0,GE_PHI_NSTEPS-1);
	hname = "GeCalib_clu" + doppler::convertInt(CLUSTER)+ "_dcT_Max";
	GeCalib_dcTMax = new TH2F(hname.c_str(),"Beam gated prompt, Target Doppler corrected #gamma-rays",GE_THETA_NSTEPS-1,0,GE_THETA_NSTEPS-1,GE_PHI_NSTEPS-1,0,GE_PHI_NSTEPS-1);
	hname = "GeCalib_clu" + doppler::convertInt(CLUSTER)+ "_dcT_MaxSigma";
	GeCalib_dcTMaxSigma = new TH2F(hname.c_str(),"Beam gated prompt, Target Doppler corrected #gamma-rays",GE_THETA_NSTEPS-1,0,GE_THETA_NSTEPS-1,GE_PHI_NSTEPS-1,0,GE_PHI_NSTEPS-1);
	for(int j=0; j<GE_THETA_NSTEPS; j++){
		for(int k=0; k<GE_PHI_NSTEPS; k++){
			hname = "GeCalib_clu"+ doppler::convertInt(CLUSTER) + "_dcT_Th"+ doppler::convertInt(j) + "Ph" + doppler::convertInt(k);
			GeCalib_dcT[j][k] = new TH1F(hname.c_str(),"Beam gated prompt, Target Doppler corrected #gamma-rays",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));//horrible hack to brute force angle calculation
		}
	}

	for(int i=0; i<24; i++){
		hname = "GeCalib_cid" + doppler::convertInt(i+1);
		GeCalib_cid[i] = new TH1F(hname.c_str(),"Beam gated prompt, Doppler corrected #gamma-rays",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	}
#endif

	// Background subtracted, Doppler corrected spectra 
	B_dcB = new TH1F("B_dcB","Beam gated gamma rays, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_dcT = new TH1F("B_dcT","Beam gated gamma rays, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_1hdcB = new TH1F("B_1hdcB","Beam gated gamma rays, 1-p, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_1hdcT = new TH1F("B_1hdcT","Beam gated gamma rays, 1-p, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	gam_dcB = new TH1F("gam_dcB","Total statistics for gamma rays, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	
	T_dcT = new TH1F("T_dcT","Target gated, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_dcB = new TH1F("T_dcB","Target gated, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_1hdcT = new TH1F("T_1hdcT","Target gated, 1-p, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_1hdcB = new TH1F("T_1hdcB","Target gated, 1-p, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	gam_dcT = new TH1F("gam_dcT","Total statistics for gamma rays, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_2hdcT = new TH1F("T_2hdcT","T_2hit;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_2hdcB = new TH1F("T_2hdcB","T_2hit DC for beam;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));

	for(int i=0; i<16; i++){
	
		hname = "T_dcB_x" + doppler::convertInt(i);
		htitle = "Target gated (strip " + doppler::convertInt(i) + "), background subtracted gamma rays, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV";
		T_dcB_x[i] = new TH1F(hname.c_str(),htitle.c_str(),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));	
		
		hname = "T_dcT_x" + doppler::convertInt(i);
		htitle = "Target gated (strip " + doppler::convertInt(i) + "), background subtracted gamma rays, Doppler corrected for target recoil;Energy [keV];Counts per 1keV";
		T_dcT_x[i] = new TH1F(hname.c_str(),htitle.c_str(),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));	
		
		hname = "B_dcB_x" + doppler::convertInt(i);
		htitle = "Beam gated (strip " + doppler::convertInt(i) + "), background subtracted gamma rays, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV";
		B_dcB_x[i] = new TH1F(hname.c_str(),htitle.c_str(),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));	
		
		hname = "B_dcT_x" + doppler::convertInt(i);
		htitle = "Beam gated (strip " + doppler::convertInt(i) + "), background subtracted gamma rays, Doppler corrected for target recoil;Energy [keV];Counts per 1keV";
		B_dcT_x[i] = new TH1F(hname.c_str(),htitle.c_str(),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));	
		
	}
	
   
	/* PART : 1 hit */
	/* Prompt/Random Particle 1 hit */
	part = new TH2F("part","Detected particle events;Lab angle [deg];Energy [MeV]",16,cd_angles,PBINS,0,PMAX);
	part1h = new TH2F("part1h","Detected particle events;Lab angle [deg];Energy [MeV]",16,cd_angles,PBINS,0,PMAX);
	part2h = new TH2F("part2h","Detected particle events;Lab angle [deg];Energy [MeV]",16,cd_angles,PBINS,0,PMAX);
	part_ann = new TH2F("part_ann","Detected particle events;Annular strip number;Energy [MeV]",16,-0.5,15.5,PBINS,0,PMAX);
	Bh = new TH2F("Bh","Beam-like events;Lab angle [deg];Energy [MeV]",16,cd_angles,PBINS,0,PMAX);
	Th = new TH2F("Th","Target-like events;Lab angle [deg];Energy [MeV]",16,cd_angles,PBINS,0,PMAX);
	B1h = new TH2F("B1h","Beam-like 1-particle events;Lab angle [deg];Energy [MeV]",16,cd_angles,PBINS,0,PMAX);
	T1h = new TH2F("T1h","Target-like 1-particle events;Lab angle [deg];Energy [MeV]",16,cd_angles,PBINS,0,PMAX);
	B2h = new TH2F("B2h","Beam-like 2-particle events;Lab angle [deg];Energy [MeV]",16,cd_angles,PBINS,0,PMAX);
	T2h = new TH2F("T2h","Target-like 2-particle events;Lab angle [deg];Energy [MeV]",16,cd_angles,PBINS,0,PMAX);
	target_ev = new TH1F("target_ev","Number of deconstructed recoil events per degree;Lab angle [deg];Number of events per degree",16,cd_angles);
	target_1pev = new TH1F("target_1pev","Number of recoil events, without coincidence, per degree;Lab angle [deg];Number of events per degree",16,cd_angles);
	target_2pev = new TH1F("target_2pev","Number of recoil events, coincident with a projectile, per degree;Lab angle [deg];Number of events per degree",16,cd_angles);
	for(int i=0; i<4; i++){
		hname = "partQ" + doppler::convertInt(i+1);
		partQ[i] = new TH2F(hname.c_str(),"Random subtracted 1-particle events;Lab angle [deg];Energy [MeV]",16,cd_angles,PBINS,0,PMAX);
	}
	
#ifdef TWOPART
	for(int i=0; i<16; i++){
		hname = "B" + doppler::convertInt(i);
		BT[i] = new TH2F(hname.c_str(),hname.c_str(),16,cd_angles,PBINS,0,PMAX);
		hname = "T" + doppler::convertInt(i);
		TB[i] = new TH2F(hname.c_str(),hname.c_str(),16,cd_angles,PBINS,0,PMAX);		
	}
#endif

	/* Second hit - simulated etc. */
	Bsim = new TH2F("Bsim","Simulated beam particle after target detection;Lab angle [deg];Energy [MeV]",90,0,180,PBINS,-PMAX,PMAX);
	Tsim = new TH2F("Tsim","Simulated target particle after beam detection;Lab angle [deg];Energy [MeV]",90,0,180,PBINS,-PMAX,PMAX);
	en2hit = new TH2F("en2hit","Energy of beam vs. Energy of target;Beam energy [MeV];Target energy [MeV]",PBINS,0,PMAX,PBINS,0,PMAX);
	sum2hit = new TH1F("sum2hit","Sum energy of beam target;Sum energy [MeV];Counts",PBINS,0,PMAX);

	/* CD picture using the gamma gating */
	cdpic = new TH2F("CDPic_g","Particle distributon in CD (#theta-#phi)",1000,-50,50,1000,-50,50);
	
	/* Testing */
	multp = new TH1F("multp","Prompt Particle multiplicity;Number of particles;Number of events",8,0.5,8.5); 
	multr = new TH1F("multr","Random Particle multiplicity;Number of particles;Number of events",8,0.5,8.5); 
	GeReject = new TH1F("GeReject","Events with Ge angle equal to zero;Cluster Number",9,-0.5,8.5); 
	GePass = new TH1F("GePass","Events with Ge angle greater than zero;Cluster Number",9,-0.5,8.5); 
	GeRatio = new TH1F("GeRatio","Ratio of Ge events with angle less than or equal to zero to those with real angles;Cluster Number",9,-0.5,8.5); 
	GeAng = new TH2F("GeAng","Detector angles;#theta;#phi",45,0,180,90,0,359);
#ifdef GEANG
	GeSiAng = new TH3F("GeSiAng","Ge and Si coincidences angles;Ge #theta;Si #theta;#phi",45,0,180,30,0,60,270,-720,360);
	for(int i=0; i<8; i++){
		hname = "GeAng_clu" + doppler::convertInt(i+1);
		htitle = "Detector angles in Cluster " + doppler::convertInt(i+1) + ";#theta;#phi";
		GeAng_clu[i] = new TH2F(hname.c_str(),htitle.c_str(),45,0,180,90,0,359);
		hname = "GeSiAng_clu" + doppler::convertInt(i+1);
		htitle = "Ge and Si coincidences angles in Cluster " + doppler::convertInt(i+1) + ";Ge #theta;Si #theta;#phi";
		GeSiAng_clu[i] = new TH3F(hname.c_str(),htitle.c_str(),45,0,180,30,0,60,270,-720,360);
	}
#endif

	// gamma - particle time difference
	tdiff = new TH1F("tdiff","Particle-#gamma time difference;#tau (t_{p} - t_{#gamma}) [ns];Counts per 12.5ns",TBINS,TMAX-25*TBINS,TMAX); // Total
	tegate[0] = 85.;
	tegate[1] = 166.;
	tegate[2] = 228.;
	tegate[3] = 881.;
	for(int i=0; i<4; i++){
		hname = "tdiffE" + doppler::convertInt(tegate[i]);
		htitle = "Particle-#gamma time difference gated around " + doppler::convertInt(tegate[i]) + " keV;#tau (t_{p} - t_{#gamma}) [ns];Counts per 25ns";		
		tdiffE[i] = new TH1F(hname.c_str(),htitle.c_str(),TBINS,TMAX-25*TBINS,TMAX);
		hname = "tdiffQ" + doppler::convertInt(i+1);
		htitle = "Particle-#gamma time difference in quadrant " + doppler::convertInt(i+1) + ";#tau (t_{p} - t_{#gamma}) [ns];Counts per 25ns";		
		tdiffQ[i] = new TH1F(hname.c_str(),hname.c_str(),TBINS,TMAX-25*TBINS,TMAX);
	}
	// particle - particle time difference
	tppdiff = new TH1F("tppdiff","Particle-particle time difference;t_{p_{1}} - t_{p_{2}} [ns];Counts per 25ns",12000/25,-6000,6000);
	tpp = new TH2F("tpp","Particle-gamma time difference matrix;t_{#gamma} - t_{p_{1}} [ns];t_{#gamma} - t_{p_{2}} [ns]",TBINS,TMAX-25*TBINS,TMAX,TBINS,TMAX-25*TBINS,TMAX);
	for(int i=0; i<2; i++){
		hname = "tQ" + doppler::convertInt(i+1) + "Q" + doppler::convertInt(i+3);
		htitle = "Q" + doppler::convertInt(i+1) + "-Q" + doppler::convertInt(i+3) + " time difference;t_{p}(Q" + doppler::convertInt(i+1) + ") - t_{p}(Q" + doppler::convertInt(i+3) + ") [ns];Counts per 25ns;Counts per 25ns";		
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

void hists::Fill1h( float GEn, float GTh, float GPh, vector <float> GCor_GEn, vector <float> GCor_GTh,
					vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd, bool electron,
					float PEn, Int_t Pann, Int_t Psec, Int_t Pquad, float weight ) {

	if( !electron ) {
	
		if( weight > 0 ) p->Fill( GEn );
		else r->Fill( GEn );

	}
	else {
	
	}

	int cut = doppler::Cut( PEn, Pann, Pquad );
	
	FillPar1h( PEn, Pann, Psec, Pquad, cut, weight );

	if( cut >= 0 ) {
	
		if( !electron ) {
	
			FillGam1h( GEn, GTh, GPh, PEn, Pann, Psec, Pquad, cut, weight );
			FillGamGam1h( GEn, GTh, GPh, GCor_GEn, GCor_GTh, GCor_GPh, GCor_CluID, GCor_Gtd,
				PEn, Pann, Psec, Pquad, cut, weight );
			PhiCalHists( GEn, GTh, GPh, PEn, Pann, Psec, Pquad, cut, weight );
		
		}

		else {
			
		}

	}
	
	return;

}

void hists::Fill2h( float GEn, float GTh, float GPh, vector <float> GCor_GEn, vector <float> GCor_GTh,
					vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd, bool electron,
					vector<float> PEn, vector<int> Pann, vector<int> Psec, vector<int> Pquad, vector<int> Pptr, vector<float> td, float weight ) {

	float Bptr, Tptr;

	float time_diff = TMath::Abs(td[Pptr[0]]-td[Pptr[1]]);
	int quad_diff = TMath::Abs(Pquad[Pptr[0]]-Pquad[Pptr[1]]);
	int cut2 = doppler::Cut_2p(PEn[Pptr[0]],Pann[Pptr[0]],Pquad[Pptr[0]],PEn[Pptr[1]],Pann[Pptr[1]],Pquad[Pptr[1]]);
	bool cutg_0 = doppler::CutG_en2hit(PEn[Pptr[1]]/1000.,PEn[Pptr[0]]/1000.);
	bool cutg_1 = doppler::CutG_en2hit(PEn[Pptr[0]]/1000.,PEn[Pptr[1]]/1000.);
	int cut_0 = doppler::Cut(PEn[Pptr[0]],Pann[Pptr[0]],Pquad[Pptr[0]]);
	int cut_1 = doppler::Cut(PEn[Pptr[1]],Pann[Pptr[1]],Pquad[Pptr[1]]);

	if( quad_diff == 2 && time_diff <= ppwin && cut2 == 0 && cutg_0 ) { // target is [0]

		Bptr = Pptr[1];
		Tptr = Pptr[0];
		FillGam2h( GEn, GTh, GPh, PEn, Pann, Psec, Pquad, Bptr, Tptr, weight );
		FillPar2h( PEn, Pann, Psec, Pquad, Bptr, Tptr, weight );
		
	}

	else if( quad_diff == 2 && time_diff <= ppwin && cut2 == 1 && cutg_1 ) { // target is [1]

		Bptr = Pptr[0];
		Tptr = Pptr[1];
		FillGam2h( GEn, GTh, GPh, PEn, Pann, Psec, Pquad, Bptr, Tptr, weight );
		FillPar2h( PEn, Pann, Psec, Pquad, Bptr, Tptr, weight );
		
	}

	else if( cut_0 == 0 ) { // Pptr[0] is a 1p recoil
	
		FillGam1h( GEn, GTh, GPh, PEn[Pptr[0]], Pann[Pptr[0]], Psec[Pptr[0]], Pquad[Pptr[0]], cut_0, weight );
		FillPar1h( PEn[Pptr[0]], Pann[Pptr[0]], Psec[Pptr[0]], Pquad[Pptr[0]], cut_0, weight );
		
	}
	
	else if( cut_1 == 0 ) { // Pptr[1] is a 1p recoil
	
		FillGam1h( GEn, GTh, GPh, PEn[Pptr[1]], Pann[Pptr[1]], Psec[Pptr[1]], Pquad[Pptr[1]], cut_1, weight );
		FillPar1h( PEn[Pptr[1]], Pann[Pptr[1]], Psec[Pptr[1]], Pquad[Pptr[1]], cut_1, weight );
		
	}
	
	return;

}

void hists::FillDelayed( float GEn, float GTh, float GPh, vector <float> GCor_GEn, vector <float> GCor_GTh,
					vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd, bool electron,
					float PEn, Int_t Pann, Int_t Psec, Int_t Pquad, float weight ) {

	if( weight > 0 ) d->Fill( GEn );

	for( unsigned int i = 0; i < GCor_GEn.size(); i++ ) {
		
		gd_td->Fill( GCor_Gtd.at(i)*25. );
			
		if( GCor_Gtd.at(i)*25. > -1000. && GCor_Gtd.at(i)*25. < -125. ) {
				
			pd->Fill( GCor_GEn.at(i), GEn, weight );
				
		}

		else if( GCor_Gtd.at(i)*25. > -125. && GCor_Gtd.at(i)*25. < 125. ) {
				
			dd->Fill( GEn, GCor_GEn.at(i), weight );
				
		}

	}
	
	return;

}

void hists::FillGam1h( float GEn, float GTh, float GPh, float PEn, Int_t Pann,
						Int_t Psec, Int_t Pquad, Int_t cut, float weight ) {


	float BEn, TEn, BTh, TTh, BPh, TPh;

	// Target
	if( cut == 0 ) {
	
		if( weight > 0 ) p_1T->Fill(GEn);
		else r_1T->Fill(GEn);
		
		TEn = PEn;
		//TEn += doppler::GetELoss(TEn,DEADLAYER,1,"TS");
		TTh = doppler::GetPTh(Pann);
		TPh = doppler::GetPPhi(Pquad,Psec);
		
		BEn = doppler::GetBEn(PEn,Pann);
		BTh = doppler::GetBTh(Pann);
		BPh = doppler::GetQPhi(Pquad,Psec);

		T_dcB_x[Pann]->Fill(GEn*doppler::DC(BEn, BTh, BPh, GTh, GPh, AP), weight);	
		T_dcT_x[Pann]->Fill(GEn*doppler::DC(TEn, TTh, TPh, GTh, GPh, AT), weight);
	
		if( Pann >= minrecoil && Pann <= maxrecoil ) {

			T_1hdcB->Fill(GEn*doppler::DC(BEn, BTh, BPh, GTh, GPh, AP), weight);
			T_1hdcT->Fill(GEn*doppler::DC(TEn, TTh, TPh, GTh, GPh, AT), weight);
			
		}

	}
	
	// Projectile
	else if( cut > 0 ){
	
		if( weight > 0 ) p_1B->Fill(GEn);
		else r_1B->Fill(GEn);
		
		BEn = PEn;
		BEn += doppler::GetELoss(BEn,DEADLAYER,1,"TS");
		BTh = doppler::GetPTh(Pann);
		BPh = doppler::GetPPhi(Pquad,Psec);
		
		TEn = doppler::GetTEn(PEn,Pann);
		TTh = doppler::GetTTh(Pann,PEn);
		TPh = doppler::GetQPhi(Pquad,Psec);

		B_dcB_x[Pann]->Fill(GEn*doppler::DC(BEn, BTh, BPh, GTh, GPh, AP), weight);	
		B_dcT_x[Pann]->Fill(GEn*doppler::DC(TEn, TTh, TPh, GTh, GPh, AT), weight);

		B_1hdcB->Fill(GEn*doppler::DC(BEn, BTh, BPh, GTh, GPh, AP), weight);
		B_1hdcT->Fill(GEn*doppler::DC(TEn, TTh, TPh, GTh, GPh, AT), weight);
		
	}

	return;

}

void hists::FillGam2h( float GEn, float GTh, float GPh, vector<float> PEn, vector<int> Pann,
						vector<int> Psec, vector<int> Pquad, Int_t Bptr, Int_t Tptr, float weight ) {

	if( weight > 0 ) p_2h->Fill(GEn);
	else r_2h->Fill(GEn);
	
	int Bann = Pann[Bptr];
	int Tann = Pann[Tptr];
	float BEn = PEn[Bptr];
	BEn += doppler::GetELoss(BEn,DEADLAYER,1,"TS");
	float TEn = PEn[Tptr];
	TEn += doppler::GetELoss(TEn,DEADLAYER,1,"TS");
	float BTh = doppler::GetPTh(Bann);
	float TTh = doppler::GetPTh(Tann);
	float BPh = doppler::GetPPhi(Pquad[Bptr],Psec[Bptr]);
	float TPh = doppler::GetPPhi(Pquad[Tptr],Psec[Tptr]);

	T_dcB_x[Tann]->Fill(GEn*doppler::DC(BEn, BTh, BPh, GTh, GPh, AP), weight);	
	T_dcT_x[Tann]->Fill(GEn*doppler::DC(TEn, TTh, TPh, GTh, GPh, AT), weight);

	B_dcB_x[Bann]->Fill(GEn*doppler::DC(BEn, BTh, BPh, GTh, GPh, AP), weight);	
	B_dcT_x[Bann]->Fill(GEn*doppler::DC(TEn, TTh, TPh, GTh, GPh, AT), weight);

	if( Tann >= minrecoil && Tann <= maxrecoil ) {

		T_2hdcB->Fill(GEn*doppler::DC(BEn,BTh,BPh,GTh,GPh,AP), weight);
		T_2hdcT->Fill(GEn*doppler::DC(TEn,TTh,TPh,GTh,GPh,AT), weight);
		
	}

	return;

}

void hists::FillGamGam0h( float GEn, float GTh, float GPh, vector <float> GCor_GEn, vector <float> GCor_GTh,
						 vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd ) {
	
	for( unsigned int i = 0; i < GCor_GEn.size(); i++ ) {
		
		if( GCor_Gtd.at(i)*25. > -100. && GCor_Gtd.at(i)*25. < 150. ) {
				
			gg->Fill( GEn, GCor_GEn.at(i) );
				
		}

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
		//TEn += doppler::GetELoss(TEn,DEADLAYER,1,"TS");
		TTh = doppler::GetPTh(Pann);
		TPh = doppler::GetPPhi(Pquad,Psec);
		
		BEn = doppler::GetBEn(PEn,Pann);
		BTh = doppler::GetBTh(Pann);
		BPh = doppler::GetQPhi(Pquad,Psec);

	}
	
	// Projectile
	else if( cut > 0 ){
		
		return; // only target gates
		
		BEn = PEn;
		BEn += doppler::GetELoss(BEn,DEADLAYER,1,"TS");
		BTh = doppler::GetPTh(Pann);
		BPh = doppler::GetPPhi(Pquad,Psec);
		
		TEn = doppler::GetTEn(PEn,Pann);
		TTh = doppler::GetTTh(Pann,PEn);
		TPh = doppler::GetQPhi(Pquad,Psec);
		
	}
	
	for( unsigned int i = 0; i < GCor_GEn.size(); i++ ) {
		
		gg_td->Fill( GCor_Gtd.at(i)*25. );
			
		if( GCor_Gtd.at(i)*25. > -100. && GCor_Gtd.at(i)*25. < 150. ) {
				
			pp->Fill(GEn, GCor_GEn.at(i), weight);
			gg_dcT->Fill( GEn*doppler::DC(TEn,TTh,TPh,GTh,GPh,AT),
						 GCor_GEn.at(i)*doppler::DC(TEn,TTh,TPh,GCor_GTh.at(i),GCor_GPh.at(i),AT) );
			gg_dcB->Fill( GEn*doppler::DC(BEn,BTh,BPh,GTh,GPh,AP),
						 GCor_GEn.at(i)*doppler::DC(BEn,BTh,BPh,GCor_GTh.at(i),GCor_GPh.at(i),AP) );
				
		}

	}
	
	return;
	
}

void hists::FillGamGam2h( float GEn, float GTh, float GPh, vector <float> GCor_GEn, vector <float> GCor_GTh,
						 vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd, vector<float> PEn,
						 vector<int> Pann, vector<int> Psec, vector<int> Pquad, Int_t Bptr, Int_t Tptr, float weight ) {
	
	int Bann = Pann[Bptr];
	int Tann = Pann[Tptr];
	float BEn = PEn[Bptr];
	BEn += doppler::GetELoss(BEn,DEADLAYER,1,"TS");
	float TEn = PEn[Tptr];
	TEn += doppler::GetELoss(TEn,DEADLAYER,1,"TS");
	float BTh = doppler::GetPTh(Bann);
	float TTh = doppler::GetPTh(Tann);
	float BPh = doppler::GetPPhi(Pquad[Bptr],Psec[Bptr]);
	float TPh = doppler::GetPPhi(Pquad[Tptr],Psec[Tptr]);
	
	if( Tann >= minrecoil && Tann <= maxrecoil ) {
		
		for( unsigned int i = 0; i < GCor_GEn.size(); i++ ) {
			
			gg_td->Fill( GCor_Gtd.at(i)*25. );
		
			if( GCor_Gtd.at(i)*25. > -100. && GCor_Gtd.at(i)*25. < 150. ) {
					
				pp->Fill(GEn, GCor_GEn.at(i), weight); // need to add DC versions
				gg_dcT->Fill( GEn*doppler::DC(TEn,TTh,TPh,GTh,GPh,AT),
							 GCor_GEn.at(i)*doppler::DC(TEn,TTh,TPh,GCor_GTh.at(i),GCor_GPh.at(i),AT) );
				gg_dcB->Fill( GEn*doppler::DC(BEn,BTh,BPh,GTh,GPh,AP),
							 GCor_GEn.at(i)*doppler::DC(BEn,BTh,BPh,GCor_GTh.at(i),GCor_GPh.at(i),AP) );
					
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
		TEn += doppler::GetELoss(TEn,DEADLAYER,1,"TS");
		TTh = doppler::GetPTh(Pann);
		TPh = doppler::GetPPhi(Pquad,Psec);
		
		BEn = doppler::GetBEn(PEn,Pann);
		BTh = doppler::GetBTh(Pann);
		BPh = doppler::GetQPhi(Pquad,Psec);

	}
	
	// Projectile
	else if( cut > 0 ){
	
		BEn = PEn;
		BEn += doppler::GetELoss(BEn,DEADLAYER,1,"TS");
		BTh = doppler::GetPTh(Pann);
		BPh = doppler::GetPPhi(Pquad,Psec);
		
		TEn = doppler::GetTEn(PEn,Pann);
		TTh = doppler::GetTTh(Pann,PEn);
		TPh = doppler::GetQPhi(Pquad,Psec);
		
	}
	
	else return;
	
	// Move BPh and TPh
	BPh -= (float)(int)((PHI_NSTEPS-1)/2) * PHI_STEP_WIDTH * TMath::DegToRad();
	TPh -= (float)(int)((PHI_NSTEPS-1)/2) * PHI_STEP_WIDTH * TMath::DegToRad();

	// Make steps either side of the offset
	for( int j = 0; j < PHI_NSTEPS; j++ ) {

		BPh += (float)j * PHI_STEP_WIDTH * TMath::DegToRad();
		TPh += (float)j * PHI_STEP_WIDTH * TMath::DegToRad();
		phical_dcT[j]->Fill( GEn*doppler::DC(TEn,TTh,TPh,GTh,GPh,AT), weight );
		phical_dcB[j]->Fill( GEn*doppler::DC(BEn,BTh,BPh,GTh,GPh,AP), weight );

	}

#endif

	return;
	
}

void hists::FillPar1h( float PEn, Int_t Pann, Int_t Psec, Int_t Pquad, Int_t cut, float weight ) {
	
	float PTh = doppler::GetPTh(Pann)*TMath::RadToDeg();	
	float BTh, TTh, BEn, TEn;

	part1h->Fill(PTh, PEn/1000.);
	partQ[Pquad]->Fill(PTh, PEn/1000.);
	part_ann->Fill(Pann, PEn/1000.);
	
	if( cut == 0 ) {
	
		BTh = doppler::GetBTh(Pann)*TMath::RadToDeg();
		BEn = doppler::GetBEn(PEn,Pann);
		BEn -= doppler::GetELoss(BEn,DEADLAYER,0,"TS");

		Th->Fill(PTh,PEn/1000.);
		T1h->Fill(PTh,PEn/1000.);
		Bsim->Fill(BTh,BEn/1000.);

	}
	
	else if( cut > 0 ) {
	
		TTh = doppler::GetTTh(Pann,PEn)*TMath::RadToDeg();
		TEn = doppler::GetTEn(PEn,Pann);
		TEn -= doppler::GetELoss(TEn,DEADLAYER,0,"TS");
	
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
	float BTh = doppler::GetPTh(Bann)*TMath::RadToDeg();
	float TTh = doppler::GetPTh(Tann)*TMath::RadToDeg();
	float BPh = doppler::GetPPhi(Bquad,Bsec)*TMath::RadToDeg();
	float TPh = doppler::GetPPhi(Tquad,Tsec)*TMath::RadToDeg();

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

void hists::AddSpectra( float bg_frac, float del_frac ) {

	pmr->Add(p,r,1.0,bg_frac);
	dmr->Add(d,r,1.0,del_frac);
	
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
