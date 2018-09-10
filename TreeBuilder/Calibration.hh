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

// Command line interface
#ifndef __COMMAND_LINE_INTERFACE
# include "CommandLineInterface.hh"
#endif

using namespace std;

/// A class to read in the calibration file in ROOT's TConfig format.
/// Each ADC and DGF channel can have offset, gain and quadratic terms.
/// The ADCs also have a threshold for each channel, to cut out the "zeroes"
/// and there is a time offset parameter for each ADC module, too.
/// There is potential to include the demux of T/C-REX in here too.

class Calibration {

public:

	Calibration( string filename );
	virtual ~Calibration();
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
	double AdcThreshold(int adc, int chan);
	double AdcTime(int adc);
	double ClusterTheta( int clu );
	double ClusterPhi( int clu );
	double ClusterAlpha( int clu );
	double ClusterR( int clu );
	double ZOffset();

private:

	int fVerbose;
	string fInputFile;

	TRandom* fRand;
  
	vector< vector<long double> > fDgfGainQuadr;
	vector< vector<double> > fDgfOffset;
	vector< vector<double> > fDgfGain;
	vector< vector<double> > fAdcOffset;
	vector< vector<double> > fAdcGain;
	vector< vector<double> > fAdcThreshold;

	vector< double > fClusterTheta;
	vector< double > fClusterPhi;
	vector< double > fClusterR;
	vector< double > fClusterAlpha;

	vector<double> fAdcTime;

	double zoffset;

	vector< double > fBeamdumpOffset;
	vector< double > fBeamdumpGain;
	vector< double > fBeamdumpGainQuadr;
  
	double RawRandomized;

	int fNofDgfs;
	int fNofDgfChans;
	int fNofAdcs;
	int fNofAdcChans;
	int fNofClusters;
 
	int fBeamdumpDgf;

	ClassDef(Calibration, 1)
   
};

#endif
