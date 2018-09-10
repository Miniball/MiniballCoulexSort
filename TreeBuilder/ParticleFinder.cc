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
	frontsector.clear();
	backsector.clear();

	frontenergy2.clear();
	backenergy2.clear();
	frontid2.clear();
	backid2.clear();
	frontsector2.clear();
	backsector2.clear();
	
	fbarrelenergy.clear();
	bbarrelenergy.clear();
	fbarrelstrip.clear();
	bbarrelstrip.clear();
	
	vector<float>().swap( frontenergy );
	vector<float>().swap( backenergy );
	vector<unsigned short>().swap( frontid );
	vector<unsigned short>().swap( backid );
	vector<unsigned short>().swap( frontsector );
	vector<unsigned short>().swap( backsector );
	
	vector<float>().swap( frontenergy2 );
	vector<float>().swap( backenergy2 );
	vector<unsigned short>().swap( frontid2 );
	vector<unsigned short>().swap( backid2 );
	vector<unsigned short>().swap( frontsector2 );
	vector<unsigned short>().swap( backsector2 );
	
	vector<float>().swap( fbarrelenergy );
	vector<float>().swap( bbarrelenergy );
	vector<unsigned short>().swap( fbarrelstrip );
	vector<unsigned short>().swap( bbarrelstrip );
	fbarrelpos = -99.;
	bbarrelpos = -99.;
	
	for( unsigned int i = 0; i < 4; i++ ) {
		
		padenergy[i] = -99.;
		
	}

	maxfrontenergy = -1;
	maxbackenergy = -1;
	
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
	vector<unsigned int>().swap( Nf );
	vector<unsigned int>().swap( Nb );
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

void ParticleFinder::FindTREXParticles() {
	
	// Loop over each of the subevents (i.e the 32 channels)
	for( unsigned int i = 0; i < subevent->Size(); i++ ) {
		
		adc_ch = subevent->AdcChannel(i);
		adc_en = subevent->AdcValue(i);
		
		PartEnergy = Cal->AdcEnergy( adc_num, adc_ch, adc_en );
		
		trex[adc_num][adc_ch]->Fill( adc_en );
		trex_cal[adc_num][adc_ch]->Fill( PartEnergy/1000. );
		
		// Check threshold for every channel individually
		if( adc_en > Cal->AdcThreshold( adc_num, adc_ch ) ) {
			
			// Barrel ∆E
			if( adc_num%2 == 0 ) {
				
				if( adc_ch < 16 ) {
					
					fbarrelenergy.push_back( adc_en );
					fbarrelstrip.push_back( adc_ch );

				}
				
				else if( adc_ch < 32 ) {
					
					bbarrelenergy.push_back( adc_en );
					bbarrelstrip.push_back( adc_ch );
					
				}
				
			}
			
			// MUX, CDs and PADs
			else if( adc_num%2 == 1 ) {
			
				// MUX
				if( adc_ch == 0 ) frontenergy.push_back( PartEnergy );
				if( adc_ch == 1 ) frontenergy2.push_back( PartEnergy );
				if( adc_ch == 2 ) {
					
					if( PartEnergy < 15.5 ) {
						
						frontsector.push_back( 0 );
						frontid.push_back( (int)(PartEnergy+0.5) );
						
					}
					
					else {
						
						frontsector.push_back( 3 );
						frontid.push_back( (int)(PartEnergy+0.5) - 16 );
						
					}
					
				}
				
				if( adc_ch == 3 ) {
					
					if( PartEnergy < 15.5 ) {
						
						frontsector2.push_back( 0 );
						frontid2.push_back( (int)(PartEnergy+0.5) );
						
					}
					
					else {
						
						frontsector2.push_back( 3 );
						frontid2.push_back( (int)(PartEnergy+0.5) - 16 );
						
					}
					
				}
					
				if( adc_ch == 4 ) backenergy.push_back( PartEnergy );
				if( adc_ch == 5 ) backenergy2.push_back( PartEnergy );
				if( adc_ch == 6 ) {
					
					if( PartEnergy < 15.5 ) {
						
						backsector.push_back( 0 );
						backid.push_back( (int)(PartEnergy+0.5) );
						
					}
					
					else {
						
						backsector.push_back( 3 );
						backid.push_back( (int)(PartEnergy+0.5) - 16 );
						
					}
					
				}
				if( adc_ch == 7 ) {

					if( PartEnergy < 15.5 ) {
						
						backsector2.push_back( 0 );
						backid2.push_back( (int)(PartEnergy+0.5) );
						
					}
					
					else {
						
						backsector2.push_back( 3 );
						backid2.push_back( (int)(PartEnergy+0.5) - 16 );
						
					}
					
				}
				
				// PADs
				if( adc_ch ==  8 ) padenergy[0] = PartEnergy;
				if( adc_ch ==  9 ) padenergy[3] = PartEnergy;
				if( adc_ch == 10 ) fbarrelpos = PartEnergy;
				if( adc_ch == 11 ) bbarrelpos = PartEnergy;
				if( adc_ch == 12 ) padenergy[1] = PartEnergy;
				if( adc_ch == 13 ) padenergy[2] = PartEnergy;
				
				// BCD front strips
				if( adc_ch >= 16 ) {
				
					frontenergy.push_back( PartEnergy );
					frontid.push_back( adc_ch );
					frontsector.push_back( 3 );
					
				}
				
			}
			
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

unsigned int ParticleFinder::ReconstructTransfer() {
	
	///////////////////////////////
	// Reconstruct the CD events //
	///////////////////////////////
	
	float E, dE;
	
	// One vs. X
	if( frontenergy.size() == 1 && frontenergy2.size() == 0 ) {
		
		// The id and sector sizes have to be the same
		if( frontid.size() != 1 ) return 0;
		if( frontsector.size() != 1 ) return 0;
		
		// One vs. one
		if( backenergy.size() == 1 && backenergy2.size() == 0 ) {
			
			cd_debug->Fill(0);

			// The id and sector sizes have to be the same
			if( backid.size() != 1 ) return 0;
			if( backsector.size() != 1 ) return 0;
			
			// Good CD sector numbers
			if( frontsector[0] < 0 || frontsector[0] > 3 ) return 0;
			if( backsector[0] < 0 || backsector[0] > 3 ) return 0;
			cd_debug->Fill(1);
			
			// Same CD for front and back
			if( frontsector[0] != backsector[0] ) return 0;
			cd_debug->Fill(2);
			
			// Particle #1
			dE = frontenergy[0];
			E = padenergy[frontsector[0]];
			
			PEn.push_back( dE + E );
			dE_En.push_back( dE );
			E_En.push_back( E );
			Nf.push_back( frontid[0] );
			Nb.push_back( backid[0] );
			Quad.push_back( adc_num/2 );
			Sector.push_back( frontsector[0] );
			time.push_back( adc_t );
			laser.push_back( laser_status );
			
			return 1;

		}
		
		// One vs. two
		if( backenergy.size() == 1 && backenergy2.size() == 1 ) {
		
			// The id and sector sizes have to be the same
			if( backid.size() != 1 || backid2.size() != 1 ) return 0;
			if( backsector.size() != 1 || backsector2.size() != 1 ) return 0;
			
			if( TMath::Abs( backid[0] - backid2[0] ) == 1 ) {
			
				cd_debug->Fill(10);
				
				// Good CD sector numbers
				if( frontsector[0] < 0 || frontsector[0] > 3 ) return 0;
				if( backsector[0] < 0 || backsector[0] > 3 ) return 0;
				if( backsector2[0] < 0 || backsector2[0] > 3 ) return 0;
				cd_debug->Fill(11);
				
				// Same CD for both backstrips
				if( backsector[0] != backsector2[0] ) return 0;
				cd_debug->Fill(12);
				
				// Same CD for front and back
				if( frontsector[0] != backsector2[0] ) return 0;
				cd_debug->Fill(13);
				
				// Neighbouring strips
				if( TMath::Abs( backid[0] - backid2[0] ) != 1 ) return 0;
				cd_debug->Fill(14);
				
				// Particle #1
				dE = frontenergy[0];
				E = padenergy[frontsector[0]];
				
				PEn.push_back( dE + E );
				dE_En.push_back( dE );
				E_En.push_back( E );
				Nf.push_back( frontid[0] );
				if( backenergy[0] > backenergy2[0] ) Nb.push_back( backid[0] );
				else Nb.push_back( backid2[0] );
				Quad.push_back( adc_num/2 );
				Sector.push_back( frontsector[0] );
				time.push_back( adc_t );
				laser.push_back( laser_status );
				
				return 1;
				
			}
			
		}
		
	}
	
	// Two vs. X
	else if( frontenergy.size() == 2 ) {
		
		// The id and sector sizes have to be the same
		if( frontid.size() != 2 ) return 0;
		if( frontsector.size() != 2 ) return 0;
		
		// Two vs. two
		if( backenergy.size() == 1 && backenergy2.size() == 1 ) {
			
			cd_debug->Fill(20);
			
			// The id and sector sizes have to be the same
			if( backid.size() != 1 || backid2.size() != 1 ) return 0;
			if( backsector.size() != 1 || backsector2.size() != 1 ) return 0;
			
			int index = 0, index2; // front strip associated with 1st/2nd hit on back

			// Good CD sector numbers
			if( frontsector[0] < 0 || frontsector[0] > 3 ) return 0;
			if( frontsector[1] < 0 || frontsector[1] > 3 ) return 0;
			if( backsector[0] < 0 || backsector[0] > 3 ) return 0;
			if( backsector2[0] < 0 || backsector2[0] > 3 ) return 0;
			cd_debug->Fill(21);
			
			// Front strips in same CD
			if( frontsector[0] == frontsector[1] ) {
				
				cd_debug->Fill(22);
				
				// Then back strips must be in same sector
				if( backsector[0] == backsector2[0] ) {
					
					float ediff = TMath::Abs( frontenergy[0] - backenergy[0] );
					if( TMath::Abs( frontenergy[1] - backenergy[0] ) < ediff ) index = 1;
					index2 = TMath::Abs( index - 1 );
					cd_debug->Fill(23);
					
				}
				
				else return 0;
				
			}
			
			// Else, back strips must also be in different sectors
			else if( backsector[0] != backsector2[0] ) {
				
				if( frontsector[0] == backsector[0]  ) index = 0;
				else if( frontsector[1] == backsector[1]  ) index = 1;
				
				index2 = TMath::Abs( index - 1 );
				
				cd_debug->Fill(24);
				
			}
			
			// Else it's a nonsense event
			else return 0;
			cd_debug->Fill(25);

			
			// Particle #1
			dE = frontenergy[index];
			E = padenergy[frontsector[index]];
			
			PEn.push_back( dE + E );
			dE_En.push_back( dE );
			E_En.push_back( E );
			Nf.push_back( frontid[index] );
			Nb.push_back( backid[0] );
			Quad.push_back( adc_num/2 );
			Sector.push_back( frontsector[index] );
			time.push_back( adc_t );
			laser.push_back( laser_status );
			
			
			// Particle #2
			dE = frontenergy[index2];
			E = padenergy[frontsector[index2]];
			
			PEn.push_back( dE + E );
			dE_En.push_back( dE );
			E_En.push_back( E );
			Nf.push_back( frontid[index2] );
			Nb.push_back( backid2[0] );
			Quad.push_back( adc_num/2 );
			Sector.push_back( frontsector[index2] );
			time.push_back( adc_t );
			laser.push_back( laser_status );
			
			return 2;
			
		}
		
		// Two vs. one
		if( backenergy.size() == 1 && backenergy2.size() == 0 ) {
			
			cd_debug->Fill(30);
			
			// The id and sector sizes have to be the same
			if( backid.size() != 1 ) return 0;
			if( backsector.size() != 1 ) return 0;
			
			// Good CD sector numbers
			if( frontsector[0] < 0 || frontsector[0] > 3 ) return 0;
			if( frontsector[1] < 0 || frontsector[1] > 3 ) return 0;
			if( backsector[0] < 0 || backsector[0] > 3 ) return 0;
			cd_debug->Fill(31);
			
			// Same CD for both front strips
			if( frontsector[0] != frontsector[1] ) return 0;
			cd_debug->Fill(32);
			
			// Same CD for front and back
			if( frontsector[0] != backsector[0] ) return 0;
			cd_debug->Fill(33);
			
			// Neighbouring strips
			if( TMath::Abs( frontid[0] - frontid[1] ) != 1 ) return 0;
			cd_debug->Fill(34);
			
			// Particle #1
			dE = backenergy[0];
			E = padenergy[frontsector[0]];
			
			PEn.push_back( dE + E );
			dE_En.push_back( dE );
			E_En.push_back( E );
			if( frontenergy[0] > frontenergy[1] ) Nf.push_back( frontid[0] );
			else Nf.push_back( frontid[1] );
			Nb.push_back( backid[0] );
			Quad.push_back( adc_num/2 );
			Sector.push_back( frontsector[0] );
			time.push_back( adc_t );
			laser.push_back( laser_status );
			
			return 1;
			
		}
		
	}
	
	return 0;
	
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


#endif