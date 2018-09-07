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
	
	frontenergy.clear();
	backenergy.clear();
	frontid.clear();
	backid.clear();
	
	vector<float>().swap( frontenergy );
	vector<float>().swap( backenergy );
	vector<unsigned short>().swap( frontid );
	vector<unsigned short>().swap( backid );
	
	maxfrontenergy = -1;
	maxbackenergy = -1;

	return;
	
}

void ParticleFinder::ClearEvt() {
	
	PEn.clear();
	Nf.clear();
	Nb.clear();
	Quad.clear();
	time.clear();
	laser.clear();
	
	vector<float>().swap( PEn );
	vector<unsigned int>().swap( Nf );
	vector<unsigned int>().swap( Nb );
	vector<unsigned int>().swap( Quad );
	vector<long long>().swap( time );
	vector<bool>().swap( laser );

	return;

}

void ParticleFinder::FindParticles() {
	
	// Loop over each of the subevents (i.e the 32 channels)
	for( unsigned int i = 0; i < subevent->Size(); i++ ) {
		
		adc_ch = subevent->AdcChannel(i);
		adc_en = subevent->AdcValue(i);
		
		PartEnergy = Cal->AdcEnergy( adc_num, adc_ch, adc_en );

		E_part_ch[adc_num][adc_ch]->Fill( adc_en );
		E_part_ch_cal[adc_num][adc_ch]->Fill( PartEnergy/1000. );
		
		// Check threshold for every channel individually
		if( adc_en > Cal->AdcThreshold( adc_num, adc_ch ) ) {

			if( adc_ch < 16 ) { // front rings
				
				frontenergy.push_back( PartEnergy );
				frontid.push_back( adc_ch );
				
				if( PartEnergy > maxfrontenergy ) {
					
					maxfrontenergy = PartEnergy;
					maxfrontid = adc_ch;
					
				}
				
			} // front rings
		
			else if( adc_ch < 28 ) { // back strips
			
				backenergy.push_back( PartEnergy );
				backid.push_back( adc_ch-16 );
				
				if( PartEnergy > maxfrontenergy ) {
					
					maxbackenergy = PartEnergy;
					maxbackid = adc_ch-16;
					
				}
				
			} // back strips
		
		} // threshold
		
	} // k
	
	return;
	
}

unsigned int ParticleFinder::ReconstructHeavyIons() {
	
	// Easy case, 1 front and 1 back!
	if( frontenergy.size() == 1 && backenergy.size() == 1 ) {
		
		PEn.push_back( frontenergy.at(0) );
		Nf.push_back( frontid.at(0) );
		Nb.push_back( backid.at(0) );
		Quad.push_back( adc_num );
		time.push_back( adc_t );
		laser.push_back( laser_status );
		
		E_f_b[adc_num]->Fill( frontenergy.at(0)/1000., backenergy.at(0)/1000. );
		
		cd_debug->Fill(0);
		return 1;
		
	} // 1 vs. 1
	
	// 1 on the front and 2 on the back
	else if( frontenergy.size() == 1 && backenergy.size() == 2 ) {
		
		// Select events in neighbouring strips with good energy
		int ndiff = TMath::Abs( backid.at(0) - backid.at(1) );
		float eback = backenergy.at(0) + backenergy.at(1);
		float ediff = TMath::Abs( frontenergy.at(0) - eback );
		
		if( ndiff != 1 && ediff > 50e3 ) return 0;
		
		PEn.push_back( frontenergy.at(0) );
		Nf.push_back( frontid.at(0) );
		Nb.push_back( maxbackid );
		Quad.push_back( adc_num );
		time.push_back( adc_t );
		laser.push_back( laser_status );
		
		E_f_b[adc_num]->Fill( frontenergy.at(0)/1000., backenergy.at(0)/1000. );
		E_f_b[adc_num]->Fill( frontenergy.at(0)/1000., backenergy.at(1)/1000. );
		
		cd_debug->Fill(1);
		return 1;
		
	} // 1 vs. 2
	
	// 2 on the front and 1 on the back
	else if( frontenergy.size() == 2 && backenergy.size() == 1 ) {
		
		// Select events in neighbouring strips with good energy
		int ndiff = TMath::Abs( frontid.at(0) - frontid.at(1) );
		float efront = frontenergy.at(0) + frontenergy.at(1);
		float ediff = TMath::Abs( backenergy.at(0) - efront );
		
		if( ndiff != 1 && ediff > 50e3 ) return 0;
		
		PEn.push_back( backenergy.at(0) );
		Nb.push_back( backid.at(0) );
		Nf.push_back( maxfrontid );
		Quad.push_back( adc_num );
		time.push_back( adc_t );
		laser.push_back( laser_status );
		
		E_f_b[adc_num]->Fill( frontenergy.at(0)/1000., backenergy.at(0)/1000. );
		E_f_b[adc_num]->Fill( frontenergy.at(1)/1000., backenergy.at(0)/1000. );
		
		cd_debug->Fill(2);
		return 1;
		
	} // 2 vs. 1
	
	// multiple on the front and multiple on the back
	else if( frontenergy.size() > 1 && backenergy.size() > 1 ) {
		
		cd_debug->Fill(3);
		
		// throw these events away!!!
		return 0;
		
	} // N vs. M

	// There maybe events with no front energy or no back energy
	// Currently we throw them away, but could be recovered if
	// one quadrant only has a single dead strip, for example
	else {
		
		cd_debug->Fill(4);
		return 0;
	
	}
	
}

#endif