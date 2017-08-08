#include <iostream>
#include <iomanip>
#include <string>

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2S.h"
#include "TMath.h"

#include "CommandLineInterface.hh"
#include "BuiltEvent.hh"
#include "Calibration.hh"
#include "mbevts.hh"

using namespace std;

// This function defines the Back strip position for the barrel
int StripPosBack(double frontene, double rearene){

	for (int i = 0; i < 16; i++){
	
		if( frontene/rearene <= 0.2 + 0.1125*(i+1) ) return i;

	}

	if( frontene/rearene >= 2 ) return 15;
	else return -1;
	
}

// This function transforms the energies into position for the barrel.
int *PosBBarrel(int quadr, vector<unsigned short> &adcval, double rear){
  
	int VLevel = 0;
	int mod = quadr;
	int stripmod = quadr;
	int firststrip = 0;

	if( (mod<0) || (rear<0) || (stripmod<0) || (firststrip<0) ) {
		
		cerr << "Error in Settings Quadrant " << quadr << " rear " << rear;
		cerr << " BStripMod " << stripmod << " BFirstStrip " << firststrip << endl;
	
	}
	
	int *posBBarrel = new int[3]; // 0 = Multiplicity; 1 = PostFront; 2 = PostBack

	if( VLevel > 0 ) cout << "rear " << rear << endl;
 	int higheststrip = -1;
	int secondstrip = -1;
	//int thirdstrip = -1;
	double highestenergy = 0.;
	double secondenergy = 0.;
	double thirdenergy = 0.;
	double MinimalStripRatio = 0.75;
	double MinimalRearpRatio = 0.2;
  
	if( VLevel > 0 ) cout << "Quadrant " << quadr << " Module " << mod << endl;

	for( int i = firststrip; i < firststrip + 16; i++ ){

		if( adcval[i] > highestenergy ){
	
			secondstrip = higheststrip;
			higheststrip = i;

			thirdenergy = secondenergy;
			secondenergy = highestenergy;
			highestenergy = adcval[i];
			
		}
		
		else if( adcval[i] > secondenergy ){

			secondstrip = i;
			thirdenergy = secondenergy;
			secondenergy = adcval[i];

		}
		
	}
	
	if( VLevel > 0 ) cout << "3energy = " << thirdenergy << ". 2energy = " << secondenergy << ". 1energy = " << highestenergy << endl;
  
	if( higheststrip>-1 && ( highestenergy/rear > MinimalRearpRatio ) ) {

		if( (secondenergy/highestenergy > MinimalStripRatio ) && ( secondenergy/rear > MinimalRearpRatio ) ){

			if( VLevel > 0 ) {

				cout << "2energy = " << setprecision(2) << secondenergy << " 1energy = ";
				cout << setprecision(2) << highestenergy << " ratio = ";
				cout << setprecision(2) << secondenergy/highestenergy << endl;
				cout << "limit = " << setprecision(2) << MinimalStripRatio << endl;

			}
	  
			if( (thirdenergy/highestenergy > MinimalStripRatio) && (thirdenergy/rear > MinimalRearpRatio) ){ // three or more hits 	

				if( VLevel > 0 ) cout << "Multiplicity 3, energy 3 " << thirdenergy << endl;
				
				posBBarrel[0] = 3;
					
			}
				
			else {

				if( VLevel > 0 ) cout << "Multiplicity 2, energy 2 " << secondenergy << " energy 1 " << highestenergy << endl;

				posBBarrel[0] = 2;

			}
	  
			secondenergy = StripPosBack(adcval[secondstrip], rear); 
			highestenergy = StripPosBack(adcval[higheststrip], rear);
			
			// reconstruction hit
			if( TMath::Abs(higheststrip-secondstrip) == 1 && thirdenergy/rear < MinimalRearpRatio ) {

				posBBarrel[0] = 1;

			}
			
		}

		else {

			highestenergy = StripPosBack(adcval[higheststrip], rear);
			posBBarrel[0] = 1;
	  
			if( VLevel > 0 ) cout << "Multiplicity 1, energy " << highestenergy << endl;

		}
	
		posBBarrel[1] = higheststrip;
		posBBarrel[2] = highestenergy;
			
	}
		
	else {
		
		posBBarrel[0] = 0;

		if( VLevel > 0 ) cout << "Bad case" << endl;
			
	}
		
	if( VLevel>0 ) {
	
		cout << "rear " << rear << " highestenergy " << highestenergy;
		cout << " quadr " << quadr << " higheststrip " << higheststrip << endl;
		
	}

  	return posBBarrel;
  		
}

