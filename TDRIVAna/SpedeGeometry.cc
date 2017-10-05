#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <TVector3.h>
#include <TAxis.h>
#include <TGraph.h>
#include <TMath.h>
#include <TROOT.h>

#include "SpedeGeometry.hh"

// Constructor and destructor
SpedeGeometry::SpedeGeometry(){

	//cout << "constructor" << endl;

}
SpedeGeometry::~SpedeGeometry(){

	//cout << "destructor" << endl;

}

// Set a user defined distance to Spede from target
void SpedeGeometry::SetSpedeR( double user_r ) {

	// Set the user value (mm)
	r = user_r;

	return;

}

// Set a user defined rotation of Spede
void SpedeGeometry::SetSpedeAlpha( double user_alpha ) {

	// Set the user value
	alpha = user_alpha;

	return;

}

// Get a segment theta
double SpedeGeometry::GetSpedeTheta( int seg ) {

	// Return theta from segment offset
	return seg_spede[seg].Theta() * TMath::RadToDeg();

}

// Get a segment theta
double SpedeGeometry::GetSpedePhi( int seg ) {

	// Return theta from segment offset
	return seg_spede[seg].Phi() * TMath::RadToDeg();

}

// Setup the cluster with coordinate values
void SpedeGeometry::SetupSpede( double user_r, double user_alpha ) {

	// Set the user value
	r = user_r;
	alpha = user_alpha;

	SetupSpede();

	return;

}

// Setup cluster main routine
void SpedeGeometry::SetupSpede() {

	double r_ring[NRING];
	double theta_pos, x_pos, y_pos;
	alpha += 22.5; // basic rotation to centre of the pixels

	r_ring[0] = 15.2; // radius of first ring
	r_ring[1] = 19.1; // radius of second ring
	r_ring[2] = 22.3; // radius of third ring

	for( int i = 0; i < NRING; i++ ) {

		for( int j = 0; j < NSECTOR; j++ ) {

			theta_pos = alpha + j * 360./(double)NSECTOR;	// Angle of the centre of the pixel
			x_pos = r_ring[i] * TMath::Cos( theta_pos ); // x coord relative to target
			y_pos = r_ring[i] * TMath::Sin( theta_pos ); // y coord relative to target

			seg_spede[i*NSECTOR+j].SetXYZ( x_pos, y_pos, -r );

		}

	}

 	return;
 	
}

