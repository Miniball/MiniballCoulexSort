#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <TVector3.h>
#include <TAxis.h>
#include <TGraph.h>
#include <TMath.h>
#include <TROOT.h>

#include "MBGeometry.hh"


// Constructor
MBGeometry::MBGeometry(){

	//cout << "constructor" << endl;

}

// Destructor
MBGeometry::~MBGeometry(){

	//cout << "destructor" << endl;

}

void MBGeometry::SetCluTheta( double user_theta ) {

	// Set the user value
	theta = user_theta;

	return;

}

void MBGeometry::SetCluPhi( double user_phi ) {
	
	// Set the user value
	phi = user_phi;

	return;

}

void MBGeometry::SetCluAlpha( double user_alpha ) {

	// Set the user value
	alpha = user_alpha;

	return;

}

void MBGeometry::SetCluR( double user_r ) {

	// Set the user value (mm)
	r = user_r;
	
	return;
	
}

void MBGeometry::SetCluZ( double user_z ) {

	// Set the user value (mm)
	z = user_z;
	
	return;
	
}

double MBGeometry::GetSegTheta( int core, int seg ) {
	
	// Calculate the segment number
	int segNo = core*6 + seg;

	// Return theta from segment offset (old)
	//return seg_offset[segNo].Theta() * TMath::RadToDeg();

	// New method of returning theta in true coordinate system
	return TrueTheta( seg_offset[segNo] );

}

double MBGeometry::GetSegPhi( int core, int seg ) {
	
	// Calculate the segment number
	int segNo = core*6 + seg;

	// Return phi from segment offset (old)
	//return seg_offset[segNo].Phi() * TMath::RadToDeg();

	// New method of returning phi in true coordinate system
	return TruePhi( seg_offset[segNo] );

}

double MBGeometry::GetCoreTheta( int core ) {
	
	// Return theta from segment offset
	return TrueTheta( det_offset[core] );

}

double MBGeometry::GetCorePhi( int core ) {
	
	// Return phi from segment offset
	return TruePhi( det_offset[core] );

}

void MBGeometry::SetupCluster( double user_theta, double user_phi, double user_alpha, double user_r, double user_z ) {

	// Set the user value
	theta = user_theta * TMath::DegToRad();
	phi = user_phi * TMath::DegToRad();
	alpha = user_alpha * TMath::DegToRad();
	r = user_r;
	z = user_z;

	if( phi > TMath::Pi() ) theta = TMath::Pi() - theta;

	SetupCluster();

	return;

}


void MBGeometry::SetupCluster() {

	// Offset of cluster
	clu_offset.SetXYZ(0, r, 0);

	// Offset from centre of cluster to centre of each detector
#define USE_EMPIRICAL 0
#if USE_EMPIRICAL // These are values determined from a Geant4 simulation
	det_offset[0].SetXYZ(-0.0166, r + 8.6739, -33.7254);
	det_offset[1].SetXYZ(-29.1946, r + 8.6628, 16.8867);
	det_offset[2].SetXYZ(29.2040, r + 8.6781, 16.8364);
	
	seg_offset[0].SetXYZ(-13.8080,  -0.9293, -8.9018);
	seg_offset[1].SetXYZ(-15.1076,  0.7917, 8.1148);
	seg_offset[2].SetXYZ(-0.0098,  -1.5906, -16.6596);
	seg_offset[3].SetXYZ(13.8680,  -0.9272, -8.9303);
	seg_offset[4].SetXYZ(-0.0149,  1.8715, 18.2628);
	seg_offset[5].SetXYZ(15.0723,  0.7839, 8.1142);
	
	seg_offset[6].SetXYZ(-0.7397,  -0.8946, 16.4265);
	seg_offset[7].SetXYZ(14.5265,  0.7779, 9.0148);
	seg_offset[8].SetXYZ(-14.3342,  -1.5688, 8.2345);
	seg_offset[9].SetXYZ(-14.6338,  -0.9115, -7.5162);
	seg_offset[10].SetXYZ(15.7437,  1.8780, -9.0642);
	seg_offset[11].SetXYZ(-0.5625,  0.7189, -17.0954);
	
	seg_offset[12].SetXYZ(14.6795,  -0.9260, -7.5641);
	seg_offset[13].SetXYZ(0.5776,  0.7675, -17.0816);
	seg_offset[14].SetXYZ(14.3709,  -1.5876, 8.2896);
	seg_offset[15].SetXYZ(0.7611,  -0.9218, 16.4052);
	seg_offset[16].SetXYZ(-15.8436,  1.8638, -9.0467);
	seg_offset[17].SetXYZ(-14.5454,  0.8040, 8.9976);
#else
	Double_t R = DIST_CORE_CORNER * 0.5; // average distance from centre of capsule to centre of segment
	
	det_offset[0].SetXYZ( 0,                                 r+9, -DIST_CORE_CORNER);
	det_offset[1].SetXYZ(-DIST_CORE_CORNER * ROOTTHREEOVER2, r+9,  DIST_CORE_CORNER / 2);
	det_offset[2].SetXYZ( DIST_CORE_CORNER * ROOTTHREEOVER2, r+9,  DIST_CORE_CORNER / 2);
	
	// Offset from centre of a detector to centre of a segment
	seg_offset[0 ].SetXYZ(-R * ROOTTHREEOVER2, 0.0, -R / 2 );
	seg_offset[1 ].SetXYZ(-R * ROOTTHREEOVER2, 0.0,  R / 2 );
	seg_offset[2 ].SetXYZ( 0.0,                0.0, -R     );
	seg_offset[3 ].SetXYZ( R * ROOTTHREEOVER2, 0.0, -R / 2 );
	seg_offset[4 ].SetXYZ( 0.0,                0.0,  R     );
	seg_offset[5 ].SetXYZ( R * ROOTTHREEOVER2, 0.0,  R / 2 );
	
	seg_offset[6 ].SetXYZ( 0.0,                0.0,  R     );
	seg_offset[7 ].SetXYZ( R * ROOTTHREEOVER2, 0.0,  R / 2 );
	seg_offset[8 ].SetXYZ(-R * ROOTTHREEOVER2, 0.0,  R / 2 );
	seg_offset[9 ].SetXYZ(-R * ROOTTHREEOVER2, 0.0, -R / 2 );
	seg_offset[10].SetXYZ( R * ROOTTHREEOVER2, 0.0, -R / 2 );
	seg_offset[11].SetXYZ( 0.0,                0.0, -R     );
	
	seg_offset[12].SetXYZ( R * ROOTTHREEOVER2, 0.0, -R / 2 );
	seg_offset[13].SetXYZ( 0.0,                0.0, -R     );
	seg_offset[14].SetXYZ( R * ROOTTHREEOVER2, 0.0,  R / 2 );
	seg_offset[15].SetXYZ( 0.0,                0.0,  R     );
	seg_offset[16].SetXYZ(-R * ROOTTHREEOVER2, 0.0, -R / 2 );
	seg_offset[17].SetXYZ(-R * ROOTTHREEOVER2, 0.0,  R / 2 );
#endif
	
   	// Add the segment offsets to the detector offsets, so now
   	// seg_offset has the offset from the centre of the cluster
	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 6; j++ )
			seg_offset[i * 6 + j] += det_offset[i];

	// Offsets
	double myalpha, mytheta, myphi;
	if( phi < TMath::Pi() ) {
		myalpha = alpha;
		myphi = TMath::Pi() / 2. + phi;
		mytheta = TMath::Pi() / 2. + theta;
	}
	else {
		myalpha = alpha - TMath::Pi();
		myphi = TMath::Pi() / 2. + phi;
		mytheta = TMath::Pi() / 2. + theta;
	}

	// Rotate cluster to appropriate angle
	clu_offset.RotateY(myalpha);
	clu_offset.RotateX(myphi);
	clu_offset.RotateZ(mytheta);
		
	// Rotate cores to appropriate angle
	for( UInt_t i = 0; i < 3; i++ ) {
		det_offset[i].RotateY(myalpha);
		det_offset[i].RotateX(myphi);
		det_offset[i].RotateZ(mytheta);
	}

	// Rotate segments to appropriate angle
	for( UInt_t i = 0; i < 18; i++ ) {
		seg_offset[i].RotateY(myalpha);
		seg_offset[i].RotateX(myphi);
		seg_offset[i].RotateZ(mytheta);
	}
	
	// Shift Miniball so that target it as origin
	mbzoffset.SetXYZ( -1.0*z, 0.0, 0.0 );
	clu_offset += mbzoffset;
	for( UInt_t i = 0; i < 3; i++ )  det_offset[i] += mbzoffset;
	for( UInt_t i = 0; i < 18; i++ ) seg_offset[i] += mbzoffset;
 
 	return;
 	
}

