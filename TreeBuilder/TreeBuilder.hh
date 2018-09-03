// TreeBuilder: The main routine that calls GammaTree or TransferTree
// Liam Gaffney (liam.gaffney@cern.ch) - 31/08/2018

#include <iostream>
#include <iomanip>
#include <string>

#ifndef GammaTree_hh
# include "GammaTree.hh"
#endif

#ifndef TransferTree_hh
# include "TransferTree.hh"
#endif

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
using namespace std;

// Command line interface
#ifndef __COMMAND_LINE_INTERFACE
# include "CommandLineInterface.hh"
#endif

vector<char*> InputFiles;
string OutputFile;
string CalibrationFile;
bool singles = false;
bool gamgam = false;
bool addback = false;
bool crex = false;
bool trex = false;
bool cdpad = false;
bool ionch = false;
bool spede = false;
bool verbose = false;
CommandLineInterface* interface = new CommandLineInterface();

void PrintInput();
