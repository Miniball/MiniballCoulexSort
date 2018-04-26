#ifndef __MBGEOMETRY_HH__
#define __MBGEOMETRY_HH__

#include <TObject.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TText.h>
#include <TMath.h>
#include <cstdio>
#include <iostream>


using namespace std;

#define ROOTTHREEOVER2 0.8660254

#define DIST_CORE_CORNER 34

#define NCLU 8
#define NSEG 18

/// Functions to convert Miniball angles read from the frame

/// --------------------------------------------------------------------
/// Miniball Geometry class
/// --------------------------------------------------------------------
/// A geometry class that converts cluster angles read from the Miniball
/// frame into real-life angles for the analysis.
/// You can have theta and phi angles of the centres of each cluster,
/// crystal and segment in frame geometry or true/beam geometry.

class MBGeometry {

	public:
   
		// Constructor and destructor
		MBGeometry();
		~MBGeometry();

		// Set values of theta and phi the cluster
		void SetupCluster();
		void SetupCluster( double user_theta, double user_phi, double user_alpha, double user_r, double user_z );
		void SetCluTheta( double user_theta );
		void SetCluPhi( double user_phi );
		void SetCluAlpha( double user_alpha );
		void SetCluR( double user_r );
		void SetCluZ( double user_z );

		// Get values of theta and phi the core
		double GetCoreTheta( int core );
		double GetCorePhi( int core );

		// Get values of theta and phi for a particular segment
		double GetSegTheta( int core, int seg );
		double GetSegPhi( int core, int seg );

		/// Get theta of a vector in Miniball system (return deg)
		inline double MBTheta(TVector3 &v) {
			double mytheta = v.Phi() * TMath::RadToDeg();
			double myphi = v.Theta() * TMath::RadToDeg();
			if( mytheta < 0 ) {
		 		myphi = 360 - myphi;
				mytheta = - mytheta;
			}
			return mytheta;
	   	};

		/// Get phi of a vector in Miniball system (return deg)
		inline double MBPhi(TVector3 &v) {
			double mytheta = v.Phi() * TMath::RadToDeg();
			double myphi = v.Theta() * TMath::RadToDeg();
			if( mytheta < 0 ) {
				myphi = 360 - myphi;
				mytheta = -mytheta;
			}
			return myphi;
		};

		/// Get true theta of a vector (return deg)
		inline double TrueTheta(TVector3 &v) {
			return std::acos(v.x() / v.Mag()) * TMath::RadToDeg();
		};

		/// Get the true phi of a vector (return deg)
		inline double TruePhi(TVector3 &v) {
			TVector3 v2(-v.y(), v.z(), v.x());
 			double phi = v2.Phi() * TMath::RadToDeg();
			if( phi < 0 ) phi += 360;
			phi -= 90;
			if( phi < 0 ) phi += 360;
			return phi;
		};
   
	private:
	
		// Current values of theta, phi, alpha and r
		double theta;	///< theta angle in deg
		double phi;		///< phi angle in deg
		double alpha;	///< alpha angle in deg
		double r; 		///< distance from target to detector in mm
		double z; 		///< distance from target to origin (beam direction is positive) in mm
	
		// Cluster offset vectors
		TVector3 clu_offset;

		// Segment offset vectors
		TVector3 seg_offset[18];

		// Core offset vectors
		TVector3 det_offset[3];
	
		// Offset of the cluster in beam direction
		TVector3 mbzoffset;	///< Offset of target from origin in direction of beam in mm.
							///< This shift is independent of the CD detector distance which is relative.

	//ClassDef(MBGeometry, 1);
	
};

#endif
