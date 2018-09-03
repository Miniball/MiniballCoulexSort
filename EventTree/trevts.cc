#ifndef trevts_cxx
#define trevts_cxx

#include "trevts.hh"

trevts::trevts() {
	
	pen=-100.;
	pen_de=-100.;
	pen_e=-100.;
	det=-1;
	ann=-1;
	sec=-1;
	time=-999.;
	t1t=-999.;
	sst=-999.;
	laser=-1;

	pcor_pen.resize(0);
	pcor_pen_de.resize(0);
	pcor_pen_e.resize(0);
	pcor_det.resize(0);
	pcor_ann.resize(0);
	pcor_sec.resize(0);
	
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

trevts::~trevts() {
	
	cout << "destructor" << endl;
	
}

void trevts::CopyData(trevts* src) {
	
	pen = src->pen;
	pen_de = src->pen_de;
	pen_e = src->pen_e;
	det = src->det;
	ann = src->ann;
	sec = src->sec;
	t1t = src->t1t;
	sst = src->sst;
	laser = src->laser;
	
	pcor_pen = src->pcor_pen;
	pcor_pen_de = src->pcor_pen_de;
	pcor_pen_e = src->pcor_pen_e;
	pcor_det = src->pcor_det;
	pcor_ann = src->pcor_ann;
	pcor_sec = src->pcor_sec;

	gen = src->gen;
	cid = src->cid;
	sid = src->sid;
	cluid = src->cluid;
	tha = src->tha;
	pha = src->pha;
	td = src->td;
	
	coin = src->coin;
	pr_ptr = src->pr_ptr;
	rndm_ptr = src->rndm_ptr;
	del_ptr = src->del_ptr;
	
	pr_hits = src->pr_hits;
	rndm_hits = src->rndm_hits;
	del_hits = src->del_hits;
	
}

void trevts::SetPen( float en, float de, float erest ) {
	
	pen = en;
	pen_de = de;
	pen_e = erest;
	
}

void trevts::SetQuad( int q ) {
	
	det = q;
	
}

void trevts::SetAnn( int id ) {
	
	ann = id;
	
}

void trevts::SetSec( int id ) {
	
	sec = id;
	
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

void trevts::SetSS( double t ) {
	
	sst = t;
	
}

void trevts::SetCorPen( float en, float de, float erest ) {
	
	pcor_pen.push_back( en );
	pcor_pen_de.push_back( de );
	pcor_pen_e.push_back( erest );
	
}

void trevts::SetCorQuad( int q ) {
	
	pcor_det.push_back( q );
	
}

void trevts::SetCorAnn( int id ) {
	
	pcor_ann.push_back( id );
	
}

void trevts::SetCorSec( int id ) {
	
	pcor_sec.push_back( id );
	
}

void trevts::SetCorTd( double t ) {
	
	pcor_ptd.push_back( t );
	
}

float trevts::GetGen( int nr ) {
	
	return gen[nr];
	
}

int trevts::GetCluid( int nr ) {
	
	return cluid[nr];

}

int trevts::GetCid( int nr ) {

	return cid[nr];

}

int trevts::GetSid( int nr ) {

	return sid[nr];

}

float trevts::GetTheta( int nr ) {

	return tha[nr];

}

float trevts::GetPhi( int nr ) {

	return pha[nr];

}

int trevts::GetNrGammas() {

	return gen.size();

}

float trevts::GetPen() {
	
	return pen;
	
}

float trevts::GetPde() {
	
	return pen_de;
	
}

float trevts::GetPerest() {
	
	return pen_e;
	
}

int trevts::GetAnn() {

	return ann;

}

int trevts::GetSec() {
	
	return sec;
	
}

int trevts::GetQuad() {
	
	return det;
	
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
