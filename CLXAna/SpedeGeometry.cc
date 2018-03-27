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

void SpedeGeometry::SetSpedeR( double user_r ) {

	/// Set the measured distance to Spede from target

	// Set the user value (mm)
	r = user_r;

	return;

}

void SpedeGeometry::SetSpedeAlpha( double user_alpha ) {

	/// Set the measured rotation of Spede with respect to the vertical
	
	// Set the user value
	alpha = user_alpha;

	return;

}

double SpedeGeometry::GetSpedeTheta( int seg ) {

	/// Get a segment theta angle in degrees

	// Return theta from segment offset
	return seg_spede[seg].Theta() * TMath::RadToDeg();

}

double SpedeGeometry::GetSpedePhi( int seg ) {

	/// Get a segment phi angle in degrees

	// Return theta from segment offset
	return seg_spede[seg].Phi() * TMath::RadToDeg();

}

void SpedeGeometry::SetupSpede( double user_r, double user_alpha ) {

	/// Setup SPEDE with coordinate values

	// Set the user value
	r = user_r;
	alpha = user_alpha;

	SetupSpede();

	return;

}

void SpedeGeometry::SetupSpede() {

	/// Main routine for setting up SPEDE

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

