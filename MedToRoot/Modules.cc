#include "Modules.hh"

ClassImp(DgfModule)
ClassImp(DgfSubEvent)
ClassImp(AdcModule)
ClassImp(AdcSubEvent)
ClassImp(PatternUnit)
ClassImp(PatternUnitSubEvent)

//DgfModule

DgfModule::DgfModule()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  fModuleNumber = 999;
  fType = 999;
}

DgfModule::~DgfModule()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  fSubEvents.clear();
}

//AdcModule

AdcModule::AdcModule()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  ModuleNumber = 999;
}

AdcModule::~AdcModule()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  SubEvents.clear();
}

//PatternUnit
// -- from Config.C -- 
//kPattAdc1Coinc                  = 0x1 << 0
//kPattAdc1Free                   = 0x1 << 1
//kPattAdc2Coinc                  = 0x1 << 2
//kPattAdc2Free                   = 0x1 << 3
//kPattAdc3Coinc                  = 0x1 << 4
//kPattAdc3Free                   = 0x1 << 5
//kPattAdc4Coinc                  = 0x1 << 6
//kPattAdc4Free                   = 0x1 << 7
//kPattLaserOn 			  = 0x1 << 8
//kPattFieldUp			  = 0x1 << 23
//kPattFieldDown		  = 0x1 << 24
//kPattFieldChange		  = 0x1 << 25

PatternUnit::~PatternUnit()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  SubEvents.clear();
}

unsigned short PatternUnit::GetNumberOfTriggers()
{
  unsigned short trigger = 0;

  for(unsigned int i = 0; i < SubEvents.size(); i++)
    {
      for(unsigned int j = 0; j < SubEvents[i].Size(); j++)
	{
	  //cout<<i<<","<<j<<": trigger = "<<trigger<<", val = "<<hex<<SubEvents[i].GetValue(j)<<", &0 = "<<(SubEvents[i].GetValue(j) & 1<<0)<<", &2 = "<<(SubEvents[i].GetValue(j) & 1<<2)<<", 0 = "<<(1<<0)<<", 2 = "<<(1<<2)<<", test0 = "<<((SubEvents[i].GetValue(j) & 1<<0) == 1<<0)<<", test0 = "<<((SubEvents[i].GetValue(j) & 1<<2) == 1<<2);
	  //first free and coinc trigger is on channel 0 and 1
	  if((SubEvents[i].GetValue(j) & 1<<0) != 0 || (SubEvents[i].GetValue(j) & 1<<1) != 0)
	    trigger++;
	  //second free and coinc trigger is on channel 2 and 3
	  if((SubEvents[i].GetValue(j) & 1<<2) != 0 || (SubEvents[i].GetValue(j) & 1<<3) != 0)
	    trigger++;
	  //cout<<", trigger = "<<dec<<trigger<<endl;
	}
    }

  return trigger;
}

bool PatternUnit::LaserOn()
{
  for(unsigned int i = 0; i < SubEvents.size(); i++)
    {
      for(unsigned int j = 0; j < SubEvents[i].Size(); j++)
	{
	  //Laser bit is on channel 8 (counting from 0)
// 	  cout << "Check Laser Status: " << SubEvents[i].GetValue(j) << endl;
	  if(SubEvents[i].GetValue(j) < 256){//Laser ON
// 	  if(SubEvents[i].GetValue(j) & 1<<8 == 1<<8){
// 	    cout << "True" << endl;
	    return true;
	  }
	}
    }

  return false;
}

bool PatternUnit::FieldUp()
{
  //cout << "vamos bien" << endl;
  for(unsigned int i = 0; i < SubEvents.size(); i++)
    {
      //cout << "y aqui?" << endl;
      for(unsigned int j = 0; j < SubEvents[i].Size(); j++)
	{
	  //Field up bit is on channel 11 (counting from 0)
	  //cout << "y aqui?? Up" << endl;
	  //cout << "SubEvents[" << i << "].GetValue(" << j << "): " << SubEvents[i].GetValue(j) << endl;
	  //if(SubEvents[i].GetValue(j) & 0<<1 << 11 == 0<<1 << 11)
	  //if(SubEvents[i].GetValue(j) >= 2048 && SubEvents[i].GetValue(j) < 4096)
// 	  if(SubEvents[i].GetValue(j) >= 8388608 && SubEvents[i].GetValue(j) < 16777216)
// 	  if(SubEvents[i].GetValue(j) >= 4194304 && SubEvents[i].GetValue(j) < 8388608) //Ramping
	  if(SubEvents[i].GetValue(j) >= 8388608 && SubEvents[i].GetValue(j) < 16777216) //Up
	  //Hit Un in PatternUnit 24
// 	  if(SubEvents[i].GetValue(j) >= 16777216 && SubEvents[i].GetValue(j) < 33554432)
	  {
// 	    cout << "vamos mejor Up" << endl;
	    return true;
	  }
	}
    }

  return false;
}

bool PatternUnit::FieldDown()
{
//   cout << "vamos bien" << endl;
  for(unsigned int i = 0; i < SubEvents.size(); i++)
    {
//       cout << "y aqui?" << endl;
      for(unsigned int j = 0; j < SubEvents[i].Size(); j++)
	{
	  //Field down bit is on channel 10 (counting from 0)
	  //cout << "y aqui?? Down" << endl;
	  //cout << "SubEvents[" << i << "].GetValue(" << j << "): " << SubEvents[i].GetValue(j) << endl;
	  //if(SubEvents[i].GetValue(j) & 0x1 << 12 == 0x1<<12)
	  if(SubEvents[i].GetValue(j) >= 16777216 && SubEvents[i].GetValue(j) < 33554432)
// 	  if(SubEvents[i].GetValue(j) > 33554432)
	  {
// 	    cout << "vamos mejor Down" << endl;
	    return true;
	  }
// 	  if(SubEvents[i].GetValue(j) & 1<<21 == 1<<21){
// 	    cout << "Bit 21" << endl;
// // 	    return true;
// 	  }
// 	  if(SubEvents[i].GetValue(j) & 1<<22 == 1<<22){
// 	    cout << "Bit 22" << endl;
// // 	    return true;
// 	  }
// 	  if(SubEvents[i].GetValue(j) & 1<<23 == 1<<23){
// 	    cout << "Bit 23" << endl;
// // 	    return true;
// 	  }
// 	  if(SubEvents[i].GetValue(j) & 1<<24 == 1<<24){
// 	    cout << "Bit 24" << endl;}
	}
    }

  return false;
}

SISScaler::~SISScaler()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  SubEvents.clear();
}

DgfScaler::~DgfScaler()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  SubEvents.clear();
}

BraggChamber::~BraggChamber()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  SubEvents.clear();
}

