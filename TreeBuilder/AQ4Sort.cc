#include "AQ4Sort.hh"

#define GBINS 4000
#define GRANGE 4000
#define GMIN -1.0*GRANGE/GBINS
#define GMAX GRANGE+GMIN
#define GAMMA_ARRAY 250

ClassImp(BuiltEvent)
ClassImp(AdcData)
ClassImp(DgfData)

int main(int argc, char* argv[]) {

	vector<char*> InputFiles;
	string OutputFile;
	string CalibrationFile;
	bool verbose = false;
	CommandLineInterface* interface = new CommandLineInterface();

	interface->Add("-i", "inputfiles", &InputFiles );
	interface->Add("-o", "outputfile", &OutputFile );
	interface->Add("-c", "calibrationfile", &CalibrationFile );
	interface->Add("-vl", "verbose", &verbose );

	interface->CheckFlags(argc, argv);

	if( CalibrationFile.size() == 0 ) {

		cout << "No calibration file given, default values will be used" << endl;
		CalibrationFile = "default";

	}

	else cout << "calibration file: " << CalibrationFile << endl;

	if( InputFiles.size() == 0 || OutputFile.size() == 0 ) {

		cerr << "You have to provide at least one input file and the output file!" << endl;
		exit(1);

	}

	cout << "input file(s):" << endl;
	for( unsigned int i=0; i<InputFiles.size(); i++ ) {

		cout << InputFiles[i] << endl;
	
	}
	
	cout << "output file: " << OutputFile << endl;

	TChain* tr;
	tr = new TChain("tr");
	for( unsigned int i = 0; i < InputFiles.size(); i++ ) {
	
		tr->Add( InputFiles[i] );
		
	}

	if( tr == NULL ) {

		cout << "could not find tree caltr in file " << endl;

		for( unsigned int i=0; i < InputFiles.size(); i++ ) {

			cout << InputFiles[i] << endl;

		}
		
		return 3;

	}

	Calibration *Cal = new Calibration( CalibrationFile.c_str() );
	if(verbose) Cal->PrintCalibration();

	BuiltEvent* event = new BuiltEvent;

	tr->SetBranchAddress( "Event", &event );

	TFile* outfile = new TFile( OutputFile.c_str(), "recreate" );
	if( outfile->IsZombie() ) return 4;
	
	unsigned int i,j,k,l;
  
	// Variables
	unsigned int adc_num = 0;
	unsigned int adc_ch = 0;
	unsigned int adc_en = 0;
	unsigned int adc_ch2 = 0;
	unsigned int adc_en2 = 0;
	float energy;
 
	// ------ Histograms ------ //
	// Particle spectra for every segment
	TH1F *E_part_ann[4][16], *E_part_ann_cal[4][16];
	TH1F *E_part_sec[4][12], *E_part_sec_cal[4][12];
	TH1F *E_part_ann_px[4][16][12], *E_part_ann_px_cal[4][16][12];
	TH1F *E_part_sec_px[4][16][12], *E_part_sec_px_cal[4][16][12];
	TH2F *mE_part_ann_cal[4];
	TH2F *mE_part_sec_cal[4];
	TH2F *mE_part_sec_px_cal[4]; //strip #15


	for( i = 0; i < 4; i++ ) {

		mE_part_ann_cal[i] = new TH2F(Form("fE_Q%d",i+1),Form("Calib. matrix fE_Q%d",i+1), 16,0,16,4000,-0.5,3999999.5);
		mE_part_ann_cal[i]->GetYaxis()->SetTitle("Energy Particles [keV]");
		mE_part_sec_cal[i] = new TH2F(Form("bE_Q%d",i+1),Form("Calib. matrix bE_Q%d",i+1), 12,0,12,4000,-0.5,3999999.5);
		mE_part_sec_cal[i]->GetYaxis()->SetTitle("Energy Particles [keV]");
		mE_part_sec_px_cal[i] = new TH2F(Form("bE_Q%d_f15",i+1),Form("Calib. matrix bE_Q%d_f15",i+1), 12,0,12,4000,-0.5,3999999.5); //strip #15
		mE_part_sec_px_cal[i]->GetYaxis()->SetTitle("Energy Particles [keV]");

		for( j = 0; j < 16; j++ ) {

			E_part_ann[i][j] = new TH1F(Form("fE_Q%d_f%d",i+1,j+1),Form("fE_Q%d_f%d",i+1,j+1),4096,-0.5,4095.5);
			E_part_ann[i][j]->GetXaxis()->SetTitle("Energy Particles [Channels]");

			E_part_ann_cal[i][j] = new TH1F(Form("fE_Q%d_f%d_cal",i+1,j+1),Form("fE_Q%d_f%d_cal",i+1,j+1),4000,-0.5,3999999.5);
			E_part_ann_cal[i][j]->GetXaxis()->SetTitle("Energy Particles [keV]");

			for( k = 0; k < 12; k++ ) {

				E_part_ann_px[i][j][k] = new TH1F(Form("fE_Q%d_f%d_b%d",i+1,j+1,k+1),Form("fE_Q%d_f%d_b%d",i+1,j+1,k+1),4096,-0.5,4095.5);
				E_part_ann_px[i][j][k]->GetXaxis()->SetTitle("Energy Particles [Channels]");

				E_part_ann_px_cal[i][j][k] = new TH1F(Form("fE_Q%d_f%d_b%d_cal",i+1,j+1,k+1),Form("fE_Q%d_f%d_b%d_cal",i+1,j+1,k+1),4000,-0.5,3999999.5);
				E_part_ann_px_cal[i][j][k]->GetXaxis()->SetTitle("Energy Particles [keV]");

			}

		}

		for( j = 0; j < 12; j++ ) {

			E_part_sec[i][j] = new TH1F(Form("bE_Q%d_b%d",i+1,j+1),Form("bE_Q%d_b%d",i+1,j+1),4096,-0.5,4095.);
			E_part_sec[i][j]->GetXaxis()->SetTitle("Energy Particles [Channels]");

			E_part_sec_cal[i][j] = new TH1F(Form("bE_Q%d_b%d_cal",i+1,j+1),Form("bE_Q%d_b%d_cal",i+1,j+1),4000,-0.5,3999999.5);
			E_part_sec_cal[i][j]->GetXaxis()->SetTitle("Energy Particles [keV]");

			for( k = 0; k < 16; k++ ) {

				E_part_sec_px[i][k][j] = new TH1F(Form("bE_Q%d_f%d_b%d",i+1,k+1,j+1),Form("bE_Q%d_f%d_b%d",i+1,k+1,j+1),4096,-0.5,4095.5);
				E_part_sec_px[i][k][j]->GetXaxis()->SetTitle("Energy Particles [Channels]");

				E_part_sec_px_cal[i][k][j] = new TH1F(Form("bE_Q%d_f%d_b%d_cal",i+1,k+1,j+1),Form("bE_Q%d_f%d_b%d_cal",i+1,k+1,j+1),4000,-0.5,3999999.5);
				E_part_sec_px_cal[i][k][j]->GetXaxis()->SetTitle("Energy Particles [keV]");

			}

		}

	}
	
	Double_t nentries = tr->GetEntries();
	Int_t nbytes = 0;
	Int_t status;
	vector<int> DgfModule;
	
	// Test - comment out for runs
	//nentries = 100000;
	
	// Start loop over number of entries
	for( i = 0; i < nentries; i++ ) {
	
		status = tr->GetEvent(i);
		if( status == -1 ) {

			cerr << "Error occured, couldn't read entry " << i << " from tree ";
			cerr << tr->GetName() << " in file " << tr->GetFile()->GetName() << endl;
			return 5;
			
		}
		
		else if( status == 0 ) {

			cerr << "Error occured, entry " << i << " in tree " << tr->GetName();
			cerr << " in file " << tr->GetFile()->GetName() << " doesn't exist" << endl;
			return 6;

		}
		
		nbytes += status;

		for( j = 0; j < event->NumberOfAdcs(); j++ ) {

			adc_num = event->Adc(j)->ModuleNumber();
			if( adc_num >= 4 ) continue;

			for( k = 0; k < event->Adc(j)->SubEvent()->Size(); k++ ) {

				adc_ch = event->Adc(j)->SubEvent()->AdcChannel(k);
				adc_en = event->Adc(j)->SubEvent()->AdcValue(k);
				energy = Cal->AdcEnergy( adc_num, adc_ch, adc_en );

				if( adc_ch < 16 ) { // front rings
						
					E_part_ann[adc_num][adc_ch]->Fill( adc_en );
					E_part_ann_cal[adc_num][adc_ch]->Fill( energy );
					mE_part_ann_cal[adc_num]->Fill(adc_ch,energy);

					for( l = 0; l < event->Adc(j)->SubEvent()->Size(); l++ ) {

						adc_ch2 = event->Adc(j)->SubEvent()->AdcChannel(l);
						adc_en2 = event->Adc(j)->SubEvent()->AdcValue(l);

						if( adc_ch2 >= 16 && adc_ch2 < 28 && adc_en2 > 100  ) {

							E_part_ann_px[adc_num][adc_ch][adc_ch2-16]->Fill( adc_en );
							E_part_ann_px_cal[adc_num][adc_ch][adc_ch2-16]->Fill( energy );

						}

					} // l

				} // front rings
					
				else if( adc_ch < 28 ) { // back strips

					E_part_sec[adc_num][adc_ch-16]->Fill( adc_en );
					E_part_sec_cal[adc_num][adc_ch-16]->Fill( energy );
					mE_part_sec_cal[adc_num]->Fill(adc_ch-16, energy);
						
					for( l = 0; l < event->Adc(j)->SubEvent()->Size(); l++ ) {

						adc_ch2 = event->Adc(j)->SubEvent()->AdcChannel(l);
						adc_en2 = event->Adc(j)->SubEvent()->AdcValue(l);

						if( adc_ch2 < 16 && adc_en2 > 100 ) {

							E_part_sec_px[adc_num][adc_ch2][adc_ch-16]->Fill( adc_en );
							E_part_sec_px_cal[adc_num][adc_ch2][adc_ch-16]->Fill( energy );
							if(adc_ch2==14)
								mE_part_sec_px_cal[adc_num]->Fill(adc_ch-16,energy) ; //strip #15

						}

					} // l

				} // back strips

			} // k

		} // j

		// Progress bar
		if( i%50000 == 0 ) cout << setw(5) << setiosflags(ios::fixed) << setprecision(1) << (100.*i)/nentries << "% done\r" << flush;

	} // i

	outfile->Write();
	outfile->Close();
	delete tr;

	return 0;
	
}

