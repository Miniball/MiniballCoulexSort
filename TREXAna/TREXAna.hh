// TREXAna: The main routine that calls tr_ana
// Liam Gaffney (liam.gaffney@liverpool.ac.uk) - 12/02/2022

#ifndef tr_ana_hh
# include "tr_ana.hh"
#endif

#include "CommandLineInterface.hh"

#include <TEnv.h>
#include <TChain.h>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int j = 0;

string outputfilename, configfilename;
vector<string> inputfilenames;

float		GammaEnergy;
int			Zb = 0;				///< Z of the beam
int			Zt = 0;				///< Z of the target
float		Ab = 0;				///< mass of the beam in u
float		At = 0;				///< mass of the target in u
float		Eb = 0.0;			///< incident beam energy in keV/u
float		Ex = 0.0;			///< excitation energy of Coulex in keV
float		thick = 0.0;		///< thickness of the target in mg/cm^2
float		depth = 0.0;		///< interaction depth in target in mg/cm^2
float		cddist = 0.0;		///< CD to target distance in mm
float		cdoffset = 242.6;	///< phi rotation of CD wrt to (det=0;sec=0) at vertical
float		deadlayer = 0.0007;	///< CD dead layer (aluminium) thickness in mm
float		contaminant = -1.0;	///< contaminant layer thickness in mg/cm^2 (negative means no contaminant)
float		spededist = 23.6;	///< SPEDE to target distance in mm
float		bg_frac = -1.0;		///< ratio of prompt and random background subtraction (negative)
string      srim = "./srim";	///< directory containing the SRIM output files for Doppler correction
bool		usekin = false;		///< Flag to use two-body kinematics for particle velocity

void PrintInput();
