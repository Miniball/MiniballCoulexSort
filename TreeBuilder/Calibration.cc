#include "Calibration.hh"
#include "TMath.h"

using namespace std;
using namespace TMath;

Calibration::Calibration( string filename ) {

	SetFile( filename );
	fVerbose = 0;
	fNofDgfs = 48;
	fNofDgfChans = 4;
	fNofAdcs = 10; // up to for TREX
	fNofAdcChans = 32;
	fNofClusters = 8;
	fBeamdumpDgf = 53;
	RawRandomized = 0.;
	ReadCalibration();
	fRand = new TRandom();
	//if( fVerbose ) PrintCalibration();
		
}

Calibration::~Calibration() {

	//cout << "destructor" << endl;

}

void Calibration::ReadCalibration() {

	TEnv *config = new TEnv(fInputFile.data());
	if( fVerbose ) cout << "reading dgfs" << endl;
	fDgfOffset.resize(fNofDgfs);
	fDgfGain.resize(fNofDgfs);
	fDgfGainQuadr.resize(fNofDgfs);

	for( int dgf = 0; dgf < fNofDgfs; dgf++ ){

		fDgfOffset[dgf].resize(fNofDgfChans);
		fDgfGain[dgf].resize(fNofDgfChans);
		fDgfGainQuadr[dgf].resize(fNofDgfChans);

		for( int chan = 0; chan < fNofDgfChans; chan++ ){

			fDgfOffset[dgf][chan] = config->GetValue(Form("dgf_%d_%d.Offset", dgf, chan),0.);
			fDgfGain[dgf][chan] = config->GetValue(Form("dgf_%d_%d.Gain", dgf, chan),1.);
			fDgfGainQuadr[dgf][chan] = config->GetValue(Form("dgf_%d_%d.GainQuadr", dgf, chan),0.);  // 1 -> 0

		}

	}
	
	if( fVerbose ) cout << "reading beamdump" << endl;
	fBeamdumpOffset.resize(fNofDgfChans);
	fBeamdumpGain.resize(fNofDgfChans);
	fBeamdumpGainQuadr.resize(fNofDgfChans);

	for( int chan = 0; chan < fNofDgfChans; chan++ ) {

		fBeamdumpOffset[chan] = config->GetValue(Form("dgf_%d_%d.Offset", fBeamdumpDgf, chan), 0.);
		fBeamdumpGain[chan] = config->GetValue(Form("dgf_%d_%d.Gain", fBeamdumpDgf, chan), 1.);
		fBeamdumpGainQuadr[chan] = config->GetValue(Form("dgf_%d_%d.GainQuadr", fBeamdumpDgf, chan), 0.);

	}


	if( fVerbose ) cout << "reading adcs" << endl;
	fAdcOffset.resize(fNofAdcs);
	fAdcGain.resize(fNofAdcs);
	fAdcThreshold.resize(fNofAdcs);
	fAdcTime.resize(fNofAdcs);

	for( int adc = 0; adc < fNofAdcs; adc++ ){

		fAdcOffset[adc].resize(fNofAdcChans);
		fAdcGain[adc].resize(fNofAdcChans);
		fAdcThreshold[adc].resize(fNofAdcChans);

		fAdcTime[adc] = config->GetValue( Form("adc_%d.TimeOffset", adc ) , 0.0 );

		for( int chan = 0; chan < fNofAdcChans; chan++ ){

			fAdcOffset[adc][chan] = config->GetValue(Form("adc_%d_%d.Offset", adc, chan),0.);
			fAdcGain[adc][chan] = config->GetValue(Form("adc_%d_%d.Gain", adc, chan),1.);
			fAdcThreshold[adc][chan] = config->GetValue(Form("adc_%d_%d.Threshold", adc, chan),100.);

		}

	}
  
	if( fVerbose ) cout << "reading Miniball angles" << endl;
	fClusterTheta.resize(fNofClusters);
	fClusterPhi.resize(fNofClusters);
	fClusterAlpha.resize(fNofClusters);
	fClusterR.resize(fNofClusters);

	for( int clu = 0; clu < fNofClusters; clu++ ) {

		fClusterR[clu] =     config->GetValue( Form("Cluster_%d.R", clu), 0. );
		fClusterTheta[clu] = config->GetValue( Form("Cluster_%d.Theta", clu), 0. );
		fClusterPhi[clu] =   config->GetValue( Form("Cluster_%d.Phi", clu), 0. );
		fClusterAlpha[clu] = config->GetValue( Form("Cluster_%d.Alpha", clu), 0. );

	}

	zoffset = config->GetValue( "zoffset", 0. );

	delete config;
	
}

void Calibration::PrintCalibration(){

	cout << "DGFs" << endl;
	for( int dgf = 0; dgf < fNofDgfs; dgf++ ){
	
		for( int chan = 0; chan < fNofDgfChans; chan++ ){
		
			cout << Form("dgf_%d_%d.Offset\t", dgf, chan) << fDgfOffset[dgf][chan] << endl;
			cout << Form("dgf_%d_%d.Gain\t", dgf, chan) << fDgfGain[dgf][chan] << endl;
			cout << Form("dgf_%d_%d.GainQuadr\t", dgf, chan) << fDgfGainQuadr[dgf][chan] << endl;
      
		}
    
	}
  
	cout << "Beamdump" << endl;
	for( int chan = 0; chan < fNofDgfChans; chan++ ){

		cout << Form("dgf_%d_%d.Offset\t", fBeamdumpDgf, chan) << fBeamdumpOffset[chan] << endl;
		cout << Form("dgf_%d_%d.Gain\t", fBeamdumpDgf, chan) << fBeamdumpGain[chan] << endl;
		cout << Form("dgf_%d_%d.GainQuadr\t", fBeamdumpDgf, chan) << fBeamdumpGainQuadr[chan] << endl;

	}
  
	cout << "ADCs" << endl;
	for( int adc = 0; adc < fNofAdcs; adc++ ){

		cout << Form( "adc_%d.TimeOffset\t", adc ) << fAdcTime[adc] << endl;

		for( int chan=0; chan < fNofAdcChans; chan++ ){

			cout << Form( "adc_%d_%d.Offset\t", adc, chan ) << fAdcOffset[adc][chan] << endl;
			cout << Form( "adc_%d_%d.Gain\t", adc, chan ) << fAdcGain[adc][chan] << endl;
			cout << Form( "adc_%d_%d.Threshold\t", adc, chan ) << fAdcThreshold[adc][chan] << endl;
	
		}

	}

	cout << "Miniball angles" << endl;
	for( int clu = 0; clu < fNofClusters; clu++ ) {
		
		cout << Form( "Cluster_%d.R", clu ) << fClusterR[clu];
		cout << Form( "Cluster_%d.Theta", clu ) << fClusterTheta[clu];
		cout << Form( "Cluster_%d.Phi", clu ) << fClusterPhi[clu];
		cout << Form( "Cluster_%d.Alpha", clu ) << fClusterAlpha[clu];
		
	}
	
	cout << "zoffset: " << zoffset << endl;

}

double Calibration::DgfEnergy(int dgf, int chan, unsigned short raw){

	double energy;

	if( (dgf>-1) && (dgf<fNofDgfs) && (chan>-1) && (chan<fNofDgfChans) ){

		cout.precision(7);
		RawRandomized = raw + 0.5 - fRand->Uniform();
		
		energy = fDgfGainQuadr[dgf][chan]*TMath::Power(RawRandomized,2) + fDgfGain[dgf][chan]*RawRandomized + fDgfOffset[dgf][chan];
		
		if(fVerbose > 1 && chan == 0 && dgf % 2 == 0) {
			
			cout << "Dgf: " << dgf << ". Chan: " << chan << ". Raw: " << raw << ". EnergyCal: " << energy << endl;

		}
		
		return energy;

	}

	else if( dgf == 53 && chan >= 0 && chan < 4 ) {
	
		cout.precision(7);
		RawRandomized = raw + 0.5 - fRand->Uniform();

		energy = fBeamdumpGainQuadr[chan]*TMath::Power(RawRandomized,2);
		energy += fBeamdumpGain[chan]*RawRandomized;
		energy += fBeamdumpOffset[chan];

		return energy; 

	}
	
	else cerr << "dgf " << dgf << " channel " << chan << " not found!" << endl;

	return -1;

}

double Calibration::AdcEnergy( int adc, int chan, unsigned short raw ) {
	
	if( adc >= 0 && adc < fNofAdcs && chan >= 0 && chan < fNofAdcChans ) {
		
		return ((fAdcGain[adc][chan]*( raw +0.5 - fRand->Uniform())) + fAdcOffset[adc][chan]);
		
	}
	
	else cerr << "adc " << adc << " channel " << chan << " not found!" << endl;
	
	return -1;
	
}

double Calibration::AdcThreshold( int adc, int chan ) {
	
	if( adc >= 0 && adc < fNofAdcs && chan >= 0 && chan < fNofAdcChans ) {
		
		return fAdcThreshold[adc][chan];
		
	}
	
	else cerr << "adc " << adc << " channel " << chan << " not found!" << endl;
	
	return -1;
	
}

double Calibration::AdcTime( int adc ){
	
	if( adc >= 0 && adc < fNofAdcs ) {
		
		return fAdcTime[adc];
		
	}
	
	else cerr << "adc " << adc << " not found!" << endl;
	
	return -1;
	
}

double Calibration::ClusterTheta( int clu ){
	
	if( clu >= 0 && clu < fNofClusters ) {
		
		return fClusterTheta[clu];
		
	}
	
	else cerr << "Cluster " << clu << " not found!" << endl;
	
	return -1;
	
}

double Calibration::ClusterPhi( int clu ){
	
	if( clu >= 0 && clu < fNofClusters ) {
		
		return fClusterPhi[clu];
		
	}
	
	else cerr << "Cluster " << clu << " not found!" << endl;
	
	return -1;
	
}

double Calibration::ClusterAlpha( int clu ){
	
	if( clu >= 0 && clu < fNofClusters ) {
		
		return fClusterAlpha[clu];
		
	}
	
	else cerr << "Cluster " << clu << " not found!" << endl;
	
	return -1;
	
}

double Calibration::ClusterR( int clu ){
	
	if( clu >= 0 && clu < fNofClusters ) {
		
		return fClusterR[clu];
		
	}
	
	else cerr << "Cluster " << clu << " not found!" << endl;
	
	return -1;
	
}

double Calibration::ZOffset(){

	return zoffset;
	
}
