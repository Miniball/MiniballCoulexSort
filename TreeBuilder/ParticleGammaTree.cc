#ifndef ParticleGammaTree_cxx
#define ParticleGammaTree_cxx

#define PBINS 800
#define PRANGE 800
#define PMIN -1.0*PRANGE/PBINS
#define PMAX PRANGE+PMIN
#define PART_ARRAY 250

#define GBINS 4000
#define GRANGE 4000
#define GMIN -1.0*GRANGE/GBINS
#define GMAX GRANGE+GMIN
#define GAMMA_ARRAY 250

#define ELBINS 2000
#define ELRANGE 2000
#define ELMIN -1.0*ELRANGE/ELBINS
#define ELMAX ELRANGE+ELMIN
#define ELECTRON_ARRAY 250

#ifndef ParticleGammaTree_hh
# include "ParticleGammaTree.hh"
#endif


void ParticleGammaTree::ClearEvt() {
	
	// Reset trees
	mb_evts.clear();
	tr_evts.clear();
	vector<mbevts>().swap( mb_evts );
	vector<trevts>().swap( tr_evts );
	
	return;
	
}

void ParticleGammaTree::FillCLXTree() {
	
	for( unsigned int j = 0; j < mb_evts.size(); j++ ) {
		
		mb_evts[j].SearchCoin();
		write_mb_evts.Initialise();
		write_mb_evts.CopyData( mb_evts[j] );
		
		// Standard particle-gamma coincidences
		if( write_mb_evts.GetNrPrompt() > 0 ||
		   write_mb_evts.GetNrRandom() > 0 ||
		   write_mb_evts.GetNrDelayed() > 0  ){
			
			g_clx->Fill();
			
		}
		
		// Gamma-gamma coincidences, with or without particles
		else if( gamgam && write_mb_evts.GetNrGammas() > 1 ) g_clx->Fill();
		
		// Gamma singles, with or without particles
		else if( singles ) g_clx->Fill();
		
	}
	
	return;
	
}

void ParticleGammaTree::FillTREXTree() {
	
	for( unsigned int j = 0; j < tr_evts.size(); j++ ) {
		
		tr_evts[j].SearchCoin();
		write_tr_evts.Initialise();
		write_tr_evts.CopyData( tr_evts[j] );
		
		p_tr->Fill();
		
	}
	
	return;
	
}

// ------------------------------------------------------------------------ //
// Gamma/electron-particle coincidences for Coulex etc.
// ------------------------------------------------------------------------ //
void ParticleGammaTree::CLXCoincidences() {
	
	// Start with gammas
	for( unsigned int i = 0; i < ab.GetGenSize(); i++ ) {
		
		// Initialise temporary event
		fill_mb_evts.Initialise();
		
		// Fill temporary event
		fill_mb_evts.SetGen( ab.GetGen(i) );
		fill_mb_evts.SetCluid( ab.GetClu(i) );
		fill_mb_evts.SetCid( ab.GetCid(i) );
		fill_mb_evts.SetSid( ab.GetSid(i) );
		fill_mb_evts.SetTheta( gamma_theta[ab.GetClu(i)][ab.GetCid(i)%3][ab.GetSid(i)] );
		fill_mb_evts.SetPhi( gamma_phi[ab.GetClu(i)][ab.GetCid(i)%3][ab.GetSid(i)] );
		
		// Do particles
		for( unsigned int j = 0; j < pf.ReconstructedSize(); j++ ) {
			
			tdiffPG = pf.GetTime(j) - ab.GetGtd(i);
			
			// electrons
			if( ab.GetClu(i) == 8 ) {
				
				tdiff_ep->Fill( tdiffPG );
				
				if( tMinRandomElectron <= tdiffPG && tdiffPG < tMaxRandomElectron ) coinc_flag = 1;
				else if( tMinPromptElectron <= tdiffPG && tdiffPG < tMaxPromptElectron ) coinc_flag = 0;
				else coinc_flag = -1;
				
			}
			
			// gamma-rays
			else {
				
				tdiff_gp->Fill( tdiffPG );
				tdiff_gp_q[pf.GetQuad(j)]->Fill( tdiffPG - Cal->AdcTime(pf.GetQuad(j)) );
				
				if( tMinDelayed <= tdiffPG && tdiffPG < tMaxDelayed ) coinc_flag = 2;
				else if( tMinRandom <= tdiffPG && tdiffPG < tMaxRandom ) coinc_flag = 1;
				else if( tMinPrompt <= tdiffPG && tdiffPG < tMaxPrompt ) coinc_flag = 0;
				else coinc_flag = -1;
				
			}
			
			// Add particle
			fill_mb_evts.SetPart( pf.GetPEn(j), pf.GetNf(j), pf.GetNb(j), pf.GetSector(j), pf.GetTime(j),
								  (double)event->SuperCycleTime(), (double)event->T1Time(),
								  (float)tdiffPG, (int)coinc_flag, pf.GetQuad(j), pf.GetLaser(j) );
			
		} // End loop All the particles
		
		
		// Look for correlated gammas or electrons
		for( unsigned int k = 0; k < ab.GetGenSize(); k++ ) {
			
			// skip if it's the same event as before
			if( k == i ) continue;
			
			// Found highest energy segment
			fill_mb_evts.SetCorGamGen( ab.GetGen(k) );
			fill_mb_evts.SetCorGamCluid( ab.GetClu(k) );
			fill_mb_evts.SetCorGamCid( ab.GetCid(k) );
			fill_mb_evts.SetCorGamSid( ab.GetSid(k) );
			fill_mb_evts.SetCorGamGtd( ab.GetGtd(k) - ab.GetGtd(i) );
			fill_mb_evts.SetCorGamTheta( gamma_theta[ab.GetClu(k)][ab.GetCid(k)%3][ab.GetSid(k)] );
			fill_mb_evts.SetCorGamPhi( gamma_phi[ab.GetClu(k)][ab.GetCid(k)%3][ab.GetSid(k)] );
			
			if( ab.GetClu(i) != 8 && ab.GetClu(k) != 8 ) tdiff_pgg->Fill( ab.GetGtd(k) - ab.GetGtd(i) );
			
		} // End search for correlated gammas
		
		// Push back the temporary event to the mb_evts vector
		mb_evts.push_back( fill_mb_evts );
		
	} // i
	
	return;
	
}

// ------------------------------------------------------------------------ //
// Particle-gamma coincidences for transfer
// ------------------------------------------------------------------------ //
void ParticleGammaTree::TREXCoincidences() {
	
	// Start with particles
	for( unsigned int i = 0; i < pf.ReconstructedSize(); i++ ) {
		
		// Initialise temporary event
		fill_tr_evts.Initialise();
		
		// Fill temporary event
		fill_tr_evts.SetPen( pf.GetPEn(i), pf.GetPdE(i), pf.GetPErest(i) );
		fill_tr_evts.SetQuad( pf.GetQuad(i) );
		fill_tr_evts.SetSector( pf.GetSector(i) );
		fill_tr_evts.SetNf( pf.GetNf(i) );
		fill_tr_evts.SetNb( pf.GetNb(i) );
		fill_tr_evts.SetTime( pf.GetTime(i) );
		fill_tr_evts.SetLaser( pf.GetLaser(i) );
		fill_tr_evts.SetT1( (double)event->T1Time() );
		fill_tr_evts.SetSS( (double)event->SuperCycleTime() );
		
		// Do gammas
		for( unsigned int j = 0; j < ab.GetGenSize(); j++ ) {
			
			tdiffPG = pf.GetTime(i) - ab.GetGtd(j);

			tdiff_gp->Fill( tdiffPG );
			tdiff_gp_q[pf.GetQuad(i)]->Fill( tdiffPG - Cal->AdcTime(pf.GetQuad(i)) );
				
			if( tMinDelayed <= tdiffPG && tdiffPG < tMaxDelayed ) coinc_flag = 2;
			else if( tMinRandom <= tdiffPG && tdiffPG < tMaxRandom ) coinc_flag = 1;
			else if( tMinPrompt <= tdiffPG && tdiffPG < tMaxPrompt ) coinc_flag = 0;
			else coinc_flag = -1;
			
			// Add particle
			fill_tr_evts.SetGamma( ab.GetGen(j), ab.GetCid(j), ab.GetSid(j), ab.GetClu(j),
								   gamma_theta[ab.GetClu(j)][ab.GetCid(j)%3][ab.GetSid(j)],
								   gamma_phi[ab.GetClu(j)][ab.GetCid(j)%3][ab.GetSid(j)],
								   tdiffPG, coinc_flag );
			
		} // gammas
		
		// Push back the temporary event to the mb_evts vector
		tr_evts.push_back( fill_tr_evts );
		
	} // i

	return;
	
}

// ------------------------------------------------------------------------ //
// Gamma-gamma coincidences
// ------------------------------------------------------------------------ //
void ParticleGammaTree::GammaGammaCoincidences() {

	for( unsigned int i = 0; i < ab.GetGenSize(); i++ ) {
		
		for( unsigned int j = 0; j < ab.GetGenSize(); j++ ) {
			
			if( j == i ) continue;
			
			// Skip over the electrons
			if( ab.GetClu(i) == 8 || ab.GetClu(j) == 8 ) continue;
			
			tdiff_gg->Fill( ab.GetGtd(i) - ab.GetGtd(j) );
			gg->Fill( ab.GetGen(i), ab.GetGen(j) );
			
		}
		
	}
	
	return;
	
}

// ------------------------------------------------------------------------ //
// Main Loop and logic of the particle-gamma builder
// ------------------------------------------------------------------------ //
void ParticleGammaTree::Loop( string outputfilename ) {
	
	// Open output file
	TFile* outfile = new TFile( outputfilename.c_str(), "recreate" );
	if( outfile->IsZombie() ) {
		
		cerr << "Problem with output file: " << outputfilename << endl;
		return;

	}
	
	// Initialise variables
	InitialiseVariables();
	
	// Setup histograms
	SetupHistograms();
	
	// Setup the addback or gamma finder
	ab.SetEvent( event );
	ab.SetCalibration( Cal );
	ab.SetOutputFile( outfile );
	ab.InitialiseHistograms();
	ab.InitialiseVariables();
	
	// Setup the particle finder
	pf.SetEvent( event );
	pf.SetCalibration( Cal );
	pf.SetOutputFile( outfile );
	pf.InitialiseHistograms();

	
	// ------------------------------------------------------------------------ //
	// Start loop over number of entries
	// ------------------------------------------------------------------------ //
	Double_t nentries = fTree->GetEntries();
	int nbytes = 0;
	int status;
	
	// Test - comment out for runs
	//nentries = 10000;
	
	// Do loop	
	for( unsigned int i = 0; i < nentries; i++ ) {

		status = fTree->GetEvent(i);
		
		if( status == -1 ) {

			cerr << "Error occured, couldn't read entry " << i << " from tree ";
			cerr << fTree->GetName() << " in file " << fTree->GetFile()->GetName() << endl;
			return;
			
		}
		
		else if( status == 0 ) {

			cerr << "Error occured, entry " << i << " in tree " << fTree->GetName();
			cerr << " in file " << fTree->GetFile()->GetName() << " doesn't exist" << endl;
			return;

		}
		
		nbytes += status;
		
		// ------------------------------------------------------------------------ //
		// Clean trees, particles and gammas/electrons
		// ------------------------------------------------------------------------ //
		ClearEvt();
		ab.ClearEvt();
		pf.ClearEvt();
		pf.NextAdc();
		

		// ------------------------------------------------------------------------ //
		// All DGF events - Build gamma-ray events in Miniball (and beam-dump)
		// ------------------------------------------------------------------------ //
		ab.MakeGammaRays( addback, reject );

		
		// ------------------------------------------------------------------------ //
		// All ADC events
		// ------------------------------------------------------------------------ //
		for( unsigned int j = 0; j < event->NumberOfAdcs(); j++ ) {

			adc_num = event->Adc(j)->ModuleNumber();
			adc_t = event->Adc(j)->Time();
			
			pf.SetModule( adc_num );
			pf.SetTime( adc_t + Cal->AdcTime( adc_num ) );
			pf.SetLaser( event->Adc(j)->LaserOn() );
			pf.SetSubEvent( event->Adc(j)->SubEvent() );

			// Standard CD/CLX setup or SPEDE
			if( ( cdpad || spede ) && adc_num < 4 ) {
				
				pf.FindCDParticles();
				ParticleCounterQ[adc_num] += pf.ReconstructCD( false, false );
				pf.NextAdc();
				
			}
			
			// CREX
			else if( crex && adc_num < 4 ) {
				
				pf.FindCREXParticles();
				ParticleCounterQ[adc_num] += pf.ReconstructBarrel( false, true );
				ParticleCounterQ[adc_num] += pf.ReconstructCD( false, true );
				pf.NextAdc();
				
			}
			
			// T-REX
			else if( trex && adc_num < 8 ) {
				
				pf.FindTREXParticles();
				
				if( adc_num%2 == 1 ) {
					
					ParticleCounterQ[adc_num/2] += pf.ReconstructBarrel( true, false );
					ParticleCounterQ[adc_num/2] += pf.ReconstructCD( true, false );
					pf.NextAdc();
					
				}
				
			}
			
			// SPEDE
			else if( spede && adc_num == 4 ) {
				
				ab.SetModule( adc_num );
				ab.SetTime( adc_t + Cal->AdcTime( adc_num ) );
				ab.SetSubEvent( event->Adc(j)->SubEvent() );
				ab.MakeElectrons();
				
			}
			
			// Ionisation chamber
			else if( ionch && ( ( !trex && adc_num == 4 ) || ( trex && adc_num == 9 ) ) ) {

				pf.IonisationChamber();

			} // ionisation chamber
			
		} // j - adcs
		

		// ------------------------------------------------------------------------ //
		// Particle-gamma and gamma-gamma coincidences
		// ------------------------------------------------------------------------ //
		if( trex ) TREXCoincidences();
		else CLXCoincidences();
		GammaGammaCoincidences();
		

		// ------------------------------------------------------------------------ //
		// Fill the CLX or the TREX tree
		// ------------------------------------------------------------------------ //
		if( trex ) FillTREXTree();
		else FillCLXTree();
		

		// Progress bar
		if( i % 50000 == 0 ) {

			cout << setw(5) << setiosflags(ios::fixed) << setprecision(1);
			cout << (100.*i)/nentries << "% done\r" << flush;

		}

	} // i

	cout << "Particle distribution:" << endl;
	for( unsigned int i = 0; i < 4; i++ ) cout << " Q" << i << " fired: " << ParticleCounterQ[i] << endl;

	outfile->Write();
	outfile->Close();

	return;
	
}

#endif