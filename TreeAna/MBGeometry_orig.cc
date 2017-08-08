#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <TVector3.h>
#include <TAxis.h>
#include <TGraph.h>
#include <TMath.h>
#include <TROOT.h>

#include "MBGeometry.hh"

// Constructor and destructor
MBGeometry::MBGeometry(){

	//cout << "constructor" << endl;

}

MBGeometry::~MBGeometry(){

	//cout << "destructor" << endl;

}

// Set a user defined theta
void MBGeometry::SetCluTheta( double user_theta ) {

	// Set the user value
	theta = user_theta;

	return;

}

// Set a user defined theta
void MBGeometry::SetCluPhi( double user_phi ) {

	// Set the user value
	phi = user_phi;

	return;

}

// Set a user defined theta
void MBGeometry::SetCluAlpha( double user_alpha ) {

	// Set the user value
	alpha = user_alpha;

	return;

}

// Set a user defined theta
void MBGeometry::SetCluR( double user_r ) {

	// Set the user value (mm)
	r = user_r;

	return;

}

// Get a segment theta
double MBGeometry::GetSegTheta( int core, int seg ) {

	// Calculate the segment number
	int segNo = core*6 + seg;

	// Returm theta from segment offset
	return seg_offset[segNo].Theta() * TMath::RadToDeg();

}

// Get a segment theta
double MBGeometry::GetSegPhi( int core, int seg ) {

	// Calculate the segment number
	int segNo = core*6 + seg;

	// Returm theta from segment offset
	return seg_offset[segNo].Phi() * TMath::RadToDeg();

}

// Get a core theta
double MBGeometry::GetCoreTheta( int core ) {

	// Returm theta from segment offset
	return det_offset[core].Theta() * TMath::RadToDeg();

}

// Get a core theta
double MBGeometry::GetCorePhi( int core ) {

	// Returm theta from segment offset
	return det_offset[core].Phi() * TMath::RadToDeg();

}

// Setup the cluster with coordinate values
void MBGeometry::SetupCluster( double user_theta, double user_phi, double user_alpha, double user_r ) {

	// Set the user value
	theta = user_theta;
	phi = user_phi;
	alpha = user_alpha;
	r = user_r;

	SetupCluster();

	return;

}

// Setup cluster main routine
void MBGeometry::SetupCluster() {

	Double_t R = DIST_CORE_CORNER * 0.6; // average distance from centre of capsule to center of segment

	// Offset from centre of cluster to centre of each detector
	det_offset[0].SetXYZ(DIST_CORE_CORNER, 0, r);
	det_offset[1].SetXYZ(-DIST_CORE_CORNER / 2, -DIST_CORE_CORNER * ROOTTHREEOVER2, r);
	det_offset[2].SetXYZ(-DIST_CORE_CORNER / 2, DIST_CORE_CORNER * ROOTTHREEOVER2,  r);

	// Offset from centre of a detector to centre of a segment
	seg_offset[0].SetXYZ(R / 2,  -R * ROOTTHREEOVER2,  0.0);
	seg_offset[1].SetXYZ(-R / 2, -R * ROOTTHREEOVER2,  0.0);
	seg_offset[2].SetXYZ(R,       0.0,                  0.0);
	seg_offset[3].SetXYZ(R / 2,   R * ROOTTHREEOVER2,   0.0);
	seg_offset[4].SetXYZ(-R,      0.0,                  0.0);
	seg_offset[5].SetXYZ(-R / 2,  R * ROOTTHREEOVER2,   0.0);
	   
	seg_offset[6].SetXYZ(-R,      0.0,                  0.0);
	seg_offset[7].SetXYZ(-R / 2,  R * ROOTTHREEOVER2,   0.0);
	seg_offset[8].SetXYZ(-R / 2,  -R * ROOTTHREEOVER2,  0.0);
	seg_offset[9].SetXYZ(R / 2,   -R * ROOTTHREEOVER2,  0.0);
	seg_offset[10].SetXYZ(R / 2,  R * ROOTTHREEOVER2,   0.0);
	seg_offset[11].SetXYZ(R,      0.0,                  0.0);
   
   	seg_offset[12].SetXYZ(R / 2,   R * ROOTTHREEOVER2,  0.0);
   	seg_offset[13].SetXYZ(R,       0.0,                 0.0);
   	seg_offset[14].SetXYZ(-R / 2,  R * ROOTTHREEOVER2,  0.0);
   	seg_offset[15].SetXYZ(-R,      0.0,                 0.0);
   	seg_offset[16].SetXYZ(R / 2,   -R * ROOTTHREEOVER2, 0.0);
   	seg_offset[17].SetXYZ(-R / 2,  -R * ROOTTHREEOVER2, 0.0);
   
   	// Add the segment offsets to the detector offsets, so now
   	// seg_offset has the offset from the centre of the cluster
	for ( int i = 0; i < 3; i++ )
		for (int j = 0; j < 6; j++ )
			seg_offset[i * 6 + j] += det_offset[i];
   	

	//cout << "1 phi " << phi << " theta " << theta << "\n";
   	// Sort out phi
   	if (phi > 180) {
		phi = 360 - phi;//'mirror' phi, e.g. 270->90
		theta = -theta;//'mirror' theta e.g. 90->-90
		//This appears to put the detector at the same place
		//90,270 is perpendicular to beam on left, 90,-90 is also
   	}
	//cout << "2 phi " << phi << " theta " << theta << "\n";
	
   	// Transform
   	TVector3 trans;
   	
   	trans.SetXYZ(1,0,0);//Generate a unit vector along 'x' axis
   	trans.RotateY(-phi * TMath::DegToRad());
   	trans.RotateX(-theta * TMath::DegToRad());
	theta = trans.Theta() * TMath::RadToDeg();
	phi   = trans.Phi() * TMath::RadToDeg();
  	 
	//cout << "3 phi " << phi << " theta " << theta << "\n\n";
	// Sort out alpha offset
	TVector3 va, vd, vn, ex(1,0,0);
  	 
   	vd.SetXYZ(0,0,1);
   	va.SetXYZ(1,0,0);
   	vd.RotateY(theta * TMath::DegToRad());
   	vd.RotateZ(phi * TMath::DegToRad());
   	va.RotateY(theta * TMath::DegToRad());
   	va.RotateZ(phi * TMath::DegToRad());
   
   	vn = ex - vd * (ex * vd);
   	vn.SetMag(1.);
   	
   	Double_t alpha_offset;
   	alpha_offset = va.Angle(vn);
   	if (vn * (va.Cross(vd)) < 0) alpha_offset = -alpha_offset;
   
   	// Rotate cores
   	for ( int i = 0; i < 3; i++ ) {
		det_offset[i].RotateZ(-alpha * TMath::DegToRad() - alpha_offset);
		det_offset[i].RotateY(theta * TMath::DegToRad());
		det_offset[i].RotateZ(phi * TMath::DegToRad());
   	}
   
   	// Rotate segments
   	for ( int i = 0; i < 18; i++ ) {
		seg_offset[i].RotateZ(-alpha * TMath::DegToRad() - alpha_offset);
		seg_offset[i].RotateY(theta * TMath::DegToRad());
		seg_offset[i].RotateZ(phi * TMath::DegToRad());
   	}
   
 	return;
 	
}

