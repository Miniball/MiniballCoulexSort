#ifndef hists_cxx
#define hists_cxx

#define GBINS 2000	// number of bins in gamma spectra
#define GMAX 2000	// maximum energy in gamma spectra
#define EBINS 1500	// number of bins in electron spectra
#define EMAX 3000	// maximum energy in electron spectra
#define PBINS 300	// number of bins in particle spectra
#define PMAX 1200	// maximum energy in particle spectra
#define TBINS 242	// number of bins in tdiff spectra
#define TMAX 1525	// maximum time in tdiff spectra

#define PTCUT_P 2.45e7	// Proton time cut limit for prompt
#define PTCUT_D 4.90e7	// Proton time cut limit for delayed
#define PD_FRAC -0.85	// scaling factor of delayed window for subtraction

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

	p = new TH1F("p","Prompt gammas;Energy [keV];Counts per 1 keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	r = new TH1F("r","Random gammas;Energy [keV];Counts per 1 keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	pe = new TH1F("pe","Prompt electrons;Energy [keV];Counts per 1 keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	re = new TH1F("re","Random electrons;Energy [keV];Counts per 1 keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	hname = "Prompt minus random;Energy [keV];Counts per " + dc.convertInt( (int)(GMAX/GBINS) ) + " keV";
	pmr = new TH1F("pmr",hname.c_str(),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	pemre = new TH1F("pemre",hname.c_str(),EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	pB = new TH1F("pB","Prompt gammas gated on beam",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	rB = new TH1F("rB","Random gammas gated on beam",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	pT = new TH1F("pT","Prompt gammas gated on target",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	rT = new TH1F("rT","Random gammas gated on target",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	peB = new TH1F("peB","Prompt electrons gated on beam",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	reB = new TH1F("reB","Random electrons gated on beam",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	peT = new TH1F("peT","Prompt electrons gated on target",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	reT = new TH1F("reT","Random electrons gated on target",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	pBmrB = new TH1F("pBmrB","Prompt gated gammas on beam minus random gated on beam",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	pTmrT = new TH1F("pTmrT","Prompt gated gammas on target minus random gated on target",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	peBmreB = new TH1F("peBmreB","Prompt gated electrons on beam minus random gated on beam",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	peTmreT = new TH1F("peTmreT","Prompt gated electrons on target minus random gated on target",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	p_1B = new TH1F("p_1B","#gamma-rays in prompt coincidence with 1-beam particle",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	r_1B = new TH1F("r_1B","#gamma-rays in random coincidence with 1-beam particle",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	p_1T = new TH1F("p_1T","#gamma-rays in prompt coincidence with 1-target particle",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	r_1T = new TH1F("r_1T","#gamma-rays in random coincidence with 1-target particle",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	p_2h = new TH1F("p_2h","#gamma-rays in prompt coincidence with 2-particles",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	r_2h = new TH1F("r_2h","#gamma-rays in random coincidence with 2-particles",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	pe_1B = new TH1F("pe_1B","electrons in prompt coincidence with 1-beam particle",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	re_1B = new TH1F("re_1B","electrons in random coincidence with 1-beam particle",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	pe_1T = new TH1F("pe_1T","electrons in prompt coincidence with 1-target particle",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	re_1T = new TH1F("re_1T","electrons in random coincidence with 1-target particle",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	pe_2h = new TH1F("pe_2h","electrons in prompt coincidence with 2-particles",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	re_2h = new TH1F("re_2h","electrons in random coincidence with 2-particles",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	
	// matrices
	gg = new TH2F("gg","Gamma-gamma matrix;Energy [keV];Energy [keV];Counts",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	//ge = new TH2F("ge","Gamma-electron matrix;Energy [keV];Energy [keV];Counts",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	gg_dcB = new TH2F("gg_dcB","Gamma-gamma matrix, DC for beam;Energy [keV];Energy [keV];Counts",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	gg_dcT = new TH2F("gg_dcT","Gamma-gamma matrix, DC for target;Energy [keV];Energy [keV];Counts",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS),GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	gg_td = new TH1F("gg_td","#gamma-#gamma time difference;#tau (t_{#gamma} - t_{#gamma}) [ns];Counts per 25 ns",TBINS,TMAX-25*TBINS,TMAX); // Total
	ge_td = new TH1F("ge_td","electron-#gamma time difference;#tau (t_{e^{-}} - t_{#gamma}) [ns];Counts per 25 ns",TBINS,TMAX-25*TBINS,TMAX); // Total
	gcor_size = new TH1F("gcor_size","gcor_size;Number of correlated gammas/electrons;frequency",100,-0.5,99.5);
	
	// Background subtracted, Doppler corrected spectra
	B_dcB = new TH1F("B_dcB","Beam gated, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_dcT = new TH1F("B_dcT","Beam gated, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_1hdcB = new TH1F("B_1hdcB","Beam gated, 1-p, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_1hdcT = new TH1F("B_1hdcT","Beam gated, 1-p, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_nodc = new TH1F("B_nodc","Beam gated, background subtracted #gamma-rays, no DC;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	gam_dcB = new TH1F("gam_dcB","Total statistics for gamma rays, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));

	Be_dcB = new TH1F("Be_dcB","Beam gated electrons, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	Be_dcT = new TH1F("Be_dcT","Beam gated electrons, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	Be_1hdcB = new TH1F("Be_1hdcB","Beam gated electrons, 1-p, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	Be_1hdcT = new TH1F("Be_1hdcT","Beam gated electrons, 1-p, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	elec_dcB = new TH1F("elec_dcB","Total statistics for electrons, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	
	T_dcT = new TH1F("T_dcT","Target gated, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_dcB = new TH1F("T_dcB","Target gated, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_1hdcT = new TH1F("T_1hdcT","Target gated, 1-p, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_1hdcB = new TH1F("T_1hdcB","Target gated, 1-p, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_nodc = new TH1F("T_nodc","Target gated, background subtracted #gamma-rays, no DC;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	gam_dcT = new TH1F("gam_dcT","Total statistics for gamma rays, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_2hdcT = new TH1F("T_2hdcT","T_2hit;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_2hdcB = new TH1F("T_2hdcB","T_2hit DC for beam;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));

	//p_thetaB = new TH2F("p_thetaB","Uncorrected gamma-ray energies versus beam particle-gamma angle; Energy [keV]; Theta [deg]", 1500, 0, 1500, 181, -0.5, 180.5);
	//p_thetaT = new TH2F("p_thetaT","Uncorrected gamma-ray energies versus target particle-gamma angle; Energy [keV]; Theta [deg]", 1500, 0, 1500, 181, -0.5, 180.5);

	Te_dcT = new TH1F("Te_dcT","Target gated electrons, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	Te_dcB = new TH1F("Te_dcB","Target gated electrons, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	Te_1hdcT = new TH1F("Te_1hdcT","Target gated electrons, 1-p, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	Te_1hdcB = new TH1F("Te_1hdcB","Target gated electrons, 1-p, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	elec_dcT = new TH1F("elec_dcT","Total statistics for electrons, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	Te_2hdcT = new TH1F("Te_2hdcT","Te_2hit;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));
	Te_2hdcB = new TH1F("Te_2hdcB","Te_2hit DC for beam;Energy [keV];Counts per 1keV",EBINS,-0.5*((float)EMAX/(float)EBINS),EMAX-0.5*((float)EMAX/(float)EBINS));

	T_dcB_x = new TH2F("T_dcB_x","Target gated by lab angle, background subtracted #gamma rays, Doppler corrected for scattered projectile;Target lab angle [deg.];Energy [keV];Counts per 1keV",64,cd_angles,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_dcT_x = new TH2F("T_dcT_x","Target gated by lab angle, background subtracted #gamma rays, Doppler corrected for scattered target;Target lab angle [deg.];Energy [keV];Counts per 1keV",64,cd_angles,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_nodc_x = new TH2F("T_nodc_x","Target gated by lab angle, background subtracted #gamma rays;Target lab angle [deg.];Energy [keV];Counts per 1keV",64,cd_angles,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_dcB_x = new TH2F("B_dcB_x","Beam gated by lab angle, background subtracted #gamma rays, Doppler corrected for scattered projectile;Beam lab angle [deg.];Energy [keV];Counts per 1keV",64,cd_angles,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_dcT_x = new TH2F("B_dcT_x","Beam gated by lab angle, background subtracted #gamma rays, Doppler corrected for scattered target;Beam lab angle [deg.];Energy [keV];Counts per 1keV",64,cd_angles,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_nodc_x = new TH2F("B_nodc_x","Beam gated by lab angle, background subtracted #gamma rays;Beam lab angle [deg.];Energy [keV];Counts per 1keV",64,cd_angles,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));

	// Proton time gated spectra
	T_T1T_dcT = new TH1F("T_T1T_dcT","Target gated, prompt proton gated, delayed proton gate subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_T1T_dcB = new TH1F("T_T1T_dcB","Target gated, prompt proton gated, delayed proton gate subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_T1T_dcT_p = new TH1F("T_T1T_dcT_p","Target gated, prompt proton gated, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_T1T_dcB_p = new TH1F("T_T1T_dcB_p","Target gated, prompt proton gated, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_T1T_dcT_d = new TH1F("T_T1T_dcT_d","Target gated, delayed proton gated, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_T1T_dcB_d = new TH1F("T_T1T_dcB_d","Target gated, delayed proton gated, background subtracted, Doppler corrected for scattered projectile;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));

	B_T1T_dcB = new TH1F("B_T1T_dcB","Beam gated, prompt proton gated, delayed proton gate subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_T1T_dcT = new TH1F("B_T1T_dcT","Beam gated, prompt proton gated, delayed proton gate subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_T1T_dcB_p = new TH1F("B_T1T_dcB_p","Beam gated, prompt proton gated, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_T1T_dcT_p = new TH1F("B_T1T_dcT_p","Beam gated, prompt proton gated, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_T1T_dcB_d = new TH1F("B_T1T_dcB_d","Beam gated, delayed proton gated, background subtracted, Doppler corrected #gamma-rays;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_T1T_dcT_d = new TH1F("B_T1T_dcT_d","Beam gated, delayed proton gated, background subtracted, Doppler corrected for target recoil;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));

//	T_2D_T1T_dcT = new TH2F("T_2D_T1T_dCT", "Target gated, background subtracted, Doppler corrected #gamma-rays vs. Proton time;#Delta T;Energy [keV]", 1000,0,3e8,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
//	T_2D_T1T_dcB = new TH2F("T_2D_T1T_dCB", "Target gated, background subtracted, Doppler corrected for scattered projectile vs. Proton time;#Delta T;Energy [keV]", 1000,0,3e8,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
//	B_2D_T1T_dcB = new TH2F("B_2D_T1T_dCB", "Beam gated, background subtracted, Doppler corrected #gamma-rays vs. Proton time;#Delta T;Energy [keV]", 1000,0,3e8,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
//	B_2D_T1T_dcT = new TH2F("B_2D_T1T_dCT", "Beam gated, background subtracted, Doppler corrected #gamma-rays vs. Proton time;#Delta T;Energy [keV]", 1000,0,3e8,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));


	// Gated on delayed gamma-rays
	T_del_2h = new TH1F("T_del_2h","T_2hit, delayed, no DC;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_del_2hdcB = new TH1F("T_del_2hdcB","T_2hit, delayed, DC for beam;Energy [keV];Counts per 1keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	
	// Doppler corrected spectra per detector
	B_dcB_cid = new TH2F("B_dcB_cid","Beam gated prompt, Doppler corrected #gamma-rays;Core ID;Energy [keV]",24,-0.5,23.5,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_dcB_cid = new TH2F("T_dcB_cid","Target gated prompt, Doppler corrected #gamma-rays;Core ID;Energy [keV]",24,-0.5,23.5,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	B_dcT_cid = new TH2F("B_dcT_cid","Beam gated prompt, Doppler corrected #gamma-rays;Core ID;Energy [keV]",24,-0.5,23.5,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	T_dcT_cid = new TH2F("T_dcT_cid","Target gated prompt, Doppler corrected #gamma-rays;Core ID;Energy [keV]",24,-0.5,23.5,GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	
	// PART : 1 hit
	// Prompt/Random Particle 1 hit
	part = new TH2F("part","Detected particle events;Lab angle [deg];Energy [MeV]",64,cd_angles,PBINS,0,PMAX);
	part1h = new TH2F("part1h","Detected particle events;Lab angle [deg];Energy [MeV]",64,cd_angles,PBINS,0,PMAX);
	part2h = new TH2F("part2h","Detected particle events;Lab angle [deg];Energy [MeV]",64,cd_angles,PBINS,0,PMAX);
	Bh = new TH2F("Bh","Beam-like events;Lab angle [deg];Energy [MeV]",64,cd_angles,PBINS,0,PMAX);
	Th = new TH2F("Th","Target-like events;Lab angle [deg];Energy [MeV]",64,cd_angles,PBINS,0,PMAX);
	B1h = new TH2F("B1h","Beam-like 1-particle events;Lab angle [deg];Energy [MeV]",64,cd_angles,PBINS,0,PMAX);
	T1h = new TH2F("T1h","Target-like 1-particle events;Lab angle [deg];Energy [MeV]",64,cd_angles,PBINS,0,PMAX);
	B2h = new TH2F("B2h","Beam-like 2-particle events;Lab angle [deg];Energy [MeV]",64,cd_angles,PBINS,0,PMAX);
	T2h = new TH2F("T2h","Target-like 2-particle events;Lab angle [deg];Energy [MeV]",64,cd_angles,PBINS,0,PMAX);
	target_ev = new TH1F("target_ev","Number of deconstructed recoil events per degree;Lab angle [deg];Number of events per degree",64,cd_angles);
	target_1pev = new TH1F("target_1pev","Number of recoil events, without coincidence, per degree;Lab angle [deg];Number of events per degree",64,cd_angles);
	target_2pev = new TH1F("target_2pev","Number of recoil events, coincident with a projectile, per degree;Lab angle [deg];Number of events per degree",64,cd_angles);
	for(int i=0; i<4; i++){
		hname = "partQ" + dc.convertInt(i+1);
		partQ[i] = new TH2F(hname.c_str(),"Random subtracted 1-particle events;Lab angle [deg];Energy [MeV]",64,cd_angles,PBINS,0,PMAX);
	}
	
#ifdef TWOPART
	for(int i=0; i<16; i++){
		hname = "B" + dc.convertInt(i);
		BT[i] = new TH2F(hname.c_str(),hname.c_str(),64,cd_angles,PBINS,0,PMAX);
		hname = "T" + dc.convertInt(i);
		TB[i] = new TH2F(hname.c_str(),hname.c_str(),64,cd_angles,PBINS,0,PMAX);
	}
#endif

	// Second hit - simulated etc.
	Bsim = new TH2F("Bsim","Simulated beam particle after target detection;Lab angle [deg];Energy [MeV]",90,0,180,PBINS,-PMAX,PMAX);
	Tsim = new TH2F("Tsim","Simulated target particle after beam detection;Lab angle [deg];Energy [MeV]",90,0,180,PBINS,-PMAX,PMAX);
	en2hit = new TH2F("en2hit","Energy of beam vs. Energy of target;Beam energy [MeV];Target energy [MeV]",PBINS,0,PMAX,PBINS,0,PMAX);
	sum2hit = new TH1F("sum2hit","Sum energy of beam target;Sum energy [MeV];Counts",PBINS,0,PMAX);
	
	// Testing
	multp = new TH1F("multp","Prompt Particle multiplicity;Number of particles;Number of events",16,-0.5,15.5); 
	multr = new TH1F("multr","Random Particle multiplicity;Number of particles;Number of events",16,-0.5,15.5);
	velo = new TH1F( "velo", "Velocity distribution", 1500, 0, 0.15 );
	velo2 = new TH1F( "velo2", "Velocity distribution", 1500, 0, 0.15 );
	GeReject = new TH1F("GeReject","Events with Ge angle equal to zero;Cluster Number",9,-0.5,8.5);
	GePass = new TH1F("GePass","Events with Ge angle greater than zero;Cluster Number",9,-0.5,8.5); 
	GeRatio = new TH1F("GeRatio","Ratio of Ge events with angle less than or equal to zero to those with real angles;Cluster Number",9,-0.5,8.5); 
	GeAng = new TH2F("GeAng","Detector angles;#theta;#phi",45,0,180,90,0,359);
#ifdef GEANG
	GeSiAng = new TH3F("GeSiAng","Ge and Si coincidences angles;Ge #theta;Si #theta;#phi",45,0,180,30,0,60,270,-720,360);
	for(int i=0; i<8; i++){
		hname = "GeAng_clu" + dc.convertInt(i+1);
		htitle = "Detector angles in Cluster " + dc.convertInt(i+1) + ";#theta;#phi";
		GeAng_clu[i] = new TH2F(hname.c_str(),htitle.c_str(),45,0,180,90,0,359);
		hname = "GeSiAng_clu" + dc.convertInt(i+1);
		htitle = "Ge and Si coincidences angles in Cluster " + dc.convertInt(i+1) + ";Ge #theta;Si #theta;#phi";
		GeSiAng_clu[i] = new TH3F(hname.c_str(),htitle.c_str(),45,0,180,30,0,60,270,-720,360);
	}
#endif

	// gamma - particle time difference
	tdiff = new TH1F("tdiff","Particle-#gamma time difference;#tau (t_{p} - t_{#gamma}) [ns];Counts per 25 ns",TBINS,TMAX-25*TBINS,TMAX); // Total
	tdiff_e = new TH1F("tdiff_e","Particle-electron time difference;#tau (t_{p} - t_{e^{-}}) [ns];Counts per 25 ns",TBINS,TMAX-25*TBINS,TMAX); // Total
	tegate[0] = 85.;
	tegate[1] = 166.;
	tegate[2] = 228.;
	tegate[3] = 881.;
	for(int i=0; i<4; i++){
		hname = "tdiffE" + dc.convertInt(tegate[i]);
		htitle = "Particle-#gamma time difference gated around " + dc.convertInt(tegate[i]) + " keV;#tau (t_{p} - t_{#gamma}) [ns];Counts per 25ns";		
		tdiffE[i] = new TH1F(hname.c_str(),htitle.c_str(),TBINS,TMAX-25*TBINS,TMAX);
		hname = "tdiffQ" + dc.convertInt(i+1);
		htitle = "Particle-#gamma time difference in quadrant " + dc.convertInt(i+1) + ";#tau (t_{p} - t_{#gamma}) [ns];Counts per 25ns";		
		tdiffQ[i] = new TH1F(hname.c_str(),hname.c_str(),TBINS,TMAX-25*TBINS,TMAX);
	}
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

void hists::Fill1h( float GEn, float GTh, float GPh, int GCid, vector <float> GCor_GEn, vector <float> GCor_GTh,
					vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd, bool electron,
					float PEn, int Pnf, int Pnb, int Psec, int Pquad, int T1T, float weight ) {

	if( !electron ) {
	
		if( weight > 0 ) p->Fill( GEn );
		else r->Fill( GEn );

	}
	else {
	
		if( weight > 0 ) pe->Fill( GEn );
		else re->Fill( GEn );

	}

	int cut = dc.Cut( PEn, Pnf, Pquad, Psec );
	
	FillPar1h( PEn, Pnf, Pnb, Psec, Pquad, cut, weight );

	if( cut >= 0 ) {
	
		if( !electron ) {
	
			FillGam1h( GEn, GTh, GPh, GCid, PEn, Pnf, Pnb, Psec, Pquad, T1T, cut, weight );
			FillGamGam1h( GEn, GTh, GPh, GCid, GCor_GEn, GCor_GTh, GCor_GPh, GCor_CluID, GCor_Gtd,
							PEn, Pnf, Pnb, Psec, Pquad, T1T, cut, weight );
		
		}

		else {
			
			FillElec1h( GEn, GTh, GPh, GCid, PEn, Pnf, Pnb, Psec, Pquad, cut, weight );

		}

	}
	
	return;

}

void hists::Fill2h( float GEn, float GTh, float GPh, int GCid, vector <float> GCor_GEn, vector <float> GCor_GTh,
					vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd, bool electron,
					vector<float> PEn, vector<int> Pnf, vector<int> Pnb, vector<int> Psec, vector<int> Pquad,
				    vector<int> Pptr, vector<float> td, int T1T, float weight ) {

	float Bptr, Tptr;

	float time_diff = TMath::Abs(td[Pptr[0]]-td[Pptr[1]]);
	int quad_diff = TMath::Abs(Pquad[Pptr[0]]-Pquad[Pptr[1]]);
	int cut2 = dc.Cut_2p(PEn[Pptr[0]],Pnf[Pptr[0]],Pquad[Pptr[0]],Psec[Pptr[0]],
						 PEn[Pptr[1]],Pnf[Pptr[1]],Pquad[Pptr[1]],Psec[Pptr[1]]);
	bool cutg_0 = dc.CutG_en2hit(PEn[Pptr[1]]/1000.,PEn[Pptr[0]]/1000.);
	bool cutg_1 = dc.CutG_en2hit(PEn[Pptr[0]]/1000.,PEn[Pptr[1]]/1000.);
	int cut_0 = dc.Cut(PEn[Pptr[0]],Pnf[Pptr[0]],Pquad[Pptr[0]],Psec[Pptr[0]]);
	int cut_1 = dc.Cut(PEn[Pptr[1]],Pnf[Pptr[1]],Pquad[Pptr[1]],Psec[Pptr[1]]);

	if( quad_diff == 2 && time_diff <= ppwin && cut2 == 0 && cutg_0 ) { // target is [0]

		Bptr = Pptr[1];
		Tptr = Pptr[0];
		if( !electron ){
			FillGam2h( GEn, GTh, GPh, GCid, PEn, Pnf, Pnb, Psec, Pquad, Bptr, Tptr, T1T, weight );
		}
		else FillElec2h( GEn, GTh, GPh, GCid, PEn, Pnf, Pnb, Psec, Pquad, Bptr, Tptr, weight );
		FillPar2h( PEn, Pnf, Pnb, Psec, Pquad, Bptr, Tptr, weight );
		
	}

	else if( quad_diff == 2 && time_diff <= ppwin && cut2 == 1 && cutg_1 ) { // target is [1]

		Bptr = Pptr[0];
		Tptr = Pptr[1];
		if( !electron ) {
			FillGam2h( GEn, GTh, GPh, GCid, PEn, Pnf, Pnb, Psec, Pquad, Bptr, Tptr, T1T, weight );
		}
		else FillElec2h( GEn, GTh, GPh, GCid, PEn, Pnf, Pnb, Psec, Pquad, Bptr, Tptr, weight );
		FillPar2h( PEn, Pnf, Pnb, Psec, Pquad, Bptr, Tptr, weight );
		
	}

	else if( cut_0 >= 0 ) { // Pptr[0] is a 1p event
	
		if( !electron ){
			FillGam1h( GEn, GTh, GPh, GCid, PEn[Pptr[0]], Pnf[Pptr[0]], Pnb[Pptr[0]], Psec[Pptr[0]], Pquad[Pptr[0]], T1T, cut_0, weight );
		}
		else FillElec1h( GEn, GTh, GPh, GCid, PEn[Pptr[0]], Pnf[Pptr[0]], Pnb[Pptr[0]], Psec[Pptr[0]], Pquad[Pptr[0]], cut_0, weight );
		FillPar1h( PEn[Pptr[0]], Pnf[Pptr[0]], Pnb[Pptr[0]], Psec[Pptr[0]], Pquad[Pptr[0]], cut_0, weight );
		
	}
	
	else if( cut_1 >= 0 ) { // Pptr[1] is a 1p event
	
		if( !electron ){
			FillGam1h( GEn, GTh, GPh, GCid, PEn[Pptr[1]], Pnf[Pptr[1]], Pnb[Pptr[1]], Psec[Pptr[1]], Pquad[Pptr[1]], T1T, cut_1, weight );
		}
		else FillElec1h( GEn, GTh, GPh, GCid, PEn[Pptr[1]], Pnf[Pptr[1]], Pnb[Pptr[1]], Psec[Pptr[1]], Pquad[Pptr[1]], cut_1, weight );
		FillPar1h( PEn[Pptr[1]], Pnf[Pptr[1]], Pnb[Pptr[1]], Psec[Pptr[1]], Pquad[Pptr[1]], cut_1, weight );
		
	}
	
	return;

}

void hists::FillGam1h( float GEn, float GTh, float GPh, int GCid, float PEn, int Pnf,
						int Pnb, int Psec, int Pquad, int T1T, int cut, float weight ) {


	float BEn, TEn, BTh, TTh, BPh, TPh;

	// Target
	if( cut == 0 ) {
	
		if( weight > 0 ) p_1T->Fill(GEn);
		else r_1T->Fill(GEn);
		
		TEn = PEn;
		TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
		TTh = dc.GetPTh(Pnf,Psec);
		TPh = dc.GetPPhi(Pquad,Pnb,Psec);
		
		BEn = dc.GetBEn(PEn,Pnf,Psec);
		BTh = dc.GetBTh(Pnf,Psec);
		BPh = dc.GetQPhi(Pquad,Pnb,Psec);

		T_dcB_x->Fill(TTh*TMath::RadToDeg(),GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
		T_dcT_x->Fill(TTh*TMath::RadToDeg(),GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);
		T_nodc_x->Fill(TTh*TMath::RadToDeg(),GEn, weight);
		T_nodc->Fill(GEn, weight);

		T_1hdcB->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
		T_1hdcT->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);
		
		T_dcB_cid->Fill(GCid,GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
		T_dcT_cid->Fill(GCid,GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);
		
		//p_thetaB->Fill(GEn, dc.GammaAng(BTh, BPh, GTh, GPh)*TMath::RadToDeg());
		//p_thetaT->Fill(GEn, dc.GammaAng(TTh, TPh, GTh, GPh)*TMath::RadToDeg());

		// Proton time gated spectra
		if( T1T >= 0 && T1T < PTCUT_P ) {

			T_T1T_dcB_p->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
			T_T1T_dcT_p->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);

			if( weight > 0 ) {

				T_T1T_dcB->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
				T_T1T_dcT->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);

			}

		}

		else if( T1T >= PTCUT_P && T1T < PTCUT_D ) {

			T_T1T_dcB_d->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
			T_T1T_dcT_d->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);

			if( weight > 0 ) {

				T_T1T_dcB->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), PD_FRAC*weight);
				T_T1T_dcT->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), PD_FRAC*weight);

			}

		}

	}
	
	// Projectile
	else if( cut > 0 ){
	
		if( weight > 0 ) p_1B->Fill(GEn);
		else r_1B->Fill(GEn);
		
		BEn = PEn;
		BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"BS");
		BTh = dc.GetPTh(Pnf,Psec);
		BPh = dc.GetPPhi(Pquad,Pnb,Psec);
		
		TEn = dc.GetTEn(PEn,Pnf,Psec);
		TTh = dc.GetTTh(Pnf,PEn,Psec);
		TPh = dc.GetQPhi(Pquad,Pnb,Psec);

		B_dcB_x->Fill(BTh*TMath::RadToDeg(),GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
		B_dcT_x->Fill(BTh*TMath::RadToDeg(),GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);
		B_nodc_x->Fill(BTh*TMath::RadToDeg(),GEn, weight);
		B_nodc->Fill(GEn, weight);

		B_1hdcB->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
		B_1hdcT->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);

		B_dcB_cid->Fill(GCid,GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
		B_dcT_cid->Fill(GCid,GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);

		//p_thetaB->Fill(GEn, dc.GammaAng(BTh, BPh, GTh, GPh)*TMath::RadToDeg());
		//p_thetaT->Fill(GEn, dc.GammaAng(TTh, TPh, GTh, GPh)*TMath::RadToDeg());

		velo->Fill( dc.Beta( BEn, dc.GetAb() * dc.u_mass() ) );
		velo2->Fill( TMath::Sqrt( 2.0 * BEn / ( dc.GetAb() * dc.u_mass() ) ) );
		
		// Proton time gated spectra
		if( T1T < PTCUT_P ) {

			B_T1T_dcB_p->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
			B_T1T_dcT_p->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);

			if( weight > 0 ) {

				B_T1T_dcB->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
				B_T1T_dcT->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);

			}

		}

		else if( T1T >= PTCUT_P && T1T < PTCUT_D ) {

			B_T1T_dcB_d->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
			B_T1T_dcT_d->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);

			if( weight > 0 ) {

				B_T1T_dcB->Fill(GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), PD_FRAC*weight);
				B_T1T_dcT->Fill(GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), PD_FRAC*weight);

			}

		}

//		B_2D_T1T_dcB->Fill(T1T,GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
//		B_2D_T1T_dcT->Fill(T1T,GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);

	}

	return;

}

void hists::FillDel2h( float GEn, float GTh, float GPh, int GCid, vector<float> PEn, vector<int> Pnf,
					vector<int> Pnb, vector<int> Psec, vector<int> Pquad, vector<int> Pptr,
					vector<float> td, float weight ) {

	int Bptr, Tptr;

	float time_diff = TMath::Abs(td[Pptr[0]]-td[Pptr[1]]);
	int quad_diff = TMath::Abs(Pquad[Pptr[0]]-Pquad[Pptr[1]]);
	int cut2 = dc.Cut_2p(PEn[Pptr[0]],Pnf[Pptr[0]],Pquad[Pptr[0]],Psec[Pptr[0]],
						 PEn[Pptr[1]],Pnf[Pptr[1]],Pquad[Pptr[1]],Psec[Pptr[1]]);
	bool cutg_0 = dc.CutG_en2hit(PEn[Pptr[1]]/1000.,PEn[Pptr[0]]/1000.);
	bool cutg_1 = dc.CutG_en2hit(PEn[Pptr[0]]/1000.,PEn[Pptr[1]]/1000.);
	int cut_0 = dc.Cut(PEn[Pptr[0]],Pnf[Pptr[0]],Pquad[Pptr[0]],Psec[Pptr[0]]);
	int cut_1 = dc.Cut(PEn[Pptr[1]],Pnf[Pptr[1]],Pquad[Pptr[1]],Psec[Pptr[1]]);

	if( quad_diff == 2 && time_diff <= ppwin && cut2 == 0 && cutg_0 ) { // target is [0]

		Bptr = Pptr[1];
		Tptr = Pptr[0];
		
	}

	else if( quad_diff == 2 && time_diff <= ppwin && cut2 == 1 && cutg_1 ) { // target is [1]

		Bptr = Pptr[0];
		Tptr = Pptr[1];
		
	}

	else return;
	
	int Bnf = Pnf[Bptr];
	int Tnf = Pnf[Tptr];
	float BEn = PEn[Bptr];
	BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"BS");
	float TEn = PEn[Tptr];
	TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
	float BTh = dc.GetPTh(Bnf,Psec[Bptr]);
	float TTh = dc.GetPTh(Tnf,Psec[Tptr]);
	float BPh = dc.GetPPhi(Pquad[Bptr],Pnb[Bptr],Psec[Bptr]);
	float TPh = dc.GetPPhi(Pquad[Tptr],Pnb[Tptr],Psec[Tptr]);

	T_del_2h->Fill( GEn, weight );	
	T_del_2hdcB->Fill( GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight );

	return;

}

void hists::FillGam2h( float GEn, float GTh, float GPh, int GCid, vector<float> PEn, vector<int> Pnf,
						vector<int> Pnb, vector<int> Psec, vector<int> Pquad, int Bptr, int Tptr,
					    int T1T, float weight ) {

	if( weight > 0 ) p_2h->Fill(GEn);
	else r_2h->Fill(GEn);
	
	int Bnf = Pnf[Bptr];
	int Tnf = Pnf[Tptr];
	float BEn = PEn[Bptr];
	BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"BS");
	float TEn = PEn[Tptr];
	TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
	float BTh = dc.GetPTh(Bnf,Psec[Bptr]);
	float TTh = dc.GetPTh(Tnf,Psec[Tptr]);
	float BPh = dc.GetPPhi(Pquad[Bptr],Pnb[Bptr],Psec[Bptr]);
	float TPh = dc.GetPPhi(Pquad[Tptr],Pnb[Tptr],Psec[Tptr]);

	T_dcB_x->Fill(TTh*TMath::RadToDeg(),GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
	T_dcT_x->Fill(TTh*TMath::RadToDeg(),GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);
	T_nodc_x->Fill(TTh*TMath::RadToDeg(),GEn, weight);
	T_nodc->Fill(GEn, weight);

	B_dcB_x->Fill(BTh*TMath::RadToDeg(),GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
	B_dcT_x->Fill(BTh*TMath::RadToDeg(),GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);
	B_nodc_x->Fill(BTh*TMath::RadToDeg(),GEn, weight);
	B_nodc->Fill(GEn, weight);

	B_dcB_cid->Fill(GCid,GEn*dc.DC(BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
	B_dcT_cid->Fill(GCid,GEn*dc.DC(TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);
	
	T_2hdcB->Fill(GEn*dc.DC(BEn,BTh,BPh,GTh,GPh,dc.GetAb()), weight);
	T_2hdcT->Fill(GEn*dc.DC(TEn,TTh,TPh,GTh,GPh,dc.GetAt()), weight);
	
	T_dcB_cid->Fill(GCid,GEn*dc.DC(BEn,BTh,BPh,GTh,GPh,dc.GetAb()), weight);
	T_dcT_cid->Fill(GCid,GEn*dc.DC(TEn,TTh,TPh,GTh,GPh,dc.GetAt()), weight);
	
	return;

}

void hists::FillElec1h( float GEn, float GTh, float GPh, int GCid, float PEn, int Pnf,
						int Pnb, int Psec, int Pquad, int cut, float weight ) {


	float BEn, TEn, BTh, TTh, BPh, TPh;

	// Target
	if( cut == 0 ) {
	
		if( weight > 0 ) pe_1T->Fill(GEn);
		else re_1T->Fill(GEn);
		
		TEn = PEn;
		TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
		TTh = dc.GetPTh(Pnf,Psec);
		TPh = dc.GetPPhi(Pquad,Pnb,Psec);
		
		BEn = dc.GetBEn(PEn,Pnf,Psec);
		BTh = dc.GetBTh(Pnf,Psec);
		BPh = dc.GetQPhi(Pquad,Pnb,Psec);
	
		if( Pnf >= minrecoil && Pnf <= maxrecoil ) {

			Te_1hdcB->Fill(dc.DC_elec(GEn, BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
			Te_1hdcT->Fill(dc.DC_elec(GEn, TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);
			
		}

	}
	
	// Projectile
	else if( cut > 0 ){
	
		if( weight > 0 ) pe_1B->Fill(GEn);
		else re_1B->Fill(GEn);
		
		BEn = PEn;
		BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"BS");
		BTh = dc.GetPTh(Pnf,Psec);
		BPh = dc.GetPPhi(Pquad,Pnb,Psec);
		
		TEn = dc.GetTEn(PEn,Pnf,Psec);
		TTh = dc.GetTTh(Pnf,PEn,Psec);
		TPh = dc.GetQPhi(Pquad,Pnb,Psec);

		Be_1hdcB->Fill(dc.DC_elec(GEn, BEn, BTh, BPh, GTh, GPh, dc.GetAb()), weight);
		Be_1hdcT->Fill(dc.DC_elec(GEn, TEn, TTh, TPh, GTh, GPh, dc.GetAt()), weight);
		
	}

	return;

}

void hists::FillElec2h( float GEn, float GTh, float GPh, int GCid, vector<float> PEn, vector<int> Pnf,
						vector<int> Pnb, vector<int> Psec, vector<int> Pquad, int Bptr, int Tptr,
					    float weight ) {

	if( weight > 0 ) pe_2h->Fill(GEn);
	else re_2h->Fill(GEn);
		
	int Bnf = Pnf[Bptr];
	int Tnf = Pnf[Tptr];
	float BEn = PEn[Bptr];
	BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"BS");
	float TEn = PEn[Tptr];
	TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
	float BTh = dc.GetPTh(Bnf,Psec[Bptr]);
	float TTh = dc.GetPTh(Tnf,Psec[Tptr]);
	float BPh = dc.GetPPhi(Pquad[Bptr],Pnb[Bptr],Psec[Bptr]);
	float TPh = dc.GetPPhi(Pquad[Tptr],Pnb[Tptr],Psec[Tptr]);

	if( Tnf >= minrecoil && Tnf <= maxrecoil ) {

		Te_2hdcB->Fill(dc.DC_elec(GEn, BEn,BTh,BPh,GTh,GPh,dc.GetAb()), weight);
		Te_2hdcT->Fill(dc.DC_elec(GEn, TEn,TTh,TPh,GTh,GPh,dc.GetAt()), weight);
		
	}

	return;

}

void hists::FillGamGam1h( float GEn, float GTh, float GPh, int GCid, vector <float> GCor_GEn, vector <float> GCor_GTh,
						 vector <float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd,
						 float PEn, int Pnf, int Pnb, int Psec, int Pquad, int T1T, int cut, float weight ) {
	
	float BEn = 0, TEn = 0, BTh = 0, TTh = 0, BPh = 0, TPh = 0;

	// Comment below for background subtraction
	if( weight < 0 ) return;
	
	// Target
	if( cut == 0 ) {
		
		if( Pnf < minrecoil || Pnf > maxrecoil ) return;
		
		TEn = PEn;
		TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
		TTh = dc.GetPTh(Pnf,Psec);
		TPh = dc.GetPPhi(Pquad,Pnb,Psec);
		
		BEn = dc.GetBEn(PEn,Pnf,Psec);
		BTh = dc.GetBTh(Pnf,Psec);
		BPh = dc.GetQPhi(Pquad,Pnb,Psec);

	}
	
	// Projectile
	else if( cut > 0 ){
		
		BEn = PEn;
		BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"BS");
		BTh = dc.GetPTh(Pnf,Psec);
		BPh = dc.GetPPhi(Pquad,Pnb,Psec);
		
		TEn = dc.GetTEn(PEn,Pnf,Psec);
		TTh = dc.GetTTh(Pnf,PEn,Psec);
		TPh = dc.GetQPhi(Pquad,Pnb,Psec);
		
	}
	
	for( unsigned int i = 0; i < GCor_GEn.size(); i++ ) {
		
		if( GCor_CluID.at(i) != 8 ) { // gamma
			
			gg_td->Fill( GCor_Gtd.at(i)*25. );
			
			if( GCor_Gtd.at(i) > -15. && GCor_Gtd.at(i) < 15. ) {
				
				gg->Fill(GEn, GCor_GEn.at(i), weight);
				gg_dcT->Fill( GEn*dc.DC(TEn,TTh,TPh,GTh,GPh,dc.GetAt()),
							  GCor_GEn.at(i)*dc.DC(TEn,TTh,TPh,GCor_GTh.at(i),GCor_GPh.at(i),dc.GetAt()),
							  weight );
				gg_dcB->Fill( GEn*dc.DC(BEn,BTh,BPh,GTh,GPh,dc.GetAb()),
							  GCor_GEn.at(i)*dc.DC(BEn,BTh,BPh,GCor_GTh.at(i),GCor_GPh.at(i),dc.GetAb()),
							  weight );
				
			}
			
		}
		
		else if( GCor_CluID[i] == 8 ) { // electron
			
			ge_td->Fill( GCor_Gtd[i]*25. );
			
			if( GCor_Gtd[i]*25. > -980. && GCor_Gtd[i]*25. < -580. ) {
				
				//ge->Fill(GEn, GCor_GEn.at(i), weight); // need to add DC versions
				
			}
			
		}
		
	}
	
	return;
	
}

void hists::FillGamGam2h( float GEn, float GTh, float GPh, int GCid, vector<float> GCor_GEn, vector<float> GCor_GTh,
						 vector<float> GCor_GPh, vector <int> GCor_CluID, vector <float> GCor_Gtd,
						 vector<float> PEn, vector<int> Pnf, vector<int> Pnb,  vector<int> Psec,
						 vector<int> Pquad, int Bptr, int Tptr, int T1T, float weight ) {
	
	int Bnf = Pnf[Bptr];
	int Tnf = Pnf[Tptr];
	float BEn = PEn[Bptr];
	BEn += dc.GetELoss(BEn,dc.GetCDDeadLayer(),1,"BS");
	float TEn = PEn[Tptr];
	TEn += dc.GetELoss(TEn,dc.GetCDDeadLayer(),1,"TS");
	float BTh = dc.GetPTh(Bnf,Psec[Bptr]);
	float TTh = dc.GetPTh(Tnf,Psec[Tptr]);
	float BPh = dc.GetPPhi(Pquad[Bptr],Pnb[Bptr],Psec[Bptr]);
	float TPh = dc.GetPPhi(Pquad[Tptr],Pnb[Tptr],Psec[Tptr]);
	
	// Comment below for background subtraction
	if( weight < 0 ) return;

	for( unsigned int i = 0; i < GCor_GEn.size(); i++ ) {
		
		if( GCor_CluID.at(i) != 8 ) { // gamma
			
			gg_td->Fill( GCor_Gtd.at(i)*25. );
			
			if( GCor_Gtd.at(i) > -15. && GCor_Gtd.at(i) < 15. ) {
				
				gg->Fill(GEn, GCor_GEn.at(i), weight); 
				gg_dcT->Fill( GEn*dc.DC(TEn,TTh,TPh,GTh,GPh,dc.GetAt()),
							  GCor_GEn.at(i)*dc.DC(TEn,TTh,TPh,GCor_GTh.at(i),GCor_GPh.at(i),dc.GetAt()),
							  weight );
				gg_dcB->Fill( GEn*dc.DC(BEn,BTh,BPh,GTh,GPh,dc.GetAb()),
							  GCor_GEn.at(i)*dc.DC(BEn,BTh,BPh,GCor_GTh.at(i),GCor_GPh.at(i),dc.GetAb()),
							  weight );
				
			}
			
		}
		
		else if( GCor_CluID.at(i) == 8 ) { // electron
				
			ge_td->Fill( GCor_Gtd.at(i)*25. );
				
			if( GCor_Gtd.at(i)*25. > -980. && GCor_Gtd.at(i)*25. < -580. ) {
					
				//ge->Fill(GEn, GCor_GEn.at(i), weight); // need to add DC versions
					
			}
				
		}
			
	}
	
	return;
	

	
}

void hists::FillPar1h( float PEn, int Pnf, int Pnb, int Psec, int Pquad, int cut, float weight ) {
	
	float PTh = dc.GetPTh(Pnf,Psec)*TMath::RadToDeg();
	float BTh, TTh, BEn, TEn;

	part1h->Fill(PTh, PEn/1000.);
	partQ[Pquad]->Fill(PTh, PEn/1000.);
	
	if( cut == 0 ) {
	
		BTh = dc.GetBTh(Pnf,Psec)*TMath::RadToDeg();
		BEn = dc.GetBEn(PEn,Pnf,Psec);
		BEn -= dc.GetELoss(BEn,dc.GetCDDeadLayer(),0,"BS");

		Th->Fill(PTh,PEn/1000.);
		T1h->Fill(PTh,PEn/1000.);
		Bsim->Fill(BTh,BEn/1000.);

	}
	
	else if( cut > 0 ) {
	
		TTh = dc.GetTTh(Pnf,PEn,Psec)*TMath::RadToDeg();
		TEn = dc.GetTEn(PEn,Pnf,Psec);
		TEn -= dc.GetELoss(TEn,dc.GetCDDeadLayer(),0,"TS");
	
		Bh->Fill(PTh,PEn/1000.);
		B1h->Fill(PTh,PEn/1000.);
		Tsim->Fill(TTh,TEn/1000.);		
	
	}

	return;

}

void hists::FillPar2h( vector<float> PEn, vector<int> Pnf, vector<int> Pnb, vector<int> Psec,
					   vector<int> Pquad, int Bptr, int Tptr, float weight ) {

	int Bnf = Pnf[Bptr];
	int Tnf = Pnf[Tptr];
	int Bnb = Pnb[Bptr];
	int Tnb = Pnb[Tptr];
	int Bquad = Pquad[Bptr];
	int Tquad = Pquad[Tptr];
	int Bsec = Psec[Bptr];
	int Tsec = Psec[Tptr];
	float BEn = PEn[Bptr];
	float TEn = PEn[Tptr];
	float BTh = dc.GetPTh(Bnf,Bsec)*TMath::RadToDeg();
	float TTh = dc.GetPTh(Tnf,Tsec)*TMath::RadToDeg();
	float BPh = dc.GetPPhi(Bquad,Bnb,Bsec)*TMath::RadToDeg();
	float TPh = dc.GetPPhi(Tquad,Tnb,Tsec)*TMath::RadToDeg();

	Bh->Fill(BTh, BEn/1000.);
	Th->Fill(TTh, TEn/1000.);
	B2h->Fill(BTh, BEn/1000.);
	T2h->Fill(TTh, TEn/1000.);
	partQ[Bquad]->Fill(BTh, BEn/1000.);
	partQ[Tquad]->Fill(TTh, TEn/1000.);
	en2hit->Fill(BEn/1000.,TEn/1000.);
	sum2hit->Fill((BEn+TEn)/1000.);	
#ifdef TWOPART
	BT[Bnf]->Fill(TTh,TEn/1000.);
	TB[Tnf]->Fill(BTh,BEn/1000.);
#endif

	return;

}

void hists::AddSpectra(float bg_frac) {

	pmr->Add(p,r,1.0,bg_frac);
	pemre->Add(pe,re,1.0,bg_frac);
	
	pB->Add(p_1B,p_2h);
	rB->Add(r_1B,r_2h);
	pBmrB->Add(pB,rB,1.0,bg_frac);
	
	pT->Add(p_1T,p_2h);
	rT->Add(r_1T,r_2h);
	pTmrT->Add(pT,rT,1.0,bg_frac);
	
	peB->Add(pe_1B,pe_2h);
	reB->Add(re_1B,re_2h);
	peBmreB->Add(peB,reB,1.0,bg_frac);
	
	peT->Add(pe_1T,pe_2h);
	reT->Add(re_1T,re_2h);
	peTmreT->Add(peT,reT,1.0,bg_frac);
	
	part2h->Add(B2h,T2h);
	part->Add(part1h,part2h);
	
	B_dcB->Add(B_1hdcB,T_2hdcB);
	B_dcT->Add(B_1hdcT,T_2hdcT);
	T_dcB->Add(T_1hdcB,T_2hdcB);
	T_dcT->Add(T_1hdcT,T_2hdcT);
	gam_dcB->Add(B_dcB,T_1hdcB);
	gam_dcT->Add(T_dcT,B_1hdcT);
	
	Be_dcB->Add(Be_1hdcB,Te_2hdcB);
	Be_dcT->Add(Be_1hdcT,Te_2hdcT);
	Te_dcB->Add(Te_1hdcB,Te_2hdcB);
	Te_dcT->Add(Te_1hdcT,Te_2hdcT);
	elec_dcB->Add(Be_dcB,Te_1hdcB);
	elec_dcT->Add(Te_dcT,Be_1hdcT);
	
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
