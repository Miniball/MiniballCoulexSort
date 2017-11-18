// Sorting for Miniball Coulex data and building histograms
// Cuts are done in doppler.hh using function, doppler::Cut()
// Liam Gaffney (liam.gaffney@cern.ch) - 26/10/2016

#define g_clx_cxx

// Select settings by uncommenting one of the following: 
//#define TWOPART 		// define to plot every 2p combination
//#define PHICAL  		// define to plot different CD rotation offsets
//#define GEANG			// define for plotting Ge angles per cluster
#define MBGEOMETRY  		// define to overwrite MB angles with MBGeometry routine

#ifndef __MBGEOMETRY_HH__
# include "MBGeometry.hh"
#endif
#ifndef hist_hh
# include "hists.hh"
#endif
#ifndef g_clx_hh
# include "g_clx.hh"
#endif

void g_clx::Loop( string outputfilename ) {

	if( fChain == 0 ) return;

	// Output file name	
	TFile *out = new TFile( outputfilename.c_str(), "RECREATE" );

	// Create doppler instance and set experimental parameters
	doppler dc;
	dc.ExpDefs( Zb, Ab, Zt, At, Eb, Ex, thick, depth, cddist, cdoffset,
					deadlayer, plunger, Bcut, Tcut );

	// Fit stopping power curves from the srim output files
	// Comment out to use the default parameters in doppler.hh
	// stoppingpowers( BT, TT, BS, TS )
	if( !dc.stoppingpowers( false, false, false, false ) ) return;

	// Ratio of prompt and random time windows
	// Alternatively, normalisation of beta-decay lines
	float bg_frac = -1.0;

	// Include errors on histograms (required for correct bg subtraction)
	TH1::SetDefaultSumw2();

	// New angles defined by Miniball geometry
#ifdef MBGEOMETRY

	//ifstream angfile;

	// Original values from frame
	//double clu_r[8] = { 116, 116, 116, 116, 116, 116, 116, 116 };
	//double clu_theta[8] = { 114.3, 67.0, 67.0, 114.3, 111.3, 67.0, 111.3, 67.0 };
	//double clu_phi[8] = { 141.7, 36.2, 107.0, 77.3, 256.9, 218.2, 323.5, 282.1 };
	//double clu_alpha[8] = { 350.0, 296.5, 252.5, 72.0, 248.5, 93.0, 67.5, 67.5 }; 


	// Nigel's values from 22Ne data taken from elog entry 19366 
	//double clu_r[8] = { 192.52, 97.56, 114.90, 106.16, 115.86, 116.30, 114.65, 115.03 };
	//double clu_theta[8] = { 106.6, 73.14, 66.01, 116.37, 110.89, 75.66, 101.80, 66.62 };
	//double clu_phi[8] = { 141.70, 36.20, 107.00, 77.30, 256.90, 218.20, 323.50, 282.10 };
	//double clu_alpha[8] = { 332.88, 280.14, 256.95, 70.47, 253.25, 89.97, 272.31, 69.11 }; 

    // Joa's values  
	double clu_r[8] = { 192.52, 97.56, 114.90, 106.16, 115.86, 116.30, 114.65, 115.03 };
	double clu_theta[8] = { 106.6, 73.14, 66.01, 116.37, 110.89, 75.66, 101.80, 66.62 };
	double clu_phi[8] = { 141.70, 36.20, 107.00, 77.30, 256.90, 218.20, 323.50, 282.10 };
	double clu_alpha[8] = { 332.88, 280.14, 256.95, 70.47, 253.25, 89.97, 272.31, 69.11 }; 


//#  theta        phi      alpha          r
//  106.60     141.70     332.88     129.52 # 17
//   73.14      36.20     280.14      97.56 # 12
//   66.01     107.00     256.95     114.90 # 16
//  116.37      77.30      70.47     106.16 # 13
//  110.89     256.90     253.25     115.86 # 22
//   75.66     218.20      89.97     116.30 # 18
//  101.80     323.50     272.31     114.65 # 14
//   66.62     282.10      69.11     115.03 # 23


	double new_theta[8][3][7]; // cluster, crystal segment
	double new_phi[8][3][7];
	double core_theta[24];   // cores only
	double core_phi[24];

	MBGeometry mbg;	
	for( int i = 0; i < 8; i++ ) { // loop over clusters

		mbg.SetupCluster( clu_theta[i], clu_phi[i], clu_alpha[i], clu_r[i] );

		for( unsigned int j = 0; j < 3; j++ ) { // loop over cores

			new_theta[i][j][0] = mbg.GetCoreTheta(j) * TMath::DegToRad();
			new_phi[i][j][0] = mbg.GetCorePhi(j) * TMath::DegToRad();

			core_theta[i*3+j] = new_theta[i][j][0];
			core_phi[i*3+j] = new_phi[i][j][0];
	
			for( int k = 0; k < 6; k++ ) { // loop over segments

				new_theta[i][j][k+1] = mbg.GetSegTheta(j,k) * TMath::DegToRad();
				new_phi[i][j][k+1] = mbg.GetSegPhi(j,k) * TMath::DegToRad();

			}

		}

	}

#endif

	// Declare the histograms here and initialise!
	hists h;
	h.Initialise( dc, core_theta, core_phi );

	// Particle-particle time difference (from tppdiff)
	h.Set_ppwin(300.);

	// Maximum strip number where recoils and projectiles are separable
	// Applicable mostly for inverse kinematics. To turn off, set to 16
	h.Set_maxrecoil(16);

	// Minimum strip number to define high CoM recoils
	// For normal kinematics, this is likely to be the limit of the safe condition
	// i.e. strips that are >= minrecoil are unsafe. Only low CoM solution is safe
	h.Set_minrecoil(0);


	// Loop over events 
	cout << "Looping over events...\n";
	Int_t nbytes = 0, nb = 0;
	Int_t skipFactor =1;
	for( Long64_t jentry=0; jentry<fChain->GetEntries()/skipFactor; jentry++ ) {	

		Long64_t ientry = LoadTree(jentry);

		if( ientry < 0 ) break;

		nb = fChain->GetEntry(jentry);   nbytes += nb;

		if( jentry%5000==0 ){
			cout << " " << jentry << "/" << fChain->GetEntries() << "  (" << jentry*100./fChain->GetEntries() << "%)    \r";
			cout << flush;
		}

		// Overwrite angles from tree with new angles
#ifdef MBGEOMETRY
		tha = new_theta[cluid][cid%3][sid];
		pha = new_phi[cluid][cid%3][sid];

		for( unsigned int i = 0; i < gcor_gen.size(); i++ ){
		
			if( gcor_cluid[i] == 8 || gcor_sid[i] < 0 ) continue; // not Ge
			gcor_tha[i] = new_theta[gcor_cluid[i]][gcor_cid[i]%3][gcor_sid[i]];	// gcor_sid broken!
			gcor_pha[i] = new_phi[gcor_cluid[i]][gcor_cid[i]%3][gcor_sid[i]];		// gcor_sid broken!
			
		}
#endif
		// Escape if angles are strange
		//if( gen >= 0 && ( tha < 0.0005 || pha < 0.0005 ) && !electron ){
		//	h.GeReject->Fill( cluid );
		//	continue;
		//}
		//h.GePass->Fill(cluid);

		// Paricle multiplicity
		if( pr_hits != 0 ) h.multp->Fill( pr_hits );
		else if( rndm_hits != 0 )  h.multr->Fill( rndm_hits );

		// Germanium angles
		//h.GeAng->Fill( tha*TMath::RadToDeg(), pha*TMath::RadToDeg() );

		// Loop over particle counter
		for( unsigned int i = 0; i < pen.size(); i++ ){

			// Escape funny events if there are any
			if( det[i]<0 || det[i]>3 || ann[i]<0 || sec[i]<0 ) continue;

			// Fill particle-gamma time spectra
			h.tdiff->Fill( td[i]*25. );

			// Germanium vs. Si angles
			//if( coin[i] == 0 )
		//		h.GeSiAng->Fill( tha * TMath::RadToDeg(), dc.GetPTh( ann[i] ) * TMath::RadToDeg(),
		//			( pha - dc.GetPPhi( det[i], sec[i] ) ) * TMath::RadToDeg() );

			// particle - particle time difference
			for( unsigned int j = i+1; j < pen.size(); j++ ) {

				h.tpp->Fill( td[i]*25., td[j]*25. );
				h.tppdiff->Fill( (td[i]-td[j])*25. );

				for( int k = 0; k < 2; k++ ){

					if( ( det[i]==k && det[j]==k+2 ) || ( det[j]==k && det[i]==k+2 ) ) {

						if( det[i]>det[j] ) h.tQQ[k]->Fill( (td[j]-td[i])*25. );
						else h.tQQ[k]->Fill( (td[i]-td[j])*25. );

					}

				}

			}
			
		} // END - Loop over particle counter

		// Check size of correlated gammas
		h.gcor_size->Fill( gcor_gen.size() );

		// Fill conditioned spectra

		// Condition on particle detection
		if( pr_hits==1 && rndm_hits==0 ) 
			h.Fill1h( gen, tha, pha, cid, gcor_gen, gcor_tha, gcor_pha, gcor_cluid, gcor_gtd,
					  pen[pr_ptr[0]], ann[pr_ptr[0]], sec[pr_ptr[0]], det[pr_ptr[0]], 1.0 );

		else if( pr_hits==2 && rndm_hits==0 )
			h.Fill2h( gen, tha, pha, cid, gcor_gen, gcor_tha, gcor_pha, gcor_cluid, gcor_gtd,
					  pen, ann, sec, det, pr_ptr, td, 1.0 );

		else if( rndm_hits==1 && pr_hits==0 ) 
			h.Fill1h( gen, tha, pha, cid, gcor_gen, gcor_tha, gcor_pha, gcor_cluid, gcor_gtd,
					  pen[rndm_ptr[0]], ann[rndm_ptr[0]], sec[rndm_ptr[0]], det[rndm_ptr[0]], bg_frac );

		else if( rndm_hits==2 && pr_hits==0 )			
			h.Fill2h( gen, tha, pha, cid, gcor_gen, gcor_tha, gcor_pha, gcor_cluid, gcor_gtd,
					  pen, ann, sec, det, rndm_ptr, td, bg_frac );

	} // for (Long64_t jentry=0; jentry<fChain->GetEntries();jentry++)
	

	// Add spectra
	h.AddSpectra(bg_frac);

	long nopart = (long)h.part->Integral();
	cout << "1-particle events = " << nopart << endl;

#ifdef PHICAL
	TCanvas *cphi;
	cphi = new TCanvas( "PhiCalibration", "Phi Calibration" );
	for( unsigned int i = 0; i < PHI_NSTEPS; i++ ) {

		h.phical_dcT[i]->SetLineColor(i+1);
		if( i == 0 ) h.phical_dcT[i]->Draw();
		else h.phical_dcT[i]->Draw("SAME");

	}
#endif

	out->Write();

}
