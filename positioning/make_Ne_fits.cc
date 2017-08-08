// Script to fit centroids of 22Ne data in every segment

string convertInt( int number ) {
	
	stringstream ss;
	ss << number;
	return ss.str();
	
}

string convertFloat( float number, int precision ) {
	
	stringstream ss;
	ss << setprecision( precision ) << number;
	return ss.str();
	
}

double gaus_lbg( Double_t *x, Double_t *par ) {

	// par[0]   background constant
	// par[1]   background slope
	// par[2]   gauss width
	// par[3]   gauss0 constant
	// par[4]   gauss0 mean
	
	static Float_t sqrt2pi = TMath::Sqrt(2*TMath::Pi()), sqrt2 = TMath::Sqrt(2.);
	Double_t arg;
	arg = (x[0] - par[4])/(sqrt2*par[2]);
	Double_t fitval = par[0] + x[0]*par[1] + 1.0/(sqrt2pi*par[2]) * par[3] * exp(-arg*arg);
	
	return fitval;
	
}


double fit22NePeak( TH1F *hist, Double_t gLowX, Double_t gUpX ) {
	
	if(gLowX < gUpX) {
	 
		// *** Creating the function of the form 'gaus_lbg' defined above ***
		TF1 *fitfunc = new TF1( "peakfit", gaus_lbg, 0, 1, 5 );
	 
		// *** Obtaining and specifying the start values for the fit ***
		double gContent = hist->Integral( hist->FindBin(gLowX), hist->FindBin(gUpX) );
		hist->GetXaxis()->SetRangeUser( gLowX, gUpX );
		double gMean    = hist->GetXaxis()->GetBinCenter( hist->GetMaximumBin() );
		double gSigma   = 1.9;
		fitfunc->SetParameters( 0, 0, gSigma, gContent, gMean );
		fitfunc->SetRange( gLowX, gUpX );
  
		// *** Fitting: 'R' means within the range specified above ***
		hist->Fit( fitfunc, "RQ" );

		return fitfunc->GetParameter(4);
		
	}
 
	else {
		
		cout << "Couldn't fit! Error: The Lower Limit is larger than the Upper Limit!" << endl;
		return 0;
		
	}
	
}

void make_Ne_fits() {
	
	// Names of dgf channels
	vector<string> dgfnames;
	dgfnames.push_back( "c" );
	dgfnames.push_back( "1" );
	dgfnames.push_back( "2" );
	dgfnames.push_back( "x" );
	dgfnames.push_back( "3" );
	dgfnames.push_back( "4" );
	dgfnames.push_back( "5" );
	dgfnames.push_back( "6" );
	
	// Names of clusters in Miniball nomenclature
	vector<string> MBnames;
	MBnames.push_back( "17" );
	MBnames.push_back( "12" );
	MBnames.push_back( "16" );
	MBnames.push_back( "13" );
	MBnames.push_back( "22" );
	MBnames.push_back( "18" );
	MBnames.push_back( "14" );
	MBnames.push_back( "23" );
	
	// Names of cores in Miniball nomenclature
	vector<string> corenames;
	corenames.push_back( "A" );
	corenames.push_back( "B" );
	corenames.push_back( "C" );
	
	// Suppress some message from root
	gErrorIgnoreLevel = kWarning;
	
	// Outut files
	ofstream resFile_E440, resFile_E1017;
	resFile_E440.open( "analyse_22Ne_E440.dat" );
	resFile_E1017.open( "analyse_22Ne_E1017.dat" );
	
	// Get the histograms and draw them
	TFile *histFile = new TFile( "22Ne_hadd_part_pos5.root" );
	TH1F *hist;
	TCanvas *c = new TCanvas();
	string name;
	string segname;
	int segNo = 0;
	double centroid;
	
	for( int i = 0; i < 8; i++ ){
		
		for( int j = 0; j < 3; j++ ){
			
			for( int k = 0; k < 8; k++ ){
				
				// Get histogram
				name = "hE" + convertInt(i+1);
				if( k < 4 ) name += convertInt(j*2+1);
				else name += convertInt(j*2+2);
				name += dgfnames[k];
				name += "Cal";
				hist = (TH1F*)histFile->Get( name.c_str() );
				
				// skip the core and empty channel
				if( k == 0 || k == 3 ) continue;
				
				// Name of segment
				segNo++;
				segname = MBnames[i] + corenames[j] + convertInt( segNo );
				
				hist->Draw();
				
				centroid = fit22NePeak( hist, 390, 490 );
				resFile_E440 << segname << " " << centroid << endl;
				cout << name << " " << segname << " " << centroid << endl;
				
				//centroid = fit22NePeak( hist, 920, 1120 );
				//resFile_E1017 << segname << " " << centroid << endl;
				
				name = "22Ne_fits/" + name + ".pdf";
				c->SaveAs( name.c_str() );
				
			}
			
			segNo = 0;
			
		}
		
	}
	
	resFile_E440.close();
	resFile_E1017.close();
	
	delete c;
	
	return;
	
}
