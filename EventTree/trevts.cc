#ifndef trevts_cxx
#define trevts_cxx

#include "trevts.hh"

trevts::trevts() {
	
	Initialise();
	
}

trevts::~trevts() {
	
	//cout << "destructor" << endl;
		
}
	
void trevts::Initialise() {

	pen=-100.;
	pen_de=-100.;
	pen_e=-100.;
	quad=-1;
	nf=-1;
	nb=-1;
	sector=-1;
	time=-999.;
	t1t=-999.;
	sst=-999.;
	laser=-1;
	first=false;

	pcor_pen.resize(0);
	pcor_pen_de.resize(0);
	pcor_pen_e.resize(0);
	pcor_quad.resize(0);
	pcor_nf.resize(0);
	pcor_nb.resize(0);
	pcor_sector.resize(0);
	pcor_ptd.resize(0);

	gen.resize(0);
	cid.resize(0);
	sid.resize(0);
	cluid.resize(0);
	tha.resize(0);
	pha.resize(0);
	td.resize(0);
	
	coin.resize(0);
	pr_ptr.resize(0);
	rndm_ptr.resize(0);
	del_ptr.resize(0);
	
	pr_hits=0;
	rndm_hits=0;
	del_hits=0;
	
}

void trevts::CopyData( trevts src ) {
	
	pen = src.pen;
	pen_de = src.pen_de;
	pen_e = src.pen_e;
	quad = src.quad;
	nf = src.nf;
	nb = src.nb;
	sector = src.sector;
	t1t = src.t1t;
	sst = src.sst;
	laser = src.laser;
	first = src.first;

	pcor_pen = src.pcor_pen;
	pcor_pen_de = src.pcor_pen_de;
	pcor_pen_e = src.pcor_pen_e;
	pcor_quad = src.pcor_quad;
	pcor_nf = src.pcor_nf;
	pcor_nb = src.pcor_nb;
	pcor_sector = src.pcor_sector;
	pcor_ptd = src.pcor_ptd;

	gen = src.gen;
	cid = src.cid;
	sid = src.sid;
	cluid = src.cluid;
	tha = src.tha;
	pha = src.pha;
	td = src.td;
	
	coin = src.coin;
	pr_ptr = src.pr_ptr;
	rndm_ptr = src.rndm_ptr;
	del_ptr = src.del_ptr;
	
	pr_hits = src.pr_hits;
	rndm_hits = src.rndm_hits;
	del_hits = src.del_hits;
	
}

void trevts::SetPen( float en, float de, float erest ) {
	
	pen = en;
	pen_de = de;
	pen_e = erest;
	
}

void trevts::SetQuad( int q ) {
	
	quad = q;
	
}

void trevts::SetNf( int id ) {
	
	nf = id;
	
}

void trevts::SetNb( int id ) {
	
	nb = id;
	
}

void trevts::SetSector( int id ) {
	
	sector = id;
	
}

void trevts::SetTime( double t ) {
	
	time = t;
	
}

void trevts::SetT1( double t ) {
	
	t1t = t;
	
}

void trevts::SetLaser( int l ) {
	
	laser = l;
	
}

void trevts::SetFirst( bool f ) {
	
	first = f;
	
}

void trevts::SetSS( double t ) {
	
	sst = t;
	
}

void trevts::SetCorPen( float en, float de, float erest ) {
	
	pcor_pen.push_back( en );
	pcor_pen_de.push_back( de );
	pcor_pen_e.push_back( erest );
	
}

void trevts::SetCorQuad( int q ) {
	
	pcor_quad.push_back( q );
	
}

void trevts::SetCorNf( int id ) {
	
	pcor_nf.push_back( id );
	
}

void trevts::SetCorNb( int id ) {
	
	pcor_nb.push_back( id );
	
}

void trevts::SetCorSector( int id ) {
	
	pcor_sector.push_back( id );
	
}

void trevts::SetCorTd( double t ) {
	
	pcor_ptd.push_back( t );
	
}


void trevts::SetGamma( float en, int c, int s, int clu, float th, float ph, double ptd, int co ) {


	gen.push_back( en );
	cid.push_back( c );
	sid.push_back( s );
	cluid.push_back( clu );
	tha.push_back( th );
	pha.push_back( ph );
	
	td.push_back( ptd );
	coin.push_back( co );
		
}

void trevts::SearchCoin() {

	for( unsigned int i = 0; i < gen.size(); i++ ){
	
		if( coin[i] == 0 ){
		
			pr_ptr.push_back( i );
			pr_hits++; 
		
		}
		
		else if( coin[i] == 1 ){
		
			rndm_ptr.push_back( i );
			rndm_hits++;
	
		}
	
		else if( coin[i] == 2 ){
		
			del_ptr.push_back( i );
			del_hits++;
	
		}
	
	}

}
#endif
