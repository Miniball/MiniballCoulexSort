#ifndef __PARTICLEFINDER_CXX
#define __PARTICLEFINDER_CXX

#define PBINS 800
#define PRANGE 800
#define PMIN -1.0*PRANGE/PBINS
#define PMAX PRANGE+PMIN

#define GBINS 4000
#define GRANGE 4000
#define GMIN -1.0*GRANGE/GBINS
#define GMAX GRANGE+GMIN

#define ELBINS 2000
#define ELRANGE 2000
#define ELMIN -1.0*ELRANGE/ELBINS
#define ELMAX ELRANGE+ELMIN

#ifndef ParticleFinder_hh
# include "ParticleFinder.hh"
#endif

void ParticleFinder::NextAdc() {
	
	fcdfrontenergy.clear();
	fcdbackenergy.clear();
	fcdfrontid.clear();
	fcdbackid.clear();
	
	bcdfrontenergy.clear();
	bcdbackenergy.clear();
	bcdfrontid.clear();
	bcdbackid.clear();
	
	fbarrelpos.clear();
	bbarrelpos.clear();
	fbarrelstrip.clear();
	bbarrelstrip.clear();
	
	vector<float>().swap( fcdfrontenergy );
	vector<float>().swap( fcdbackenergy );
	vector<int>().swap( fcdfrontid );
	vector<int>().swap( fcdbackid );
	
	vector<float>().swap( bcdfrontenergy );
	vector<float>().swap( bcdbackenergy );
	vector<int>().swap( bcdfrontid );
	vector<int>().swap( bcdbackid );
	
	vector<float>().swap( fbarrelpos );
	vector<float>().swap( bbarrelpos );
	vector<int>().swap( fbarrelstrip );
	vector<int>().swap( bbarrelstrip );
	fbarrelE = -1.;
	bbarrelE = -1.;
	
	for( unsigned int i = 0; i < 4; i++ ) {
		
		padenergy[i] = -1.;
		
	}

	maxfrontenergy_fcd = -1;
	maxbackenergy_fcd = -1;
	maxfrontenergy_bcd = -1;
	maxbackenergy_bcd = -1;
	
	return;
	
}

void ParticleFinder::ClearEvt() {
	
	PEn.clear();
	dE_En.clear();
	E_En.clear();
	Nf.clear();
	Nb.clear();
	Quad.clear();
	Sector.clear();
	time.clear();
	laser.clear();
	
	vector<float>().swap( PEn );
	vector<float>().swap( dE_En );
	vector<float>().swap( E_En );
	vector<int>().swap( Nf );
	vector<int>().swap( Nb );
	vector<unsigned int>().swap( Quad );
	vector<unsigned int>().swap( Sector );
	vector<long long>().swap( time );
	vector<bool>().swap( laser );

	return;

}

void ParticleFinder::FindCDParticles() {
	
	// Loop over each of the subevents (i.e the 32 channels)
	for( unsigned int i = 0; i < subevent->Size(); i++ ) {
		
		adc_ch = subevent->AdcChannel(i);
		adc_en = subevent->AdcValue(i);
		
		PartEnergy = Cal->AdcEnergy( adc_num, adc_ch, adc_en );

		adc_spec[adc_num][adc_ch]->Fill( adc_en );
		adc_spec_cal[adc_num][adc_ch]->Fill( PartEnergy/1000. );
		
		// Check threshold for every channel individually
		if( adc_en > Cal->AdcThreshold( adc_num, adc_ch ) && adc_en < 3839 ) {

			if( adc_ch < 16 ) { // front rings
				
				fcdfrontenergy.push_back( PartEnergy );
				fcdfrontid.push_back( adc_ch );
				
				if( PartEnergy > maxfrontenergy_fcd ) {
					
					maxfrontenergy_fcd = PartEnergy;
					maxfrontid_fcd = adc_ch;
					
				}
				
				part_raw->Fill( adc_ch, PartEnergy/1000. );
				
			} // front rings
		
			else if( adc_ch < 28 ) { // back strips
			
				fcdbackenergy.push_back( PartEnergy );
				fcdbackid.push_back( adc_ch-16 );
				
				if( PartEnergy > maxfrontenergy_fcd ) {
					
					maxbackenergy_fcd = PartEnergy;
					maxbackid_fcd = adc_ch-16;
					
				}
				
			} // back strips
		
		} // threshold
		
	} // k
	
	return;
	
}

void ParticleFinder::FindCREXParticles() {
	
	// Loop over each of the subevents (i.e the 32 channels)
	for( unsigned int i = 0; i < subevent->Size(); i++ ) {
		
		adc_ch = subevent->AdcChannel(i);
		adc_en = subevent->AdcValue(i);
		
		PartEnergy = Cal->AdcEnergy( adc_num, adc_ch, adc_en );
		
		// Raw spectra
		adc_spec[adc_num][adc_ch]->Fill( adc_en );
		
		// Calibrated spectra - Mux not divided by 1000.
		if( adc_ch == 19 || adc_ch == 21 || adc_ch == 23 || adc_ch == 25 )
			adc_spec_cal[adc_num][adc_ch]->Fill( PartEnergy );
		else
			adc_spec_cal[adc_num][adc_ch]->Fill( PartEnergy/1000. );
		
		// Check threshold for every channel individually
		if( adc_en > Cal->AdcThreshold( adc_num, adc_ch ) && adc_en < 3839 ) {
			
			// Barrel ∆E - backward
			if( adc_ch < 16 ) {
				
				bbarrelpos.push_back( PartEnergy );
				bbarrelstrip.push_back( adc_ch );
				
			}

			// PADs
			if( adc_ch == 16 ) bbarrelE = PartEnergy;
			if( adc_ch == 17 ) padenergy[2] = PartEnergy;
			if( adc_ch == 26 ) padenergy[0] = PartEnergy;
			if( adc_ch == 27 ) padenergy[3] = PartEnergy;
			
			// MUX - front hits
			if( adc_ch == 19 || adc_ch == 21 ) {
				
				mux_id = DeMux( adc_ch, adc_en, -1 );
				
				if( mux_id < 18 && MuxEnergy > 0 ) { // FCD
					
					fcdfrontid.push_back( mux_id );
					fcdfrontenergy.push_back( MuxEnergy );
					
					if( MuxEnergy > maxfrontenergy_fcd ){
						
						maxfrontenergy_fcd = fcdfrontenergy.back();
						maxfrontid_fcd = fcdfrontid.back();
						
					}
					
				} // FCD
				
				else if( MuxEnergy > 0 ) { // BCD
					
					bcdfrontid.push_back( mux_id-32 );
					bcdfrontenergy.push_back( MuxEnergy );
					
					if( MuxEnergy > maxfrontenergy_bcd ){
						
						maxfrontenergy_bcd = bcdfrontenergy.back();
						maxfrontid_bcd = bcdfrontid.back();
						
					}
					
				} // BCD
				
			} // MUX front
			
			// MUX - back hits
			if( adc_ch == 23 || adc_ch == 25 ) {
				
				mux_id = DeMux( adc_ch, adc_en, -1 );
				
				if( mux_id < 18 && MuxEnergy > 0 ) { // FCD
					
					fcdbackid.push_back( mux_id );
					fcdbackenergy.push_back( MuxEnergy );
					
					if( MuxEnergy > maxbackenergy_fcd ){
						
						maxbackenergy_fcd = fcdbackenergy.back();
						maxbackid_fcd = fcdbackid.back();
						
					}
					
				} // FCD
				
				else if( MuxEnergy > 0 ) { // BCD
					
					bcdbackid.push_back( mux_id-32 );
					bcdbackenergy.push_back( MuxEnergy );
					
					if( MuxEnergy > maxbackenergy_bcd ){
						
						maxbackenergy_bcd = bcdbackenergy.back();
						maxbackid_bcd = bcdbackid.back();
						
					}
					
				} // BCD
				
			} // MUX back
			
		} // threshold
		
	} // k
	
	return;
	
}

void ParticleFinder::FindTREXParticles() {
	
	// Loop over each of the subevents (i.e the 32 channels)
	for( unsigned int i = 0; i < subevent->Size(); i++ ) {
		
		adc_ch = subevent->AdcChannel(i);
		adc_en = subevent->AdcValue(i);
		
		PartEnergy = Cal->AdcEnergy( adc_num, adc_ch, adc_en );
		
		// Raw spectra
		adc_spec[adc_num][adc_ch]->Fill( adc_en );

		// Calibrated spectra - Mux not divided by 1000.
		if( adc_ch == 2 || adc_ch == 3 || adc_ch == 6 || adc_ch == 7 )
			adc_spec_cal[adc_num][adc_ch]->Fill( PartEnergy );
		else
			adc_spec_cal[adc_num][adc_ch]->Fill( PartEnergy/1000. );
		
		//cout << "ADC #" << adc_num << "; adc_ch = " << adc_ch << "; adc_en = " << adc_en << endl;
		
		// Check threshold for every channel individually
		if( adc_en > Cal->AdcThreshold( adc_num, adc_ch ) && adc_en < 3839 ) {
			
			// First ADC
			if( adc_num%2 == 0 ) {
				
				// Barrel ∆E - forward
				if( adc_ch < 16 ) {
					
					fbarrelpos.push_back( PartEnergy );
					fbarrelstrip.push_back( adc_ch );
					
				}
				
				// Barrel ∆E - backward
				else if( adc_ch >= 16 && adc_ch < 32 ) {
					
					bbarrelpos.push_back( PartEnergy );
					bbarrelstrip.push_back( adc_ch-16 );
					
				}
				
			} // First ADC
			
			// Second ADC
			if( adc_num%2 == 1 ) {
				
				// PADs
				if( adc_ch ==  8 ) padenergy[0] = PartEnergy;
				if( adc_ch ==  9 ) padenergy[3] = PartEnergy;
				if( adc_ch == 10 ) padenergy[1] = PartEnergy;
				if( adc_ch == 11 ) fbarrelE = PartEnergy;
				if( adc_ch == 12 ) padenergy[2] = PartEnergy;
				if( adc_ch == 13 ) bbarrelE = PartEnergy;
				
				// BCD front strips
				if( adc_ch >= 16 ) {
					
					bcdfrontenergy.push_back( PartEnergy );
					bcdfrontid.push_back( adc_ch-16 );
					
					if( PartEnergy > maxfrontenergy_fcd ){
						
						maxfrontenergy_bcd = bcdfrontenergy.back();
						maxfrontid_bcd = bcdfrontid.back();
						
					}
					
				}
				
				// MUX - front hits
				if( adc_ch == 2 || adc_ch == 3 ) {
					
					mux_id = DeMux( adc_ch, adc_en, 2 );
					
					if( MuxEnergy > 0 ) {
						
						fcdfrontid.push_back( mux_id );
						fcdfrontenergy.push_back( MuxEnergy );

						if( MuxEnergy > maxfrontenergy_fcd ){
							
							maxfrontenergy_fcd = fcdfrontenergy.back();
							maxfrontid_fcd = fcdfrontid.back();
							
						}
						
					}
					
				} // MUX front
				
				// MUX - back hits
				if( adc_ch == 6 || adc_ch == 7 ) {
					
					mux_id = DeMux( adc_ch, adc_en, 2 );
					
					if( mux_id < 18 && MuxEnergy > 0 ) { // FCD
						
						fcdbackid.push_back( mux_id );
						fcdbackenergy.push_back( MuxEnergy );
						
						if( MuxEnergy > maxbackenergy_fcd ){
							
							maxbackenergy_fcd = fcdbackenergy.back();
							maxbackid_fcd = fcdbackid.back();
							
						}
						
					} // FCD
					
					else if( MuxEnergy > 0 ) { // BCD
						
						bcdbackid.push_back( mux_id-32 );
						bcdbackenergy.push_back( MuxEnergy );
						
						if( MuxEnergy > maxbackenergy_bcd ){
							
							maxbackenergy_bcd = bcdbackenergy.back();
							maxbackid_bcd = bcdbackid.back();
							
						}
						
					} // BCD
					
				} // MUX back
				
			} // Second ADC
			
		} // threshold
		
	} // k
	
	return;
	
}

unsigned int ParticleFinder::ReconstructCD( bool trex, bool crex ) {
	
	///////////////////////////////
	// Reconstruct the CD events //
	///////////////////////////////
	
	float E, dE, Etot;
	int counter = 0;

	/////////////////
	// Forwards CD //
	/////////////////
	
	// Easy case, 1 front and 1 back!
	if( fcdfrontenergy.size() == 1 && fcdbackenergy.size() == 1 ) {
		
		// Particle #1
		dE = fcdfrontenergy.at(0);
		E = padenergy[0];
		if( trex ) Etot = dE + E;
		else Etot = dE;

		PEn.push_back( Etot );
		dE_En.push_back( dE );
		E_En.push_back( E );
		Nf.push_back( fcdfrontid.at(0) );
		Nb.push_back( fcdbackid.at(0) );
		if( trex ) Quad.push_back( adc_num/2 );
		else Quad.push_back( adc_num );
		if( crex ) Sector.push_back( 0 );
		else Sector.push_back( 4 );
		time.push_back( adc_t );
		laser.push_back( laser_status );
		
		E_f_b[adc_num]->Fill( fcdfrontenergy.at(0)/1000., fcdbackenergy.at(0)/1000. );
		
		CD_front_energy[Quad.back()]->Fill( fcdfrontid.at(0), fcdfrontenergy.at(0)/1000. );
		CD_back_energy[Quad.back()]->Fill( fcdbackid.at(0), fcdbackenergy.at(0)/1000. );
		
		cd_debug->Fill(0);
		counter++;
		
	} // 1 vs. 1
	
	// 1 on the front and 2 on the back
	else if( fcdfrontenergy.size() == 1 && fcdbackenergy.size() == 2 ) {
		
		// Select events in neighbouring strips with good energy
		int ndiff = TMath::Abs( fcdbackid.at(0) - fcdbackid.at(1) );
		float eback = fcdbackenergy.at(0) + fcdbackenergy.at(1);
		float ediff = TMath::Abs( fcdfrontenergy.at(0) - eback );
		
		if( ndiff != 1 || ediff > 50e3 ) return 0;
		
		dE = fcdfrontenergy.at(0);
		E = padenergy[0];
		if( trex ) Etot = dE + E;
		else Etot = dE;
		
		PEn.push_back( Etot );
		dE_En.push_back( dE );
		E_En.push_back( E );
		Nf.push_back( fcdfrontid.at(0) );
		Nb.push_back( maxbackid_fcd );
		if( trex ) Quad.push_back( adc_num/2 );
		else Quad.push_back( adc_num );
		if( crex ) Sector.push_back( 0 );
		else Sector.push_back( 4 );
		time.push_back( adc_t );
		laser.push_back( laser_status );
		
		E_f_b[adc_num]->Fill( fcdfrontenergy.at(0)/1000., fcdbackenergy.at(0)/1000. );
		E_f_b[adc_num]->Fill( fcdfrontenergy.at(0)/1000., fcdbackenergy.at(1)/1000. );
		
		CD_front_energy[Quad.back()]->Fill( fcdfrontid.at(0), fcdfrontenergy.at(0)/1000. );
		CD_back_energy[Quad.back()]->Fill( fcdbackid.at(0), fcdbackenergy.at(0)/1000. );

		cd_debug->Fill(1);
		counter++;
		
	} // 1 vs. 2
	
	// 2 on the front and 1 on the back
	else if( fcdfrontenergy.size() == 2 && fcdbackenergy.size() == 1 ) {
		
		// Select events in neighbouring strips with good energy
		int ndiff = TMath::Abs( fcdfrontid.at(0) - fcdfrontid.at(1) );
		float efront = fcdfrontenergy.at(0) + fcdfrontenergy.at(1);
		float ediff = TMath::Abs( fcdbackenergy.at(0) - efront );
		
		if( ndiff != 1 || ediff > 50e3 ) return 0;
		
		dE = fcdbackenergy.at(0);
		E = padenergy[0];
		if( trex ) Etot = dE + E;
		else Etot = dE;
		
		PEn.push_back( Etot );
		dE_En.push_back( dE );
		E_En.push_back( E );
		Nb.push_back( fcdbackid.at(0) );
		Nf.push_back( maxfrontid_fcd );
		if( trex ) Quad.push_back( adc_num/2 );
		else Quad.push_back( adc_num );
		if( crex ) Sector.push_back( 0 );
		else Sector.push_back( 4 );
		time.push_back( adc_t );
		laser.push_back( laser_status );
		
		E_f_b[adc_num]->Fill( fcdfrontenergy.at(0)/1000., fcdbackenergy.at(0)/1000. );
		E_f_b[adc_num]->Fill( fcdfrontenergy.at(1)/1000., fcdbackenergy.at(0)/1000. );
		
		CD_front_energy[Quad.back()]->Fill( fcdfrontid.at(0), fcdfrontenergy.at(0)/1000. );
		CD_back_energy[Quad.back()]->Fill( fcdbackid.at(0), fcdbackenergy.at(0)/1000. );
		
		cd_debug->Fill(2);
		counter++;
		
	} // 2 vs. 1
	
	// multiple on the front and multiple on the back
	else if( fcdfrontenergy.size() > 1 && fcdbackenergy.size() > 1 ) {
		
		cd_debug->Fill(3);
		
		// throw these events away!!!
		
	} // N vs. M
	
	// There may be events with no front energy or no back energy
	// Currently we throw them away, but could be recovered if
	// one quadrant only has a single dead strip, for example
	else if( fcdfrontenergy.size() == 0 && bcdfrontenergy.size() == 0
				&& ( padenergy[0] > 0 || padenergy[1] > 0 ||
					 padenergy[2] > 0 || padenergy[3] > 0 )
				&& PEn.size() == 0 ) cd_debug->Fill(4);
	
	else if( fcdbackenergy.size() == 0 && bcdfrontenergy.size() == 0
				&& ( padenergy[0] > 0 || padenergy[1] > 0 ||
					 padenergy[2] > 0 || padenergy[3] > 0 )
				&& PEn.size() == 0 ) cd_debug->Fill(5);
	
	//////////////////
	// Backwards CD //
	//////////////////
	
	// Easy case, 1 front and 1 back!
	if( bcdfrontenergy.size() == 1 && bcdbackenergy.size() == 1 ) {
		
		dE = bcdfrontenergy.at(0);
		E = padenergy[3];
		if( trex ) Etot = dE + E;
		else Etot = dE;
		
		PEn.push_back( Etot );
		dE_En.push_back( dE );
		E_En.push_back( E );
		Nf.push_back( bcdfrontid.at(0) );
		Nb.push_back( bcdbackid.at(0) );
		if( trex ) Quad.push_back( adc_num/2 );
		else Quad.push_back( adc_num );
		Sector.push_back( 3 );
		time.push_back( adc_t );
		laser.push_back( laser_status );
		
		//E_f_b[adc_num]->Fill( bcdfrontenergy.at(0)/1000., bcdbackenergy.at(0)/1000. );
		
		CD_front_energy[Quad.back()]->Fill( bcdfrontid.at(0)+16, bcdfrontenergy.at(0)/1000. );
		CD_back_energy[Quad.back()]->Fill( bcdbackid.at(0)+16, bcdbackenergy.at(0)/1000. );
		
		cd_debug->Fill(10);
		counter++;
		
	} // 1 vs. 1
	
	// 1 on the front and 2 on the back
	else if( bcdfrontenergy.size() == 1 && bcdbackenergy.size() == 2 ) {
		
		// Select events in neighbouring strips with good energy
		int ndiff = TMath::Abs( bcdbackid.at(0) - bcdbackid.at(1) );
		float eback = bcdbackenergy.at(0) + bcdbackenergy.at(1);
		float ediff = TMath::Abs( bcdfrontenergy.at(0) - eback );
		
		if( ndiff != 1 || ediff > 50e3 ) return 0;
		
		dE = bcdfrontenergy.at(0);
		E = padenergy[3];
		if( trex ) Etot = dE + E;
		else Etot = dE;
		
		PEn.push_back( Etot );
		dE_En.push_back( dE );
		E_En.push_back( E );
		Nf.push_back( bcdfrontid.at(0) );
		Nb.push_back( maxbackid_bcd );
		if( trex ) Quad.push_back( adc_num/2 );
		else Quad.push_back( adc_num );
		Sector.push_back( 3 );
		time.push_back( adc_t );
		laser.push_back( laser_status );
		
		//E_f_b[adc_num]->Fill( bcdfrontenergy.at(0)/1000., bcdbackenergy.at(0)/1000. );
		//E_f_b[adc_num]->Fill( bcdfrontenergy.at(0)/1000., bcdbackenergy.at(1)/1000. );
		
		CD_front_energy[Quad.back()]->Fill( bcdfrontid.at(0)+16, bcdfrontenergy.at(0)/1000. );
		CD_back_energy[Quad.back()]->Fill( bcdbackid.at(0)+16, bcdbackenergy.at(0)/1000. );
		
		cd_debug->Fill(11);
		counter++;
		
	} // 1 vs. 2
	
	// 2 on the front and 1 on the back
	else if( bcdfrontenergy.size() == 2 && bcdbackenergy.size() == 1 ) {
		
		// Select events in neighbouring strips with good energy
		int ndiff = TMath::Abs( bcdfrontid.at(0) - bcdfrontid.at(1) );
		float efront = bcdfrontenergy.at(0) + bcdfrontenergy.at(1);
		float ediff = TMath::Abs( bcdbackenergy.at(0) - efront );
		
		if( ndiff != 1 || ediff > 50e3 ) return 0;
		
		dE = bcdbackenergy.at(0);
		E = padenergy[3];
		if( trex ) Etot = dE + E;
		else Etot = dE;
		
		PEn.push_back( Etot );
		dE_En.push_back( dE );
		E_En.push_back( E );
		Nb.push_back( bcdbackid.at(0) );
		Nf.push_back( maxfrontid_bcd );
		if( trex ) Quad.push_back( adc_num/2 );
		else Quad.push_back( adc_num );
		Sector.push_back( 3 );
		time.push_back( adc_t );
		laser.push_back( laser_status );
		
		//E_f_b[adc_num]->Fill( bcdfrontenergy.at(0)/1000., bcdbackenergy.at(0)/1000. );
		//E_f_b[adc_num]->Fill( bcdfrontenergy.at(1)/1000., bcdbackenergy.at(0)/1000. );
		
		CD_front_energy[Quad.back()]->Fill( bcdfrontid.at(0)+16, bcdfrontenergy.at(0)/1000. );
		CD_back_energy[Quad.back()]->Fill( bcdbackid.at(0)+16, bcdbackenergy.at(0)/1000. );
		
		cd_debug->Fill(12);
		counter++;
		
	} // 2 vs. 1
	
	// multiple on the front and multiple on the back
	else if( bcdfrontenergy.size() > 1 && bcdbackenergy.size() > 1 ) {
		
		cd_debug->Fill(13);
		
		// throw these events away!!!
		
	} // N vs. M
	
	// There may be events with no front energy or no back energy
	// Currently we throw them away, but could be recovered if
	// one quadrant only has a single dead strip, for example
	else if( bcdfrontenergy.size() == 0
			&& ( padenergy[0] > 0 || padenergy[1] > 0 ||
				 padenergy[2] > 0 || padenergy[3] > 0 )
			&& PEn.size() == 0 ) cd_debug->Fill(14);
	
	else if( bcdbackenergy.size() == 0
			&& ( padenergy[0] > 0 || padenergy[1] > 0 ||
				 padenergy[2] > 0 || padenergy[3] > 0 )
			&& PEn.size() == 0 ) cd_debug->Fill(15);
	
	else if( PEn.size() == 0 ) cd_debug->Fill(20);
	
	// Plotting
	for( unsigned int i = 0; i < PEn.size(); i++ )
		part->Fill( Nf.at(i) + 16*Sector.at(i), PEn.at(i)/1000. );

	return counter;
	
}

unsigned int ParticleFinder::ReconstructBarrel( bool trex, bool crex ) {
	
	///////////////////////////////////
	// Reconstruct the Barrel events //
	///////////////////////////////////
	
	float E, dE, Etot;
	unsigned int counter = 0;
	
	if( fbarrelpos.size() > 0 ) barrel_debug->Fill(0);
	if( bbarrelpos.size() > 0 ) barrel_debug->Fill(10);
	
	if( fbarrelpos.size() == 1 ) {
		
		dE = fbarrelE;
		E = padenergy[1];
		if( trex ) Etot = dE + E;
		else Etot = dE;
		
		PEn.push_back( Etot );
		dE_En.push_back( dE );
		E_En.push_back( E );
		Nf.push_back( fbarrelstrip[0] );
		Nb.push_back( StripPosBarrel( dE, fbarrelpos[0] ) );
		if( trex ) Quad.push_back( adc_num/2 );
		else Quad.push_back( adc_num );
		Sector.push_back( 1 );
		time.push_back( adc_t );
		laser.push_back( laser_status );
		
		Barrel_energy[Quad.back()]->Fill( fbarrelstrip[0], dE/1000. );
		
		counter++;
		barrel_debug->Fill(1);
		
	}
	
	if( bbarrelpos.size() == 1 ) {
		
		dE = bbarrelE;
		E = padenergy[2];
		if( trex ) Etot = dE + E;
		else Etot = dE;
		
		PEn.push_back( Etot );
		dE_En.push_back( dE );
		E_En.push_back( E );
		Nf.push_back( bbarrelstrip[0] );
		Nb.push_back( StripPosBarrel( dE, bbarrelpos[0] ) );
		if( trex ) Quad.push_back( adc_num/2 );
		else Quad.push_back( adc_num );
		Sector.push_back( 2 );
		time.push_back( adc_t );
		laser.push_back( laser_status );
		
		Barrel_energy[Quad.back()]->Fill( bbarrelstrip[0]+16, dE/1000. );
		
		counter++;
		barrel_debug->Fill(11);
		
	}
	
	return counter;
	
}

void ParticleFinder::IonisationChamber() {

	for( unsigned int i = 0; i < subevent->Size(); i++ ) {
		
		adc_ch = subevent->AdcChannel(i);
		adc_en = subevent->AdcValue(i);
		
		icDE_Energy = Cal->AdcEnergy( adc_num, adc_ch, adc_en );
		
		if( adc_ch != 0 ) continue; // Gas
		
		for( unsigned int j = 0; j < subevent->Size(); j++ ){
			
			adc_ch2 = subevent->AdcChannel(j);
			adc_en2 = subevent->AdcValue(j);
			
			if( adc_ch2 != 1 ) continue; // Si
			
			icE_Energy = Cal->AdcEnergy( adc_num, adc_ch2, adc_en2 );
			
			dEE->Fill( icE_Energy, icDE_Energy );
			
		} // l
		
	} // k

	return;
	
}

int ParticleFinder::StripPosBarrel( float frontEn, float backEn ) {
	
	for( unsigned int i = 0; i < 16; i++ ) {
		
		if( frontEn/backEn <= 0.2 + 0.1125*(i+1) ) return i;
		
	}
	
	if( frontEn/backEn >= 2 ) return 15;
	else return -1;
	
}

unsigned int ParticleFinder::DeMux( unsigned int mux_ch, unsigned int mux_en, int ch_offset ) {
	
	// Get the id from the mux calibration (must do better)
	float mux_id = Cal->AdcEnergy( adc_num, mux_ch, mux_en ) + 0.5;
	
	// Look for the corresponding energy in the subevents
	for( unsigned int i = 0; i < subevent->Size(); i++ ) {
		
		adc_ch2 = subevent->AdcChannel(i);
		adc_en2 = subevent->AdcValue(i);
		
		// Set the MUX energy when the matching channel is found
		if( adc_ch2 == mux_ch + ch_offset ) {
			
			if( adc_en2 < 3839 ) MuxEnergy = Cal->AdcEnergy( adc_num, adc_ch2, adc_en2 );
			else MuxEnergy = -99.;
			break;
			
		}
		
	}
	
	//if( mux_ch == 2 || mux_ch == 3 ) cout << mux_id << endl;
	//if( mux_ch == 6 || mux_ch == 7 ) cout << mux_id << endl;
	
	return (int)mux_id;
	
}


#endif
