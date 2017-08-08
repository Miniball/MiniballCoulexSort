// Simultaneous fit of all segment angles in a MB cluster

#include "Fit/Fitter.h"
#include "Fit/BinData.h"
#include "Fit/UnBinData.h"
#include "Fit/LogLikelihoodFCN.h"
#include "TMinuit.h"
#include "Math/Minimizer.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TList.h"
#include "TMath.h"
#include "Math/WrappedMultiTF1.h"
#include "Math/SpecFunc.h"
#include "Math/SpecFuncMathMore.h"
#include "HFitInterface.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TVector3.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TROOT.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

#define ROOTTHREEOVER2 0.8660254
#define DIST_CORE_CORNER 34

#define NCLU 2
#define NSEG 18
#define IPARS 5 	// per data set: r, theta, phi, alpha, beta
#define NPARS 8 	// global fit: 2*r + theta + 2*phi + 2*alpha + beta

// data holders
double energies[8][NSEG];
double en_error[8][NSEG];
int clu_num[8];

// definition of shared parameters
int ipar[NCLU*NSEG][IPARS];

double par0[NPARS];

// initial parameters and names
string parname[NPARS] = { "r_0", "phi_0", "alpha_0", "r_1", "phi_1", "alpha_1", "theta", "beta" };

// Ordering of cluster numbers for setup
void setup_clusters() {
	
	// Set the cluster numbers
	clu_num[0] = 17;	// = 13
	clu_num[1] = 12;	// = 16
	clu_num[2] = 16;	// = 12
	clu_num[3] = 13;	// = 17
	clu_num[4] = 22;	// = 14
	clu_num[5] = 18;	// = 23
	clu_num[6] = 14;	// = 22
	clu_num[7] = 23;	// = 18
	
	return;
	
}

// Find the index of the cluster
int lookup_clu( int myclu ) {
	
	for( int i = 0; i < 8; i++ ) {
		
		if( myclu == clu_num[i] ) return(i);
		
	}
	
	return(-1);

}

// String convert function
string convertInt( int number ) {
	
	stringstream ss;
	ss << number;
	return ss.str();
	
}

///////////////////////////
// Fit stuff starts here //
///////////////////////////

// Make initial starting values from the frame readings
void initialise_parameters( int *myclu ) {
	
	double r[8] = { 153.1, 153.4, 129.3, 127.7, 130.6, 138.8, 131.8, 128.1 };
	double theta[8] = { 142.5, 54.7, 54.7, 142.5, 144.8, 56.5, 144.8, 56.5 };
	double phi[8] = { 127.0, 58.5, 133.5, 57.0, 220.0, 222.5, 295.5, 298.5 };
	double alpha[8] = { 312.0, 180.0, 120.0, 120.0, 60.0, 5.0, 120.0, 5.0 };
	double beta = 0.0656;
	
	par0[0] = r[lookup_clu(myclu[0])];
	par0[1] = phi[lookup_clu(myclu[0])];
	par0[2] = alpha[lookup_clu(myclu[0])];

	par0[3] = r[lookup_clu(myclu[1])];
	par0[4] = phi[lookup_clu(myclu[1])];
	par0[5] = alpha[lookup_clu(myclu[1])];
	
	par0[6] = theta[lookup_clu(myclu[0])];
	par0[7] = beta;
	
	for( int i = 0; i < NCLU; i++ )
		cout << "Cluster " << myclu[i] << " has index " << lookup_clu(myclu[i]) << endl;
	
	return;
	
}

// Make lists of parameters for each segment
void assign_parameters() {

	for( int i = 0; i < NCLU; i++ ) {
		
		for( int j = 0; j < NSEG; j++ ) {
			
			ipar[i*NSEG+j][0] = i*3 + 0;  // r
			ipar[i*NSEG+j][1] = i*3 + 1;  // phi
			ipar[i*NSEG+j][2] = i*3 + 2;  // alpha
			ipar[i*NSEG+j][3] = NCLU*3 + (int)(i/2);  // theta (pair wise)
			ipar[i*NSEG+j][4] = NPARS - 1;  // beta (global)
			
		}
	
	}
	
	return;

}

// Get data from file
void read_energies() {
	
	FILE *fp_dn;
	Double_t energy_dn, error_dn;
	Int_t myclu_dn, seg_dn, status_dn, pos_dn;
	char line_dn[1024], det_dn;
	
	// Open file
	fp_dn = fopen( "analyse_22Ne_E440.dat", "r" );
	if( !fp_dn ) {
		fprintf( stderr, "Unable to read analyse_22Ne_E440.dat\n");
		exit(-1);
	}
	
	// Initialise
	for( int i = 0; i < NCLU; i++ ) {

		for( int j = 0; j < NSEG; j++ ) {
		
			energies[i][j] = -999.;
			en_error[i][j] =  999.;
			
		}
		
	}
	
	// Read file
	while(1) {
		
		// Read line
		if( fgets( line_dn, sizeof(line_dn) - 1, fp_dn ) == NULL) break;
		
		// Get values
		status_dn = sscanf( line_dn, "%d%c%d %lf %lf", &myclu_dn, &det_dn, &seg_dn, &energy_dn, &error_dn );
		if( status_dn != 5 ) continue;
		
		// Store it
		pos_dn = (det_dn - 'A') * 6 + (seg_dn - 1);
		energies[lookup_clu(myclu_dn)][pos_dn] = energy_dn;
		en_error[lookup_clu(myclu_dn)][pos_dn] = error_dn;
		
	}
	
	// Close file
	fclose(fp_dn);

	return;

}

double seg_fit_function( double *x, double *par ) {
	
	// Get parameters from list
	double r = par[0];
	double phi = par[1];
	double alpha = par[2];
	double theta = par[3];
	double beta = par[4];
	int segNo = (int)x[0];
	
	// Angles and geometry
	TVector3 seg_offset[18], det_offset[3];
	Double_t R = DIST_CORE_CORNER * 0.6; // average distance from centre of capsule to center of segment
	
	// Offset from centre of cluster to centre of each detector
	det_offset[0].SetXYZ(DIST_CORE_CORNER,      0,                                 r);
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
	for( int i = 0; i < NSEG; i++ )
		seg_offset[i] += det_offset[i/6];
	
	// Sort out phi
	if( phi > 180 ) {
		phi = 360 - phi;
		theta = -theta;
	}
	
	// Transform
	TVector3 trans;
	
	trans.SetXYZ(1,0,0);
	trans.RotateY( -phi * TMath::DegToRad() );
	trans.RotateX( -theta * TMath::DegToRad() );
	theta = trans.Theta() * TMath::RadToDeg();
	phi   = trans.Phi() * TMath::RadToDeg();
	
	// Sort out alpha offset
	TVector3 va, vd, vn, ex(1,0,0);
	
	vd.SetXYZ(0,0,1);
	va.SetXYZ(1,0,0);
	vd.RotateY( theta * TMath::DegToRad() );
	vd.RotateZ( phi * TMath::DegToRad() );
	va.RotateY( theta * TMath::DegToRad() );
	va.RotateZ( phi * TMath::DegToRad() );
	
	vn = ex - vd * (ex * vd);
	vn.SetMag(1.);
	
	Double_t alpha_offset;
	alpha_offset = va.Angle(vn);
	if( vn * (va.Cross(vd)) < 0 ) alpha_offset = -alpha_offset;
	
	// Rotate segments
	for( int i = 0; i < NSEG; i++ ) {
		
		seg_offset[i].RotateZ( -alpha * TMath::DegToRad() - alpha_offset );
		seg_offset[i].RotateY( theta * TMath::DegToRad() );
		seg_offset[i].RotateZ( phi * TMath::DegToRad() );
		
	}
	
	// Calculate the energies
	double dc = ( 1 - beta * TMath::Cos( seg_offset[segNo].Theta() ) ) / TMath::Sqrt( 1.0 - beta*beta );
	
	return TMath::Abs( 439.99 / dc );

}


struct Global {
	
	Global( ROOT::Math::IMultiGenFunction & f0,  ROOT::Math::IMultiGenFunction & f1,
				ROOT::Math::IMultiGenFunction & f2,  ROOT::Math::IMultiGenFunction & f3,
			    ROOT::Math::IMultiGenFunction & f4,  ROOT::Math::IMultiGenFunction & f5,
			    ROOT::Math::IMultiGenFunction & f6,  ROOT::Math::IMultiGenFunction & f7,
			    ROOT::Math::IMultiGenFunction & f8,  ROOT::Math::IMultiGenFunction & f9,
			    ROOT::Math::IMultiGenFunction & f10, ROOT::Math::IMultiGenFunction & f11,
			    ROOT::Math::IMultiGenFunction & f12, ROOT::Math::IMultiGenFunction & f13,
			    ROOT::Math::IMultiGenFunction & f14, ROOT::Math::IMultiGenFunction & f15,
			    ROOT::Math::IMultiGenFunction & f16, ROOT::Math::IMultiGenFunction & f17,
			    ROOT::Math::IMultiGenFunction & f18, ROOT::Math::IMultiGenFunction & f19,
			    ROOT::Math::IMultiGenFunction & f20, ROOT::Math::IMultiGenFunction & f21,
			    ROOT::Math::IMultiGenFunction & f22, ROOT::Math::IMultiGenFunction & f23,
			    ROOT::Math::IMultiGenFunction & f24, ROOT::Math::IMultiGenFunction & f25,
			    ROOT::Math::IMultiGenFunction & f26, ROOT::Math::IMultiGenFunction & f27,
			    ROOT::Math::IMultiGenFunction & f28, ROOT::Math::IMultiGenFunction & f29,
			    ROOT::Math::IMultiGenFunction & f30, ROOT::Math::IMultiGenFunction & f31,
			    ROOT::Math::IMultiGenFunction & f32, ROOT::Math::IMultiGenFunction & f33,
			    ROOT::Math::IMultiGenFunction & f34, ROOT::Math::IMultiGenFunction & f35 ) :
	fll_0(&f0),   fll_1(&f1),   fll_2(&f2),   fll_3(&f3),   fll_4(&f4),   fll_5(&f5),
	fll_6(&f6),   fll_7(&f7),   fll_8(&f8),   fll_9(&f9),   fll_10(&f10), fll_11(&f11),
	fll_12(&f12), fll_13(&f13), fll_14(&f14), fll_15(&f15), fll_16(&f16), fll_17(&f17),
	fll_18(&f18), fll_19(&f19), fll_20(&f20), fll_21(&f21), fll_22(&f22), fll_23(&f23),
	fll_24(&f24), fll_25(&f25), fll_26(&f26), fll_27(&f27), fll_28(&f28), fll_29(&f29),
	fll_30(&f30), fll_31(&f31), fll_32(&f32), fll_33(&f33), fll_34(&f34), fll_35(&f35) {}
	
	
	double operator() (const double *par) const {
		
		double p[NCLU*NSEG][IPARS];
		
		for ( int i = 0; i < NCLU*NSEG; i++ ) {
			
			for ( int j = 0; j < IPARS; j++ ) {
				
				p[i][j]  = par[ ipar[i][j] ];
				
			}
			
		}
		
		return (*fll_0)(p[0]) + (*fll_1)(p[1]) + (*fll_2)(p[2]) + (*fll_3)(p[4]) + (*fll_4)(p[4])
			+ (*fll_5)(p[5]) + (*fll_6)(p[6]) + (*fll_7)(p[7]) + (*fll_8)(p[8]) + (*fll_9)(p[9])
			+ (*fll_10)(p[10]) + (*fll_11)(p[11]) + (*fll_12)(p[12]) + (*fll_13)(p[13])
			+ (*fll_14)(p[14]) + (*fll_15)(p[15]) + (*fll_16)(p[16]) + (*fll_17)(p[17])
			+ (*fll_18)(p[18]) + (*fll_19)(p[19]) + (*fll_20)(p[20]) + (*fll_21)(p[21])
			+ (*fll_22)(p[22]) + (*fll_23)(p[23]) + (*fll_24)(p[24]) + (*fll_25)(p[25])
			+ (*fll_26)(p[26]) + (*fll_27)(p[27]) + (*fll_28)(p[28]) + (*fll_29)(p[29])
			+ (*fll_30)(p[30]) + (*fll_31)(p[31]) + (*fll_32)(p[32]) + (*fll_33)(p[33])
			+ (*fll_34)(p[34]) + (*fll_35)(p[35]);
		
	}
	
	const ROOT::Math::IMultiGenFunction *fll_0, *fll_1, *fll_2, *fll_3, *fll_4, *fll_5,
		*fll_6, *fll_7, *fll_8, *fll_9, *fll_10, *fll_11, *fll_12, *fll_13, *fll_14,
		*fll_15, *fll_16, *fll_17, *fll_18, *fll_19, *fll_20, *fll_21, *fll_22, *fll_23,
		*fll_24, *fll_25, *fll_26, *fll_27, *fll_28, *fll_29, *fll_30, *fll_31, *fll_32,
		*fll_33, *fll_34, *fll_35;
	
};

void fit_MB_clusters_LL( int clu0, int clu1 ) {
	
	// Some setup
	setup_clusters();
	int myclu[NCLU];
	myclu[0] = clu0;
	myclu[1] = clu1;
	
	// Initialise parameters
	assign_parameters();
	initialise_parameters( myclu );

	// Read in experimental data from file
	read_energies();
	
	// Fit functions for angles
	TF1 *fAng[NCLU*NSEG];
	string name;
	for( int i = 0; i < NCLU; i++ ) {
		
		for( int j = 0; j < NSEG; j++ ) {
			
			name = "fAng_" + convertInt(i) + "_" + convertInt(j);
			fAng[i*NSEG+j] = new TF1( name.c_str(), seg_fit_function, (double)j-0.5, (double)j+0.5, IPARS );
			
		}
		
	}
	
	//////////////////////////
	// Perform a global fit //
	//////////////////////////
	
	// First make individual fit wrappers
	ROOT::Math::WrappedMultiTF1 wClu_0( *fAng[0], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_1( *fAng[1], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_2( *fAng[2], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_3( *fAng[3], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_4( *fAng[4], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_5( *fAng[5], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_6( *fAng[6], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_7( *fAng[7], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_8( *fAng[8], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_9( *fAng[9], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_10( *fAng[10], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_11( *fAng[11], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_12( *fAng[12], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_13( *fAng[13], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_14( *fAng[14], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_15( *fAng[15], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_16( *fAng[16], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_17( *fAng[17], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_18( *fAng[18], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_19( *fAng[19], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_20( *fAng[20], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_21( *fAng[21], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_22( *fAng[22], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_23( *fAng[23], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_24( *fAng[24], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_25( *fAng[25], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_26( *fAng[26], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_27( *fAng[27], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_28( *fAng[28], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_29( *fAng[29], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_30( *fAng[30], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_31( *fAng[31], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_32( *fAng[32], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_33( *fAng[33], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_34( *fAng[34], 1 );
	ROOT::Math::WrappedMultiTF1 wClu_35( *fAng[35], 1 );
	
	// Data initialisers and filling
	ROOT::Fit::DataOptions opt;
	ROOT::Fit::DataRange range(-0.5,NSEG+0.5);
	ROOT::Fit::UnBinData data[NCLU*NSEG];

	for( int i = 0; i < NCLU; i++ ) {

		for( int j = 0; j < NSEG; j++ ) {
			
			//data[i*NSEG+j].Initialize( 1, 2, false );
			//data[i*NSEG+j].Add( (double)j, energies[lookup_clu(myclu[i])][j] );

			data[i*NSEG+j].Initialize( 1, 2, true );
			data[i*NSEG+j].Add( (double)j, energies[lookup_clu(myclu[i])][j],
							   1. / en_error[lookup_clu(myclu[i])][j] );

		}
		
	}

	// Get data size
	int data_size = 0;
	for( int i = 0; i < NCLU*NSEG; i++ ) data_size += data[i].Size();
	
	// Define chi2 functions
	ROOT::Fit::LogLikelihoodFunction ll_0( data[0], wClu_0 );
	ROOT::Fit::LogLikelihoodFunction ll_1( data[1], wClu_1 );
	ROOT::Fit::LogLikelihoodFunction ll_2( data[2], wClu_2 );
	ROOT::Fit::LogLikelihoodFunction ll_3( data[3], wClu_3 );
	ROOT::Fit::LogLikelihoodFunction ll_4( data[4], wClu_4 );
	ROOT::Fit::LogLikelihoodFunction ll_5( data[5], wClu_5 );
	ROOT::Fit::LogLikelihoodFunction ll_6( data[6], wClu_6 );
	ROOT::Fit::LogLikelihoodFunction ll_7( data[7], wClu_7 );
	ROOT::Fit::LogLikelihoodFunction ll_8( data[8], wClu_8 );
	ROOT::Fit::LogLikelihoodFunction ll_9( data[9], wClu_9 );
	ROOT::Fit::LogLikelihoodFunction ll_10( data[10], wClu_10 );
	ROOT::Fit::LogLikelihoodFunction ll_11( data[11], wClu_11 );
	ROOT::Fit::LogLikelihoodFunction ll_12( data[12], wClu_12 );
	ROOT::Fit::LogLikelihoodFunction ll_13( data[13], wClu_13 );
	ROOT::Fit::LogLikelihoodFunction ll_14( data[14], wClu_14 );
	ROOT::Fit::LogLikelihoodFunction ll_15( data[15], wClu_15 );
	ROOT::Fit::LogLikelihoodFunction ll_16( data[16], wClu_16 );
	ROOT::Fit::LogLikelihoodFunction ll_17( data[17], wClu_17 );
	ROOT::Fit::LogLikelihoodFunction ll_18( data[18], wClu_18 );
	ROOT::Fit::LogLikelihoodFunction ll_19( data[19], wClu_19 );
	ROOT::Fit::LogLikelihoodFunction ll_20( data[20], wClu_20 );
	ROOT::Fit::LogLikelihoodFunction ll_21( data[21], wClu_21 );
	ROOT::Fit::LogLikelihoodFunction ll_22( data[22], wClu_22 );
	ROOT::Fit::LogLikelihoodFunction ll_23( data[23], wClu_23 );
	ROOT::Fit::LogLikelihoodFunction ll_24( data[24], wClu_24 );
	ROOT::Fit::LogLikelihoodFunction ll_25( data[25], wClu_25 );
	ROOT::Fit::LogLikelihoodFunction ll_26( data[26], wClu_26 );
	ROOT::Fit::LogLikelihoodFunction ll_27( data[27], wClu_27 );
	ROOT::Fit::LogLikelihoodFunction ll_28( data[28], wClu_28 );
	ROOT::Fit::LogLikelihoodFunction ll_29( data[29], wClu_29 );
	ROOT::Fit::LogLikelihoodFunction ll_30( data[30], wClu_30 );
	ROOT::Fit::LogLikelihoodFunction ll_31( data[31], wClu_31 );
	ROOT::Fit::LogLikelihoodFunction ll_32( data[32], wClu_32 );
	ROOT::Fit::LogLikelihoodFunction ll_33( data[33], wClu_33 );
	ROOT::Fit::LogLikelihoodFunction ll_34( data[34], wClu_34 );
	ROOT::Fit::LogLikelihoodFunction ll_35( data[35], wClu_35 );

	Global globalLL( ll_0, ll_1, ll_2, ll_3, ll_4, ll_5, ll_6, ll_7, ll_8,
						  ll_9, ll_10, ll_11, ll_12, ll_13, ll_14, ll_15, ll_16,
						  ll_17, ll_18, ll_19, ll_20, ll_21, ll_22, ll_23, ll_24,
						  ll_25, ll_26, ll_27, ll_28, ll_29, ll_30, ll_31, ll_32,
						  ll_33, ll_34, ll_35 );

	ROOT::Fit::Fitter fitter;

	// create before the parameter settings in order to fix or set range on them
	fitter.Config().SetParamsSettings( NPARS, par0 );

	// set parameter names
	for( int i = 0; i < NPARS; i++ )
		fitter.Config().ParSettings(i).SetName( parname[i].c_str() );

	// set parameter limits
	for( int i = 0; i < NCLU; i++ ) {

		// r values must be positive
		fitter.Config().ParSettings(ipar[i*NSEG+0][0]).SetLowerLimit( 0.0 );
		
		// phi values between ±360
		fitter.Config().ParSettings(ipar[i*NSEG+0][1]).SetLowerLimit(   0.0 );
		fitter.Config().ParSettings(ipar[i*NSEG+0][1]).SetUpperLimit( 360.0 );
	
		// alpha values between ±360
		fitter.Config().ParSettings(ipar[i*NSEG+0][2]).SetLowerLimit(   0.0 );
		fitter.Config().ParSettings(ipar[i*NSEG+0][2]).SetUpperLimit( 360.0 );

	}
	
	// fix parameters by uncommenting and/or adding new lines
	fitter.Config().ParSettings(NPARS-1).Fix();	// beta = 0.0656

	fitter.Config().SetMinimizer( "Minuit2", "minimize" ); // Minuit2 and Migrad as defaults (best!)
	fitter.Config().MinimizerOptions().SetMaxIterations(9999999);
	fitter.Config().MinimizerOptions().SetMaxFunctionCalls(9999999);
	fitter.Config().MinimizerOptions().SetPrintLevel(1);
	
	//fitter.Config().MinimizerOptions().SetPrecision(1.0e-9);
	fitter.Config().MinimizerOptions().SetTolerance(1.0e-5);
	cout << "Minimizer precision = " << fitter.Config().MinimizerOptions().Precision() << endl;
	cout << "Minimizer tolerance = " << fitter.Config().MinimizerOptions().Tolerance() << endl;

	// fit FCN function directly
	fitter.FitFCN( NPARS, globalLL, par0, data_size, false );
	
	// Make comparison of values
	double segNo[1];
	double tmp_par[IPARS];
	double calc_en, exp_en, err_en;
	
	cout << "Clu-Seg\t     Exp.\t     Err.\t     Calc.\t  Sigma" << endl;
	for( int i = 0; i < NCLU; i++ ) {
		
		for( int j = 0; j < NSEG; j++ ) {
			
			cout << myclu[i] << "-" << j;
			
			segNo[0] = j;
			for( int k = 0; k < IPARS; k++ )
				tmp_par[k] = par0[ ipar[i*NSEG+j][k] ];
			
			exp_en = energies[lookup_clu(myclu[i])][j];
			err_en = en_error[lookup_clu(myclu[i])][j];
			calc_en = seg_fit_function( segNo, tmp_par );
			
			cout << "\t" << setfill(' ') << setw(9) << exp_en;
			cout << "\t" << setfill(' ') << setw(9) << err_en;
			cout << "\t" << setfill(' ') << setw(9) << calc_en;
			cout << "\t" << setfill(' ') << setw(7) << (exp_en - calc_en) / err_en;
			cout << endl;
			
		}
		
	}
	
	// Print result to screen/file
	ROOT::Fit::FitResult result = fitter.Result();
	ofstream fitfile;
	fitfile.open( "fitresult.txt", ios::out );
	result.Print( std::cout );
	result.Print( fitfile );
	result.PrintCovMatrix( fitfile );

	return;

}