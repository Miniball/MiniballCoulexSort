#ifndef doppler_cxx
#define doppler_cxx

#ifndef doppler_hh
# include "doppler.hh"
#endif

TRandom3 doppler::rand = 1;

void doppler::ExpDefs( int Zb_, float Ab_, int Zt_, float At_, float Eb_, float Ex_, float thick_,
						float depth_, float cddist_, float cdoffset_, float deadlayer_, float contaminant_,
						float spededist_, TCutG *Bcut_, TCutG *Tcut_, string srimfile_ ) {

	/// Initialisation of experimental definitions from command line of config file
	
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
	contaminant = contaminant_;
	spededist = spededist_;
	Bcut = Bcut_;
	Tcut = Tcut_;
	srimfile = srimfile_;

	return;

}

bool doppler::stoppingpowers( bool BT, bool TT, bool BS, bool TS, bool BC, bool TC ) {

	/// Initialisation of stopping powers
	
	bool success = true;
	
	for( int i = 0; i < 6; i++ )
		gSP[i] = new TGraph();

	if( BT ) success *= stoppingpowers( "BT" );
	if( TT ) success *= stoppingpowers( "TT" );
	if( BS ) success *= stoppingpowers( "BS" );
	if( TS ) success *= stoppingpowers( "TS" );
	if( BC && contaminant >= 0 ) success *= stoppingpowers( "BC" );
	if( TC && contaminant >= 0 ) success *= stoppingpowers( "TC" );

	return success;

}

bool doppler::stoppingpowers( string opt ) {
	 
	/// Open stopping power files and make TGraphs of data
	/// naming convention of files...
	
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

	unsigned int index = 0; // BT = 0, TT = 1, BS = 2, TS = 3
	string title = "Stopping powers for ";
	
	// Beam or target like..?
	if( opt.substr(0,1) == "B" ) {
		
		srimfile += "/" + convertInt(Ab+0.5) + gElName[Zb-1];
		title += convertInt(Ab+0.5) + gElName[Zb-1];
		
	}
	
	else if( opt.substr(0,1) == "T" ) {

		srimfile += "/" + convertInt(At+0.5) + gElName[Zt-1];
		title += convertInt(At+0.5) + gElName[Zt-1];
		index++;
		
	}
	
	else {
		
		cout << "opt must equal BT, TT, BS, TS, BC or TC \n";
		return false;
	
	}
	
	// Target, contaminant or silicon dead layer..?
	if( opt.substr(1,1) == "T" ) {
		
		srimfile += "_" + convertInt(At+0.5) + gElName[Zt-1] + ".txt";
		title += " in " + convertInt(At+0.5) + gElName[Zt-1];
		title += ";Ion energy [keV];Stopping power [MeV/(mg/cm^2)]";
		
	}
	
	else if( opt.substr(1,1) == "S" ) {
		
		srimfile += "_Si.txt";
		title += " in the Si dead layer";
		title += ";Ion energy [keV];Stopping power [MeV/mm]";
		index += 2;
		
	}
	
	else if( opt.substr(1,1) == "C" ) {
		
		srimfile += "_contaminant.txt";
		title += " in the contaminant layer";
		title += ";Ion energy [keV];Stopping power [MeV/(mg/cm^2)]";
		index += 4;
		
	}
	
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

	gSP[index]->SetTitle( title.c_str() );

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
		
		gSP[index]->SetPoint( p, BEn, total );
		  
		// Get next line
		getline( infile, line );
		p++;
		  
		// If we've reached the end, stop
		if( line.substr( 0, 9 ) == "---------" ) endflag = true;
		if( line.substr( 0, 9 ) == " Multiply" ) endflag = true;
		  
	}
	 
	TCanvas *c = new TCanvas();
	gSP[index]->Draw("A*");
	//gSP[index]->GetXaxis()->SetTitleOffset(1.3);
	//gSP[index]->GetYaxis()->SetTitleOffset(1.3);
	//TGaxis::SetMaxDigits(3);
	string pdfname = srimfile.substr( 0, srimfile.find_last_of(".") ) + ".pdf";
	c->SetLogx();
	c->SaveAs( pdfname.c_str() );
	 
	delete c;
	 
	return true;
	 
}

int doppler::Cut( float PEn, float anno, int quad ) {

	/// Check if entry passes any particle gates
	/// Function returns 1 for projectile or 0 for target
	/// -1 is returned if particle is outside of gates.
	/// Graphical cuts are used if they are given in the config file
	/// or on the command line with the -cut option.
	/// If not, there are some default polynomials defined here that
	/// you are welcome to change, but not encouraged to do so.

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

	/// Check if entry passes the 2 particle condition
	/// Return value is 1 if target is p1 or 2 if target is p2
	/// -1 is returned if condition is not filled
	/// It calls Cut() twice with each of the two particles passed to this function.
	/// If one of them is a particle and one of them is a target, then you get a good return
	int identity=-1;

	// inverse kinematics, include overlap region
	if( Ab > At ){

		if( ( Cut(PEn2,anno2,quad2) >0 || ((int)anno2>=12 && PEn2<550000.) ) &&
			( Cut(PEn1,anno1,quad1)==0 || ((int)anno1>=12 && PEn1>300000. && PEn1<700000.) ) ){
			identity=0; // target is particle number 1
		}
		if( ( Cut(PEn1,anno1,quad1) >0 || ((int)anno1>=12 && PEn1<550000.) ) &&
			( Cut(PEn2,anno2,quad2)==0 || ((int)anno2>=12 && PEn2>300000. && PEn2<700000.) ) ){
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

	/// Returns true or false if the 2d graphical cut on beam and target enegry passes
	/// Look into the "en2hit" histogram after changing return value of this function

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

	/// Return offset of the CD in the phi rotation from vertical in degrees
	
	return cdoffset;

}

float doppler::GetSpedeDist() {

	/// Return distance of Spede detector in mm
	
	return spededist;

}

float doppler::GetCDDeadLayer() {
	
	/// Return dead layer of the Si in mm
	
	return deadlayer;
	
}

int doppler::GetZb() {

	/// Return Z of the projectile as an int
	
	return Zb;

}

float doppler::GetAb() {

	/// Return A of the projectile as a float
	
	return Ab;

}

int doppler::GetZt() {

	/// Return Z of the target as an int
	
	return Zt;

}

float doppler::GetAt() {

	/// Return A of the target as a float
	
	return At;

}

float doppler::GetPTh( float anno ) {

	/// Returns theta angle from ann strip number in radians
	
	return TMath::ATan( (9.+(15.5-anno)*2.) / cddist );

}

float doppler::GetPPhi( int quad, int seg ) { 

	/// Returns phi angle from quadrant and ohm strip number in radians
	
	return GetPPhi( quad, seg, cdoffset );

}

float doppler::GetPPhi( int quad, int seg, float offset ) { 

	/// Returns phi angle from quadrant and ohm strip number in radians
	
	float ph_det[4] = { (float)0+cdoffset, (float)90+cdoffset, (float)180+cdoffset, (float)270+cdoffset };
	float pphi = ( ph_det[quad] + seg * 7.0 );
	if( pphi < 360. ) return pphi * TMath::DegToRad();
	else return ( pphi - 360. ) * TMath::DegToRad();

}

float doppler::GetQPhi( int quad, int seg ) {

	/// Returns phi angle of B/T using angle of T/B
	
	return GetPPhi( quad, seg, cdoffset ) + TMath::Pi();

}

float doppler::GetTTh( float Banno, float BEn ) {

	/// Returns theta angle of T using angle and energy of B
	
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

	/// Returns theta angle of B using angle and energy of T
	
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

	/// Function to calculate the energy of the recoiling target
	/// given the energy and the angle of the scattered beam
	/// Returns the energy after the target in keV
	
	// energy at interaction point
	double Ereac = (float)Eb * (float)Ab;
	if( contaminant >= 0 ) Ereac -= GetELoss( Ereac, contaminant, 0, "BC" );
	Ereac -= GetELoss( Ereac, depth, 0, "BT" );

	// Correct for dead layer loss
	double dist = TMath::Abs( deadlayer / TMath::Cos( GetPTh( Banno ) ) );
	double Eproj = BEn + GetELoss( BEn, dist, 1, "BS" );

	// Trace energy loss back through target to get energy at interaction point
	dist = TMath::Abs( ( thick - depth ) / TMath::Cos( GetPTh( Banno ) ) );
	Eproj += GetELoss( Eproj, dist, 1, "BT" );

	double Etarg = Ereac - Eproj;
	if( Etarg < 0.1 ) return 0.1; // recoil is stopped in target

	double angle = GetTTh( Banno, BEn );
	if( angle < 0.501*TMath::Pi() && angle > 0.499*TMath::Pi() ) return 0.1; // stopped

	dist = TMath::Abs( ( thick - depth ) / TMath::Cos( angle ) );
	Etarg -= GetELoss( Etarg, dist, 0, "TT" );

	if( Etarg < 0.1 ) return 0.1;
	else return Etarg;
	
}

float doppler::GetBEn( float TEn, float Tanno ) {

	/// Function to calculate the energy of the scattered beam
	/// given the energy and the angle of the recoiling target
	/// Returns the energy after the target in keV
	
	// energy at interaction point
	double Ereac = (float)Eb * (float)Ab;
	if( contaminant >= 0 ) Ereac -= GetELoss( Ereac, contaminant, 0, "BC" );
	Ereac -= GetELoss( Ereac, depth, 0, "BT" );
	
	// Correct for dead layer loss
	double dist = TMath::Abs( deadlayer / TMath::Cos( GetPTh( Tanno ) ) );
	double Etarg = TEn + GetELoss( TEn, dist, 1, "TS" );

	// Trace energy loss back through target to get energy at interaction point
	dist = TMath::Abs( ( thick - depth ) / TMath::Cos( GetPTh( Tanno ) ) );
	Etarg += GetELoss( Etarg, dist, 1, "TT" );

	double Eproj = Ereac - Etarg;
	if( Eproj < 0.1 ) return 0.1; // projectile is stopped in target

	double angle = GetBTh( Tanno );
	if( angle < 0.501*TMath::Pi() && angle > 0.499*TMath::Pi() ) return 0.1; // stopped

	dist = TMath::Abs( ( thick - depth ) / TMath::Cos( angle ) );
	Eproj -= GetELoss( Eproj, dist, 0, "BT" );

	if( Eproj < 0.1 ) return 0.1;
	else return Eproj;
	
}

float doppler::GetELoss( float Ei, float dist, int opt, string combo ) {

	/// Returns the energy loss at a given initial energy and distance travelled
	/// in the target, the contaminant layer or Si dead layer
	/// Ei is the initial energy in keV, return value is also in keV
	/// dist is the distance travelled in the target in mg/cm2
	/// opt = 0 calculates normal energy loss as particle moves through target (default)
	/// opt = 1 calculates energy increase, i.e. tracing particle back to reaction point
	/// combo = "BT", "TT", "BC", "TC", "BS" or "TS" for the beam in target, target in target,
	/// beam in contaminant, target in contaminant, beam in Si or target in Si, respectively.
	/// Stopping power data is taken from SRIM the output files must be placed in the './srim/'
	/// folder with the format 62Fe_109Ag.txt, 62Fe_Si.txt, 109Ag_109Ag.txt or 109Ag_Si.txt,
	/// for combo = "BT", "TT", "BS" and "TS", repsectively.
	/// The srim file should be in units of MeV/(mg/cm^2)
	
	double dedx = 0;
	int Nmeshpoints = 20; // number of steps to take in integration
	double dx = dist/(double)Nmeshpoints;
	double E = Ei;
	
	for( int i = 0; i < Nmeshpoints; i++ ){

		if( E < 1000. ) break; // when we fall below 1 MeV we assume maximum energy loss

		if( combo == "BT" ) dedx = gSP[0]->Eval(E);
		else if( combo == "TT" ) dedx = gSP[1]->Eval(E);
		else if( combo == "BS" ) dedx = gSP[2]->Eval(E);
		else if( combo == "TS" ) dedx = gSP[3]->Eval(E);
		else if( combo == "BC" ) dedx = gSP[4]->Eval(E);
		else if( combo == "TC" ) dedx = gSP[5]->Eval(E);

		if( opt == 1 )
			E += 1000.*dedx*dx;

		else
			E -= 1000.*dedx*dx;
		
	}
	
	//if( opt == 0 && combo == "BT" ) cout << "Eloss = " << Ei - E << endl;
	
	if( opt == 0 ) return Ei - E;
	else return E - Ei;

}

float doppler::GammaAng( float PTh, float PPhi, float GTh, float GPhi ) {
	
	/// Returns angle between particle and gamma	in radians
	
	double costheta = sin(PTh)*sin(GTh)*cos(PPhi-GPhi)+(cos(PTh)*cos(GTh));
	
	return TMath::ACos( costheta );
	
}

float doppler::Beta( float Ek, float m ) {
	
	/// Returns beta after Taylor expansion to third order
	
	double beta2 = -0.5 * m + TMath::Sqrt( m * ( 0.25 * m + 1.5 * Ek ) );
	beta2 /= 0.75 * m;
	
	return TMath::Sqrt( beta2 );
	
}

float doppler::DC( float PEn, float PTh, float PPhi, float GTh, float GPhi, float A ) {

	/// Returns Doppler correction factor for given particle and gamma
	/// angular combination.  Factors in detected particle energy too
	
	double beta = Beta( PEn, A * u_mass() );
//	double beta = TMath::Sqrt( 2.0*PEn / ( A * u_mass() ) );
	double gamma = 1. / TMath::Sqrt( 1. - beta*beta );
	double costheta = sin(PTh)*sin(GTh)*cos(PPhi-GPhi)+(cos(PTh)*cos(GTh));
	
	double corr = 1. - beta*costheta;
	corr /= gamma;
	
	return corr;
	
}

float doppler::DC_elec( float een, float PEn, float PTh, float PPhi, float GTh, float GPhi, float A ) {

	/// Returns Doppler correction factor for given particle and electron
	/// angular combination.  Factors in detected particle energy too
	
	double beta = TMath::Sqrt( 2*PEn / ( A * u_mass() ) );
	double mass_e = 511.;
	double gamma = 1. / TMath::Sqrt( 1. - beta*beta );
	double costheta = sin(PTh)*sin(GTh)*cos(PPhi-GPhi)+(cos(PTh)*cos(GTh));
	
	double energy = een + mass_e - beta*costheta*TMath::Sqrt( een*een + 2.*mass_e*een );
	energy /= gamma;
	energy -= mass_e;	

	return energy;
	
}

string doppler::convertInt( int number ) {
	
	/// Convert an integer into a string
	
	stringstream ss;
	ss << number;
	return ss.str();
	
}

string doppler::convertFloat( float number ) {
	
	/// Convert an float into a string
	
	stringstream ss;
	ss << number;
	return ss.str();
	
}
#endif
