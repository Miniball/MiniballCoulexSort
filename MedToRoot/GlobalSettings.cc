#include "GlobalSettings.hh"

using namespace std;
using namespace TMath;

ClassImp(GlobalSettings)

GlobalSettings::GlobalSettings( unsigned int argc, char* argv[] )
{
  //cout<<__PRETTY_FUNCTION__<<endl;
  //set defaults
  fLastEvent = -1;
  fVerboseLevel = 0;
  fSourceRun = false;
  fIncludeScaler = false;
  fSettings = NULL;

  CommandLineInterface* interface = new CommandLineInterface();

  interface->Add("-le","last event to be read out",&fLastEvent);
  interface->Add("-mf","path/name of .med file",&fMedFile);
  interface->Add("-sf","path/name of settings file",&fSettingsFile);
  interface->Add("-so","source run, ignore ebis window",&fSourceRun);
  interface->Add("-ic","include scaler data into on/offbeam trees",&fIncludeScaler);
  interface->Add("-vl","verbose level",&fVerboseLevel);

  interface->CheckFlags(argc,argv);

  if(fMedFile.empty() || fSettingsFile.empty())
    {
      cerr<<"Error, either the name of the MED-file ("<<fMedFile<<") or of the settings-file ("<<fSettingsFile<<") were not provided!"<<endl;

      exit(1);
    }

  unsigned short i;

  size_t LastDot = fMedFile.rfind('.');

  fOnBeamFile = fMedFile;
  //replace the tailing .med with OnBeam.root
  if(fSourceRun)
    {
      fOnBeamFile.replace(LastDot,(fMedFile.length()-LastDot),"_Source.root");
    }
  else
    {
      fOnBeamFile.replace(LastDot,(fMedFile.length()-LastDot),"_OnBeam.root");
    }

  fOnBeamBackgroundFile = fMedFile;
  //replace the tailing .med with OnBeamBackground.root
  fOnBeamBackgroundFile.replace(LastDot,(fMedFile.length()-LastDot),"_OnBeamBackground.root");

  fOffBeamFile = fMedFile;
  fOffBeamFile.replace(LastDot,(fMedFile.length()-LastDot),"_OffBeam.root");

  fScalerFile = fMedFile;
  fScalerFile.replace(LastDot,(fMedFile.length()-LastDot),"_Scaler.root");

  //read the settings from file
  fSettings = new TEnv(fSettingsFile.c_str());

  fSettings->ReadFile(fSettingsFile.c_str(),kEnvLocal);

  ReadSettings();
  ReadUnpackerSettings();

  if(fVerboseLevel > 0)
    {
      cout<<"Settings are:"<<endl
	  <<"First Miniball Dgf = "<<fFirstMiniballDgf<<endl
	  <<"Last Miniball Dgf = "<<fLastMiniballDgf<<endl
	  <<"Beamdumo Dgf = "<<fBeamdumpDgf<<endl
	  <<"First Adc = "<<fFirstAdc<<endl
	  <<"Last Adc = "<<fLastAdc<<endl
	  <<"Pattern Unit = "<<fPatternUnit<<endl
	  <<"# of Dgfs = "<<fNumberOfDgfModules<<endl
	  <<"# of Adcs = "<<fNumberOfAdcModules<<endl
	  <<"# of Patternunits = "<<fNumberOfPatternUnits<<endl
	  <<"# of Timestamp modules = "<<fNumberOfTimestampModules<<endl;
      for(i = 0; i < fNumberOfTimestampModules; i++)
	{
	  cout<<i+1<<". module: module number = "<<fTimestampModule[i]<<", offset = "<<fTimestampModuleOffset[i]<<endl;
	}
      cout<<"Timestamp Channel = "<<fTimestampChannel<<endl
	  <<"# of Adcs per Timestamp = "<<fNumberOfAdcsPerTimestampModule<<endl
	  <<"Ebis, T1 and SuperCycle module = "<<fEbisT1AndSuperCycleModule<<endl
	  <<"Ebis Channel = "<<fEbisChannel<<endl
	  <<"T1 Channel = "<<fT1Channel<<endl
	  <<"SuperCycle Channel = "<<fSuperCycleChannel<<endl
	  <<"Marabou Dgf Module Offset = "<<fMarabouDgfModuleOffset<<endl
	  <<"Marabou Adc Module Offset = "<<fMarabouAdcModuleOffset<<endl
	  <<"Number of Caens = "<<fNofCaenAdc<<endl
	  <<"Last Event = "<<fLastEvent<<endl
	  <<"Verbose Level = "<<fVerboseLevel<<endl
	  <<".med-file = "<<fMedFile<<endl
	  <<"Onbeam-file = "<<fOnBeamFile<<endl
	  <<"Onbeam-Background-file = "<<fOnBeamBackgroundFile<<endl
	  <<"Offbeam-file = "<<fOffBeamFile<<endl
	  <<"Scaler-file = "<<fScalerFile<<endl;
    }

  if(!Verify())
    {
      cerr<<"Sorry, the settings file "<<fSettingsFile<<" is not of the correct format"<<endl;
      exit(1);
    }
}

GlobalSettings::~GlobalSettings()
{
  //cout<<__PRETTY_FUNCTION__<<endl;
//  if(fVerboseLevel > 2)
//    {
//      cout<<"called ~GlobalSettings with fTimestampModule = "<<fTimestampModule<<" and fTimestampModuleOffset = "<<fTimestampModuleOffset<<endl;
//    }
//
//  if(fTimestampModule != NULL)
//    delete[] fTimestampModule;
//
//  if(fTimestampModuleOffset != NULL)
//    delete[] fTimestampModuleOffset;
//  if(fSettings != NULL)
//    delete fSettings;
}

void GlobalSettings::ReadUnpackerSettings()
{
  if(fSettings == NULL)
    {
      cerr<<__PRETTY_FUNCTION__<<": you need to create a new TEnv first, fSettings in NULL!"<<endl;

      exit(1);
    }

  fCoincidenceWindowWidth = fSettings->GetValue("Coincidence.Window",40.);
  fReferencePointOfCoincidenceWindow = fSettings->GetValue("Coincidence.Reference", 0.);

  fEbisWindowLowerEdge = fSettings->GetValue("EbisWindow.LowerEdge",0);
  fEbisWindowLowerEdge *= 1000/25;//convert from us into dgf timing (25 ns units)
  fEbisWindowUpperEdge = fSettings->GetValue("EbisWindow.UpperEdge",800);
  fEbisWindowUpperEdge *= 1000/25;//convert from us into dgf timing (25 ns units)

  fLowestCoincidence  = (long long) (fReferencePointOfCoincidenceWindow - fCoincidenceWindowWidth/2);
  fHighestCoincidence = (long long) (fReferencePointOfCoincidenceWindow + fCoincidenceWindowWidth/2);

  fMarabouDgfModuleOffset = fSettings->GetValue("Dgf.Offset", 1);
  fMarabouAdcModuleOffset = fSettings->GetValue("Adc.Offset", 55);
  fMarabouPatternUnitOffset = fSettings->GetValue("PatternUnit.Offset", 66);

  fNumberOfPatternUnits = fSettings->GetValue("PatternUnit.NumberOfModules", 1);

  fTimestampChannel = fSettings->GetValue("Timestamp.Channel", 0);
  fNumberOfAdcsPerTimestampModule = fSettings->GetValue("Timestamp.NumberOfAdcs", 3);

  fEbisT1AndSuperCycleModule = fSettings->GetValue("Time.ModuleNumber", 53);
  fEbisChannel = fSettings->GetValue("Time.EbisChannel", 0);
  fT1Channel = fSettings->GetValue("Time.T1Channel", 1);
  fSuperCycleChannel = fSettings->GetValue("Time.SuperCycleChannel", 2);

  fFirstMiniballDgf = fSettings->GetValue("Dgf.FirstMiniballModule", 1);
  fLastMiniballDgf = fSettings->GetValue("Dgf.LastMiniballModule", 9);

  fFirstAdc = fSettings->GetValue("Adc.FirstModule", 11);
  fLastAdc = fSettings->GetValue("Adc.LastModule", 17);

  fBraggChamber = fSettings->GetValue("BraggChamber.Module", 18);
  fScaler = fSettings->GetValue("Scaler.Module", 19);
  fPatternUnit = fSettings->GetValue("PatternUnit.Module", 20);
  fFirstDgfScaler = fSettings->GetValue("Dgf.Scaler.FirstModule", 21);
  fLastDgfScaler = fSettings->GetValue("Dgf.Scaler.LastModule", 23);

  //dgf scaler parameter indices
  fRealTimeIndex = fSettings->GetValue("Dgf.Indices.RealTime", -1);
  fRunTimeIndex = fSettings->GetValue("Dgf.Indices.RunTime", -1);
  fGSLTTimeIndex = fSettings->GetValue("Dgf.Indices.GSLTTime", -1);
  fNumberOfEventsIndex = fSettings->GetValue("Dgf.Indices.NumberOfEvents", -1);
  fChannelIndicesOffset = new unsigned short[fNumberOfDgfChannels];
  for(unsigned short i = 0; i < fNumberOfDgfChannels; i++)
    {
      fChannelIndicesOffset[i] = fSettings->GetValue(Form("Dgf.Indices.Offset.%d",i), -1);
    }
  fLiveTimeIndex = fSettings->GetValue("Dgf.Indices.LiveTime", -1);
  fFastPeakIndex = fSettings->GetValue("Dgf.Indices.FastPeak", -1);

  fBraggTraceLength = fSettings->GetValue("Bragg.Trace.Length", 512);

  fMesytecAdc = fSettings->GetValue("Mesytec.Adc", false);
  fSPEDEChamb = fSettings->GetValue("SPEDE.Chamb", false);
  fNofCaenAdc = fSettings->GetValue("NumberOf.CaenAdc", 0);
  if(fMesytecAdc==false)
    fNofCaenAdc = 0;
  fTypeOfSetup = fSettings->GetValue("TypeOf.Setup", 1);
  fDgfInitDelay = fSettings->GetValue("Dgf.Init.Delay", 65556);
}

void GlobalSettings::ReadSettings()
{
  if(fSettings == NULL)
    {
      cerr<<__PRETTY_FUNCTION__<<": you need to create a new TEnv first, fSettings in NULL!"<<endl;

      exit(1);
    }

  fNumberOfAdcModules = fSettings->GetValue("Adc.NumberOfModules", 7);
  fNumberOfDgfModules = fSettings->GetValue("Dgf.NumberOfModules", 54);
  fNumberOfDgfChannels = fSettings->GetValue("Dgf.NumberOfChannels", 4);

  fNumberOfTimestampModules = fSettings->GetValue("Timestamp.NumberOfModules", 2);

  fTimestampModule = new unsigned short[fNumberOfTimestampModules];
  fTimestampModuleOffset = new double[fNumberOfTimestampModules];

  for(unsigned short i = 0; i < fNumberOfTimestampModules; i++)
    {
      fTimestampModule[i] = fSettings->GetValue(Form("Timestamp.%d.ModuleNumber",i), 49);
      fTimestampModuleOffset[i] = fSettings->GetValue(Form("Timestamp.%d.Offset",i), 0.);
    }

  fBeamdumpDgf = fSettings->GetValue("Dgf.Beamdump.Module", 10);
}

ostream& operator <<(ostream &os,const GlobalSettings &obj)
{
  os<<"Settings are:"<<endl
    <<"First Miniball Dgf = "<<obj.fFirstMiniballDgf<<endl
    <<"Last Miniball Dgf = "<<obj.fLastMiniballDgf<<endl
    <<"Beamdump Dgf = "<<obj.fBeamdumpDgf<<endl
    <<"First Adc = "<<obj.fFirstAdc<<endl
    <<"Last Adc = "<<obj.fLastAdc<<endl
    <<"Pattern Unit = "<<obj.fPatternUnit<<endl
    <<"# of Dgfs = "<<obj.fNumberOfDgfModules<<endl
    <<"# of Adcs = "<<obj.fNumberOfAdcModules<<endl
    <<"# of Patternunits = "<<obj.fNumberOfPatternUnits<<endl
    <<"# of Timestamp modules = "<<obj.fNumberOfTimestampModules<<endl;
  for(unsigned short i = 0; i < obj.fNumberOfTimestampModules; i++)
    {
      os<<i+1<<". module: module number = "<<obj.fTimestampModule[i]<<", offset = "<<obj.fTimestampModuleOffset[i]<<endl;
    }
  os<<"Timestamp Channel = "<<obj.fTimestampChannel<<endl
    <<"# of Adcs per Timestamp = "<<obj.fNumberOfAdcsPerTimestampModule<<endl
    <<"Ebis, T1 and SuperCycle module = "<<obj.fEbisT1AndSuperCycleModule<<endl
    <<"Ebis Channel = "<<obj.fEbisChannel<<endl
    <<"T1 Channel = "<<obj.fT1Channel<<endl
    <<"SuperCycle Channel = "<<obj.fSuperCycleChannel<<endl
    <<"Marabou Dgf Module Offset = "<<obj.fMarabouDgfModuleOffset<<endl
    <<"Marabou Adc Module Offset = "<<obj.fMarabouAdcModuleOffset<<endl
    <<"Last Event = "<<obj.fLastEvent<<endl
    <<"Verbose Level = "<<obj.fVerboseLevel<<endl
    <<".med-file = "<<obj.fMedFile<<endl
    <<"Onbeam-file = "<<obj.fOnBeamFile<<endl
    <<"Onbeam-Background-file = "<<obj.fOnBeamBackgroundFile<<endl
    <<"Offbeam-file = "<<obj.fOffBeamFile<<endl
    <<"Scaler-file = "<<obj.fScalerFile<<endl;

  return os;
}

bool GlobalSettings::Verify()
{
  //check that all timestamp modules are dgfs
  for(unsigned short i=0;i<fNumberOfTimestampModules;i++)
    {
      if(fTimestampModule[i] > fNumberOfDgfModules)
	{
	  cerr<<__PRETTY_FUNCTION__<<": Error, inconsistent settings: TimestampModule "<<i<<" = "<<fTimestampModule[i]<<", but total number of dgf modules is only "<<fNumberOfDgfModules<<endl;

	  return false;
	}
    }

  //check that the number of modules are consistent
  if(fNumberOfAdcModules != fLastAdc - fFirstAdc + 1)
    {
      cerr<<"number of adcs wrong"<<endl;
      return false;
    }

  //check that module number ranges make sense
  if(fLastMiniballDgf < fFirstMiniballDgf)
    {
      cerr<<"last miniball wrong"<<endl;
      return false;
    }

  if(fLastAdc < fFirstAdc)
    {
      cerr<<"last adc wrong"<<endl;
      return false;
    }

  //check that module numbers don't overlap
  if(fFirstAdc <= fLastMiniballDgf)
    {
      cerr<<"first adc wrong"<<endl;
      return false;
    }

  if(fBeamdumpDgf <= fLastMiniballDgf && !fSPEDEChamb && fBeamdumpDgf > 0)
    {
      cerr<<"beam dump wrong"<<endl;
      return false;
    }

  if(fFirstAdc <= fLastMiniballDgf)
    {
      cerr<<"first adc wrong"<<endl;
      return false;
    }

  if(fFirstAdc <= fBeamdumpDgf && fBeamdumpDgf > 0)
    {
      cerr<<"first adc/beam dump wrong"<<endl;
      return false;
    }

  if(fPatternUnit <= fLastMiniballDgf)
    {
      cerr<<"pattern unit wrong"<<endl;
      return false;
    }

  if(fPatternUnit <= fBeamdumpDgf && fBeamdumpDgf > 0)
    {
      cerr<<"pattern unit/beam dump wrong"<<endl;
      return false;
    }

  if(fPatternUnit <= fLastAdc)
    {
      cerr<<"pattern unit/last adc wrong"<<endl;
      return false;
    }

  // check that the number of adcs is either exactly the number of adcs per timestamp * number of timestamps or one more (for the collimator/diamond detector)
//  cout << " fNumberOfAdcModules = " << fNumberOfAdcModules << endl;
//  cout<<" fNumberOfAdcsPerTimestampModule = "<<fNumberOfAdcsPerTimestampModule<<"; fNumberOfTimestampModules = "<<fNumberOfTimestampModules<<"; fNofCaenAdc = "<<fNofCaenAdc<<endl;
//  cout << " fNumberOfAdcsPerTimestampModule * fNumberOfTimestampModules = " << fNumberOfAdcsPerTimestampModule * fNumberOfTimestampModules << endl;
//  cout << " fNumberOfAdcsPerTimestampModule * fNumberOfTimestampModules + fNofCaenAdc = " << fNumberOfAdcsPerTimestampModule * fNumberOfTimestampModules + fNofCaenAdc << endl;
  if(fNumberOfAdcModules != fNumberOfAdcsPerTimestampModule * fNumberOfTimestampModules && fNumberOfAdcModules != fNumberOfAdcsPerTimestampModule * fNumberOfTimestampModules + fNofCaenAdc + fTypeOfSetup && !fSPEDEChamb)
    {
      cerr<<"number of adcs doesn't match timestamps vamos casi lo tienes"<<endl;
      return false;
    }

  if(fRealTimeIndex == 0xffff)
    {
      cerr<<"real time wrong"<<endl;
      return false;
    }
  if(fRunTimeIndex == 0xffff)
    {
      cerr<<"run time wrong"<<endl;
      return false;
    }
  if(fGSLTTimeIndex == 0xffff)
    {
      cerr<<"GSLT time wrong"<<endl;
      return false;
    }
  if(fNumberOfEventsIndex == 0xffff)
    {
      cerr<<"nof events wrong"<<endl;
      return false;
    }
  for(unsigned short i = 0; i < fNumberOfDgfChannels; i++)
    {
      if(fChannelIndicesOffset[i] == 0xffff)
      {
	cerr<<"channel "<<i<<" wrong"<<endl;
	return false;
      }
    }
  if(fLiveTimeIndex == 0xffff)
    {
      cerr<<"live time wrong"<<endl;
      return false;
    }
  if(fFastPeakIndex == 0xffff)
    {
      cerr<<"fast peak wrong"<<endl;
      return false;
    }

  return true;
}
