#ifndef ParticleGammaTree_cxx
#define ParticleGammaTree_cxx

#ifndef ParticleGammaTree_hh
# include "ParticleGammaTree.hh"
#endif

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

void ParticleGammaTree::Loop( string outputfilename ) {
	
	// Open output file
	TFile* outfile = new TFile( outputfilename.c_str(), "recreate" );
	if( outfile->IsZombie() ) {
		
		cerr << "Problem with output file: " << outputfilename << endl;
		return;

	}
	
	// Initialise variables
	InitialiseVariables();
	
	// Crap segments list (i.e. those that need to be vetoed)
	// Counting from 0 to 167, i.e. including cores - clu*21 + core*7 + seg
	bool veto_gamma = false;
	unsigned int gSeg;
	vector< unsigned int > dead_segments;
	//dead_segments.push_back( 106 ); // clu 5, core 0, seg 1 = 18A1
	//dead_segments.push_back( 107 ); // clu 5, core 0, seg 2 = 18A2
	
	
	// ------------------------------------------------------------------------ //
 
	
	// ------------------------------------------------------------------------ //
	// Variables
	// ------------------------------------------------------------------------ //
	unsigned int i,j,k,l;
	float GammaEnergy = 0.;
	float GammaEnergy2 = 0.;
	float ElectronEnergy = 0.;
	float PartEnergy = 0.;
	float icE_Energy = 0.;
	float icDE_Energy = 0.;
	float PadEnergy[4] = {0.,0.,0.,0.};
	unsigned int GammaCtr = 0;
	unsigned int PartCtr = 0;
	float tdiffPG = 0.;
	float MaxSegEnergy = -99.;
	int MaxSegClu = -1;
	int MaxSegCore = -1;
	int MaxSegId = -1;
	int coinc_flag;
	int dgf_num = 0;
	int dgf_num2 = 0;
	int dgf_ch = 0;
	int dgf_ch2 = 0;
	int dgf_en = 0;
	int dgf_en2 = 0;
	long long dgf_t = 0;
	long long dgf_t2 = 0;
	unsigned int adc_num = 0;
	unsigned int adc_ch = 0;
	unsigned int adc_en = 0;
	unsigned int adc_ch2 = 0;
	unsigned int adc_en2 = 0;
	long long adc_t =0;

	vector<float> cd_ringenergy[4];
	vector<float> cd_stripenergy[4];
	vector<unsigned short> cd_ringid[4];
	vector<unsigned short> cd_stripid[4];
	vector<bool> cd_ringused[4];
	vector<bool> cd_stripused[4];

	vector<float> gen_array;
	vector<long long> gtd_array;
	vector<unsigned short> clu_array;
	vector<unsigned short> cid_array;
	vector<unsigned short> sid_array;
	vector<float> sen_array; 
	bool ab_evt = false;
	unsigned short ab_mul = 0;

	vector<unsigned int> Quad; // 0: Top, 1: Bottom, 2: Left and 3; Right
	vector<unsigned int> Chan_front; // Rings for CDs, Total Energy for Pad or Strips for Barrel
	vector<float> Ener_front;
	vector<unsigned int> Chan_back; // Strips for CDs and Total Energy for Barrel
	vector<float> Ener_back;
	vector<long long> time;
	vector<bool> laser;
  
	int StripNum = 0;
	int RingNum = 0;
	float StripEnergy = 0;
	float RingEnergy = 0;
	float StripEnergyDiff = 0;
	float RingEnergyDiff = 0;
	bool RingMult = false;
	float tempDiff = 0;
	
	// Check Code
	int CounterAdcCDFired[5] = {0,0,0,0,0};
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
	vector<int> DgfModule;
	
	// Test - comment out for runs
	//nentries = 10000;
	
	// Do loop	
	for( i = 0; i < nentries; i++ ) {

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
		for( j = 0; j < GammaCtr; j++ ) {
		
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
		for( j = 0; j < GAMMA_ARRAY; j++ ) {
		
			mb_evts[j]->Initialize();
		
		}
		GammaCtr = 0;

		DgfModule.clear();

		for( j = 0; j < event->NumberOfDgfs(); j++ ) {

			for( k = 0; k < DgfModule.size(); k++ ) 

				if( event->Dgf(j)->ModuleNumber() == DgfModule[k] ) break;
			
			if( k == DgfModule.size() ) DgfModule.push_back( event->Dgf(j)->ModuleNumber() );

		}
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Clean particles and gammas/electrons
		// ------------------------------------------------------------------------ //
		for( j = 0; j < 4; j++ ) {

			cd_ringenergy[j].clear();
			cd_stripenergy[j].clear();
			cd_ringid[j].clear();
			cd_stripid[j].clear();
			cd_ringused[j].clear();
			cd_stripused[j].clear();

		}
	
		Quad.clear();
		Chan_front.clear();
		Ener_front.clear();
		Chan_back.clear();
		Ener_back.clear();
		time.clear();
		laser.clear();

		gen_array.clear();
		gtd_array.clear();
		clu_array.clear();
		cid_array.clear();
		sid_array.clear();
		sen_array.clear();
	  
		adc->Fill( event->NumberOfAdcs() );
		dgf->Fill( DgfModule.size() );

		if( event->NumberOfAdcs() == 0 ) {
	
			free_dgf->Fill( DgfModule.size() );

		}
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Only DGF events
		// ------------------------------------------------------------------------ //
		for( j = 0; j < event->NumberOfDgfs(); j++ ) {

			ab_evt = false;
			ab_mul = 0;
			veto_gamma = false;
			dgf_num = event->Dgf(j)->ModuleNumber();
			dgf_ch  = event->Dgf(j)->Channel();
			dgf_en  = event->Dgf(j)->Energy();
			dgf_t   = event->Dgf(j)->Time();
				
			if( 0 <= dgf_num && dgf_num < 48 && 0 <= dgf_ch && dgf_ch < 4 ) { // miniball

				GammaEnergy = Cal->DgfEnergy( dgf_num, dgf_ch, dgf_en );

				if( dgf_num % 2 == 0 && dgf_ch < 3 ) { // cores plus seg1 and seg2

					E_gam_seg[dgf_num/6][dgf_num%6/2][dgf_ch]->Fill( dgf_en );
					E_gam_seg_cal[dgf_num/6][dgf_num%6/2][dgf_ch]->Fill( GammaEnergy );
					if( dgf_ch == 0 ) E_gam->Fill( GammaEnergy );

				} // even DGF number

				else {

					E_gam_seg[dgf_num/6][dgf_num%6/2][dgf_ch+3]->Fill( dgf_en );
					E_gam_seg_cal[dgf_num/6][dgf_num%6/2][dgf_ch+3]->Fill( GammaEnergy );

				} // odd DGF number

			  	// Look for a core event
				if( dgf_ch == 0 && dgf_num % 2 == 0 ) {
			
					MaxSegClu = dgf_num / 6;
					MaxSegCore = dgf_num % 6 / 2;
					MaxSegId = 0; // initialise as core (if no segment hit (dead), use core!)

					// Check for highest energy segment in same detector
					for( k = 0; k < event->NumberOfDgfs(); k++ ) {
			
						dgf_num2 = event->Dgf(k)->ModuleNumber();
						dgf_ch2  = event->Dgf(k)->Channel();
						dgf_en2  = event->Dgf(k)->Energy();
						dgf_t2   = event->Dgf(k)->Time();
					
						// We don't care if it's the same event
						if( k == j ) continue;
						
						// Make sure it's a Miniball DGF
						if( dgf_num2 < 0 || dgf_num2 > 47 ) continue;

						// Get global segment number (0-167)
						gSeg = ( dgf_num2 / 2 ) * 7;
						if( dgf_num2 % 2 == 0 ) gSeg += dgf_ch2;
						else gSeg += dgf_ch2+3;
						
						// Is it a crap segment?
						for( unsigned int ds = 0; ds < dead_segments.size(); ds++ ) {
							
							if( gSeg == dead_segments[ds] ) {
								
								veto_gamma = true;
								
							}

						}

						// time difference between cores
						if( dgf_ch2 == 0 && dgf_num2 % 2 == 0 )
							tdiff_gg->Fill( dgf_t2 - dgf_t );

						// Skip if a different detector
						if( dgf_num2 != dgf_num && dgf_num2 != dgf_num + 1 ) continue;
					
						GammaEnergy2 = Cal->DgfEnergy( dgf_num2, dgf_ch2, dgf_en2 );

						// Test maximum energy segment
						if( GammaEnergy2 < MaxSegEnergy ) continue;
					
						// Reassign energy and id
						MaxSegEnergy = GammaEnergy2;
						if( dgf_num2 % 2 == 0 ) MaxSegId = dgf_ch2;
						else MaxSegId = dgf_ch2+3;
			
					} // k

					// Found highest energy segment
					
					// Do the veto of crap segments
					if( veto_gamma ) continue;

					// Check previous gammas
					for( l = 0; l < gen_array.size(); l++ ) {

						if( gen_array[l] < GammaEnergy+0.1 && gen_array[l] > GammaEnergy-0.1 )
							continue; // same event

						// Do the addback if same cluster
						if( clu_array[l] == MaxSegClu && addback ) {

							gen_array[l] += GammaEnergy;
							ab_evt = true;
							ab_mul++;

							if( sen_array[l] < MaxSegEnergy ) {

								gtd_array[l] = dgf_t;
								clu_array[l] = MaxSegClu;
								cid_array[l] = MaxSegClu*3 + MaxSegCore;
								sid_array[l] = MaxSegId;
								sen_array[l] = MaxSegEnergy;	

							}

						} // addback

					} // previous gammas

					if( ab_evt ) continue; // get next gamma
					hABmult->Fill( ab_mul );

					gen_array.push_back( GammaEnergy );
					gtd_array.push_back( dgf_t );
					clu_array.push_back( MaxSegClu );
					cid_array.push_back( MaxSegClu*3 + MaxSegCore );
					sid_array.push_back( MaxSegId );
					sen_array.push_back( MaxSegEnergy );

				} // core event

				// Reset
				MaxSegEnergy = -99.;
				MaxSegClu = -1;
				MaxSegCore = -1;
				MaxSegId = -1;
					
			}

			else if( dgf_num == 53 && dgf_ch >= 0 && dgf_ch < 2 ) {

				GammaEnergy = Cal->DgfEnergy( dgf_num, dgf_ch, dgf_en );
				E_BeamDump[dgf_ch]->Fill( GammaEnergy );
				T_BeamDump[dgf_ch]->Fill( (dgf_t)/40000000 );

				for( k = 0; k < event->NumberOfDgfs(); k++ ) {

					if( k == j ) continue;
					dgf_num2 = event->Dgf(k)->ModuleNumber();
					dgf_ch2  = event->Dgf(k)->Channel();
					dgf_en2  = event->Dgf(k)->Energy();
					dgf_t2   = event->Dgf(k)->Time();

					if( dgf_num2 != 53 ) continue;
					if( dgf_ch != 0 || dgf_ch2 != 1 ) continue;

					GammaEnergy2 = Cal->DgfEnergy( dgf_num2, dgf_ch2, dgf_en2 );

					tdiff_BD->Fill( dgf_t-dgf_t2 );

					if( TMath::Abs( dgf_t-dgf_t2 ) < 999999999. )

						bd_bd->Fill( GammaEnergy, GammaEnergy2 );

				}


			}

		}
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Only Particles (and SPEDE if it is in ADC5)
		// ------------------------------------------------------------------------ //
		for( j = 0; j < event->NumberOfAdcs(); j++ ) {

			adc_num = event->Adc(j)->ModuleNumber();
			adc_t = event->Adc(j)->Time();
			RingMult = true;

			for( k = 0; k < event->Adc(j)->SubEvent()->Size(); k++ ) {

				adc_ch = event->Adc(j)->SubEvent()->AdcChannel(k);
				adc_en = event->Adc(j)->SubEvent()->AdcValue(k);

				if( adc_num < 4 ) { // CD

					if( adc_ch < 16 ) { // front rings
						
						PartEnergy = Cal->AdcEnergy( adc_num, adc_ch, adc_en );

						part->Fill( adc_ch, PartEnergy/1000. );
						E_part_ann[adc_num][adc_ch]->Fill( adc_en );
						E_part_ann_cal[adc_num][adc_ch]->Fill( PartEnergy/1000. );

						CD_front_energy[adc_num]->Fill( adc_ch, adc_en );
						CD_front_energy_cal[adc_num]->Fill( adc_ch, PartEnergy/1000. );

						if( adc_en > Cal->AdcThreshold( adc_num, adc_ch ) ) { // threshold

							cd_ringenergy[adc_num].push_back( PartEnergy );
							cd_ringid[adc_num].push_back( adc_ch );
							cd_ringused[adc_num].push_back( false );

						} // threshold

					} // front rings
					
					else if( adc_ch < 28 ) { // back strips

						PartEnergy = Cal->AdcEnergy( adc_num, adc_ch, adc_en );

						E_part_sec[adc_num][adc_ch-16]->Fill( adc_en );
						E_part_sec_cal[adc_num][adc_ch-16]->Fill( PartEnergy/1000. );
						
						CD_back_energy[adc_num]->Fill( adc_ch-16, adc_en );
						CD_back_energy_cal[adc_num]->Fill( adc_ch-16, PartEnergy/1000. );

						if( adc_en > Cal->AdcThreshold( adc_num, adc_ch ) ) { // threshold

							cd_stripenergy[adc_num].push_back( PartEnergy );
							cd_stripid[adc_num].push_back( adc_ch-16 );
							cd_stripused[adc_num].push_back( false );

						} // threshold

					} // back strips

					else if( adc_ch == 31 && cdpad ) { // something in the pad!

						if( adc_en < Cal->AdcThreshold( adc_num, adc_ch ) ) continue; // threshold

						PadEnergy[adc_num] = Cal->AdcEnergy( adc_num, adc_ch, adc_en );
					
						padE[adc_num]->Fill( adc_en );
						padE_cal[adc_num]->Fill( PadEnergy[adc_num] );
						padE_sum->Fill( PadEnergy[adc_num] );

						gen_array.push_back( PadEnergy[adc_num] );
						gtd_array.push_back( adc_t + dtAdc[adc_num] );
						clu_array.push_back( 8 );
						cid_array.push_back( adc_num+1 );
						sid_array.push_back( 0 );
						sen_array.push_back( 0 );

					} // pad
					
				} // particle in CD
			
				else if( adc_num == 4 && spede ) { // SPEDE
					
					ElectronEnergy = Cal->AdcEnergy( 4, adc_ch, adc_en );
					
					// STM-16 one
					if( adc_ch < 12 ) {
						
						E_spede_seg[adc_ch]->Fill( adc_en );
						E_spede_seg_cal[adc_ch]->Fill( ElectronEnergy );
						E_spede->Fill( ElectronEnergy );
					
						gen_array.push_back( ElectronEnergy );
						gtd_array.push_back( adc_t );
						clu_array.push_back( 8 );
						cid_array.push_back( 0 );
						sid_array.push_back( adc_ch );
						sen_array.push_back( 0 );

					}
					
					// STM-16 two
					else if( adc_ch > 15 && adc_ch < 28 ) {
						
						E_spede_seg[adc_ch-4]->Fill( adc_en );
						E_spede_seg_cal[adc_ch-4]->Fill( ElectronEnergy );
						E_spede->Fill( ElectronEnergy );
						
						gen_array.push_back( ElectronEnergy );
						gtd_array.push_back( adc_t );
						clu_array.push_back( 8 );
						cid_array.push_back( 0 );
						sid_array.push_back( adc_ch-4 );
						sen_array.push_back( 0 );

					}
					
				} // SPEDE
				
				else if( adc_num == 4 && ionch && adc_ch == 0 ) { // ionisation chamber (gas trigger)
				
					icDE_Energy = Cal->AdcEnergy( 4, adc_ch, adc_en );
					
					for( l = 0; l < event->Adc(j)->SubEvent()->Size(); l++ ){

						adc_ch2 = event->Adc(j)->SubEvent()->AdcChannel(l);
						adc_en2 = event->Adc(j)->SubEvent()->AdcValue(l);

						if( adc_ch2 != 1 ) continue;

						icE_Energy = Cal->AdcEnergy( 4, adc_ch2, adc_en2 );

						dEE->Fill( icE_Energy, icDE_Energy );

					}

				} // ionisation chamber

			} // k

			// if we did have SPEDE (or ion. chamber), don't carry on with particle stuff
			if( adc_num == 4 ) continue;

			// ------------------------------------------- //
			// Particle reconstruction                     //
			// ------------------------------------------- //

			// Easy case, 1 front and 1 back!
			if( cd_ringenergy[adc_num].size() == 1 && cd_stripenergy[adc_num].size() == 1 ) {

				Quad.push_back( adc_num );
				time.push_back( adc_t + dtAdc[adc_num] );
				laser.push_back( event->Adc(j)->LaserOn() );

				Chan_front.push_back( cd_ringid[adc_num][0] );
				Ener_front.push_back( cd_ringenergy[adc_num][0] );

				Chan_back.push_back( cd_stripid[adc_num][0] );
				Ener_back.push_back( cd_stripenergy[adc_num][0] );
				
				CD_front_back[adc_num][0]->Fill( cd_ringenergy[adc_num][0]/1000., cd_stripenergy[adc_num][0]/1000. );

				cd_debug->Fill(0);
				CounterAdcCDFired[adc_num]++;

			} // 1 vs. 1
			
			// 1 on the front and multiple on the back
			else if( cd_ringenergy[adc_num].size() == 1 && cd_stripenergy[adc_num].size() > 1 ) {

				Quad.push_back( adc_num );
				time.push_back( adc_t + dtAdc[adc_num] );
				laser.push_back( event->Adc(j)->LaserOn() );

				Chan_front.push_back( cd_ringid[adc_num][0] );
				Ener_front.push_back( cd_ringenergy[adc_num][0] );

				StripNum = cd_stripid[adc_num][0];
				StripEnergy = cd_stripenergy[adc_num][0];
				StripEnergyDiff = StripEnergy - cd_ringenergy[adc_num][0];
				StripEnergyDiff = TMath::Abs( StripEnergyDiff );

				// Get closest match between front and back
				for( k = 0; k < cd_stripenergy[adc_num].size(); k++ ) {

					tempDiff = cd_stripenergy[adc_num][k] - cd_ringenergy[adc_num][0];
					tempDiff = TMath::Abs( tempDiff );

					if( tempDiff < StripEnergyDiff ) {

						StripNum = cd_stripid[adc_num][k];
						StripEnergy = cd_stripenergy[adc_num][k];
						StripEnergyDiff = tempDiff;

					}

					CD_front_back[adc_num][1]->Fill(
						cd_ringenergy[adc_num][0]/1000., cd_stripenergy[adc_num][k]/1000. );

				} // k

				// Addback neighbouring strips
				for( k = 0; k < cd_stripenergy[adc_num].size(); k++ ) {

					if( TMath::Abs( StripNum - cd_stripid[adc_num][k] ) == 1 )

						StripEnergy += cd_stripenergy[adc_num][k];

				} // k

				Chan_back.push_back( StripNum );
				Ener_back.push_back( StripEnergy );

				cd_debug->Fill(1);
				CounterAdcCDFired[adc_num]++;

			} // 1 vs. N

			// multiple on the front and 1 on the back
			else if( cd_ringenergy[adc_num].size() > 1 && cd_stripenergy[adc_num].size() == 1 ) {

				Quad.push_back( adc_num );
				time.push_back( adc_t + dtAdc[adc_num] );
				laser.push_back( event->Adc(j)->LaserOn() );

				Chan_back.push_back( cd_stripid[adc_num][0] );
				Ener_back.push_back( cd_stripenergy[adc_num][0] );

				RingNum = cd_ringid[adc_num][0];
				RingEnergy = cd_ringenergy[adc_num][0];
				RingEnergyDiff = RingEnergy - cd_stripenergy[adc_num][0];
				RingEnergyDiff = TMath::Abs( RingEnergyDiff );

				// Get closest match between front and back
				for( k = 0; k < cd_ringenergy[adc_num].size(); k++ ) {

					tempDiff = cd_ringenergy[adc_num][k] - cd_stripenergy[adc_num][0];
					tempDiff = TMath::Abs( tempDiff );

					if( tempDiff < RingEnergyDiff ) {

						RingNum = cd_ringid[adc_num][k];
						RingEnergy = cd_ringenergy[adc_num][k];
						RingEnergyDiff = tempDiff;

					}

					CD_front_back[adc_num][2]->Fill(
						cd_ringenergy[adc_num][k]/1000., cd_stripenergy[adc_num][0]/1000. );

				} // k

				// Addback neighbouring rings
				for( k = 0; k < cd_ringenergy[adc_num].size(); k++ ) {

					if( TMath::Abs( RingNum - cd_ringid[adc_num][k] ) == 1 )

						RingEnergy += cd_ringenergy[adc_num][k];

				} // k

				Chan_front.push_back( RingNum );
				Ener_front.push_back( RingEnergy );

				cd_debug->Fill(2);
				CounterAdcCDFired[adc_num]++;

			} // N vs. 1

			// multiple on the front and multiple on the back
			else if( cd_ringenergy[adc_num].size() > 1 && cd_stripenergy[adc_num].size() > 1 ) {

				cd_debug->Fill(3);

				// throw these events away!!!
				continue;

				// Loop for multiplicity where separated by >1 rings
				while( RingMult ) {

					RingMult = false;
					Quad.push_back( adc_num );
					time.push_back( adc_t + dtAdc[adc_num] );
					laser.push_back( event->Adc(j)->LaserOn() );

					// Find highest energy on the front first
					RingEnergy = 0;
					for( k = 0; k < cd_ringenergy[adc_num].size(); k++ ) {

						CD_front_back[adc_num][3]->Fill(
							cd_ringenergy[adc_num][k]/1000., cd_stripenergy[adc_num][0]/1000. );
						
						// Check highest energy that we've not already used
						if( cd_ringenergy[adc_num][k] > RingEnergy && !cd_ringused[adc_num][k] ) {

							RingNum = cd_ringid[adc_num][k];
							RingEnergy = cd_ringenergy[adc_num][k];

						}

					}

					// Addback rings and "use" the highest energy strip
					for( k = 0; k < cd_ringenergy[adc_num].size(); k++ ) {

						if( cd_ringused[adc_num][k] == true ) continue;

						else if( TMath::Abs( RingNum - cd_ringid[adc_num][k] ) == 1 ) {

							RingEnergy += cd_ringenergy[adc_num][k];
							cd_ringused[adc_num][k] = true;

						}

						else if( RingNum == cd_ringid[adc_num][k] )

							cd_ringused[adc_num][k] = true;

						else if( TMath::Abs( RingNum - cd_ringid[adc_num][k] ) > 1 )

							RingMult = true;

					}

					Chan_front.push_back( RingNum );
					Ener_front.push_back( RingEnergy );

					CounterAdcCDFired[adc_num]++;

				} // while RingMult

				// Get closest match between front and back
				StripEnergyDiff = 99999999.;
				for( l = 0; l < Ener_front.size(); l++ ) {

					if( Quad[l] != adc_num ) continue;

					for( k = 0; k < cd_stripenergy[adc_num].size(); k++ ) {

						tempDiff = cd_stripenergy[adc_num][k] - Ener_front[l];
						tempDiff = TMath::Abs( tempDiff );

						if( tempDiff < StripEnergyDiff && !cd_stripused[adc_num][k] ) {

							StripNum = cd_stripid[adc_num][k];
							StripEnergy = cd_stripenergy[adc_num][k];
							StripEnergyDiff = tempDiff;

						}

					} // k

					Chan_back.push_back( StripNum );
					Ener_back.push_back( StripEnergy );

					for( k = 0; k < cd_stripenergy[adc_num].size(); k++ ) {

						if( cd_stripid[adc_num][k] == StripNum )

							cd_stripused[adc_num][k] = true;

					} // k

				} // l

			} // N vs. M
			// --------------------------------------- //

		
		} // j - numberofadcs
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Gamma/electron-particle coincidences
		// ------------------------------------------------------------------------ //
		for( j = 0; j < gen_array.size(); j++ ) {

			mb_evts[GammaCtr]->SetGen( gen_array[j] );
			mb_evts[GammaCtr]->SetCluid( clu_array[j] );
			mb_evts[GammaCtr]->SetCid( cid_array[j] );
			mb_evts[GammaCtr]->SetSid( sid_array[j] );
			mb_evts[GammaCtr]->SetTheta( gamma_theta[clu_array[j]][cid_array[j]%3][sid_array[j]] );
			mb_evts[GammaCtr]->SetPhi( gamma_phi[clu_array[j]][cid_array[j]%3][sid_array[j]] );
				
			// Do particles
			for( k = 0; k < Ener_front.size(); k++ ) {

				tdiffPG = time[k] - gtd_array[j];

				// electrons
				if( clu_array[j] == 8 ) { 

					tdiff_ep->Fill( tdiffPG );

					if( tMinRandomElectron <= tdiffPG && tdiffPG < tMaxRandomElectron ) coinc_flag = 1;
					else if( tMinPromptElectron <= tdiffPG && tdiffPG < tMaxPromptElectron ) coinc_flag = 0;
					else coinc_flag = -1;
						
				}

				// gamma-rays
				else {

					tdiff_gp->Fill( tdiffPG );
					tdiff_gp_q[(int)Quad[k]]->Fill( tdiffPG - dtAdc[(int)Quad[k]] );

					if( tMinDelayed <= tdiffPG && tdiffPG < tMaxDelayed ) coinc_flag = 2;
					else if( tMinRandom <= tdiffPG && tdiffPG < tMaxRandom ) coinc_flag = 1;
					else if( tMinPrompt <= tdiffPG && tdiffPG < tMaxPrompt ) coinc_flag = 0;
					else coinc_flag = -1;			

				}
				
				// Add particle
				mb_evts[GammaCtr]->SetPart( Ener_front[k], (int)Chan_front[k], (int)Chan_back[k],
					(double)time[k], (double)event->SuperCycleTime(), (double)event->T1Time(),
					(float)tdiffPG,	(int)coinc_flag, Quad[k], (int)laser[k] );
					
				PartCtr++;

			} // End loop All the particles

			// Look for correlated gammas or electrons
			for( l = 0; l < gen_array.size(); l++ ) {
				
				// skip if it's the same event as before
				if( l == j ) continue;
					
				// Found highest energy segment
				mb_evts[GammaCtr]->SetCorGamGen( gen_array[l] );
				mb_evts[GammaCtr]->SetCorGamCluid( clu_array[l] );
				mb_evts[GammaCtr]->SetCorGamCid( cid_array[l] );
				mb_evts[GammaCtr]->SetCorGamSid( sid_array[l] );
				mb_evts[GammaCtr]->SetCorGamGtd( gtd_array[l] - gtd_array[j] );
				mb_evts[GammaCtr]->SetCorGamTheta( gamma_theta[clu_array[l]][cid_array[l]%3][sid_array[l]] );
				mb_evts[GammaCtr]->SetCorGamPhi( gamma_phi[clu_array[l]][cid_array[l]%3][sid_array[l]] );
				
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
		for( j = 0; j < gen_array.size(); j++ ) {

		    for( k = 0; k < gen_array.size(); k++ ) {

		      if( j == k ) continue;
		      gg->Fill( gen_array[j], gen_array[k] );

		    }

		}

		// Progress bar
		if( i % 50000 == 0 ) {

			cout << setw(5) << setiosflags(ios::fixed) << setprecision(1);
			cout << (100.*i)/nentries << "% done\r" << flush;

		}

	} // i

	cout << "Check ADCs Fired:" << endl;
	for( i = 0; i < 5; i++ ) cout << " Adc # " << i << " fired: " << CounterAdcCDFired[i] << endl;

	outfile->Write();
	outfile->Close();

	return;
	
}

#endif