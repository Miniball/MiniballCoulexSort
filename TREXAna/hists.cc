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

	gp = new TH1F("gp","Prompt gammas;Energy [keV];Counts per 1 keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	gr = new TH1F("gr","Random gammas;Energy [keV];Counts per 1 keV",GBINS,-0.5*((float)GMAX/(float)GBINS),GMAX-0.5*((float)GMAX/(float)GBINS));
	
	// matrices
	pcor_size = new TH1F("pcor_size","gcor_size;Number of correlated particles;frequency",100,-0.5,99.5);

	


	
	// Testing
	multp = new TH1F("multp","Prompt Particle multiplicity;Number of particles;Number of events",16,-0.5,15.5); 
	multr = new TH1F("multr","Random Particle multiplicity;Number of particles;Number of events",16,-0.5,15.5);
	GeReject = new TH1F("GeReject","Events with Ge angle equal to zero;Cluster Number",9,-0.5,8.5);
	GePass = new TH1F("GePass","Events with Ge angle greater than zero;Cluster Number",9,-0.5,8.5); 
	GeRatio = new TH1F("GeRatio","Ratio of Ge events with angle less than or equal to zero to those with real angles;Cluster Number",9,-0.5,8.5); 
	GeAng = new TH2F("GeAng","Detector angles;#theta;#phi",45,0,180,90,0,359);
	GeSiAng = new TH3F("GeSiAng","Ge and Si coincidences angles;Ge #theta;Si #theta;#phi",45,0,180,30,0,60,270,-720,360);
	for(int i=0; i<8; i++){
		hname = "GeAng_clu" + dc.convertInt(i+1);
		htitle = "Detector angles in Cluster " + dc.convertInt(i+1) + ";#theta;#phi";
		GeAng_clu[i] = new TH2F(hname.c_str(),htitle.c_str(),45,0,180,90,0,359);
		hname = "GeSiAng_clu" + dc.convertInt(i+1);
		htitle = "Ge and Si coincidences angles in Cluster " + dc.convertInt(i+1) + ";Ge #theta;Si #theta;#phi";
		GeSiAng_clu[i] = new TH3F(hname.c_str(),htitle.c_str(),45,0,180,30,0,60,270,-720,360);
	}

	return;

}

void hists::FillSingles( ) {

	
	return;

}

void hists::FillPrompt( ) {

	
	return;

}

void hists::FillRandom( ) {

	
	return;

}


#endif
