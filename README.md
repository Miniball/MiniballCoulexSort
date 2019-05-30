# MiniballCoulexSort [![DOI](https://zenodo.org/badge/99667438.svg)](https://zenodo.org/badge/latestdoi/99667438)
Analysis code for MINIBALL experiments at ISOLDE. This code is designed to take the raw med data files and convert them to ROOT trees, then subsequently analyse them to produce particle-gated, Doppler corrected spectra. It is broken down in to 3 parts:

### MedToRoot
Based on (read: essentially a copy of) code from Vinzenz Bildstein.
Written originally for transfer experiments with T-REX, adapted for Coulex with C-REX and now finally for Coulex with SPEDE or with the standard Coulex configuration.

### TreeBuilder
Written by Liam Gaffney, inpsired by initial work by Andres Illana Sison. Daniel Cox implemented SPEDE into this code performed a lot of testing. This takes the converted ROOT file and build an 'mbevts' tree of particle-gamma(-gamma) events that can be used for analysing Coulex events. For transfer experiments with T-REX, it can also be used to build a 'trevts' tree with reconstructed particles and coincident gamma-rays. 

It is in this section that the calibration file is read in, including the Miniball Ge angles, gain and offsets for DGF/ADC energies, and time offsets and thresholds for the ADC's.

### CLXAna
The g_clx part of this is based on codes by Oliver Niedemeier, with contributions by Nigel Warr and Michael Albers. However, it has been completely rewritten by Liam Gaffney to include separate classes for filling histograms and performing Doppler correction. There is still much work to do and the current iteration is being developed by Liam Gaffney (CERN) and Daniel Cox (University of Jyväskylä) for the SPEDE parts.

### mb_sorter

A GUI is also available called mb_sorter. Here you can perform all of the above steps in a simple way. You can save the configuration to a text file to be read in again later.

### Other codes

```
NeSort
AQ4Sort
TDRIVAna
MntAna
```

### Basic process for analysis

To generate initial root file
```
MedToRoot -mf data_file.med -sf settings_file.dat
```
data_file_OnBeam.root (and others) will be created.

The OnBeam file is needed for analysis. Adding the option -so will create a file without the condition on the EBIS pulse and is used for runs calibration sources. You can also limit the number of processed events with -le (last event) and -fe (first event).

```
TreeBuilder -i data_file1_Source.root [data_file2_OnBeam.root ...] -o Output_tree.root -c calibration_file.dat
```
This step takes a number of possible flags:
```
	[-s                      : singles]
	[-addback                : addback]
	[-reject                 : reject]
	[-crex                   : CREX]
	[-trex                   : TREX]
	[-spede                  : SPEDE]
	[-cdpad                  : CD-PAD]
	[-ionch                  : IonChamber]
	[-vl                     : verbose]
```
Generated file can now be used as input to final analysis code

```
CLXAna -i Output_tree.root -o Final_Output_hists.root
```
This step takes a number of possible inputs:
```
[-i           <vector<string>>: Input file list]
[-o           <string        >: Output file name]
[-c           <string        >: Configuration file]
[-cut         <string        >: Cutfile [cutfile.root:Bcut:Tcut]]
[-Zb          <int           >: Zb]
[-Ab          <float         >: Ab]
[-Zt          <int           >: Zt]
[-At          <float         >: At]
[-Eb          <float         >: Eb]
[-Ex          <float         >: Ex]
[-thick       <float         >: Target thickness (mg/cm^2)]
[-depth       <float         >: Depth of interation in target (mg/cm^2)]
[-cddist      <float         >: Relative distance of CD and target (mm)]
[-cdoffset    <float         >: Rotation of CD detector about phi from vertical (deg)]
[-deadlayer   <float         >: Thickness of Si dead layer (mm)]
[-contaminant <float         >: Thickness of contaminant layer on target (mg/cm^2)]
[-spededist   <float         >: Relative distance of SPEDE and target (mm)]
[-bg_frac     <float         >: Ratio of prompt and random for background subtraction]
[-srim        <string        >: Directory containing the SRIM output files for Doppler correction]
```

## Download and Installation

The recommended way to install this suite of codes is to first clone it using 'git'.
```
git clone http://github.com/Miniball/MiniballCoulexSort.git
```
Go to the directory that has just been created and run make,
assuming all dependencies (specifically ROOT) are installed.
```
cd MiniballCoulexSort
make
```
You then need to add the bin and lib directories to your standard paths.
This may be done in your ~/.profile or ~/.bashrc files if your shell is bash.
```
export PATH=$PATH:/path/to/MiniballCoulexSort/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/MiniballCoulexSort/lib
```
If you are using a Mac, then you need to replace LD_LIBRARY_PATH with DYLD_LIBRARY_PATH.

Open a new terminal and launch the GUI with the command 'mb_sorter'.


## Requirements

MiniballCoulexSort requires ROOT v5 or v6 and an up to date C++ compiler with the c++11 standard as minimum.
SRIM files. In order to perform the Doppler correction properly, one needs to have the appropriate SRIM files. (to be completed)
