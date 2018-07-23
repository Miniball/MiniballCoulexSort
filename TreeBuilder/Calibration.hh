#ifndef __CALIBRATION_HH
#define __CALIBRATION_HH

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>

#include "TSystem.h"
#include "TEnv.h"
#include "TRandom.h"
#include "TMath.h"

#include "CommandLineInterface.hh"

using namespace std;

/// A class to read in the calibration file in ROOT's TConfig format.
/// Each ADC and DGF channel can have offset, gain and quadratic terms.
/// There is potential to include the demux of T/C-REX in here too.

class Calibration {

public:

	Calibration( string filename );
	~Calibration();
	void ReadCalibration();
	void PrintCalibration();
	void SetFile( string filename ){
		fInputFile = filename;
	}
	const string InputFile(){
		return fInputFile;
	}
	double DgfEnergy(int dgf, int chan, unsigned short raw);
	double AdcEnergy(int adc, int chan, unsigned short raw);
	double AdcTime(int adc);
	double ClusterTheta( int clu );
	double ClusterPhi( int clu );
	double ClusterAlpha( int clu );
	double ClusterR( int clu );
	double ZOffset();
	int PosFBCDRing(int Quad, unsigned short raw);
	int PosFBCDStrip(int Quad, unsigned short raw);
	int PosRing(int Quad, unsigned short raw);
	int PosStrip(int Quad, unsigned short raw);
	int StripPosBarrel(unsigned short strraw, unsigned short rearraw);

private:

	int fVerbose;
	string fInputFile;

	TRandom* fRand;
  
	vector< vector<long double> > fDgfGainQuadr;
	vector< vector<double> > fDgfOffset;
	vector< vector<double> > fDgfGain;
	vector< vector<double> > fAdcOffset;
	vector< vector<double> > fAdcGain;
	vector< vector<double> > fFCDPosStrip;
	vector< vector<double> > fFCDPosRing;
	vector< vector<double> > fBCDPosStrip;
	vector< vector<double> > fBCDPosRing;

	vector< double > fClusterTheta;
	vector< double > fClusterPhi;
	vector< double > fClusterR;
	vector< double > fClusterAlpha;

	vector<double> fAdcTime;

	double zoffset;

	double fBeamdumpOffset;
	double fBeamdumpGain;
	double fBeamdumpGainQuadr;
  
	double RawRandomized;

	int fNofDgfs;
	int fNofDgfChans;
	int fNofAdcs;
	int fNofAdcsCD;
	int fNofAdcChans;
	int fNofCDSegm;
	int fNofClusters;
 
	double fFWHMPosMux;
	vector<double> fLimitFBCD;

	int fBeamdumpDgf;

	ClassDef(Calibration, 1)
   
};

#endif
