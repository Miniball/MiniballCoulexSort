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
   
	/// Constructor
	MBGeometry();
	
	/// Destructor
	~MBGeometry();

	/// Setup cluster main routine
	void SetupCluster();
	/// Setup the cluster with coordinate values
	/// \param user_theta in the MB frame of reference [degrees]
	/// \param user_phi in the MB frame of reference [degrees]
	/// \param user_alpha in the MB frame of reference [degrees]
	/// \param user_r distance from target to detector [mm]
	/// \param user_z distance from target to origin in beam direction [mm]
	void SetupCluster( double user_theta, double user_phi, double user_alpha, double user_r, double user_z );

	/// Set the theta angle measured from the frame
	/// \param user_theta in the MB frame of reference [degrees]
	void SetCluTheta( double user_theta );
	
	/// Set the phi angle measured from the frame
	/// \param user_phi in the MB frame of reference [degrees]
	void SetCluPhi( double user_phi );
	
	/// Set the alpha angle measured from the frame
	/// \param user_alpha in the MB frame of reference [degrees]
	void SetCluAlpha( double user_alpha );
	
	/// Set the distance between the target and face of the cluster
	/// \param user_r distance from target to detector [mm]
	void SetCluR( double user_r );
	
	/// Set the distance between the origin and the target position along the beam axis
	/// \param user_z distance from target to origin in beam direction [mm]
	void SetCluZ( double user_z );

	/// Get the theta angle of the core with respect to the beam
	/// \param core number of the MB Ge crystal counting from 0 to 23
	/// \return theta of core in beam reference (degrees)
	double GetCoreTheta( int core );

	/// Get the phi angle of the core with respect to the beam
	/// \param core number of the MB Ge crystal counting from 0 to 23
	/// \return phi of core in beam reference (degrees)
	double GetCorePhi( int core );

	/// Get the theta angle of a segment with respect to the beam
	/// \param core number of the MB Ge crystal counting from 0 to 23
	/// \param seg number of the segment within the crystal: 0 is core, 1-6 for segments
	/// \return theta of segment in beam reference (degrees)
	double GetSegTheta( int core, int seg );

	/// Get the phi angle of a segment with respect to the beam
	/// \param seg number of the segment within the crystal: 0 is core, 1-6 for segments
	/// \return phi of segment in beam reference (degrees)
	double GetSegPhi( int core, int seg );

	/// Get theta of a vector in Miniball system (return deg)
	/// \param v vector from target to detector
	/// \return theta in the Miniball system (degrees)
	inline double MBTheta( TVector3 &v ) {
		double mytheta = v.Phi() * TMath::RadToDeg();
		double myphi = v.Theta() * TMath::RadToDeg();
		if( mytheta < 0 ) {
			myphi = 360 - myphi;
			mytheta = - mytheta;
		}
		return mytheta;
	};

	/// Get phi of a vector in Miniball system (return deg)
	/// \param v vector from target to detector
	/// \return phi in the Miniball system (degrees)
	inline double MBPhi( TVector3 &v ) {
		double mytheta = v.Phi() * TMath::RadToDeg();
		double myphi = v.Theta() * TMath::RadToDeg();
		if( mytheta < 0 ) {
			myphi = 360 - myphi;
			mytheta = -mytheta;
		}
		return myphi;
	};
	
	/// Get true theta of a vector (return deg)
	/// \param v vector from target to detector
	/// \return theta in the beam system (degrees)
	inline double TrueTheta(TVector3 &v) {
		return std::acos(v.x() / v.Mag()) * TMath::RadToDeg();
	};
	
	/// Get the true phi of a vector (return deg)
	/// \param v vector from target to detector
	/// \return phi in the beam system (degrees)
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
	
	TVector3 clu_offset;		///< vector for cluster centre
	TVector3 seg_offset[18];	///< vector for segment centre
	TVector3 det_offset[3];		///< vector for core centre
	TVector3 mbzoffset;			///< Offset of target from origin in direction of beam in mm.
								///< This shift is independent of the CD detector distance which is relative.

	//ClassDef(MBGeometry, 1);
	
};

#endif
