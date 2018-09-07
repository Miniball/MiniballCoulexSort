#ifndef ParticleGammaTree_cxx
#define ParticleGammaTree_cxx

#define PBINS 800
#define PRANGE 800
#define PMIN -1.0*PRANGE/PBINS
#define PMAX PRANGE+PMIN

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


	return;

}

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
	SetupHistograms( outfile );
	
	// Setup the addback or gamma finder
	AddBack ab( event );
	ab.SetCalibration( Cal );
	ab.SetOutputFile( outfile );
	ab.InitialiseHistograms();
	
	// Setup the particle finder
	ParticleFinder pf( event );
	pf.SetCalibration( Cal );
	pf.SetOutputFile( outfile );
	pf.InitialiseHistograms();
	
	// ------------------------------------------------------------------------ //
 
	
	// ------------------------------------------------------------------------ //
	// Variables
	// ------------------------------------------------------------------------ //
	float icE_Energy = 0.;
	float icDE_Energy = 0.;
	float tdiffPG = 0.;
	int coinc_flag;

	// Check Code
	int ParticleCounterQ[5] = {0,0,0,0,0};
 	// ------------------------------------------------------------------------ //

  


	// ------------------------------------------------------------------------ //
	// Write to mb_evts/g_clx tree
	// ------------------------------------------------------------------------ //
	mbevts* write_mb_evts = new mbevts();
	mbevts* mb_evts[GAMMA_ARRAY];
	for( unsigned int i = 0; i < GAMMA_ARRAY; i++ ) {
		
		mb_evts[i] = new mbevts();
		
	}
	
	TTree* g_clx = new TTree( "g_clx", "g_clx" );
	g_clx->Branch( "mbevts", "mbevts", &write_mb_evts );

	// ------------------------------------------------------------------------ //
	
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
		// Sort out mb_evts
		// ------------------------------------------------------------------------ //
		for( unsigned int j = 0; j < GammaCtr; j++ ) {
		
			mb_evts[j]->SearchCoin();  
			write_mb_evts->Initialize();
			write_mb_evts->CopyData( mb_evts[j] );
			
			// Standard particle-gamma coincidences
			if( write_mb_evts->GetNrPrompt() > 0 ||
			    write_mb_evts->GetNrRandom() > 0 ||
				write_mb_evts->GetNrDelayed() > 0  ){
				
				g_clx->Fill();
				
			}
			
			// Gamma-gamma coincidences, with or without particles
			else if( gamgam && write_mb_evts->GetNrGammas() > 1 ) g_clx->Fill();
			
			// Gamma singles, with or without particles
			else if( singles ) g_clx->Fill();
			
		}
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Reset
		// ------------------------------------------------------------------------ //
		for( unsigned int j = 0; j < GAMMA_ARRAY; j++ ) {
		
			mb_evts[j]->Initialize();
		
		}
		GammaCtr = 0;

		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Clean particles and gammas/electrons
		// ------------------------------------------------------------------------ //
		
		ClearEvt();
		ab.ClearEvt();
		pf.ClearEvt();
		
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// All DGF events
		// ------------------------------------------------------------------------ //

		// Build gamma-ray events in Miniball (and fill beam-dump spectra)
		ab.MakeGammaRays( addback );
		
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// All ADC events
		// ------------------------------------------------------------------------ //
		for( unsigned int j = 0; j < event->NumberOfAdcs(); j++ ) {

			adc_num = event->Adc(j)->ModuleNumber();
			adc_t = event->Adc(j)->Time();
			
			// Standard CD/CLX setup or SPEDE
			if( ( cdpad || spede ) && adc_num < 4 ) {
				
				pf.NextAdc();
				pf.SetModule( adc_num );
				pf.SetTime( adc_t + Cal->AdcTime( adc_num ) );
				pf.SetLaser( event->Adc(j)->LaserOn() );
				pf.SetSubEvent( event->Adc(j)->SubEvent() );
				pf.FindParticles();
				
			}
			
			// SPEDE
			else if( adc_num == 4 && spede ) { // SPEDE
				
				ab.SetModule( adc_num );
				ab.SetTime( adc_t + Cal->AdcTime( adc_num ) );
				ab.SetSubEvent( event->Adc(j)->SubEvent() );
				

			} // SPEDE
			
			// Ionisation chamber
			else if( adc_num == 4 && ionch ) { // ionisation chamber
				
				for( unsigned int k = 0; k < event->Adc(j)->SubEvent()->Size(); k++ ) {
					
					adc_ch = event->Adc(j)->SubEvent()->AdcChannel(k);
					adc_en = event->Adc(j)->SubEvent()->AdcValue(k);
					
					icDE_Energy = Cal->AdcEnergy( 4, adc_ch, adc_en );
					
					if( adc_ch != 0 ) continue; // Gas
					
					for( unsigned int l = 0; l < event->Adc(j)->SubEvent()->Size(); l++ ){
						
						adc_ch2 = event->Adc(j)->SubEvent()->AdcChannel(l);
						adc_en2 = event->Adc(j)->SubEvent()->AdcValue(l);
						
						if( adc_ch2 != 1 ) continue; // Si
						
						icE_Energy = Cal->AdcEnergy( 4, adc_ch2, adc_en2 );
						
						dEE->Fill( icE_Energy, icDE_Energy );
						
					} // l
					
				} // k
				
			} // ionisation chamber
			

			// ------------------------------------------- //
			// Particle reconstruction                     //
			// ------------------------------------------- //
			ParticleCounterQ[adc_num] += pf.ReconstructHeavyIons();
			
			
		} // j - particle reconstruction
		
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Gamma/electron-particle coincidences
		// ------------------------------------------------------------------------ //
		for( unsigned int j = 0; j < ab.GetGenArray().size(); j++ ) {

			mb_evts[GammaCtr]->SetGen( ab.GetGenArray().at(j) );
			mb_evts[GammaCtr]->SetCluid( ab.GetCluArray().at(j) );
			mb_evts[GammaCtr]->SetCid( ab.GetCidArray().at(j) );
			mb_evts[GammaCtr]->SetSid( ab.GetSidArray().at(j) );
			mb_evts[GammaCtr]->SetTheta( gamma_theta[ab.GetCluArray().at(j)][ab.GetCidArray().at(j)%3][ab.GetSidArray().at(j)] );
			mb_evts[GammaCtr]->SetPhi( gamma_phi[ab.GetCluArray().at(j)][ab.GetCidArray().at(j)%3][ab.GetSidArray().at(j)] );
				
			// Do particles
			for( unsigned int k = 0; k < pf.ReconstructedSize(); k++ ) {
				
				tdiffPG = pf.GetTime(k) - ab.GetGtdArray().at(j);

				// electrons
				if( ab.GetCluArray().at(j) == 8 ) { 

					tdiff_ep->Fill( tdiffPG );

					if( tMinRandomElectron <= tdiffPG && tdiffPG < tMaxRandomElectron ) coinc_flag = 1;
					else if( tMinPromptElectron <= tdiffPG && tdiffPG < tMaxPromptElectron ) coinc_flag = 0;
					else coinc_flag = -1;
						
				}

				// gamma-rays
				else {

					tdiff_gp->Fill( tdiffPG );
					tdiff_gp_q[pf.GetQuad(k)]->Fill( tdiffPG - Cal->AdcTime(pf.GetQuad(k)) );

					if( tMinDelayed <= tdiffPG && tdiffPG < tMaxDelayed ) coinc_flag = 2;
					else if( tMinRandom <= tdiffPG && tdiffPG < tMaxRandom ) coinc_flag = 1;
					else if( tMinPrompt <= tdiffPG && tdiffPG < tMaxPrompt ) coinc_flag = 0;
					else coinc_flag = -1;			

				}
				
				// Add particle
				mb_evts[GammaCtr]->SetPart( pf.GetPEn(k), pf.GetNf(k), pf.GetNb(k),
					pf.GetTime(k), (double)event->SuperCycleTime(), (double)event->T1Time(),
					(float)tdiffPG,	(int)coinc_flag, pf.GetQuad(k), pf.GetLaser(k) );
					
				PartCtr++;

			} // End loop All the particles

			// Look for correlated gammas or electrons
			for( unsigned int l = 0; l < ab.GetGenArray().size(); l++ ) {
				
				// skip if it's the same event as before
				if( l == j ) continue;
					
				// Found highest energy segment
				mb_evts[GammaCtr]->SetCorGamGen( ab.GetGenArray().at(l) );
				mb_evts[GammaCtr]->SetCorGamCluid( ab.GetCluArray().at(l) );
				mb_evts[GammaCtr]->SetCorGamCid( ab.GetCidArray().at(l) );
				mb_evts[GammaCtr]->SetCorGamSid( ab.GetSidArray().at(l) );
				mb_evts[GammaCtr]->SetCorGamGtd( ab.GetGtdArray().at(l) - ab.GetGtdArray().at(j) );
				mb_evts[GammaCtr]->SetCorGamTheta( gamma_theta[ab.GetCluArray().at(l)][ab.GetCidArray().at(l)%3][ab.GetSidArray().at(l)] );
				mb_evts[GammaCtr]->SetCorGamPhi( gamma_phi[ab.GetCluArray().at(l)][ab.GetCidArray().at(l)%3][ab.GetSidArray().at(l)] );
				
			} // End search for correlated gammas
			
			// Count the gamma!
			GammaCtr++;
				
			// Reset
			PartCtr = 0;
			
			
		} // j
		// ------------------------------------------------------------------------ //
		
		// ------------------------------------------------------------------------ //
		// Gamma-gamma coincidences
		// ------------------------------------------------------------------------ //
		for( unsigned int j = 0; j < ab.GetGenArray().size(); j++ ) {

		    for( unsigned int k = 0; k < ab.GetGenArray().size(); k++ ) {

		      if( j == k ) continue;
				
				tdiff_gg->Fill( ab.GetGtdArray().at(j), ab.GetGtdArray().at(k) );
				gg->Fill( ab.GetGenArray().at(j), ab.GetGenArray().at(k) );

		    }

		}

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