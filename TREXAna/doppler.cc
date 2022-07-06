#ifndef doppler_cxx
#define doppler_cxx

#ifndef doppler_hh
# include "doppler.hh"
#endif

TRandom3 doppler::rand = 1;

void doppler::ExpDefs( int Zb_, float Ab_, int Zt_, float At_, float Eb_, float Ex_,
                      float thick_,	float depth_, float cddist_, float cdoffset_,
                      float deadlayer_, float contaminant_,	float spededist_,
                      string srimfile_, bool usekin_ ) {

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
	srimfile = srimfile_;
	usekin = usekin_;

	return;

}

void doppler::reactionEnergy() {
	
	/// Calculate the energy at interaction point
	Ereac = Eb * Ab;
	if( contaminant > 0 ) Ereac -= GetELoss( Ereac, contaminant, 0, "BC" );
	Ereac -= GetELoss( Ereac, depth, 0, "BT" );
	
	cout << "Reaction energy = " << Ereac/1000. << " MeV";
	cout << " or " << Ereac/1000./Ab << " MeV/u" << endl;
	
	return;
	
}

bool doppler::stoppingpowers( bool BT, bool TT, bool BS, bool TS ) {

	/// Initialisation of stopping powers
	bool success = true;
	
	gErrorIgnoreLevel = kWarning;
	
	for( int i = 0; i < 4; i++ )
		gSP[i] = new TGraph();

	if( BT ) success *= stoppingpowers( "BT" );
	if( TT ) success *= stoppingpowers( "TT" );
	if( BS ) success *= stoppingpowers( "BS" );
	if( TS ) success *= stoppingpowers( "TS" );

	if( success ) reactionEnergy();
	
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
	string srimfilename = srimfile;
	
	// Beam or target like..?
	if( opt.substr(0,1) == "B" ) {
		
		srimfilename += "/" + convertInt(Ab+0.5) + gElName[Zb-1];
		title += convertInt(Ab+0.5) + gElName[Zb-1];
		
	}
	
	else if( opt.substr(0,1) == "T" ) {

		srimfilename += "/" + convertInt(At+0.5) + gElName[Zt-1];
		title += convertInt(At+0.5) + gElName[Zt-1];
		index++;
		
	}
	
	else {
		
		cout << "opt must equal BT, TT, BS, TS, BC or TC \n";
		return false;
	
	}
	
	// Target, contaminant or silicon dead layer..?
	if( opt.substr(1,1) == "T" ) {
		
		srimfilename += "_" + convertInt(At+0.5) + gElName[Zt-1] + ".txt";
		title += " in " + convertInt(At+0.5) + gElName[Zt-1];
		title += ";Ion energy [keV];Stopping power [MeV/(mg/cm^2)]";
		
	}
	
	else if( opt.substr(1,1) == "S" ) {
		
		srimfilename += "_Si.txt";
		title += " in the Si dead layer";
		title += ";Ion energy [keV];Stopping power [MeV/mm]";
		index += 2;
		
	}

	else {
		
		cout << "opt must equal BT, TT, BS or TS \n";
		return false;

	}
	 
	ifstream infile;
	infile.open( srimfilename.c_str(), ios::in );
	 
	if( !infile.is_open() ) {
		  
		cout << "Cannot open " << srimfilename << endl;
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
	string pdfname = srimfilename.substr( 0, srimfilename.find_last_of(".") ) + ".pdf";
	c->SetLogx();
	c->SaveAs( pdfname.c_str() );
	 
	delete c;
	 
	return true;
	 
}

int doppler::Cut( float PEn, float nf, int quad, int sector ) {

	/// Check if entry passes any particle gates
	/// Function returns 1 for projectile or 0 for target
	/// -1 is returned if particle is outside of gates.
	/// Graphical cuts are used if they are given in the config file
	/// or on the command line with the -cut option.
	/// If not, there are some default polynomials defined here that
	/// you are welcome to change, but not encouraged to do so.

	int identity = -1;
	int str = quad*16 + nf;
	float ang = GetPTh(nf, sector) * TMath::RadToDeg();

	// Do something
	
	return identity;

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

float doppler::GetPTh( float nf, int sector ) {

	/// Returns theta angle from ann strip number in radians
	
	float angle = 0.0;
	
	// Forward CD - Standard CD
	if( sector == 4 ) angle = TMath::ATan( ( 9.0 + (15.5-nf) * 2.0 ) / cddist );
	
	// Forward CD - CREX
	if( sector == 0 ) angle = TMath::ATan( ( 9.0 + (nf+0.5) * 2.0 ) / cddist );
	
	// Forwards Barrel
	if( sector == 1 ) angle = 0.5*TMath::Pi() - TMath::ATan( ( 8.0 + (nf+0.5) * 3.125 ) / 29.0 );
	
	// Backwards Barrel
	if( sector == 2 ) angle = 0.5*TMath::Pi() + TMath::ATan( ( 8.0 + (nf+0.5) * 3.125 ) / 29.0 );
	
	// Backwards CD
	if( sector == 3 ) angle = TMath::Pi() - TMath::ATan( ( 9.0 + (nf+0.5) * 2.0 ) / 64.0 );
	
	return angle;
	
}

float doppler::GetPPhi( int quad, int seg, int sector ) {

	/// Returns phi angle from quadrant and ohm strip number in radians
	
	float ph_det[4];
	if( sector == 4 ) { // standard CD
		
		ph_det[0] =   0.0 + cdoffset; // top
		ph_det[1] =  90.0 + cdoffset; // right
		ph_det[2] = 180.0 + cdoffset; // bottom
		ph_det[3] = 270.0 + cdoffset; // left
		
	}
	
	else { // CREX and TREX
		
		ph_det[0] =   0.0 + cdoffset; // top
		ph_det[1] = 180.0 + cdoffset; // bottom
		ph_det[2] = 270.0 + cdoffset; // left
		ph_det[3] =  90.0 + cdoffset; // right
		
	}
	
	float pphi = ph_det[quad];
	if( sector == 4 ) pphi += seg * 7.0;	// standard CD
	else if( sector < 4 ) {					// CREX and TREX
		
		pphi += 1.75; // centre of first strip
		if( seg < 4 ) pphi += seg * 3.5; // first 4 strips singles (=4 segs)
		else if( seg < 12 ) pphi += 14. + ( seg - 4 ) * 7.0; // middle 16 strips doubles (=8 segs)
		else pphi += 70. + ( seg - 12 ) * 3.5; // last 4 strips singles (=4 segs)
	
	}
	
	if( pphi < 360. ) return pphi * TMath::DegToRad();
	else return ( pphi - 360. ) * TMath::DegToRad();

}

float doppler::GetQPhi( int quad, int seg, int sector ) {

	/// Returns phi angle of B/T using angle of T/B
	
	return GetPPhi( quad, seg, sector ) + TMath::Pi();

}

float doppler::GetTTh( float Bnf, float BEn, int sector ) {

	/// Returns theta angle of T using angle and energy of B
	
	float tau = Ab/At;
	float Eprime = Eb*Ab - Ex*(1+tau);
	float epsilon = TMath::Sqrt(Eb*Ab/Eprime);
	float x, y, TTh;
	if( tau > 1 ) { // inverse kinematics: maximum scattering angle may be exceeded...
		y = TMath::ASin(1./(tau*epsilon)); // maximum projectile angle in lab
		if( GetPTh(Bnf, sector) < y ) y = GetPTh(Bnf, sector);
		y = TMath::Tan(y);
	} else {
		y = TMath::Tan(GetPTh(Bnf, sector)); // y = tan(Theta_projlab)
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

float doppler::GetBTh( float Tnf, int sector ) {

	/// Returns theta angle of B using angle and energy of T
	
	float tau = Ab/At;
	float Eprime = Eb*Ab - Ex*(1+tau);
	float epsilon = TMath::Sqrt(Eb*Ab/Eprime);
	float x, y, BTh;
	y = TMath::Tan(GetPTh(Tnf, sector)); // y = tan(Theta_targetlab)
	x = (y*y*epsilon - TMath::Sqrt(-y*y*epsilon*epsilon + y*y + 1) ) / (1+y*y);
	//	cout << "Centre of mass angle: " << TMath::ACos(x)*TMath::RadToDeg() << endl;
	BTh = TMath::ATan( TMath::Sqrt( 1 - x*x ) / ( tau*epsilon + x ) );
	if( BTh < 0 ) BTh += TMath::Pi();
	//	cout << "Simulated beam angle: " << BTh*TMath::RadToDeg() << endl;
	return BTh;

}

float doppler::GetTEn( float BEn, float Bnf, int sector ) {

	/// Function to calculate the energy of the recoiling target
	/// given the energy and the angle of the scattered beam
	/// Returns the energy after the target in keV
	
	// Correct for dead layer loss
	float dist = TMath::Abs( deadlayer / TMath::Cos( GetPTh( Bnf, sector ) ) );
	float Eproj = BEn + GetELoss( BEn, dist, 1, "BS" );

	// Trace energy loss back through target to get energy at interaction point
	dist = TMath::Abs( ( thick - depth ) / TMath::Cos( GetPTh( Bnf, sector ) ) );
	Eproj += GetELoss( Eproj, dist, 1, "BT" );

	float Etarg = Ereac - Eproj;
	if( Etarg < 0.1 ) return 0.1; // recoil is stopped in target

	float angle = GetTTh( Bnf, BEn, sector );
	if( angle < 0.501*TMath::Pi() && angle > 0.499*TMath::Pi() ) return 0.1; // stopped

	dist = TMath::Abs( ( thick - depth ) / TMath::Cos( angle ) );
	Etarg -= GetELoss( Etarg, dist, 0, "TT" );

	if( Etarg < 0.1 ) return 0.1;
	else return Etarg;
	
}

float doppler::GetBEn( float TEn, float Tnf, int sector ) {

	/// Function to calculate the energy of the scattered beam
	/// given the energy and the angle of the recoiling target
	/// Returns the energy after the target in keV
	
	// Correct for dead layer loss
	float dist = TMath::Abs( deadlayer / TMath::Cos( GetPTh( Tnf, sector ) ) );
	float Etarg = TEn + GetELoss( TEn, dist, 1, "TS" );

	// Trace energy loss back through target to get energy at interaction point
	dist = TMath::Abs( ( thick - depth ) / TMath::Cos( GetPTh( Tnf, sector ) ) );
	Etarg += GetELoss( Etarg, dist, 1, "TT" );

	float Eproj = Ereac - Etarg;
	if( Eproj < 0.1 ) return 0.1; // projectile is stopped in target

	float angle = GetBTh( Tnf, sector );
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
	/// combo = "BT", "TT", "BS" or "TS" for the beam in target, target in target,
	/// beam in Si or target in Si, respectively.
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

		if( opt == 1 )
			E += 1000.*dedx*dx;

		else
			E -= 1000.*dedx*dx;
		
	}
	
	//if( opt == 0 && combo == "BT" ) cout << "Eloss = " << Ei - E << endl;
	
	if( opt == 0 ) return Ei - E;
	else return E - Ei;

}

float doppler::GetBThLab( float CoM ) {
	
	/// Calculate the beam angle in the lab from the centre of mass angle (radians)
	/// @param CoM theta angle of the beam in the centre of mass frame

	float tau = Ab/At;
	float Eprime = Ereac - Ex * ( 1 + tau );
	float epsilon = TMath::Sqrt(Ereac/Eprime);

	// y = tan(theta_lab)
	float y = TMath::Sin(CoM) / ( TMath::Cos(CoM) + tau*epsilon );
	
	float BTh = TMath::ATan(y);
	if( BTh < 0. ) BTh += TMath::Pi();
	
	return BTh;
	
}

float doppler::GetTThLab( float CoM ) {
	
	/// Calculate the target angle in the lab from the centre of mass angle (radians)
	/// @param CoM theta angle of the beam in the centre of mass frame

	float tau = Ab/At;
	float Eprime = Ereac - Ex * ( 1 + tau );
	float epsilon = TMath::Sqrt(Ereac/Eprime);

	// y = tan(theta_lab)
	float y = TMath::Sin(TMath::Pi()-CoM) / ( TMath::Cos(TMath::Pi()-CoM) + epsilon );
	
	float TTh = TMath::ATan(y);
	if( TTh < 0. ) TTh += TMath::Pi();
	
	return TTh;
	
}

float doppler::GetBThLabT( float TTh, bool kinflag ) {
	
	/// Calculate the beam angle in the lab from the target lab angle
	/// @param TTh theta angle of the target in the laboratory frame

	return GetBThLab( GetTThCoM( TTh, kinflag ) );
	
}

float doppler::GetTThLabB( float BTh, bool kinflag ) {
	
	/// Calculate the target angle in the lab from the beam lab angle
	/// @param BTh theta angle of the beam in the laboratory frame
	
	return GetTThLab( GetBThCoM( BTh, kinflag ) );
	
}

float doppler::GetBThCoM( float BTh, bool kinflag ) {
	
	/// Calculates CoM scattering angle from the beam laboratory angle in radians
	/// @param BTh theta angle of the beam in laboratory frame
	/// @param kinflag kinematics flag such that true is the backwards solution (i.e. CoM > 90 deg)
	
	float tau = Ab/At;
	float Eprime = Ereac - Ex * ( 1 + tau );
	float epsilon = TMath::Sqrt(Ereac/Eprime);
	
	// maximum scattering angle may be exceeded...
	float maxang = TMath::ASin( 1. / ( tau * epsilon ) );
	if( tau*epsilon > 1 && BTh > maxang ) BTh = maxang;
	
	if( kinflag && tau*epsilon < 1 ){
		
		cerr << "Only one solution for the beam, kinflag = false" << endl;
		kinflag = false;
		
	}

	float y = epsilon * tau * TMath::Sin( BTh );
	if( kinflag ) y = TMath::ASin( -y );
	else y = TMath::ASin( y );

	float CoM = BTh + y;
	
	if( CoM < 0. ) CoM += TMath::Pi();
	if( CoM > TMath::Pi() ) CoM -= TMath::Pi();

	return CoM;

}

float doppler::GetTThCoM( float TTh, bool kinflag ) {
	
	/// Calculates CoM scattering angle from the target laboratory angle in radians
	/// @param TTh theta angle of the target in laboratory frame
	/// @param kinflag kinematics flag such that true is the backwards solution (i.e. CoM > 90 deg)

	float tau = Ab/At;
	float Eprime = Ereac - Ex * ( 1 + tau );
	float epsilon = TMath::Sqrt(Ereac/Eprime);
	
	// maximum scattering angle may be exceeded...
	float maxang = TMath::ASin( 1. / ( epsilon ) );
	if( TTh > maxang ) TTh = maxang;
	
	float y = epsilon * TMath::Sin( TTh );
	if( kinflag ) y = TMath::ASin( -y );
	else y = TMath::ASin( y );

	float CoM = TTh + y;
	CoM = TMath::Pi() - CoM;
	if( CoM < 0. ) CoM += TMath::Pi();
	if( CoM > TMath::Pi() ) CoM -= TMath::Pi();

	return CoM;
	
}

float doppler::GetBEnKin( float CoM ) {
	
	/// Calculate the beam energy for a given centre of mass angle
	/// using two-body kinematics calculations only
	/// @param CoM theta angle of the beam in the centre of mass frame

	float tau = Ab/At;
	float Eprime = Ereac - Ex * ( 1 + tau );
	float epsilon = TMath::Sqrt(Ereac/Eprime);
	
	float Eproj = TMath::Power( At/(At+Ab), 2.0 );
	Eproj *= 1. + tau*tau*epsilon*epsilon + 2.*tau*epsilon*TMath::Cos( CoM );
	Eproj *= Eprime;
	
	return Eproj;
	
}

float doppler::GetTEnKin( float CoM ) {
	
	/// Calculate the target energy for a given centre of mass angle
	/// using two-body kinematics calculations only
	/// @param CoM theta angle of the beam in the centre of mass frame

	float tau = Ab/At;
	float Eprime = Ereac - Ex * ( 1 + tau );
	float epsilon = TMath::Sqrt(Ereac/Eprime);

	float Etarg = (At*Ab) / TMath::Power( (At+Ab), 2.0 );
	Etarg *= 1. + epsilon*epsilon + 2.*epsilon*TMath::Cos( TMath::Pi() - CoM );
	Etarg *= Eprime;
	
	return Etarg;

}

float doppler::GetBEnKinB( float BTh, bool kinflag ) {
	
	/// Calculate the beam energy for a given beam
	/// using two-body kinematics calculations only
	/// @param BTh theta angle of the beam in laboratory frame
	/// @param kinflag kinematics flag such that true is the backwards solution (i.e. CoM > 90 deg)

	return GetBEnKin( GetBThCoM( BTh, kinflag ) );
	
}

float doppler::GetBEnKinT( float TTh, bool kinflag ) {
	
	/// Calculate the beam energy for a given target
	/// using two-body kinematics calculations only
	/// @param TTh theta angle of the target in laboratory frame

	return GetBEnKin( GetTThCoM( TTh, kinflag ) );
	
}

float doppler::GetTEnKinB( float BTh, bool kinflag ) {
	
	/// Calculate the target energy for a given beam
	/// using two-body kinematics calculations only
	/// @param BTh theta angle of the beam in laboratory frame
	/// @param kinflag kinematics flag such that true is the backwards solution (i.e. CoM > 90 deg)

	return GetTEnKin( GetBThCoM( BTh, kinflag ) );
	
}

float doppler::GetTEnKinT( float TTh, bool kinflag ) {
	
	/// Calculate the target energy for a given target
	/// using two-body kinematics calculations only
	/// @param TTh theta angle of the target in laboratory frame

	return GetTEnKin( GetTThCoM( TTh, kinflag ) );
	
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
	/// angular combination.
	/// Velocity is calculated from detected particle energy, unless
	/// the 'usekin' flag is set to true, in which case it uses the velocity
	/// calculated from two-body kinematics
	
	float beta = Beta( PEn, A * u_mass() );
	float gamma = 1. / TMath::Sqrt( 1. - beta*beta );
	float costheta = sin(PTh)*sin(GTh)*cos(PPhi-GPhi)+(cos(PTh)*cos(GTh));
    
	float corr = 1. - beta*costheta;
	corr *= gamma;
	
	return corr;
	
}

float doppler::DC_elec( float een, float PEn, float PTh, float PPhi, float GTh, float GPhi, float A ) {

	/// Returns Doppler correction factor for given particle and electron
	/// angular combination.  Factors in detected particle energy too
	
	float beta = TMath::Sqrt( 2*PEn / ( A * u_mass() ) );
	float mass_e = 511.;
	float gamma = 1. / TMath::Sqrt( 1. - beta*beta );
	float costheta = sin(PTh)*sin(GTh)*cos(PPhi-GPhi)+(cos(PTh)*cos(GTh));
	
	float energy = een + mass_e - beta*costheta*TMath::Sqrt( een*een + 2.*mass_e*een );
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