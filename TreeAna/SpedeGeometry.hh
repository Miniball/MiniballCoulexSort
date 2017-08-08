#ifndef __SpedeGeometry_HH__
#define __SpedeGeometry_HH__

#include <TObject.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TText.h>
#include <cstdio>
#include <iostream>

using namespace std;

#define NRING 3
#define NSECTOR 8
#define NSPEDE NRING*NSECTOR

// ---------------------------------------------------------------------------
// Miniball Geometry class
// ---------------------------------------------------------------------------
//class SpedeGeometry : public TObject {
class SpedeGeometry {

	public:
   
		// Constructor and destructor
		SpedeGeometry();
		~SpedeGeometry();

		// Set values of theta and phi the cluster
		void SetupSpede();
		void SetupSpede( double user_r,double user_alpha );
		void SetSpedeR( double user_r );
		void SetSpedeAlpha( double user_alpha );

		// Get values of theta and phi for a particular segment
		double GetSpedeTheta( int seg );
		double GetSpedePhi( int seg );

	private:
	
		// Current values of theta, phi, alpha and r
		double r; 		// mm
		double alpha;	// deg
		
		// Segment offset vectors
		TVector3 seg_spede[NSPEDE];

	//ClassDef(SpedeGeometry, 1);
	
};

#endif
