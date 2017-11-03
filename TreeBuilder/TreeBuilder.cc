#include "TreeBuilder.hh"

#define PBINS 800
#define PRANGE 800
#define PMIN -1.0*PRANGE/PBINS
#define PMAX PRANGE+PMIN

#define GBINS 4000
#define GRANGE 4000
#define GMIN -1.0*GRANGE/GBINS
#define GMAX GRANGE+GMIN
#define GAMMA_ARRAY 250

#define ELBINS 2000
#define ELRANGE 2000
#define ELMIN -1.0*ELRANGE/ELBINS
#define ELMAX ELRANGE+ELMIN
#define ELECTRON_ARRAY 250

ClassImp(BuiltEvent)
ClassImp(AdcData)
ClassImp(DgfData)

int main(int argc, char* argv[]) {

	// ------------------------------------------------------------------------ //
	// Read command line
	// ------------------------------------------------------------------------ //
	vector<char*> InputFiles;
	string OutputFile;
	string CalibrationFile;
	bool singles = false;
	bool gamgam = false;
	bool addback = false;
	bool crex = false;
	bool cdpad = false;
	bool ionch = false;
	bool spede = false;
	bool verbose = false;
	CommandLineInterface* interface = new CommandLineInterface();

	interface->Add("-i", "inputfiles", &InputFiles );
	interface->Add("-o", "outputfile", &OutputFile );
	interface->Add("-c", "calibrationfile", &CalibrationFile );
	interface->Add("-s", "singles", &singles );
	interface->Add("-gg", "gamma-gamma", &gamgam );
	interface->Add("-addback", "addback", &addback );
	interface->Add("-crex", "CREX", &crex );
	interface->Add("-spede", "SPEDE", &spede );
	interface->Add("-cdpad", "CD-PAD", &cdpad );
	interface->Add("-ionch", "IonChamber", &ionch );
	interface->Add("-vl", "verbose", &verbose );

	interface->CheckFlags(argc, argv);

	if( InputFiles.size() == 0 || OutputFile.size() == 0 ) {

		cerr << "You have to provide at least one input file and the output file!" << endl;
		exit(1);

	}

	if( CalibrationFile.size() == 0 ) {

		cout << "No Calibration File given, you have to provide one!" << endl;
		exit(1);

	}

	cout << "input file(s):" << endl;
	for( unsigned int i=0; i<InputFiles.size(); i++ ) {

		cout << InputFiles[i] << endl;
	
	}
	
	cout << "calibration file: " << CalibrationFile << endl;
	cout << "output file: " << OutputFile << endl;
	// ------------------------------------------------------------------------ //


	// ------------------------------------------------------------------------ //
	// Initialise events and output file
	// ------------------------------------------------------------------------ //
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
	// ------------------------------------------------------------------------ //

	
	// ------------------------------------------------------------------------ //
	// Write to mb_evts/g_clx tree
	// ------------------------------------------------------------------------ //
 	mbevts* write_mb_evts = new mbevts();
	mbevts* mb_evts[GAMMA_ARRAY];
	for( unsigned int i = 0; i < GAMMA_ARRAY; i++ ) {
	
		mb_evts[i] = new mbevts();
		mb_evts[i]->Initialize();

	}
	
	TTree* g_clx = new TTree( "g_clx", "g_clx" );
	g_clx->Branch( "mbevts", "mbevts", &write_mb_evts );
	// ------------------------------------------------------------------------ //
 
  
	// ------------------------------------------------------------------------ //
	// Hard-coded parameters... TODO read in from file?
	// ------------------------------------------------------------------------ //
	// How many ticks need to align the prompt, in ticks.
	Double_t dtAdc[4] = {-5.1,-10.0,-5.2,-6.1};  // IS547
	//Double_t dtAdc[4] = {-6.2,-11.4,-5.2,-6.8};  // IS562
	//Double_t dtAdc[4] = {-4.9,-10.2,-5.3,-3.8};  // IS546
	//Double_t dtAdc[4] = {-13.0,-11.5,-9.0,-11.0};  // IS558
	//Double_t dtAdc[4] = {14.0,15.5,16.5,8.0};  // IS558
	//Double_t dtAdc[4] = {14.0,15.5,16.5,8.0};  // IS553
	//Double_t dtAdc[4] = {35.0,38.0,40.0,30.0};  // IS553 - timestamp corrected
  
	// IS553
	Double_t tMinPrompt = -12., tMaxPrompt = 6.;
	Double_t tMinRandom = 12., tMaxRandom = 30.;
	Double_t tMinDelayed = -50., tMaxDelayed = -50.;
 
	Double_t tMinPromptElectron = -6., tMaxPromptElectron = 6.;
	Double_t tMinRandomElectron = 18., tMaxRandomElectron = 30.;
 
	Double_t WeightPR = abs(tMinPrompt-tMaxPrompt)/abs(tMinRandom-tMaxRandom);
	
	if( verbose ) cout << "WeightPR: " << WeightPR << endl;

	Double_t Threshold_CDRing_E[4] = {120.,120.,120.,120.};
	Double_t Threshold_CDStrip_E[4] = {120.,120.,160.,130.};
	// ------------------------------------------------------------------------ //
 


	// ------------------------------------------------------------------------ //
	// Variables
	// ------------------------------------------------------------------------ //
	unsigned int i,j,k,l;
	float GammaEnergy = 0.;
	float GammaEnergy2 = 0.;
	float ElectronEnergy = 0.;
	float PartEnergy = 0.;
	float icE_Energy = 0.;
	float icDE_Energy = 0.;
	float PadEnergy[4] = {0.,0.,0.,0.};
	unsigned int GammaCtr = 0;
	unsigned int PartCtr = 0;
	float tdiffPG = 0.;
	float MaxSegEnergy = -99.;
	int MaxSegClu = -1;
	int MaxSegCore = -1;
	int MaxSegId = -1;
	int coinc_flag;
	int dgf_num = 0;
	int dgf_num2 = 0;
	int dgf_ch = 0;
	int dgf_ch2 = 0;
	int dgf_en = 0;
	int dgf_en2 = 0;
	long long dgf_t = 0;
	long long dgf_t2 = 0;
	unsigned int adc_num = 0;
	unsigned int adc_ch = 0;
	unsigned int adc_en = 0;
	unsigned int adc_ch2 = 0;
	unsigned int adc_en2 = 0;
	long long adc_t =0;

	vector<float> cd_ringenergy[4];
	vector<float> cd_stripenergy[4];
	vector<unsigned short> cd_ringid[4];
	vector<unsigned short> cd_stripid[4];
	vector<bool> cd_ringused[4];
	vector<bool> cd_stripused[4];

	vector<float> gen_array;
	vector<long long> gtd_array;
	vector<unsigned short> clu_array;
	vector<unsigned short> cid_array;
	vector<unsigned short> sid_array;
	vector<float> sen_array;
	bool ab_evt = false;
	unsigned short ab_mul = 0;

	vector<unsigned int> Quad; // 0: Top, 1: Bottom, 2: Left and 3; Right
	vector<unsigned int> Elem_fired; // 0: FCD, 1: Barrel, 2: BCD and 3: Pad
	vector<unsigned int> Chan_front; // Rings for CDs, Total Energy for Pad or Strips for Barrel
	vector<float> Ener_front;
	vector<unsigned int> Chan_back; // Strips for CDs and Total Energy for Barrel
	vector<float> Ener_back;
	vector<long long> time;
	vector<bool> laser;
  
	int StripNum = 0;
	int RingNum = 0;
	float StripEnergy = 0;
	float RingEnergy = 0;
	float StripEnergyDiff = 0;
	float RingEnergyDiff = 0;
	bool RingMult = false;
	float tempDiff = 0;
	
	// Check Code
	int CounterAdcCDFired[5] = {0,0,0,0,0};
 	// ------------------------------------------------------------------------ //

  
	// ------------------------------------------------------------------------ //
	// Histograms 
	// ------------------------------------------------------------------------ //
	TH1::SetDefaultSumw2(); // Histogram errors
  
	// diagnostics
	TH1F* adc = new TH1F("adc","adc",80,-0.5,79.5);
	adc->GetXaxis()->SetTitle("Number of ADC's");
	TH1F* dgf = new TH1F("dgf","dgf",56,-0.5,55.5);
	dgf->GetXaxis()->SetTitle("Number of DGF's");
	TH1F* free_dgf = new TH1F("free_dgf","free_dgf",56,-0.5,55.5);
	free_dgf->GetXaxis()->SetTitle("Number of free DGF's");
	TH1F* hABmult = new TH1F("hABmult","hABmult",4,-0.5,3.5);
	hABmult->GetXaxis()->SetTitle("Addback multiplicity");

	// time differences
	TH1F* tdiff_gp = new TH1F("tdiff_gp","tdiff_gp",201,-100.5,100.5);
	tdiff_gp->GetXaxis()->SetTitle("time diff (particle - gamma) [us]");
	TH1F* tdiff_ep = new TH1F("tdiff_ep","tdiff_ep",201,-100.5,100.5);
	tdiff_ep->GetXaxis()->SetTitle("time diff (particle - electron) [us]");
	TH1F* tdiff_gg = new TH1F("tdiff_gg","tdiff_gg",201,-100.5,100.5);
	tdiff_gg->GetXaxis()->SetTitle("time diff between one Dgf and the others [us]");
	TH1F *tdiff_gp_q[4]; 
	for( i = 0; i < 4; i++ ) 
		tdiff_gp_q[i] = new TH1F(Form("tdiff_gp_%d",i),Form("tdiff_gp_%d",i),201,-100.5,100.5);

	// Total gamma - no Doppler correction
	TH1F* E_gam = new TH1F("E_gam","E_gam",GBINS,GMIN,GMAX);
	E_gam->GetXaxis()->SetTitle("Energy Gamma Rays [Channels]");

	// Gamma spectra for every segment - no Doppler correction
	TH1F* E_gam_seg[8][3][7];
	TH1F* E_gam_seg_cal[8][3][7];
	TDirectory *gam_dir = outfile->mkdir("E_gam_seg");
	gam_dir->cd();
	for( i = 0; i < 8; i++ ) {

		for( j = 0; j < 3; j++ ) {

			for( k = 0; k < 7; k++ ) {

				E_gam_seg[i][j][k] = new TH1F(Form("E_gam_%d_%d_%d",i,j,k),Form("E_gam_%d_%d_%d",i,j,k),16384,-0.5,65535.5);
				E_gam_seg[i][j][k]->GetXaxis()->SetTitle("Energy Gamma Rays [Channels]");
				E_gam_seg_cal[i][j][k] = new TH1F(Form("E_gam_%d_%d_%d_cal",i,j,k),Form("E_gam_%d_%d_%d_cal",i,j,k),GBINS,GMIN,GMAX);
				E_gam_seg_cal[i][j][k]->GetXaxis()->SetTitle("Energy Gamma Rays [keV]");

			}

		}

	}
	
	gDirectory->cd("/");
	
	// Particle spectra for every segment
	TH1F* E_part_ann[4][16];
	TH1F* E_part_ann_cal[4][16];
	TH1F* E_part_sec[4][12];
	TH1F* E_part_sec_cal[4][12];
	TDirectory *part_dir = outfile->mkdir("E_part");
	part_dir->cd();
	for( i = 0; i < 4; i++ ) {

		for( j = 0; j < 16; j++ ) {

			E_part_ann[i][j] = new TH1F(Form("E_part_ann_%d_%d",i,j),Form("E_part_ann_%d_%d",i,j),4096,-0.5,4095.5);
			E_part_ann[i][j]->GetXaxis()->SetTitle("Energy Particles [Channels]");
			E_part_ann_cal[i][j] = new TH1F(Form("E_part_ann_%d_%d_cal",i,j),Form("E_part_ann_%d_%d_cal",i,j),PBINS,PMIN,PMAX);
			E_part_ann_cal[i][j]->GetXaxis()->SetTitle("Energy Particles [MeV]");

		}

		for( j = 0; j < 12; j++ ) {

			E_part_sec[i][j] = new TH1F(Form("E_part_sec_%d_%d",i,j),Form("E_part_sec_%d_%d",i,j),4096,-0.5,4095.);
			E_part_sec[i][j]->GetXaxis()->SetTitle("Energy Particles [Channels]");
			E_part_sec_cal[i][j] = new TH1F(Form("E_part_sec_%d_%d_cal",i,j),Form("E_part_sec_%d_%d_cal",i,j),PBINS,PMIN,PMAX);
			E_part_sec_cal[i][j]->GetXaxis()->SetTitle("Energy Particles [MeV]");

		}

	}
	
	gDirectory->cd("/");
	
	TH1F* E_spede = NULL;
	TH1F* E_spede_seg[24];
	TH1F* E_spede_seg_cal[24];
	if( spede ) {
		
		// Total electron - no Doppler correction
		E_spede = new TH1F("E_spede","E_spede",ELBINS,ELMIN,ELMAX);
		E_spede->GetXaxis()->SetTitle("Energy Electrons [Channels]");
		
		// Electron spectra for every segment - no Doppler correction
		TDirectory *spede_dir = outfile->mkdir("E_spede_seg");
		spede_dir->cd();
		for( i = 0; i < 24; i++ ) {
			
			E_spede_seg[i] = new TH1F(Form("E_spede_%d",i),Form("E_spede_%d",i),16384,-0.5,65535.5);
			E_spede_seg[i]->GetXaxis()->SetTitle("Energy Electrons [Channels]");
			E_spede_seg_cal[i] = new TH1F(Form("E_spede_%d_cal",i),Form("E_spede_%d_cal",i),ELBINS,ELMIN,ELMAX);
			E_spede_seg_cal[i]->GetXaxis()->SetTitle("Energy Electrons [keV]");
			
		}
		
		gDirectory->cd("/");
		
	}

	TH1F* E_BeamDump = new TH1F("E_BeamDump","E_BeamDump",GBINS,GMIN,GMAX);
	E_BeamDump->GetXaxis()->SetTitle("Energy of the Beam Dump [keV]");
	TH1F* T_BeamDump = new TH1F("T_BeamDump","T_BeamDump",7200,0,7200);	
	T_BeamDump->GetXaxis()->SetTitle("Time of the Beam Dump [s]");

	// particle-spectra
	TH2F *CD_front_energy[4], *CD_front_energy_cal[4];
	TH2F *CD_back_energy[4], *CD_back_energy_cal[4];
	TH2F *CD_front_back[4];
	
	for( i = 0; i < 4; i++ ) {
			
		CD_front_energy[i] = new TH2F(Form("CD_front_energy_%d",i),Form("CD_front_energy_%d",i),16,-0.5,15.5,4096,0,4096);
		CD_front_energy[i]->GetXaxis()->SetTitle("Ring number");
		CD_front_energy[i]->GetYaxis()->SetTitle("Energy particle [adc ch.]");
		CD_back_energy[i] = new TH2F(Form("CD_back_energy_%d",i),Form("CD_back_energy_%d",i),12,-0.5,11.5,4096,0,4096);
		CD_back_energy[i]->GetXaxis()->SetTitle("Strip number");
		CD_back_energy[i]->GetYaxis()->SetTitle("Energy particle [adc ch.]");
			
		CD_front_energy_cal[i] = new TH2F(Form("CD_front_energy_cal_%d",i),Form("CD_front_energy_cal_%d",i),16,-0.5,15.5,PBINS,PMIN,PMAX);
		CD_front_energy_cal[i]->GetXaxis()->SetTitle("Ring number");
		CD_front_energy_cal[i]->GetYaxis()->SetTitle("Energy particle [MeV]");
		CD_back_energy_cal[i] = new TH2F(Form("CD_back_energy_cal_%d",i),Form("CD_back_energy_cal_%d",i),12,-0.5,11.5,PBINS,PMIN,PMAX);
		CD_back_energy_cal[i]->GetXaxis()->SetTitle("Strip number");
		CD_back_energy_cal[i]->GetYaxis()->SetTitle("Energy particle [MeV]");
		
		CD_front_back[i] = new TH2F(Form("CD_front_back_%d",i),Form("CD_front_back_%d",i),PBINS,PMIN,PMAX,PBINS,PMIN,PMAX);
		CD_front_back[i]->GetXaxis()->SetTitle("Front Energy particle [MeV]");
		CD_front_back[i]->GetYaxis()->SetTitle("Back Energy particle [MeV]");
		
	}

	// Ionisation chamber
	TH2F* dEE = NULL;
	if( ionch ) dEE = new TH2F( "dEE", "ionisation chamber;E_{rest};delta-E", 4096, -0.5, 4095.5, 4096, -0.5, 4095.5 );

	// CD-pad detector, freely triggered
	TH1F *padE_sum = NULL, *padE[4], *padE_cal[4];
	TH2F *cd_dEE_sum, *cd_dEE_anti, *cd_dEE[4];
	if( cdpad ) {
	
		padE_sum = new TH1F( "padE_sum", "Energy on the PAD detector;Energy [keV];", 4096, -0.5, 4095.5 );
		cd_dEE_sum = new TH2F( "cd_dEE_sum", "dE-E of the CD-PAD;Energy [keV];Energy [keV];", 4096, -2., 16382., 4096, -2., 16382. );
		cd_dEE_anti = new TH2F( "cd_dEE_anti", "dE-E of the CD-PAD not in coincidence with same quadrant;Energy [keV];Energy [keV];", 4096, -2., 16382., 4096, -2., 16382. );

		for( i = 0; i < 4; i++ ) {

			padE[i] = new TH1F( Form("padE_%d",i), "Energy on the PAD detector;Energy [adc ch.];", 4096, -0.5, 4095.5 );
			padE_cal[i] = new TH1F( Form("padE%d_cal",i), "Energy on the PAD detector;Energy [keV];", ELBINS, ELMIN, ELMAX );
			cd_dEE[i] = new TH2F( Form("cd_dEE%d",i), "dE-E of the CD-PAD;Energy [keV];Energy [keV];", 4096, -2., 16382., 4096, -2., 16382. );

		}
		
	}
	// ------------------------------------------------------------------------ //


	// ------------------------------------------------------------------------ //
	// Start loop over number of entries
	// ------------------------------------------------------------------------ //
	Double_t nentries = tr->GetEntries();
	int nbytes = 0;
	int status;
	vector<int> DgfModule;
	
	// Test - comment out for runs
	//nentries = 10000;
	
	// Do loop	
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

		// ------------------------------------------------------------------------ //
		// Sort out mb_evts
		// ------------------------------------------------------------------------ //
		for( j = 0; j < GammaCtr; j++ ) {
		
			mb_evts[j]->SearchCoin();  
			write_mb_evts->Initialize();
			write_mb_evts->CopyData( mb_evts[j] );
			if( write_mb_evts->pr_hits > 0 || write_mb_evts->rndm_hits > 0 || write_mb_evts->del_hits > 0  ) g_clx->Fill();
			else if( gamgam && write_mb_evts->gcor_gen.size() > 1 ) g_clx->Fill();
			else if( singles ) g_clx->Fill();
			
		}
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Reset
		// ------------------------------------------------------------------------ //
		for( j = 0; j < GAMMA_ARRAY; j++ ) {
		
			mb_evts[j]->Initialize();
		
		}
		GammaCtr = 0;

		DgfModule.clear();

		for( j = 0; j < event->NumberOfDgfs(); j++ ) {

			for( k = 0; k < DgfModule.size(); k++ ) 

				if( event->Dgf(j)->ModuleNumber() == DgfModule[k] ) break;
			
			if( k == DgfModule.size() ) DgfModule.push_back( event->Dgf(j)->ModuleNumber() );

		}
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Clean particles and gammas/electrons
		// ------------------------------------------------------------------------ //
		for( j = 0; j < 4; j++ ) {

			cd_ringenergy[j].clear();
			cd_stripenergy[j].clear();
			cd_ringid[j].clear();
			cd_stripid[j].clear();
			cd_ringused[j].clear();
			cd_stripused[j].clear();

		}
	
		Quad.clear();
		Elem_fired.clear();
		Chan_front.clear();
		Ener_front.clear();
		Chan_back.clear();
		Ener_back.clear();
		time.clear();
		laser.clear();

		gen_array.clear();
		gtd_array.clear();
		clu_array.clear();
		cid_array.clear();
		sid_array.clear();
		sen_array.clear();
	  
		adc->Fill( event->NumberOfAdcs() );
		dgf->Fill( DgfModule.size() );

		if( event->NumberOfAdcs() == 0 ) {
	
			free_dgf->Fill( DgfModule.size() );

		}
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Only DGF events
		// ------------------------------------------------------------------------ //
		for( j = 0; j < event->NumberOfDgfs(); j++ ) {

			ab_evt = false;
			ab_mul = 0;
			dgf_num = event->Dgf(j)->ModuleNumber();
			dgf_ch  = event->Dgf(j)->Channel();
			dgf_en  = event->Dgf(j)->Energy();
			dgf_t   = event->Dgf(j)->Time();
				
			if( 0 <= dgf_num && dgf_num < 48 && 0 <= dgf_ch && dgf_ch < 4 ) { // miniball

				GammaEnergy = Cal->DgfEnergy( dgf_num, dgf_ch, dgf_en );

				if( dgf_num % 2 == 0 && dgf_ch < 3 ) { // cores plus seg1 and seg2

					E_gam_seg[dgf_num/6][dgf_num%6/2][dgf_ch]->Fill( dgf_en );
					E_gam_seg_cal[dgf_num/6][dgf_num%6/2][dgf_ch]->Fill( GammaEnergy );
					if( dgf_ch == 0 ) E_gam->Fill( GammaEnergy );

				} // even DGF number

				else {

					E_gam_seg[dgf_num/6][dgf_num%6/2][dgf_ch+3]->Fill( dgf_en );
					E_gam_seg_cal[dgf_num/6][dgf_num%6/2][dgf_ch+3]->Fill( GammaEnergy );

				} // odd DGF number

			  	// Look for a core event
				if( dgf_ch == 0 && dgf_num % 2 == 0 ) {
			
					MaxSegClu = dgf_num / 6;
					MaxSegCore = dgf_num % 6 / 2;
					MaxSegId = 0; // initialise as core (if no segment hit (dead), use core!)						

					// Check for highest energy segment in same detector
					for( k = 0; k < event->NumberOfDgfs(); k++ ) {
			
						dgf_num2 = event->Dgf(k)->ModuleNumber();
						dgf_ch2  = event->Dgf(k)->Channel();
						dgf_en2  = event->Dgf(k)->Energy();
						dgf_t2   = event->Dgf(k)->Time();
					
						// We don't care if it's the same event
						if( k == j ) continue;

						// time difference between cores
						if( dgf_ch2 == 0 && dgf_num2 % 2 == 0 )
							tdiff_gg->Fill( dgf_t2 - dgf_t );

						// Skip if a different detector
						if( dgf_num2 != dgf_num && dgf_num2 != dgf_num + 1 ) continue;
					
						GammaEnergy2 = Cal->DgfEnergy( dgf_num2, dgf_ch2, dgf_en2 );

						// Test maximum energy segment
						if( GammaEnergy2 < MaxSegEnergy ) continue;
					
						// Reassign energy and id
						MaxSegEnergy = GammaEnergy2;
						if( dgf_num2 % 2 == 0 ) MaxSegId = dgf_ch2;
						else MaxSegId = dgf_ch2+3;
			
					} // k

					// Found highest energy segment

					// Check previous gammas
					for( l = 0; l < gen_array.size(); l++ ) {

						if( gen_array[l] < GammaEnergy+0.1 && gen_array[l] > GammaEnergy-0.1 )
							continue; // same event

						// Do the addback if same cluster
						if( clu_array[l] == MaxSegClu && addback ) {

							gen_array[l] += GammaEnergy;
							ab_evt = true;
							ab_mul++;

							if( sen_array[l] < MaxSegEnergy ) {

								gtd_array[l] = dgf_t;
								clu_array[l] = MaxSegClu;
								cid_array[l] = MaxSegClu*3 + MaxSegCore;
								sid_array[l] = MaxSegId;
								sen_array[l] = MaxSegEnergy;	

							}

						} // addback

					} // previous gammas

					if( ab_evt ) continue; // get next gamma
					hABmult->Fill( ab_mul );

					gen_array.push_back( GammaEnergy );
					gtd_array.push_back( dgf_t );
					clu_array.push_back( MaxSegClu );
					cid_array.push_back( MaxSegClu*3 + MaxSegCore );
					sid_array.push_back( MaxSegId );
					sen_array.push_back( MaxSegEnergy );

				} // core event

				// Reset
				MaxSegEnergy = -99.;
				MaxSegClu = -1;
				MaxSegCore = -1;
				MaxSegId = -1;
					
			}

			else if( dgf_num == 53 && dgf_ch == 0 ) {

				GammaEnergy = Cal->DgfEnergy( dgf_num, dgf_ch, dgf_en );
				E_BeamDump->Fill(Cal->DgfEnergy(53,0,dgf_en));
				T_BeamDump->Fill((dgf_t)/40000000);

			}

		}
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Only Particles (and SPEDE if it is in ADC5)
		// ------------------------------------------------------------------------ //
		for( j = 0; j < event->NumberOfAdcs(); j++ ) {

			adc_num = event->Adc(j)->ModuleNumber();
			adc_t = event->Adc(j)->Time();
			RingMult = true;

			for( k = 0; k < event->Adc(j)->SubEvent()->Size(); k++ ) {

				adc_ch = event->Adc(j)->SubEvent()->AdcChannel(k);
				adc_en = event->Adc(j)->SubEvent()->AdcValue(k);

				if( adc_num < 4 ) { // CD

					if( adc_ch < 16 ) { // front rings
						
						PartEnergy = Cal->AdcEnergy( adc_num, adc_ch, adc_en );

						E_part_ann[adc_num][adc_ch]->Fill( adc_en );
						E_part_ann_cal[adc_num][adc_ch]->Fill( PartEnergy/1000. );						

						CD_front_energy[adc_num]->Fill( adc_ch, adc_en );
						CD_front_energy_cal[adc_num]->Fill( adc_ch, PartEnergy/1000. );

						if( adc_en > Threshold_CDRing_E[adc_num] ) { // threshold

							cd_ringenergy[adc_num].push_back( PartEnergy );
							cd_ringid[adc_num].push_back( adc_ch );
							cd_ringused[adc_num].push_back( false );

						} // threshold

					} // front rings
					
					else if( adc_ch < 28 ) { // back strips

						PartEnergy = Cal->AdcEnergy( adc_num, adc_ch, adc_en );

						E_part_sec[adc_num][adc_ch-16]->Fill( adc_en );
						E_part_sec_cal[adc_num][adc_ch-16]->Fill( PartEnergy/1000. );
						
						CD_back_energy[adc_num]->Fill( adc_ch-16, adc_en );
						CD_back_energy_cal[adc_num]->Fill( adc_ch-16, PartEnergy/1000. );

						if( adc_en > Threshold_CDStrip_E[adc_num] ) { // threshold

							cd_stripenergy[adc_num].push_back( PartEnergy );
							cd_stripid[adc_num].push_back( adc_ch-16 );
							cd_stripused[adc_num].push_back( false );

						} // threshold

					} // back strips

					else if( adc_ch == 31 && cdpad ) { // something in the pad!

						PadEnergy[adc_num] = Cal->AdcEnergy( adc_num, adc_ch, adc_en );
					
						padE[adc_num]->Fill( adc_en );
						padE_cal[adc_num]->Fill( PadEnergy[adc_num] );
						padE_sum->Fill( PadEnergy[adc_num] );

						gen_array.push_back( PadEnergy[adc_num] );
						gtd_array.push_back( adc_t + dtAdc[adc_num] );
						clu_array.push_back( 8 );
						cid_array.push_back( adc_num+1 );
						sid_array.push_back( 0 );
						sen_array.push_back( 0 );

					} // pad
					
				} // particle in CD
			
				else if( adc_num == 4 && spede ) { // SPEDE
					
					ElectronEnergy = Cal->AdcEnergy( 4, adc_ch, adc_en );
					
					// STM-16 one
					if( adc_ch < 12 ) {
						
						E_spede_seg[adc_ch]->Fill( adc_en );
						E_spede_seg_cal[adc_ch]->Fill( ElectronEnergy );
						E_spede->Fill( ElectronEnergy );
					
						gen_array.push_back( ElectronEnergy );
						gtd_array.push_back( adc_t );
						clu_array.push_back( 8 );
						cid_array.push_back( 0 );
						sid_array.push_back( adc_ch );
						sen_array.push_back( 0 );

					}
					
					// STM-16 two
					else if( adc_ch > 15 && adc_ch < 28 ) {
						
						E_spede_seg[adc_ch-4]->Fill( adc_en );
						E_spede_seg_cal[adc_ch-4]->Fill( ElectronEnergy );
						E_spede->Fill( ElectronEnergy );
						
						gen_array.push_back( ElectronEnergy );
						gtd_array.push_back( adc_t );
						clu_array.push_back( 8 );
						cid_array.push_back( 0 );
						sid_array.push_back( adc_ch-4 );
						sen_array.push_back( 0 );

					}
					
				} // SPEDE
				
				else if( adc_num == 4 && ionch && adc_ch == 0 ) { // ionisation chamber (gas trigger)
				
					icDE_Energy = Cal->AdcEnergy( 4, adc_ch, adc_en );
					
					for( l = 0; l < event->Adc(j)->SubEvent()->Size(); l++ ){

						adc_ch2 = event->Adc(j)->SubEvent()->AdcChannel(l);
						adc_en2 = event->Adc(j)->SubEvent()->AdcValue(l);

						if( adc_ch2 != 1 ) continue;

						icE_Energy = Cal->AdcEnergy( 4, adc_ch2, adc_en2 );

						dEE->Fill( icE_Energy, icDE_Energy );

					}

				} // ionisation chamber

			} // k

			// if we did have SPEDE (or ion. chamber), don't carry on with particle stuff
			if( adc_num == 4 ) continue;

			// ------------------------------------------- //
			// Particle reconstruction                     //
			// ------------------------------------------- //

			// Easy case, 1 front and 1 back!
			if( cd_ringenergy[adc_num].size() == 1 && cd_stripenergy[adc_num].size() == 1 ) {

				Quad.push_back( adc_num );
				Elem_fired.push_back( 0 );
				time.push_back( adc_t + dtAdc[adc_num] );
				laser.push_back( event->Adc(j)->LaserOn() );

				Chan_front.push_back( cd_ringid[adc_num][0] );
				Ener_front.push_back( cd_ringenergy[adc_num][0] );

				Chan_back.push_back( cd_stripid[adc_num][0] );
				Ener_back.push_back( cd_stripenergy[adc_num][0] );

				CD_front_back[adc_num]->Fill( cd_ringenergy[adc_num][0]/1000., cd_stripenergy[adc_num][0]/1000. );

				CounterAdcCDFired[adc_num]++;

			} // 1 vs. 1
			
			// 1 on the front and multiple on the back
			else if( cd_ringenergy[adc_num].size() == 1 && cd_stripenergy[adc_num].size() > 1 ) {

				Quad.push_back( adc_num );
				Elem_fired.push_back( 0 );
				time.push_back( adc_t + dtAdc[adc_num] );
				laser.push_back( event->Adc(j)->LaserOn() );

				Chan_front.push_back( cd_ringid[adc_num][0] );
				Ener_front.push_back( cd_ringenergy[adc_num][0] );

				StripNum = cd_stripid[adc_num][0];
				StripEnergy = cd_stripenergy[adc_num][0];
				StripEnergyDiff = StripEnergy - cd_ringenergy[adc_num][0];
				StripEnergyDiff = TMath::Abs( StripEnergyDiff );

				// Get closest match between front and back
				for( k = 0; k < cd_stripenergy[adc_num].size(); k++ ) {

					tempDiff = cd_stripenergy[adc_num][k] - cd_ringenergy[adc_num][0];
					tempDiff = TMath::Abs( tempDiff );

					if( tempDiff < StripEnergyDiff ) {

						StripNum = cd_stripid[adc_num][k];
						StripEnergy = cd_stripenergy[adc_num][k];
						StripEnergyDiff = tempDiff;

					}

					CD_front_back[adc_num]->Fill(
						cd_ringenergy[adc_num][0]/1000., cd_stripenergy[adc_num][k]/1000. );

				} // k

				// Addback neighbouring strips
				for( k = 0; k < cd_stripenergy[adc_num].size(); k++ ) {

					if( TMath::Abs( StripNum - cd_stripid[adc_num][k] ) == 1 )

						StripEnergy += cd_stripenergy[adc_num][k];

				} // k

				Chan_back.push_back( StripNum );
				Ener_back.push_back( StripEnergy );

				CounterAdcCDFired[adc_num]++;

			} // 1 vs. N

			// multiple on the front and 1 on the back
			else if( cd_ringenergy[adc_num].size() > 1 && cd_stripenergy[adc_num].size() == 1 ) {

				Quad.push_back( adc_num );
				Elem_fired.push_back( 0 );
				time.push_back( adc_t + dtAdc[adc_num] );
				laser.push_back( event->Adc(j)->LaserOn() );

				Chan_back.push_back( cd_stripid[adc_num][0] );
				Ener_back.push_back( cd_stripenergy[adc_num][0] );

				RingNum = cd_ringid[adc_num][0];
				RingEnergy = cd_ringenergy[adc_num][0];
				RingEnergyDiff = RingEnergy - cd_stripenergy[adc_num][0];
				RingEnergyDiff = TMath::Abs( RingEnergyDiff );

				// Get closest match between front and back
				for( k = 0; k < cd_ringenergy[adc_num].size(); k++ ) {

					tempDiff = cd_ringenergy[adc_num][k] - cd_stripenergy[adc_num][0];
					tempDiff = TMath::Abs( tempDiff );

					if( tempDiff < RingEnergyDiff ) {

						RingNum = cd_ringid[adc_num][k];
						RingEnergy = cd_ringenergy[adc_num][k];
						RingEnergyDiff = tempDiff;

					}

					CD_front_back[adc_num]->Fill(
						cd_ringenergy[adc_num][k]/1000., cd_stripenergy[adc_num][0]/1000. );

				} // k

				// Addback neighbouring rings
				for( k = 0; k < cd_ringenergy[adc_num].size(); k++ ) {

					if( TMath::Abs( RingNum - cd_ringid[adc_num][k] ) == 1 )

						RingEnergy += cd_ringenergy[adc_num][k];

				} // k

				Chan_front.push_back( RingNum );
				Ener_front.push_back( RingEnergy );

				CounterAdcCDFired[adc_num]++;

			} // N vs. 1

			// multiple on the front and multiple on the back
			else if( cd_ringenergy[adc_num].size() > 1 && cd_stripenergy[adc_num].size() > 1 ) {

				// Loop for multiplicity where separated by >1 rings
				while( RingMult ) {

					RingMult = false;
					Quad.push_back( adc_num );
					Elem_fired.push_back( 0 );
					time.push_back( adc_t + dtAdc[adc_num] );
					laser.push_back( event->Adc(j)->LaserOn() );

					// Find highest energy on the front first
					RingEnergy = 0;
					for( k = 0; k < cd_ringenergy[adc_num].size(); k++ ) {

						// Check highest energy that we've not already used
						if( cd_ringenergy[adc_num][k] > RingEnergy && !cd_ringused[adc_num][k] ) {

							RingNum = cd_ringid[adc_num][k];
							RingEnergy = cd_ringenergy[adc_num][k];

						}

					}

					// Addback rings and "use" the highest energy strip
					for( k = 0; k < cd_ringenergy[adc_num].size(); k++ ) {

						if( cd_ringused[adc_num][k] == true ) continue;

						else if( TMath::Abs( RingNum - cd_ringid[adc_num][k] ) == 1 ) {

							RingEnergy += cd_ringenergy[adc_num][k];
							cd_ringused[adc_num][k] = true;

						}

						else if( RingNum == cd_ringid[adc_num][k] )

							cd_ringused[adc_num][k] = true;

						else if( TMath::Abs( RingNum - cd_ringid[adc_num][k] ) > 1 )

							RingMult = true;

					}

					Chan_front.push_back( RingNum );
					Ener_front.push_back( RingEnergy );

					CounterAdcCDFired[adc_num]++;

				} // while RingMult

				// Get closest match between front and back
				StripEnergyDiff = 99999999.;
				for( l = 0; l < Ener_front.size(); l++ ) {

					if( Quad[l] != adc_num ) continue;

					for( k = 0; k < cd_stripenergy[adc_num].size(); k++ ) {

						tempDiff = cd_stripenergy[adc_num][k] - Ener_front[l];
						tempDiff = TMath::Abs( tempDiff );

						if( tempDiff < StripEnergyDiff && !cd_stripused[adc_num][k] ) {

							StripNum = cd_stripid[adc_num][k];
							StripEnergy = cd_stripenergy[adc_num][k];
							StripEnergyDiff = tempDiff;

						}

					} // k

					Chan_back.push_back( StripNum );
					Ener_back.push_back( StripEnergy );

					for( k = 0; k < cd_stripenergy[adc_num].size(); k++ ) {

						if( cd_stripid[adc_num][k] == StripNum )

							cd_stripused[adc_num][k] = true;

					} // k

				} // l

			} // N vs. M
			// --------------------------------------- //

		
		} // j - numberofadcs
		// ------------------------------------------------------------------------ //


		// ------------------------------------------------------------------------ //
		// Gamma/electron-particle coincidences
		// ------------------------------------------------------------------------ //
		for( j = 0; j < gen_array.size(); j++ ) {

			mb_evts[GammaCtr]->SetGen( gen_array[j] );
			mb_evts[GammaCtr]->SetCluid( clu_array[j] );
			mb_evts[GammaCtr]->SetCid( cid_array[j] );
			mb_evts[GammaCtr]->SetSid( sid_array[j] );
			mb_evts[GammaCtr]->SetTheta( 0. );
			mb_evts[GammaCtr]->SetPhi( 0. );
				
			// Do particles
			for( k = 0; k < Ener_front.size(); k++ ) {

				tdiffPG = time[k] - gtd_array[j];

				// electrons
				if( clu_array[j] == 8 ) { 

					tdiff_ep->Fill( tdiffPG );

					if( tMinRandomElectron <= tdiffPG && tdiffPG < tMaxRandomElectron ) coinc_flag = 1;
					else if( tMinPromptElectron <= tdiffPG && tdiffPG < tMaxPromptElectron ) coinc_flag = 0;
					else coinc_flag = -1;
						
				}

				// gamma-rays
				else {

					tdiff_gp->Fill( tdiffPG );
					tdiff_gp_q[(int)Quad[k]]->Fill( tdiffPG - dtAdc[(int)Quad[k]] );

					if( tMinDelayed <= tdiffPG && tdiffPG < tMaxDelayed ) coinc_flag = 2;
					else if( tMinRandom <= tdiffPG && tdiffPG < tMaxRandom ) coinc_flag = 1;
					else if( tMinPrompt <= tdiffPG && tdiffPG < tMaxPrompt ) coinc_flag = 0;
					else coinc_flag = -1;			

				}
				
				// Deal with the front CD only with mb_evts (for now)
				if( Elem_fired[k] != 0 ) continue;

				// Add particle
				mb_evts[GammaCtr]->SetPart( Ener_front[k], (int)Chan_front[k], (int)Chan_back[k],
					(double)time[k], (double)event->SuperCycleTime()/1000000, (float)tdiffPG,
					(int)coinc_flag, Quad[k], (int)laser[k] );
					
				PartCtr++;

			} // End loop All the particles

			// Look for correlated gammas or electrons
			for( l = 0; l < gen_array.size(); l++ ) {
				
				// skip if it's the same event as before
				if( l == j ) continue;
					
				// Found highest energy segment
				mb_evts[GammaCtr]->SetCorGamGen( gen_array[l] );
				mb_evts[GammaCtr]->SetCorGamCluid( clu_array[l] );
				mb_evts[GammaCtr]->SetCorGamCid( cid_array[l] );
				mb_evts[GammaCtr]->SetCorGamSid( sid_array[l] );
				mb_evts[GammaCtr]->SetCorGamGtd( gtd_array[l] - gtd_array[j] );
				mb_evts[GammaCtr]->SetCorGamTheta( 0. );
				mb_evts[GammaCtr]->SetCorGamPhi( 0. );
				
			} // End search for correlated gammas
			
			// Count the gamma!
			GammaCtr++;
				
			// Reset
			PartCtr = 0;
			
		} // j
		// ------------------------------------------------------------------------ //
		
		// Progress bar
		if( i % 50000 == 0 ) {

			cout << setw(5) << setiosflags(ios::fixed) << setprecision(1);
			cout << (100.*i)/nentries << "% done\r" << flush;

		}

	} // i

	cout << "Check ADCs Fired:" << endl;
	for( i = 0; i < 5; i++ ) cout << " Adc # " << i << " fired: " << CounterAdcCDFired[i] << endl;

	outfile->Write();
	outfile->Close();
	delete tr;

	return 0;
	
}

