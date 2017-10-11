#ifndef __MBGEOMETRY_HH__
#define __MBGEOMETRY_HH__

#include <TObject.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TText.h>
#include <cstdio>
#include <iostream>

using namespace std;

#define ROOTTHREEOVER2 0.8660254

#define DIST_CORE_CORNER 34

#define NCLU 8
#define NSEG 18

// ---------------------------------------------------------------------------
// Miniball Geometry class
// ---------------------------------------------------------------------------
//class MBGeometry : public TObject {
class MBGeometry {

	public:
   
		// Constructor and destructor
		MBGeometry();
		~MBGeometry();

		// Set values of theta and phi the cluster
		void SetupCluster();
		void SetupCluster( double user_theta, double user_phi, double user_alpha, double user_r );
		void SetCluTheta( double user_theta );
		void SetCluPhi( double user_phi );
		void SetCluAlpha( double user_alpha );
		void SetCluR( double user_r );

		// Get values of theta and phi the core
		double GetCoreTheta( int core );
		double GetCorePhi( int core );

		// Get values of theta and phi for a particular segment
		double GetSegTheta( int core, int seg );
		double GetSegPhi( int core, int seg );

	private:
	
		// Current values of theta, phi, alpha and r
		double theta;	// deg
		double phi;		// deg
		double alpha;	// deg
		double r; 		// mm
		
		// Segment offset vectors
		TVector3 seg_offset[18];

		// Core offset vectors
		TVector3 det_offset[3];

	//ClassDef(MBGeometry, 1);
	
};

#endif
