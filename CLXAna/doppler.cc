#ifndef doppler_cxx
#define doppler_cxx

#ifndef doppler_hh
# include "doppler.hh"
#endif

TRandom3 doppler::rand = 1;

double doppler::BTELoss_pars[6] = { -0.88, 0.91, -0.064, -27., 4.5, -0.16 };
double doppler::TTELoss_pars[6] = { -0.88, 0.91, -0.064, -27., 4.5, -0.16 };
double doppler::BSELoss_pars[6] = { 3.6, -0.15, 0.010, -37., 6.1, -0.22 };
double doppler::TSELoss_pars[6] = { 3.6, -0.15, 0.010, -37., 6.1, -0.22 };

double doppler::SP_function( double *x, double *par ) { // energy units of keV
	 
	 double SP_nucl = par[0] + par[1] * TMath::Log( x[0] );
	 SP_nucl += par[2] * TMath::Log( x[0] ) * TMath::Log( x[0] );
	 SP_nucl = TMath::Exp( SP_nucl );
	 
	 double SP_elec = par[3] + par[4] * TMath::Log( x[0] );
	 SP_elec += par[5] * TMath::Log( x[0] ) * TMath::Log( x[0] );
	 SP_elec = TMath::Exp( SP_elec );
	 
	 return SP_nucl + SP_elec;
	 
}

void doppler::ExpDefs( int Zb_, int Ab_, int Zt_, int At_, float Eb_, float Ex_, float thick_,
						float depth_, float cddist_, float cdoffset_, float deadlayer_, float spededist_,
						TCutG *Bcut_, TCutG *Tcut_ ) {

	Zb = Zb_;
	Ab = Ab_;
	Zt = Zt_;
	At = At_;
	Eb = Eb_;
	Ex = Ex_;
	thick = thick_;
	depth = depth_;
	cddist = cddist_;
	cdoffset = cdoffset_;
	deadlayer = deadlayer_;
	spededist = spededist_;
	Bcut = Bcut_;
	Tcut = Tcut_;

	return;

}

bool doppler::stoppingpowers( bool BT, bool TT, bool BS, bool TS ) {

	bool success = true;

	if( BT ) success *= stoppingpowers( "BT" );
	if( TT ) success *= stoppingpowers( "TT" );
	if( BS ) success *= stoppingpowers( "BS" );
	if( TS ) success *= stoppingpowers( "TS" );

	return success;

}

bool doppler::stoppingpowers( string opt ) {
	 
	string gElName[110] = {
		"H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg",
		"Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr",
		"Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr",
		"Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd",
		"In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd",
		"Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf",
		"Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po",
		"At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm",
		"Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs",
		"Mt","Ds" };

	string srimfile = "./srim/"; // prefix
	if( opt.substr(0,1) == "B" ) srimfile += convertInt(Ab) + gElName[Zb-1];
	else if( opt.substr(0,1) == "T" ) srimfile += convertInt(At) + gElName[Zt-1];
	else {
		cout << "opt must equal BT, TT, BS or TS \n";
		return false;
	}
	 
	if( opt.substr(1,1) == "T" ) srimfile += "_" + convertInt(At) + gElName[Zt-1] + ".txt";
	else if( opt.substr(1,1) == "S" ) srimfile += "_Si.txt";
	else {
		cout << "opt must equal BT, TT, BS or TS \n";
		return false;
	}
	 
	ifstream infile;
	infile.open( srimfile.c_str(), ios::in );
	 
	if( !infile.is_open() ) {
		  
		cout << "Cannot open " << srimfile << endl;
		return false;
		  
	}
	 
	TGraph *gSP = new TGraph();
	string title = "Stopping powers for ";
	if( opt.substr(0,1) == "B" ) title += convertInt(Ab) + gElName[Zb-1];
	else if( opt.substr(0,1) == "T" )  title += convertInt(At) + gElName[Zt-1];
	if( opt.substr(1,1) == "T" ) title += " in " + convertInt(At) + gElName[Zt-1];
	else if( opt.substr(1,1) == "S" ) title += " in the Si dead layer";
	title += ";Ion energy [keV];Stopping power [MeV/(mg/cm^2)]";
	gSP->SetTitle( title.c_str() );
	 
	double limits[2] = { 8E2, 1E6 };
	TF1 *fSP = new TF1( "fSP", SP_function, limits[0], limits[1], 6 );
	 
	string line, units, tmp_str;
	stringstream line_ss;
	bool endflag = false;
	double BEn, nucl, elec, total, tmp_dbl;
	int p = 0;
	 
	// Test file format
	getline( infile, line );
	if( line.substr( 0, 5 ) == " ====" ) {
		  
		while( line.substr( 0, 5 ) != "  ---" ) // Err, what if you use SRIM-2006, which doesn't have the spaces?
			getline( infile, line );
		  
		getline( infile, line ); // read first line of data
		  
	}
	 
	while( !infile.eof() && !endflag ) {
		  
		// Read in data
		line_ss.str("");
		line_ss << line;
		line_ss >> BEn >> units >> nucl >> elec >> tmp_dbl >> tmp_str >> tmp_dbl >> tmp_str;
		
		if( units == "eV" ) BEn *= 1E-3;
		else if( units == "keV" ) BEn *= 1E0;
		else if( units == "MeV" ) BEn *= 1E3;
		else if( units == "GeV" ) BEn *= 1E6;
		
		total = nucl + elec ; // MeV / ( mg / cm^2 )
		
		gSP->SetPoint( p, BEn, total );
		  
		// Get next line
		getline( infile, line );
		p++;
		  
		// If we've reached the end, stop
		if( line.substr( 0, 9 ) == "---------" ) endflag = true;
		if( line.substr( 0, 9 ) == " Multiply" ) endflag = true;
		  
	}
	 
	if( opt == "BT" ) fSP->SetParameters( BTELoss_pars );
	else if( opt == "TT" ) fSP->SetParameters( TTELoss_pars );
	else if( opt == "BS" ) fSP->SetParameters( BSELoss_pars );
	else if( opt == "TS" ) fSP->SetParameters( TSELoss_pars );
	 
	gSP->Fit( fSP, "QRWM" );
	 
	if( opt == "BT" ) fSP->GetParameters( BTELoss_pars );
	else if( opt == "TT" ) fSP->GetParameters( TTELoss_pars );
	else if( opt == "BS" ) fSP->GetParameters( BSELoss_pars );
	else if( opt == "TS" ) fSP->GetParameters( TSELoss_pars );

	 
	TCanvas *c = new TCanvas();
	gSP->Draw("A*");
	//gSP->GetXaxis()->SetTitleOffset(1.3);
	//gSP->GetYaxis()->SetTitleOffset(1.3);
	//TGaxis::SetMaxDigits(3);
	string pdfname = srimfile.substr( 0, srimfile.find_last_of(".") ) + ".pdf";
	c->SetLogx();
	c->SaveAs( pdfname.c_str() );
	 
	delete c;
	 
	return true;
	 
}

int doppler::Cut( float PEn, float anno, int quad ) {

	// Check if entry passes any particle gates
	// Function returns 1 for projectile or 0 for target
	// -1 is returned if particle is outside of gates

	int identity = -1;
	int str = quad*16 + anno;
	float ang = GetPTh(anno) * TMath::RadToDeg();

	// Graphical cuts given at command line
	if( Bcut->GetN() > 0 && Tcut->GetN() > 0 ) {

		if( Bcut->IsInside( ang, PEn/1000. ) ) identity = 1; 

		else if( Tcut->IsInside( ang, PEn/1000. ) ) identity = 0; 

	}

	// inverse kinematics, include overlap region
	else if( Ab > At ){

		double a = 349.07, b = -4.997, c = -0.0145;
		
		if( Ab < 143 ) a -= 25.;

		if( PEn/1000. <= ( a + b*ang + c*ang*ang ) && anno < 15 )

			identity = 1; // beam
			
		else if( anno < 15 )
		
			identity = 0; // target

	}

	// normal kinematics without Beam/Target separation
	else if( 0 == 1 ) identity = 1; // no gate

	// normal kinematics with Beam/Target separation (change the else above to be false)
	else {
	
		double a = 497.602, b = -4.67677, c = -0.0274333, l=0;
		double d = 435.186, e = -7.84811, f = 0.0199164, k=0;
		double g = 0, h = 0, i = 0, n=0;
		double ang = GetPTh(anno) * TMath::RadToDeg();
		
		if( Ab == 22 ) {

			//need fixing
			a = 57.706; b = 0.0120384; c = -0.00478394;
			//d = 20; e = 0.0120384; f = -0.00478394;
			d = 20; e = 0; f = 0;
		}

		else if( Ab == 140 && Zb == 60 ) {

			a = 576.308; b = 6.98955; c = -0.462751; l = 0.00418589;
			d = -123.133; e = 45.6129; f = -1.28252; k = 0.00979219;
			g = 506.363; h = -6.52989; i = -0.162027; n = 0.00222857;

		}

		else if( Ab == 142 && Zb == 62 ) {

			a	=	786.059;	b	=	-8.20774;	c	=	-0.0760138;	l	=	0.0009756319;
			d	=	22.8353;	e	=	33.2133;	f	=	-0.955229;	k	=	0.0071980504;
			g	=	274.263;	h	=	11.925;		i	=	-0.558269;	n	=	0.0048949597;

			//a = 935.41021; b = -9.7672106; c = -0.090456422; l = 0.001161002;
			//d = 27.174007; e = 39.523827; f = -1.13672251; k = 0.00856568;
			//g = 326.37297; h = 14.19075; i = -0.66434011; n = 0.005825002;

		}

		else if( Ab == 140 ) {

			// Hack to use strip number instead of angle
			ang = anno;
			a = 290.000; b = 8.28043; c = 0.567469;
			d = 127.447; e = 13.3571; f = 0.415562;
			g = 44.9824; h = 2.83287; i = 1.10326; 

		}

		if( PEn/1000. <= ( a + b*ang + c*ang*ang + l*ang*ang*ang ) &&
			PEn/1000. >= ( d + e*ang + f*ang*ang + k*ang*ang*ang ) )

			identity = 1; // beam
			
		else if( PEn/1000. <= ( d + e*ang + f*ang*ang + k*ang*ang*ang ) &&
				 PEn/1000. >= ( g + h*ang + i*ang*ang + n*ang*ang*ang ) )
		
			identity = 0; // target
		
	}

	return identity;

}

int doppler::Cut_2p( float PEn1, float anno1, int quad1, float PEn2, float anno2, int quad2 ) {

	// Check if entry passes the 2 particle condition
	// Return value is 1 if target is p1 or 2 if p2	
	// -1 is returned if condition is not filled
	int identity=-1;

	// inverse kinematics, include overlap region
	if( Ab > At ){

		if( ( Cut(PEn2,anno2,quad2) >0 || ((int)anno2>=10 && PEn2<450000.) ) &&
			( Cut(PEn1,anno1,quad1)==0 || ((int)anno1>=12 && PEn1>280000. && PEn1<600000.) ) ){
			identity=0; // target is particle number 1
		}
		if( ( Cut(PEn1,anno1,quad1) >0 || ((int)anno1>=10 && PEn1<450000.) ) &&
			( Cut(PEn2,anno2,quad2)==0 || ((int)anno2>=12 && PEn2>280000. && PEn2<600000.) ) ){
			identity=1; // target is particle number 2
		}

	}

	// normal kinematics without Beam/Target separation
	else if( 0 == 1 ) {

		if( Cut(PEn2,anno2,quad2) > 0 && Cut(PEn1,anno1,quad1) > 0 ) {

			if( anno1 < 10 && anno2 < 10 ) {

				if( PEn1 < PEn2 ) identity=0; // target is particle number 1
				else identity=1; // target is particle number 2

			}

		}

	}

	// normal kinematics with Beam/Target separation (change the 'else if' above to be false)
	else {

		if( Cut(PEn1,anno1,quad1)==0 && Cut(PEn2,anno2,quad2)==1 )
			identity=0; // target is particle number 1

		else if( Cut(PEn1,anno1,quad1)==1 && Cut(PEn2,anno2,quad2)==0 )
			identity=1; // target is particle number 2
	
	}
	
	return identity;

}

bool doppler::CutG_en2hit( float BEn, float TEn ) {

	// Returns true or false if the 2d graphical cut on beam and target enegry passes
	// Look into the "en2hit" histogram after changing return value of this function

	TCutG *cutg = new TCutG("CUTG",18);
	cutg->SetPoint(0,94.485,426.337);
	cutg->SetPoint(1,120.133,412.173);
	cutg->SetPoint(2,139.99,391.714);
	cutg->SetPoint(3,211.971,317.746);
	cutg->SetPoint(4,256.648,240.631);
	cutg->SetPoint(5,303.808,198.139);
	cutg->SetPoint(6,308.772,144.63);
	cutg->SetPoint(7,272.368,53.3511);
	cutg->SetPoint(8,236.792,42.3347);
	cutg->SetPoint(9,172.257,87.9743);
	cutg->SetPoint(10,131.716,143.057);
	cutg->SetPoint(11,113.514,202.86);
	cutg->SetPoint(12,74.6282,289.418);
	cutg->SetPoint(13,44.0158,325.615);
	cutg->SetPoint(14,36.5695,394.861);
	cutg->SetPoint(15,50.6347,427.911);
	cutg->SetPoint(16,68.8367,432.632);
	cutg->SetPoint(17,94.485,426.337);

	
	// Switch comments around to apply the cut or not
	//return cutg->IsInside( BEn, TEn );
	return true; // no cut applied

}

float doppler::GetCDOffset() {

	// Return offset of the CD in the phi rotation from vertical
	return cdoffset;

}

float doppler::GetSpedeDist() {

	// Return distance of Spede detector
	return spededist;

}

float doppler::GetCDDeadLayer() {

	// Return dead layer of the Si in um
	return deadlayer;

}

int doppler::GetZb() {

	// Return Z of the projectile
	return Zb;

}

int doppler::GetAb() {

	// Return A of the projectile
	return Ab;

}

int doppler::GetZt() {

	// Return Z of the target
	return Zt;

}

int doppler::GetAt() {

	// Return A of the target
	return At;

}

float doppler::GetPTh( float anno ) {

	// Returns theta angle from ann strip number in radians
	return TMath::ATan( (9.+(15.5-anno)*2.) / cddist );

}

float doppler::GetPPhi( int quad, int seg ) { 

	// Returns phi angle from quadrant and ohm strip number in radians 
	return GetPPhi( quad, seg, cdoffset );

}

float doppler::GetPPhi( int quad, int seg, float offset ) { 

	// Returns phi angle from quadrant and ohm strip number in radians 
	float ph_det[4] = { 0.+cdoffset, 90.+cdoffset, 180.+cdoffset, 270.+cdoffset };
	float pphi = ( ph_det[quad] + seg * 7.0 );
	if( pphi < 360. ) return pphi * TMath::DegToRad();
	else return ( pphi - 360. ) * TMath::DegToRad();

}

float doppler::GetQPhi( int quad, int seg ) {

	// Returns phi angle of B/T using angle of T/B
	return GetPPhi( quad, seg, cdoffset ) + TMath::Pi();

}

float doppler::GetTTh( float Banno, float BEn ) {

	// Returns theta angle of T using angle and energy of B
	double tau = (float)Ab/(float)At;
	double Eprime = (float)Eb*(float)Ab - (float)Ex*(1+tau);
	double epsilon = TMath::Sqrt((float)Eb*(float)Ab/Eprime);
	double fac = rand.Rndm(8)*0.9-0.45; // randomize angle across strip
	double x, y, TTh;
	if( tau > 1 ) { // inverse kinematics: maximum scattering angle may be exceeded...
		y = TMath::ASin(1./(tau*epsilon)); // maximum projectile angle in lab
		if( GetPTh(Banno+fac)<y ) y = GetPTh(Banno+fac);
		y = TMath::Tan(y);
	} else {
		y = TMath::Tan(GetPTh(Banno+fac)); // y = tan(Theta_projlab)
	}
	if( tau > 1 && rand.Gaus(BEn,30000.)<50000. ) {
		x = (y*y*epsilon*tau + TMath::Sqrt(-y*y*epsilon*epsilon*tau*tau + y*y + 1) ) / (1+y*y);
	} else {
		x = (y*y*epsilon*tau - TMath::Sqrt(-y*y*epsilon*epsilon*tau*tau + y*y + 1) ) / (1+y*y);
	}
	//	cout << "Centre of mass angle: " << TMath::ACos(x)*TMath::RadToDeg() << endl;
	TTh = TMath::ATan( TMath::Sqrt( 1 - x*x ) / ( epsilon + x ) ); // choose kinematic branch using energy cut... as I haven't a clue how to do it any other way?!
	if( TTh < 0 ) TTh += TMath::Pi();
	//	cout << "Simulated target angle: " << TTh*TMath::RadToDeg() << endl;
	return TTh;
}

float doppler::GetBTh( float Tanno ) {

	// Returns theta angle of B using angle and energy of T
	double tau = (float)Ab/(float)At;
	double Eprime = (float)Eb*(float)Ab - (float)Ex*(1+tau);
	double epsilon = TMath::Sqrt((float)Eb*(float)Ab/Eprime);
	double fac = rand.Rndm(8)*0.9-0.45; // randomize angle across strip
	double x, y, BTh;
	y = TMath::Tan(GetPTh(Tanno+fac)); // y = tan(Theta_targetlab)
	x = (y*y*epsilon - TMath::Sqrt(-y*y*epsilon*epsilon + y*y + 1) ) / (1+y*y);
	//	cout << "Centre of mass angle: " << TMath::ACos(x)*TMath::RadToDeg() << endl;
	BTh = TMath::ATan( TMath::Sqrt( 1 - x*x ) / ( tau*epsilon + x ) );
	if( BTh < 0 ) BTh += TMath::Pi();
	//	cout << "Simulated beam angle: " << BTh*TMath::RadToDeg() << endl;
	return BTh;

}

float doppler::GetTEn( float BEn, float Banno ) {

	// Returns energy of target using energy and angle of Rn 
	double dist, angle;

	// energy at interaction point
	double Ereac = (float)Eb*(float)Ab - GetELoss( (float)Eb*(float)Ab, depth, 0, "BT" );

	// Trace energy loss back through target to get energy at interaction point
	double Eproj = BEn + GetELoss(BEn,deadlayer,1,"BS"); // correct for dead layer loss first
	dist = TMath::Abs((double)(thick-depth)/TMath::Cos(GetPTh(Banno)));
	Eproj += GetELoss(Eproj,dist,1,"BT");

	double Etarg = Ereac - Eproj;
	if( Etarg < 0. ) return 0.1; // recoil is stopped in target

	angle = GetTTh(Banno,BEn);
	if( angle < 0.501*TMath::Pi() && angle > 0.499*TMath::Pi() ) return 0.1; // stopped

	dist = TMath::Abs((double)(thick-depth)/TMath::Cos(angle));
	Etarg -= GetELoss(Etarg,dist,0,"TT");

	if( Etarg < 0 ) return 0.1;
	else return Etarg;
	
}

float doppler::GetBEn( float TEn, float Tanno ) {

	// Returns energy of Rn using energy and angle of target
	double dist, angle;

	// energy at interaction point
	double Ereac = (float)Eb*(float)Ab - GetELoss((float)Eb*(float)Ab,depth,0,"BT");

	// Trace energy loss back through target to get energy at interaction point
	double Etarg = TEn + GetELoss(TEn,deadlayer,1,"TS"); // correct for dead layer loss first
	dist = TMath::Abs((double)(thick-depth)/TMath::Cos(GetPTh(Tanno)));
	Etarg += GetELoss(Etarg,dist,1,"TT");

	double Eproj = Ereac - Etarg;
	if( Eproj < 0. ) return 0.1; // projectile is stopped in target
	
	angle = GetBTh(Tanno);
	if( angle < 0.501*TMath::Pi() && angle > 0.499*TMath::Pi() ) return 0.1; // stopped

	dist = TMath::Abs((double)(thick-depth)/TMath::Cos(angle));
	Eproj -= GetELoss(Eproj,dist,0,"BT");

	if( Eproj < 0 ) return 0.1;
	else return Eproj;
	
}

float doppler::GetELoss( float Ei, float dist, int opt, string combo ) {

	// Returns the energy loss at a given initial energy and distance travelled in the target or Si dead layer
	// Ei is the initial energy in keV
	// dist is the distance travelled in the target in mg/cm2
	// opt = 0 calculates normal energy loss as particle moves through target (default)
	// opt = 1 calculates energy increase, i.e. tracing particle back to reaction point
	// combo = "BT", "TT", "BS" or "TS" for the beam in target, target in target,
	// beam in Si or target in Si, respectively.
	// The energy dependent function is a phenomenological considering electronic + nuclear
	// Each function is of the form: ln(StopPow) = a + b*ln(x-d) + c*ln(x-d)^2 
	// Stopping power data is taken from SRIM the output files must be placed in the './srim/'
	// folder with the format 62Fe_109Ag.txt, 62Fe_Si.txt, 109Ag_109Ag.txt or 109Ag_Si.txt,
	// for combo = "BT", "TT", "BS" and "TS", repsectively.
	
	double dedx = 0;
	int Nmeshpoints = 20; // number of steps to take in integration
	double dx = dist/(double)Nmeshpoints;
	double E[1] = {Ei};
	
	for( int i=0; i<Nmeshpoints; i++ ){

		if( E[0] < 1000. ) break; // when we fall below 1 MeV we assume maximum energy loss

		if( combo == "BT" ) dedx = SP_function( E, BTELoss_pars );
		else if( combo == "TT" ) dedx = SP_function( E, TTELoss_pars );
		else if( combo == "BS" ) dedx = SP_function( E, BSELoss_pars );
		else if( combo == "TS" ) dedx = SP_function( E, TSELoss_pars );

		if(opt==1) {
			E[0] += 1000.*dedx*dx;
		} else { 
			E[0] -= 1000.*dedx*dx;
		}
		
	}
	
	//if( opt == 0 && combo == "BT" ) cout << "Eloss = " << (Ei - E[0]) << endl;
	
	if( opt == 0 ) return (Ei - E[0]);
	else return (E[0] - Ei);

}

float doppler::GammaAng( float PTh, float PPhi, float GTh, float GPhi ) {

	// Returns angle between particle and gamma	in radians

	double costheta = sin(PTh)*sin(GTh)*cos(PPhi-GPhi)+(cos(PTh)*cos(GTh));
	
	return acos( costheta );
	
}

float doppler::DC( float PEn, float PTh, float PPhi, float GTh, float GPhi, float A ) {

	// Returns Doppler correction factor for given particle and gamma
	// angular combination.  Factors in detected particle energy too
	
	double beta = TMath::Sqrt( 2*PEn / (A*931494.028) );
	double gamma = 1. / TMath::Sqrt( 1. - beta*beta );
	double costheta = sin(PTh)*sin(GTh)*cos(PPhi-GPhi)+(cos(PTh)*cos(GTh));
	
	double corr = 1. - beta*costheta;
	corr /= gamma;
	
	return corr;
	
}

float doppler::DC_elec( float een, float PEn, float PTh, float PPhi, float GTh, float GPhi, float A ) {

	// Returns Doppler correction factor for given particle and electron
	// angular combination.  Factors in detected particle energy too
	
	double beta = TMath::Sqrt( 2*PEn / (A*931494.028) );
	double mass_e = 511.;
	double gamma = 1. / TMath::Sqrt( 1. - beta*beta );
	double costheta = sin(PTh)*sin(GTh)*cos(PPhi-GPhi)+(cos(PTh)*cos(GTh));
	
	double energy = een + mass_e - beta*costheta*TMath::Sqrt( een*een + 2.*mass_e*een );
	energy /= gamma;
	energy -= mass_e;	

	return energy;
	
}

string doppler::convertInt( int number ) {
	
	stringstream ss;
	ss << number;
	return ss.str();
	
}

string doppler::convertFloat( float number ) {
	
	stringstream ss;
	ss << number;
	return ss.str();
	
}
#endif
