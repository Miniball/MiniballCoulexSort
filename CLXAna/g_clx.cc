// Sorting for Miniball Coulex data and building histograms
// Cuts are done in doppler.hh using function, doppler::Cut()
// Liam Gaffney (liam.gaffney@cern.ch) - 26/10/2016

#define g_clx_cxx

// Select settings by uncommenting one of the following: 
//#define TWOPART 		// define to plot every 2p combination
//#define GEANG			// define for plotting Ge angles per cluster
#define SPEDEGEOMETRY	// define to overwrite Spede angles with SpedeGeometry routine

#ifndef __SpedeGeometry_HH__
# include "SpedeGeometry.hh"
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
					deadlayer, contaminant, spededist, Bcut, Tcut, srim );

	// Create stopping power curves from the srim output files
	// Comment out to use the default parameters in doppler.hh
	// stoppingpowers( BT, TT, BA, TA, BC, TC )
	if( !dc.stoppingpowers( true, true, true, true, true, true ) ) return;

	// Test if it's an electron or gamma
	bool electron;

	// Include errors on histograms (required for correct bg subtraction)
	TH1::SetDefaultSumw2();

	// Declare the histograms here and initialise!
	hists h;
	h.Initialise( dc );

	// Particle-particle time difference (from tppdiff)
	h.Set_ppwin(100.);

	// Maximum strip number where recoils and projectiles are separable
	// Applicable mostly for inverse kinematics. To turn off, set to 16
	h.Set_maxrecoil(16);

	// Minimum strip number to define high CoM recoils
	// For normal kinematics, this is likely to be the limit of the safe condition
	// i.e. strips that are >= minrecoil are unsafe. Only low CoM solution is safe
	h.Set_minrecoil(0);

	// New angles defined by Spede geometry
#ifdef SPEDEGEOMETRY
	double spede_r = dc.GetSpedeDist();
	double spede_alpha = 0.0;

	double spede_theta[24];
	double spede_phi[24];

	SpedeGeometry spg;	
	spg.SetupSpede( spede_r, spede_alpha );
	for( unsigned int j = 0; j < 24; j++ ) { // loop over segments

		spede_theta[j] = spg.GetSpedeTheta(j) * TMath::DegToRad();
		spede_phi[j] = spg.GetSpedePhi(j) * TMath::DegToRad();
	}

#endif

	float rad, phi; // for cdpic

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

		// Is it an electron or gamma?
		if( cluid < 8 ) electron = false;
		else if( cluid == 8 ) electron = true;
		else break; // shouldn't be anything else

#ifdef SPEDEGEOMETRY
		if( electron ) { // check if it's SPEDE/PAD

			if( cid == 0 ) { // spede

				tha = spede_theta[sid];
				pha = spede_phi[sid];

				for( unsigned int i = 0; i < gcor_gen.size(); i++ ){
			
					if( gcor_cluid[i] != 8 || gcor_sid[i] < 0 || gcor_sid[i] > 23 ) continue; // not spede/PAD
					gcor_tha[i] = spede_theta[gcor_sid[i]];		// gcor_sid broken!
					gcor_pha[i] = spede_phi[gcor_sid[i]];		// gcor_sid broken!
				
				}

			}
			
			else { // PAD

				tha = dc.GetPTh( 10, 4 );
				pha = dc.GetPPhi( cid-1, 6, 4 );

				for( unsigned int i = 0; i < gcor_gen.size(); i++ ){
			
					if( gcor_cluid[i] != 8 || gcor_sid[i] < 0 || gcor_sid[i] > 23 ) continue; // not spede/PAD
					gcor_tha[i] = spede_theta[gcor_sid[i]];		// gcor_sid broken!
					gcor_pha[i] = spede_phi[gcor_sid[i]];		// gcor_sid broken!
				
				}


			}

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
		if( !electron ) { // check if it's Miniball

			h.GeAng->Fill(tha*TMath::RadToDeg(),pha*TMath::RadToDeg());
#ifdef GEANG
			h.GeAng_clu[cluid]->Fill(tha*TMath::RadToDeg(),pha*TMath::RadToDeg());
#endif
		}

		// Loop over particle counter
		for( unsigned int i = 0; i < pen.size(); i++ ){

			// Escape funny events if there are any
			if( det[i]<0 || det[i]>3 || nf[i]<0 || nb[i]<0 ) continue;

			// Fill particle-gamma time spectra
			if( !electron ) {
				h.tdiff->Fill(td[i]*25.);
				h.tdiffQ[det[i]]->Fill(td[i]*25.);
				for( unsigned int k=0; k<4; k++ )
					if( gen>h.tegate[k]-20 && gen<h.tegate[k]+20 )
						h.tdiffE[k]->Fill(td[i]*25.);

				// particle - particle time difference
				for( unsigned int j=i+1; j<pen.size(); j++ ) {
					h.tpp->Fill(td[i]*25.,td[j]*25.);
					h.tppdiff->Fill((td[i]-td[j])*25.);
					for( int k=0; k<2; k++ ){
						if( (det[i]==k && det[j]==k+2) || (det[j]==k && det[i]==k+2) ) {
							if( det[i]>det[j] ) h.tQQ[k]->Fill((td[j]-td[i])*25.);
							else h.tQQ[k]->Fill((td[i]-td[j])*25.);
						}
					}
				}
			} // cluid < 8
			else h.tdiff_e->Fill(td[i]*25.);

			// CD pic
			if( nf[i]>(-1) && nf.size()>0 && ( sector[i]==0 || sector[i]==4 ) ){
				rad=39-2*nf[i];
				rad+=gRandom->Rndm(1)*1.9;
				phi=(nb[i]-16.)*0.12+(det[i]*TMath::PiOver2());
				phi+=gRandom->Rndm(1)*0.114;
				if( nb[i]<29 && nb[i]>15 ) h.cdpic->Fill(rad*cos(phi),rad*sin(phi));
			}

			// Germanium angles vs. Silicon angles
			if( !electron ) {
#ifdef GEANG
				h.GeSiAng->Fill(tha*TMath::RadToDeg(),dc.GetPTh(nf[i],sector[i])*TMath::RadToDeg(),(pha-dc.GetPPhi(det[i],nb[i],sector[i]))*TMath::RadToDeg());
				h.GeSiAng_clu[cluid]->Fill(tha*TMath::RadToDeg(),dc.GetPTh(nf[i],sector[i])*TMath::RadToDeg(),(pha-dc.GetPPhi(det[i],nb[i],sector[i]))*TMath::RadToDeg());
#endif
			} // !electron
			
		} // END - Loop over particle counter

		// Check size of correlated gammas
		h.gcor_size->Fill( gcor_gen.size() );

		// Fill conditioned spectra

		// Condition on particle detection
		if( pr_hits==1 && rndm_hits==0 ) 
			h.Fill1h(gen, tha, pha, cid, gcor_gen, gcor_tha, gcor_pha, gcor_cluid, gcor_gtd, electron,
					 pen[pr_ptr[0]], nf[pr_ptr[0]], nb[pr_ptr[0]], sector[pr_ptr[0]],
					 det[pr_ptr[0]], time[pr_ptr[0]]-t1t[pr_ptr[0]], 1.0);

		else if( pr_hits==2 && rndm_hits==0 )
			h.Fill2h(gen, tha, pha, cid, gcor_gen, gcor_tha, gcor_pha, gcor_cluid, gcor_gtd, electron,
					 pen, nf, nb, sector, det, pr_ptr, td, 1.0);

		else if( del_hits==2 && rndm_hits==0 )
			h.FillDel2h(gen, tha, pha, cid, pen, nf, nb, sector, det, del_ptr, td, 1.0);

		else if( rndm_hits==1 && pr_hits==0 ) 
			h.Fill1h(gen, tha, pha, cid, gcor_gen, gcor_tha, gcor_pha, gcor_cluid, gcor_gtd, electron,
					 pen[rndm_ptr[0]], nf[rndm_ptr[0]], nb[rndm_ptr[0]], sector[rndm_ptr[0]],
					 det[rndm_ptr[0]], time[rndm_ptr[0]]-t1t[rndm_ptr[0]], bg_frac);

		else if( rndm_hits==2 && pr_hits==0 ) {
		
			h.Fill2h(gen, tha, pha, cid, gcor_gen, gcor_tha, gcor_pha, gcor_cluid, gcor_gtd, electron,
					 pen, nf, nb, sector, det, rndm_ptr, td, bg_frac);

			h.FillDel2h(gen, tha, pha, cid, pen, nf, nb, sector, det, rndm_ptr, td, bg_frac);

		}

	} // for (Long64_t jentry=0; jentry<fChain->GetEntries();jentry++)
	

	// Add spectra
	h.AddSpectra(bg_frac);

	long nopart = (long)h.part->Integral();
	cout << "1-particle events = " << nopart << endl;

	out->Write();

}
