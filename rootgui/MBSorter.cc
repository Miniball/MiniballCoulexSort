// Very basic ROOT GUI to perform sorting of Miniball data
//
// Liam Gaffney (liam.gaffney@cern.ch) - 02/05/2017

#include "MBSorter.hh"

#include <iostream>
#include <string>
#include <TString.h>
#include <TSystem.h>

using namespace std;

MBSorter::MBSorter() {
	
	///////////////////////
	// Create main frame //
	///////////////////////
	main_frame = new TGMainFrame( gClient->GetRoot(), 1300, 360, kMainFrame | kHorizontalFrame );

	// terminate ROOT session when window is closed
	main_frame->Connect( "CloseWindow()", "TApplication", gApplication, "Terminate()" );
	main_frame->DontCallClose();
	

	//////////////////////////////////////
	// Create sub frames and separators //
	//////////////////////////////////////

	// Left frame - Run list box
	left_frame = new TGVerticalFrame( main_frame, 250, 360 );
	left_frame->SetName( "left_frame" );
	main_frame->AddFrame( left_frame, new TGLayoutHints( kLHintsLeft ) );
	
	// Vertical separator
	fVertical_0 = new TGVertical3DLine( main_frame, 8, 360 );
	fVertical_0->SetName( "fVertical_0" );
	main_frame->AddFrame( fVertical_0, new TGLayoutHints( kLHintsLeft | kLHintsTop, 2, 2, 2, 2 ) );
	
	// Centre frame - Files, directories, buttons, etc
	centre_frame = new TGVerticalFrame( main_frame, 800, 360 );
	centre_frame->SetName( "centre_frame" );
	main_frame->AddFrame( centre_frame, new TGLayoutHints( kLHintsCenterX ) );
	
	// Vertical separator
	fVertical_1 = new TGVertical3DLine( main_frame, 8, 360 );
	fVertical_1->SetName( "fVertical_1" );
	main_frame->AddFrame( fVertical_1, new TGLayoutHints( kLHintsLeft | kLHintsTop, 2, 2, 2, 2 ) );
	
	// Right frame - Experimental definitions and Doppler
	right_frame = new TGHorizontalFrame( main_frame, 100, 360 );
	right_frame->SetName( "right_frame" );
	main_frame->AddFrame( right_frame, new TGLayoutHints( kLHintsRight ) );
	
	// Centre frame 0 - rsync
	comp_frame_0 = new TGCompositeFrame( centre_frame, 800, 90, kHorizontalFrame | kFixedWidth );
	comp_frame_0->SetName( "comp_frame_0" );
	centre_frame->AddFrame( comp_frame_0, new TGLayoutHints( kLHintsExpandX ) );
	
	// Horizontal separator 0
	fHorizontal_0 = new TGHorizontal3DLine( centre_frame, 800, 8 );
	fHorizontal_0->SetName( "fHorizontal_0" );
	centre_frame->AddFrame( fHorizontal_0, new TGLayoutHints( kLHintsLeft | kLHintsTop, 2, 2, 2, 2 ) );
	
	// Centre frame 1 - MedToRoot
	comp_frame_1 = new TGCompositeFrame( centre_frame, 800, 90, kHorizontalFrame | kFixedWidth );
	comp_frame_1->SetName( "comp_frame_1" );
	centre_frame->AddFrame( comp_frame_1, new TGLayoutHints( kLHintsExpandX ) );
	
	// Horizontal separator 1
	fHorizontal_1 = new TGHorizontal3DLine( centre_frame, 800, 8 );
	fHorizontal_1->SetName( "fHorizontal_1" );
	centre_frame->AddFrame( fHorizontal_1, new TGLayoutHints( kLHintsLeft | kLHintsTop, 2, 2, 2, 2 ) );
	
	// Centre frame 2 - TreeBuilder
	comp_frame_2 = new TGCompositeFrame( centre_frame, 800, 135, kHorizontalFrame | kFixedWidth );
	comp_frame_2->SetName( "comp_frame_2" );
	centre_frame->AddFrame( comp_frame_2, new TGLayoutHints( kLHintsExpandX ) );
	
	// Horizontal separator 2
	fHorizontal_2 = new TGHorizontal3DLine( centre_frame, 800, 8 );
	fHorizontal_2->SetName( "fHorizontal_2" );
	centre_frame->AddFrame( fHorizontal_2, new TGLayoutHints( kLHintsLeft | kLHintsTop, 2, 2, 2, 2 ) );
	
	// Centre frame 3 - CLXAna
	comp_frame_3 = new TGCompositeFrame( centre_frame, 800, 180, kHorizontalFrame | kFixedWidth );
	comp_frame_3->SetName( "comp_frame_3" );
	centre_frame->AddFrame( comp_frame_3, new TGLayoutHints( kLHintsExpandX ) );
	
	// Sub frame 0 - Directory boxes
	sub_frame_0 = new TGVerticalFrame( comp_frame_0, 700, 90 );
	sub_frame_0->SetName( "sub_frame_0" );
	comp_frame_0->AddFrame( sub_frame_0, new TGLayoutHints( kLHintsLeft ) );
	
	// Sub frame 1 - DAQ directory box
	sub_frame_1 = new TGHorizontalFrame( sub_frame_0, 700, 45 );
	sub_frame_1->SetName( "sub_frame_1" );
	sub_frame_0->AddFrame( sub_frame_1, new TGLayoutHints( kLHintsTop ) );
	
	// Sub frame 2 - Local directory box
	sub_frame_2 = new TGHorizontalFrame( sub_frame_0, 700, 45 );
	sub_frame_2->SetName( "sub_frame_2" );
	sub_frame_0->AddFrame( sub_frame_2, new TGLayoutHints( kLHintsBottom ) );
	
	// Sub frame 3 - File button box
	sub_frame_3 = new TGHorizontalFrame( left_frame, 150, 50 );
	sub_frame_3->SetName( "sub_frame_3" );
	left_frame->AddFrame( sub_frame_3, new TGLayoutHints( kLHintsBottom | kLHintsExpandX ) );
	
	// Sub frame 4 - MedToRoot boxes
	sub_frame_4 = new TGVerticalFrame( comp_frame_1, 700, 90 );
	sub_frame_4->SetName( "sub_frame_4" );
	comp_frame_1->AddFrame( sub_frame_4, new TGLayoutHints( kLHintsLeft ) );
	
	// Sub frame 5 - Settings file
	sub_frame_5 = new TGHorizontalFrame( sub_frame_4, 700, 45 );
	sub_frame_5->SetName( "sub_frame_5" );
	sub_frame_4->AddFrame( sub_frame_5, new TGLayoutHints( kLHintsTop | kLHintsExpandX ) );
	
	// Sub frame 6 - Conversion options for MedToRoot
	sub_frame_6 = new TGHorizontalFrame( sub_frame_4, 700, 45 );
	sub_frame_6->SetName( "sub_frame_6" );
	sub_frame_4->AddFrame( sub_frame_6, new TGLayoutHints( kLHintsBottom | kLHintsExpandX ) );
	
	// Sub frame 7 - TreeBuilder boxes
	sub_frame_7 = new TGVerticalFrame( comp_frame_2, 700, 90 );
	sub_frame_7->SetName( "sub_frame_7" );
	comp_frame_2->AddFrame( sub_frame_7, new TGLayoutHints( kLHintsLeft ) );
	
	// Sub frame 8 - Output filename base
	sub_frame_8 = new TGHorizontalFrame( sub_frame_7, 700, 45 );
	sub_frame_8->SetName( "sub_frame_8" );
	sub_frame_7->AddFrame( sub_frame_8, new TGLayoutHints( kLHintsTop | kLHintsExpandX ) );
	
	// Sub frame 9 - Calibration file
	sub_frame_9 = new TGHorizontalFrame( sub_frame_7, 700, 45 );
	sub_frame_9->SetName( "sub_frame_9" );
	sub_frame_7->AddFrame( sub_frame_9, new TGLayoutHints( kLHintsTop | kLHintsExpandX ) );
	
	// Sub frame 10 - TreeBuilder check buttons
	sub_frame_10 = new TGHorizontalFrame( sub_frame_7, 700, 45 );
	sub_frame_10->SetName( "sub_frame_10" );
	sub_frame_7->AddFrame( sub_frame_10, new TGLayoutHints( kLHintsBottom | kLHintsExpandX ) );
	
	// Sub frame 12 - AQ4Sort and NeSort action buttons
	sub_frame_12 = new TGVerticalFrame( comp_frame_2, 100, 135 );
	sub_frame_12->SetName( "sub_frame_12" );
	comp_frame_2->AddFrame( sub_frame_12, new TGLayoutHints( kLHintsBottom | kLHintsExpandY ) );
	
	// Sub frame 11 - Analysis boxes
	sub_frame_11 = new TGVerticalFrame( comp_frame_3, 700, 180 );
	sub_frame_11->SetName( "sub_frame_11" );
	comp_frame_3->AddFrame( sub_frame_11, new TGLayoutHints( kLHintsLeft ) );
	
	// Sub frame 13 - Kinematic cut file
	sub_frame_13 = new TGHorizontalFrame( sub_frame_11, 700, 45 );
	sub_frame_13->SetName( "sub_frame_13" );
	sub_frame_11->AddFrame( sub_frame_13, new TGLayoutHints( kLHintsTop | kLHintsExpandX ) );
	
	// Sub frame 14 - SRIM directory
	sub_frame_14 = new TGHorizontalFrame( sub_frame_11, 700, 45 );
	sub_frame_14->SetName( "sub_frame_14" );
	sub_frame_11->AddFrame( sub_frame_14, new TGLayoutHints( kLHintsTop | kLHintsExpandX ) );
	
	// Sub frame 15 - Analysis check buttons
	sub_frame_15 = new TGHorizontalFrame( sub_frame_11, 700, 45 );
	sub_frame_15->SetName( "sub_frame_15" );
	sub_frame_11->AddFrame( sub_frame_15, new TGLayoutHints( kLHintsTop | kLHintsExpandX ) );
	
	// Sub frame 16 - Open/save configuration buttons
	sub_frame_16 = new TGHorizontalFrame( left_frame, 700, 45 );
	sub_frame_16->SetName( "sub_frame_16" );
	left_frame->AddFrame( sub_frame_16, new TGLayoutHints( kLHintsTop | kLHintsExpandX ) );
	
	// Doppler frame 0 - Labels
	dop_frame_0 = new TGVerticalFrame( right_frame, 30, 360 );
	dop_frame_0->SetName( "dop_frame_0" );
	right_frame->AddFrame( dop_frame_0, new TGLayoutHints( kLHintsLeft ) );
	
	// Doppler frame 1 - Labels
	dop_frame_1 = new TGVerticalFrame( right_frame, 30, 360 );
	dop_frame_1->SetName( "dop_frame_1" );
	right_frame->AddFrame( dop_frame_1, new TGLayoutHints( kLHintsCenterX ) );
	
	// Doppler frame 2 - Labels
	dop_frame_2 = new TGVerticalFrame( right_frame, 30, 360 );
	dop_frame_2->SetName( "dop_frame_2" );
	right_frame->AddFrame( dop_frame_2, new TGLayoutHints( kLHintsRight ) );
	
	
	/////////////////////////////
	// Open/Save config button //
	/////////////////////////////
	
	// Setup file open
	but_open = new TGTextButton( sub_frame_16, "Open setup", -1, TGTextButton::GetDefaultGC()(),
								 TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_open->SetTextJustify( 36 );
	but_open->SetMargins( 0, 0, 0, 0 );
	but_open->SetWrapLength( -1 );
	but_open->Resize( 25, 30 );
	sub_frame_16->AddFrame( but_open, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	// Setup file save
	but_save = new TGTextButton( sub_frame_16, "Save setup", -1, TGTextButton::GetDefaultGC()(),
								TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_save->SetTextJustify( 36 );
	but_save->SetMargins( 0, 0, 0, 0 );
	but_save->SetWrapLength( -1 );
	but_save->Resize( 25, 30 );
	sub_frame_16->AddFrame( but_save, new TGLayoutHints( kLHintsRight | kLHintsExpandX, 2, 2, 2, 2 ) );

	///////////////////
	// Create labels //
	///////////////////
	
	// Run files
	lab_run_files = new TGLabel( left_frame, "List of file names" );
	lab_run_files->SetTextJustify( 36 );
	lab_run_files->SetMargins( 0, 0, 0, 0 );
	lab_run_files->SetWrapLength( -1 );
	lab_run_files->Resize( 80, lab_run_files->GetDefaultHeight() );
	left_frame->AddFrame( lab_run_files,
						 new TGLayoutHints( kLHintsCenterX | kLHintsTop, 2, 2, 2, 2 ) );
	
	// DAQ directory
	lab_daq_dir = new TGLabel( sub_frame_1, "DAQ directory" );
	lab_daq_dir->SetTextJustify( 36 );
	lab_daq_dir->SetMargins( 0, 0, 0, 0 );
	lab_daq_dir->SetWrapLength( -1 );
	lab_daq_dir->Resize( 80, lab_daq_dir->GetDefaultHeight() );
	sub_frame_1->AddFrame( lab_daq_dir,
						   new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );
	
	// Local directory
	lab_local_dir = new TGLabel( sub_frame_2, "Local directory" );
	lab_local_dir->SetTextJustify( 36 );
	lab_local_dir->SetMargins( 0, 0, 0, 0 );
	lab_local_dir->SetWrapLength( -1 );
	lab_local_dir->Resize( 80, lab_local_dir->GetDefaultHeight() );
	sub_frame_2->AddFrame( lab_local_dir,
						  new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );
	
	// Settings file
	lab_settings = new TGLabel( sub_frame_5, "Settings file" );
	lab_settings->SetTextJustify( 36 );
	lab_settings->SetMargins( 0, 0, 0, 0 );
	lab_settings->SetWrapLength( -1 );
	lab_settings->Resize( 80, lab_settings->GetDefaultHeight() );
	sub_frame_5->AddFrame( lab_settings,
						  new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );
	
	// First entry to convert from med to root
	lab_first_entry = new TGLabel( sub_frame_6, "First entry" );
	lab_first_entry->SetTextJustify( 36 );
	lab_first_entry->SetMargins( 0, 0, 0, 0 );
	lab_first_entry->SetWrapLength( -1 );
	lab_first_entry->Resize( 80, lab_first_entry->GetDefaultHeight() );
	
	// Last entry to convert from med to root
	lab_last_entry = new TGLabel( sub_frame_6, "Last entry" );
	lab_last_entry->SetTextJustify( 36 );
	lab_last_entry->SetMargins( 0, 0, 0, 0 );
	lab_last_entry->SetWrapLength( -1 );
	lab_last_entry->Resize( 80, lab_last_entry->GetDefaultHeight() );
	
	// Output filename: just the base, used for <base>_tree.root and <base>_hists.root
	lab_outfile = new TGLabel( sub_frame_8, "Output filename" );
	lab_outfile->SetTextJustify( 36 );
	lab_outfile->SetMargins( 0, 0, 0, 0 );
	lab_outfile->SetWrapLength( -1 );
	lab_outfile->Resize( 80, lab_outfile->GetDefaultHeight() );
	sub_frame_8->AddFrame( lab_outfile,
						  new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );
	
	// Output filename suffix to be displayed
	lab_outsuffix = new TGLabel( sub_frame_8, "_tree.root" );
	lab_outsuffix->SetTextJustify( 36 );
	lab_outsuffix->SetMargins( 0, 0, 0, 0 );
	lab_outsuffix->SetWrapLength( -1 );
	lab_outsuffix->Resize( 16, lab_outsuffix->GetDefaultHeight() );
	sub_frame_8->AddFrame( lab_outsuffix,
						  new TGLayoutHints( kLHintsRight | kFixedWidth, 0, 2, 2, 2 ) );
	
	// Calibration file
	lab_calfile = new TGLabel( sub_frame_9, "Calibration filename" );
	lab_calfile->SetTextJustify( 36 );
	lab_calfile->SetMargins( 0, 0, 0, 0 );
	lab_calfile->SetWrapLength( -1 );
	lab_calfile->Resize( 80, lab_calfile->GetDefaultHeight() );
	sub_frame_9->AddFrame( lab_calfile,
						  new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );
	
	
	// Kinematic cut file
	lab_cutfile = new TGLabel( sub_frame_13, "Kinematic cuts file" );
	lab_cutfile->SetTextJustify( 36 );
	lab_cutfile->SetMargins( 0, 0, 0, 0 );
	lab_cutfile->SetWrapLength( -1 );
	lab_cutfile->Resize( 80, lab_cutfile->GetDefaultHeight() );
	sub_frame_13->AddFrame( lab_cutfile,
						  new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );
	
	// SRIM directory
	lab_srimdir = new TGLabel( sub_frame_14, "SRIM file directory" );
	lab_srimdir->SetTextJustify( 36 );
	lab_srimdir->SetMargins( 0, 0, 0, 0 );
	lab_srimdir->SetWrapLength( -1 );
	lab_srimdir->Resize( 80, lab_srimdir->GetDefaultHeight() );
	sub_frame_14->AddFrame( lab_srimdir,
						  new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );
	
	// Doppler - Zb
	lab_dop_zb = new TGLabel( dop_frame_0, "Z_b =" );
	lab_dop_zb->SetTextJustify( kTextRight ); // not working!!
	lab_dop_zb->SetMargins( 0, 0, 0, 0 );
	lab_dop_zb->SetWrapLength( -1 );
	lab_dop_zb->Resize( 40, lab_dop_zb->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_zb,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Zb
	lab_unit_zb = new TGLabel( dop_frame_2, "" );
	lab_unit_zb->SetTextJustify( 36 );
	lab_unit_zb->SetMargins( 0, 0, 0, 0 );
	lab_unit_zb->SetWrapLength( -1 );
	lab_unit_zb->Resize( 40, lab_unit_zb->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_zb,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - Zt
	lab_dop_zt = new TGLabel( dop_frame_0, "Z_t =" );
	lab_dop_zt->SetTextJustify( kTextRight );
	lab_dop_zt->SetMargins( 0, 0, 0, 0 );
	lab_dop_zt->SetWrapLength( -1 );
	lab_dop_zt->Resize( 40, lab_dop_zt->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_zt,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Zt
	lab_unit_zt = new TGLabel( dop_frame_2, "" );
	lab_unit_zt->SetTextJustify( 36 );
	lab_unit_zt->SetMargins( 0, 0, 0, 0 );
	lab_unit_zt->SetWrapLength( -1 );
	lab_unit_zt->Resize( 40, lab_unit_zt->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_zt,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - Ab
	lab_dop_ab = new TGLabel( dop_frame_0, "A_b =" );
	lab_dop_ab->SetTextJustify( kTextRight ); // not working!!
	lab_dop_ab->SetMargins( 0, 0, 0, 0 );
	lab_dop_ab->SetWrapLength( -1 );
	lab_dop_ab->Resize( 40, lab_dop_ab->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_ab,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Ab
	lab_unit_ab = new TGLabel( dop_frame_2, "" );
	lab_unit_ab->SetTextJustify( 36 );
	lab_unit_ab->SetMargins( 0, 0, 0, 0 );
	lab_unit_ab->SetWrapLength( -1 );
	lab_unit_ab->Resize( 40, lab_unit_ab->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_ab,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - At
	lab_dop_at = new TGLabel( dop_frame_0, "A_t =" );
	lab_dop_at->SetTextJustify( kTextRight );
	lab_dop_at->SetMargins( 0, 0, 0, 0 );
	lab_dop_at->SetWrapLength( -1 );
	lab_dop_at->Resize( 40, lab_dop_at->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_at,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - At
	lab_unit_at = new TGLabel( dop_frame_2, "" );
	lab_unit_at->SetTextJustify( 36 );
	lab_unit_at->SetMargins( 0, 0, 0, 0 );
	lab_unit_at->SetWrapLength( -1 );
	lab_unit_at->Resize( 40, lab_unit_at->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_at,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - Eb
	lab_dop_eb = new TGLabel( dop_frame_0, "E_b =" );
	lab_dop_eb->SetTextJustify( kTextRight );
	lab_dop_eb->SetMargins( 0, 0, 0, 0 );
	lab_dop_eb->SetWrapLength( -1 );
	lab_dop_eb->Resize( 40, lab_dop_eb->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_eb,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Eb
	lab_unit_eb = new TGLabel( dop_frame_2, "keV/u" );
	lab_unit_eb->SetTextJustify( 36 );
	lab_unit_eb->SetMargins( 0, 0, 0, 0 );
	lab_unit_eb->SetWrapLength( -1 );
	lab_unit_eb->Resize( 40, lab_unit_eb->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_eb,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - Ex
	lab_dop_ex = new TGLabel( dop_frame_0, "E_x =" );
	lab_dop_ex->SetTextJustify( kTextRight );
	lab_dop_ex->SetMargins( 0, 0, 0, 0 );
	lab_dop_ex->SetWrapLength( -1 );
	lab_dop_ex->Resize( 40, lab_dop_ex->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_ex,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Ex
	lab_unit_ex = new TGLabel( dop_frame_2, "keV" );
	lab_unit_ex->SetTextJustify( 36 );
	lab_unit_ex->SetMargins( 0, 0, 0, 0 );
	lab_unit_ex->SetWrapLength( -1 );
	lab_unit_ex->Resize( 40, lab_unit_ex->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_ex,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - Target thickness
	lab_dop_th = new TGLabel( dop_frame_0, "Target thickness" );
	lab_dop_th->SetTextJustify( kTextRight );
	lab_dop_th->SetMargins( 0, 0, 0, 0 );
	lab_dop_th->SetWrapLength( -1 );
	lab_dop_th->Resize( 40, lab_dop_th->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_th,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Target thickness
	lab_unit_th = new TGLabel( dop_frame_2, "mg/cm^2" );
	lab_unit_th->SetTextJustify( 36 );
	lab_unit_th->SetMargins( 0, 0, 0, 0 );
	lab_unit_th->SetWrapLength( -1 );
	lab_unit_th->Resize( 40, lab_unit_th->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_th,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - Interaction depth
	lab_dop_id = new TGLabel( dop_frame_0, "Interaction depth" );
	lab_dop_id->SetTextJustify( kTextRight );
	lab_dop_id->SetMargins( 0, 0, 0, 0 );
	lab_dop_id->SetWrapLength( -1 );
	lab_dop_id->Resize( 40, lab_dop_id->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_id,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Interaction depth
	lab_unit_id = new TGLabel( dop_frame_2, "mg/cm^2" );
	lab_unit_id->SetTextJustify( 36 );
	lab_unit_id->SetMargins( 0, 0, 0, 0 );
	lab_unit_id->SetWrapLength( -1 );
	lab_unit_id->Resize( 40, lab_unit_id->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_id,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - CD distance
	lab_dop_cd = new TGLabel( dop_frame_0, "CD distance" );
	lab_dop_cd->SetTextJustify( kTextRight );
	lab_dop_cd->SetMargins( 0, 0, 0, 0 );
	lab_dop_cd->SetWrapLength( -1 );
	lab_dop_cd->Resize( 40, lab_dop_cd->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_cd,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - CD distance
	lab_unit_cd = new TGLabel( dop_frame_2, "mm" );
	lab_unit_cd->SetTextJustify( 36 );
	lab_unit_cd->SetMargins( 0, 0, 0, 0 );
	lab_unit_cd->SetWrapLength( -1 );
	lab_unit_cd->Resize( 40, lab_unit_cd->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_cd,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - CD rotation
	lab_dop_cd = new TGLabel( dop_frame_0, "CD rotation" );
	lab_dop_cd->SetTextJustify( kTextRight );
	lab_dop_cd->SetMargins( 0, 0, 0, 0 );
	lab_dop_cd->SetWrapLength( -1 );
	lab_dop_cd->Resize( 40, lab_dop_cd->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_cd,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - CD rotation
	lab_unit_ro = new TGLabel( dop_frame_2, "deg" );
	lab_unit_ro->SetTextJustify( 36 );
	lab_unit_ro->SetMargins( 0, 0, 0, 0 );
	lab_unit_ro->SetWrapLength( -1 );
	lab_unit_ro->Resize( 40, lab_unit_ro->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_ro,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - Dead layer
	lab_dop_dl = new TGLabel( dop_frame_0, "Si dead layer" );
	lab_dop_dl->SetTextJustify( kTextRight );
	lab_dop_dl->SetMargins( 0, 0, 0, 0 );
	lab_dop_dl->SetWrapLength( -1 );
	lab_dop_dl->Resize( 40, lab_dop_dl->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_dl,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Dead layer
	lab_unit_dl = new TGLabel( dop_frame_2, "mm" );
	lab_unit_dl->SetTextJustify( 36 );
	lab_unit_dl->SetMargins( 0, 0, 0, 0 );
	lab_unit_dl->SetWrapLength( -1 );
	lab_unit_dl->Resize( 40, lab_unit_dl->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_dl,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - Spede distance
	lab_dop_sp = new TGLabel( dop_frame_0, "Spede distance" );
	lab_dop_sp->SetTextJustify( kTextRight );
	lab_dop_sp->SetMargins( 0, 0, 0, 0 );
	lab_dop_sp->SetWrapLength( -1 );
	lab_dop_sp->Resize( 40, lab_dop_sp->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_sp,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Spede distance
	lab_unit_sp = new TGLabel( dop_frame_2, "mm" );
	lab_unit_sp->SetTextJustify( 36 );
	lab_unit_sp->SetMargins( 0, 0, 0, 0 );
	lab_unit_sp->SetWrapLength( -1 );
	lab_unit_sp->Resize( 40, lab_unit_sp->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_sp,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - Plunger distance
	lab_dop_pd = new TGLabel( dop_frame_0, "Plunger distance" );
	lab_dop_pd->SetTextJustify( kTextRight );
	lab_dop_pd->SetMargins( 0, 0, 0, 0 );
	lab_dop_pd->SetWrapLength( -1 );
	lab_dop_pd->Resize( 40, lab_dop_pd->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_pd,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Plunger distance
	lab_unit_pd = new TGLabel( dop_frame_2, "µm" );
	lab_unit_pd->SetTextJustify( 36 );
	lab_unit_pd->SetMargins( 0, 0, 0, 0 );
	lab_unit_pd->SetWrapLength( -1 );
	lab_unit_pd->Resize( 40, lab_unit_pd->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_pd,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	
	// Doppler - Background fraction
	lab_dop_bg = new TGLabel( dop_frame_0, "Background fraction" );
	lab_dop_bg->SetTextJustify( kTextRight );
	lab_dop_bg->SetMargins( 0, 0, 0, 0 );
	lab_dop_bg->SetWrapLength( -1 );
	lab_dop_bg->Resize( 40, lab_dop_bg->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_bg,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Background fraction
	lab_unit_bg = new TGLabel( dop_frame_2, "" );
	lab_unit_bg->SetTextJustify( 36 );
	lab_unit_bg->SetMargins( 0, 0, 0, 0 );
	lab_unit_bg->SetWrapLength( -1 );
	lab_unit_bg->Resize( 40, lab_unit_bg->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_bg,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	
	/////////////////////
	// Create list box //
	/////////////////////
	
	// Run files
	run_list_box = new TGListBox( left_frame, -1, kSunkenFrame );
	run_list_box->SetName( "run_list_box" );
	run_list_box->Resize( 250, 256 );
	left_frame->AddFrame( run_list_box, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	run_selected = new TList;
	run_list_box->SetMultipleSelections( true );
	
	/////////////////////////
	// Create text entries //
	/////////////////////////
	
	// Add file
	text_add_file = new TGTextEntry( left_frame );
	text_add_file->SetMaxLength( 4096 );
	text_add_file->SetAlignment( kTextLeft );
	text_add_file->Resize( 250, text_add_file->GetDefaultHeight() );
	left_frame->AddFrame( text_add_file, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	// DAQ directory
	text_daq_dir = new TGTextEntry( sub_frame_1 );
	text_daq_dir->SetMaxLength( 4096 );
	text_daq_dir->SetAlignment( kTextLeft );
	text_daq_dir->Resize( 410, text_daq_dir->GetDefaultHeight() );
	sub_frame_1->AddFrame( text_daq_dir, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	// Med file prefix - label
	lab_med_pre = new TGLabel( sub_frame_1, "prefix" );
	lab_med_pre->SetTextJustify( 36 );
	lab_med_pre->SetMargins( 0, 0, 0, 0 );
	lab_med_pre->SetWrapLength( -1 );
	lab_med_pre->Resize( 40, lab_med_pre->GetDefaultHeight() );
	sub_frame_1->AddFrame( lab_med_pre,
						   new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );

	// Med file prefix
	text_med_pre = new TGTextEntry( sub_frame_1 );
	text_med_pre->SetMaxLength( 4096 );
	text_med_pre->SetAlignment( kTextLeft );
	text_med_pre->Resize( 60, text_med_pre->GetDefaultHeight() );
	sub_frame_1->AddFrame( text_med_pre, new TGLayoutHints( kLHintsRight , 2, 2, 2, 2 ) );
	
	// Local directory
	text_local_dir = new TGTextEntry( sub_frame_2 );
	text_local_dir->SetMaxLength( 4096 );
	text_local_dir->SetAlignment( kTextLeft );
	text_local_dir->Resize( 500, text_local_dir->GetDefaultHeight() );
	sub_frame_2->AddFrame( text_local_dir, new TGLayoutHints( kLHintsRight | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	// Settings files
	text_settings = new TGTextEntry( sub_frame_5 );
	text_settings->SetMaxLength( 4096 );
	text_settings->SetAlignment( kTextLeft );
	text_settings->Resize( 500, text_settings->GetDefaultHeight() );
	sub_frame_5->AddFrame( text_settings, new TGLayoutHints( kLHintsRight | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	// Output filename
	text_outfile = new TGTextEntry( sub_frame_8 );
	text_outfile->SetMaxLength( 4096 );
	text_outfile->SetAlignment( kTextLeft );
	text_outfile->Resize( 500, text_outfile->GetDefaultHeight() );
	sub_frame_8->AddFrame( text_outfile, new TGLayoutHints( kLHintsCenterX | kLHintsExpandX, 2, 0, 2, 2 ) );
	
	// Calibration file
	text_calfile = new TGTextEntry( sub_frame_9 );
	text_calfile->SetMaxLength( 4096 );
	text_calfile->SetAlignment( kTextLeft );
	text_calfile->Resize( 500, text_calfile->GetDefaultHeight() );
	sub_frame_9->AddFrame( text_calfile, new TGLayoutHints( kLHintsRight | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	// Kinematic cut file
	text_cutfile = new TGTextEntry( sub_frame_13 );
	text_cutfile->SetMaxLength( 4096 );
	text_cutfile->SetAlignment( kTextLeft );
	text_cutfile->Resize( 370, text_cutfile->GetDefaultHeight() );
	sub_frame_13->AddFrame( text_cutfile, new TGLayoutHints( kLHintsRight | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	// SRIM directory
	text_srimdir = new TGTextEntry( sub_frame_14 );
	text_srimdir->SetMaxLength( 4096 );
	text_srimdir->SetAlignment( kTextLeft );
	text_srimdir->Resize( 370, text_srimdir->GetDefaultHeight() );
	sub_frame_14->AddFrame( text_srimdir, new TGLayoutHints( kLHintsRight | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	
	//////////////////////////
	// Create check buttons //
	//////////////////////////
	
	check_beamdump = new TGCheckButton( sub_frame_6, "Beamdump" );
	sub_frame_6->AddFrame( check_beamdump, new TGLayoutHints( kLHintsRight, 2, 2, 2, 2 ) );
	
	check_source = new TGCheckButton( sub_frame_6, "Source run" );
	sub_frame_6->AddFrame( check_source, new TGLayoutHints( kLHintsRight, 2, 2, 2, 2 ) );
	
	check_spede = new TGCheckButton( sub_frame_10, "SPEDE" );
	sub_frame_10->AddFrame( check_spede, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_crex = new TGCheckButton( sub_frame_10, "C-REX" );
	sub_frame_10->AddFrame( check_crex, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_trex = new TGCheckButton( sub_frame_10, "T-REX" );
	sub_frame_10->AddFrame( check_trex, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_cdpad = new TGCheckButton( sub_frame_10, "CD+PAD" );
	sub_frame_10->AddFrame( check_cdpad, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
//	check_ionch = new TGCheckButton( sub_frame_10, "Ion. Ch." );
//	sub_frame_10->AddFrame( check_ionch, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_singles = new TGCheckButton( sub_frame_10, "Singles" );
	sub_frame_10->AddFrame( check_singles, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_gamgam = new TGCheckButton( sub_frame_10, "Gamma-Gamma" );
	sub_frame_10->AddFrame( check_gamgam, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_addback = new TGCheckButton( sub_frame_10, "Addback" );
	sub_frame_10->AddFrame( check_addback, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_reject = new TGCheckButton( sub_frame_10, "Reject" );
	sub_frame_10->AddFrame( check_reject, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_verbose = new TGCheckButton( sub_frame_10, "Verbose" );
	sub_frame_10->AddFrame( check_verbose, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_cutfile = new TGCheckButton( sub_frame_15, "Use cut file" );
	sub_frame_15->AddFrame( check_cutfile, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	
	///////////////////////////
	// Create number entries //
	///////////////////////////
	
	sub_frame_6->AddFrame( lab_first_entry,
						  new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );
	num_first_entry = new TGNumberEntry( sub_frame_6, -1, 9, -1, 
		TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMin,
		-1, 99999999 );
	sub_frame_6->AddFrame( num_first_entry, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	sub_frame_6->AddFrame( lab_last_entry,
						  new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );
	num_last_entry = new TGNumberEntry( sub_frame_6, -1, 9, -1, 
		TGNumberFormat::kNESInteger, TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMin,
		-1, 99999999 );
	sub_frame_6->AddFrame( num_last_entry, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_zb = new TGNumberEntry( dop_frame_1, 50, 3, -1,
		TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		1, 120 );
	dop_frame_1->AddFrame( num_dop_zb, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_zt = new TGNumberEntry( dop_frame_1, 82, 3, -1,
		TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		1, 120 );
	dop_frame_1->AddFrame( num_dop_zt, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_ab = new TGNumberEntry( dop_frame_1, 132, 4, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		1, 300 );
	dop_frame_1->AddFrame( num_dop_ab, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_at = new TGNumberEntry( dop_frame_1, 206, 4, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		1, 300 );
	dop_frame_1->AddFrame( num_dop_at, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_eb = new TGNumberEntry( dop_frame_1, 5000, 5, -1,
		TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		0.00001, 999999 );
	dop_frame_1->AddFrame( num_dop_eb, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_ex = new TGNumberEntry( dop_frame_1, 1206, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		0.00001, 999999 );
	dop_frame_1->AddFrame( num_dop_ex, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_th = new TGNumberEntry( dop_frame_1, 2000, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		0.00001, 999999 );
	dop_frame_1->AddFrame( num_dop_th, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_id = new TGNumberEntry( dop_frame_1, 1000, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		0.00001, 999999 );
	dop_frame_1->AddFrame( num_dop_id, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_cd = new TGNumberEntry( dop_frame_1, 21.1, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		1, 100 );
	dop_frame_1->AddFrame( num_dop_cd, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_ro = new TGNumberEntry( dop_frame_1, 242.6, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax,
		-999, 999 );
	dop_frame_1->AddFrame( num_dop_ro, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_dl = new TGNumberEntry( dop_frame_1, 0.0007, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax,
		0, 10 );
	dop_frame_1->AddFrame( num_dop_dl, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_sp = new TGNumberEntry( dop_frame_1, 26.2, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		1, 100 );
	dop_frame_1->AddFrame( num_dop_sp, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_pd = new TGNumberEntry( dop_frame_1, 50., 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		0.00001, 999999 );
	dop_frame_1->AddFrame( num_dop_pd, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_bg = new TGNumberEntry( dop_frame_1, -0.461538, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax,
		-999, 999 );
	dop_frame_1->AddFrame( num_dop_bg, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	
	///////////////////////////
	// Create action buttons //
	///////////////////////////
	
	// rsync
	but_rsync = new TGTextButton( comp_frame_0, "Copy from DAQ", -1, TGTextButton::GetDefaultGC()(),
								 TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_rsync->SetTextJustify( 36 );
	but_rsync->SetMargins( 0, 0, 0, 0 );
	but_rsync->SetWrapLength( -1 );
	but_rsync->Resize( 50, 56 );
	comp_frame_0->AddFrame( but_rsync, new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	// MedToRoot
	but_medroot = new TGTextButton( comp_frame_1, "Convert to ROOT", -1, TGTextButton::GetDefaultGC()(),
								   TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_medroot->SetTextJustify( 36 );
	but_medroot->SetMargins( 0, 0, 0, 0 );
	but_medroot->SetWrapLength( -1 );
	but_medroot->Resize( 50, 56 );
	comp_frame_1->AddFrame( but_medroot, new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	// TreeBuilder
	but_build = new TGTextButton( comp_frame_2, "Build tree", -1, TGTextButton::GetDefaultGC()(),
								 TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_build->SetTextJustify( 36 );
	but_build->SetMargins( 0, 0, 0, 0 );
	but_build->SetWrapLength( -1 );
	but_build->Resize( 50, 56 );
	comp_frame_2->AddFrame( but_build, new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	// NeSort
	but_nesort = new TGTextButton( sub_frame_12, "NeSort", -1, TGTextButton::GetDefaultGC()(),
								  TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_nesort->SetTextJustify( 36 );
	but_nesort->SetMargins( 0, 0, 0, 0 );
	but_nesort->SetWrapLength( -1 );
	but_nesort->Resize( 50, 56 );
	sub_frame_12->AddFrame( but_nesort,
				new TGLayoutHints( kLHintsTop | kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	// AQ4Sort
	but_aq4sort = new TGTextButton( sub_frame_12, "AQ4", -1, TGTextButton::GetDefaultGC()(),
								  TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_aq4sort->SetTextJustify( 36 );
	but_aq4sort->SetMargins( 0, 0, 0, 0 );
	but_aq4sort->SetWrapLength( -1 );
	but_aq4sort->Resize( 50, 56 );
	sub_frame_12->AddFrame( but_aq4sort,
				new TGLayoutHints( kLHintsBottom | kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	// CLXAna
	but_ana = new TGTextButton( comp_frame_3, "Coulex analysis", -1, TGTextButton::GetDefaultGC()(),
							   TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_ana->SetTextJustify( 36 );
	but_ana->SetMargins( 0, 0, 0, 0 );
	but_ana->SetWrapLength( -1 );
	but_ana->Resize( 50, 56 );
	comp_frame_3->AddFrame( but_ana, new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );

	// MntAna
	but_mnt = new TGTextButton( comp_frame_3, "MNT analysis", -1, TGTextButton::GetDefaultGC()(),
							   TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_mnt->SetTextJustify( 36 );
	but_mnt->SetMargins( 0, 0, 0, 0 );
	but_mnt->SetWrapLength( -1 );
	but_mnt->Resize( 50, 56 );
	comp_frame_3->AddFrame( but_mnt, new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	// TDRIVAna
	but_tdriv = new TGTextButton( comp_frame_3, "TDRIV analysis", -1, TGTextButton::GetDefaultGC()(),
							   TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_tdriv->SetTextJustify( 36 );
	but_tdriv->SetMargins( 0, 0, 0, 0 );
	but_tdriv->SetWrapLength( -1 );
	but_tdriv->Resize( 50, 56 );
	comp_frame_3->AddFrame( but_tdriv, new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	// Add files
	but_add = new TGTextButton( sub_frame_3, "Add", -1, TGTextButton::GetDefaultGC()(),
							   TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_add->SetTextJustify( 36 );
	but_add->SetMargins( 0, 0, 0, 0 );
	but_add->SetWrapLength( -1 );
	but_add->Resize( 65, 46 );
	sub_frame_3->AddFrame( but_add, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	// Remove files
	but_del = new TGTextButton( sub_frame_3, "Remove", -1, TGTextButton::GetDefaultGC()(),
							   TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_del->SetTextJustify( 36 );
	but_del->SetMargins( 0, 0, 0, 0 );
	but_del->SetWrapLength( -1 );
	but_del->Resize( 65, 46 );
	sub_frame_3->AddFrame( but_del, new TGLayoutHints( kLHintsRight | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	
	/////////////////////
	// Open the window //
	/////////////////////
	
	// Set a name to the main frame
	main_frame->SetWindowName( "mb_sorter" );
	
	// Map all sub windows
	main_frame->MapSubwindows();
	left_frame->MapSubwindows();
	centre_frame->MapSubwindows();
	comp_frame_0->MapSubwindows();
	comp_frame_1->MapSubwindows();
	comp_frame_2->MapSubwindows();
	comp_frame_3->MapSubwindows();
	sub_frame_0->MapSubwindows();
	sub_frame_3->MapSubwindows();
	sub_frame_4->MapSubwindows();
	sub_frame_7->MapSubwindows();
	
	// Initialize the layout algorithm via Resize()
	main_frame->Resize( main_frame->GetDefaultSize() );
	
	// Map windows
	main_frame->MapWindow();
	//main_frame->Print();
	
	
	//////////////
	// Defaults //
	//////////////
	
	text_daq_dir->SetText( "/mbdata/miniball/is562-180806" );
	text_local_dir->SetText( "/run/media/miniball/MiniballAnalysis/is562/medfiles" );
	text_med_pre->SetText( "106Sn" );

	text_add_file->SetText( "106Sn_206Pb_pos6_005" );

	text_settings->SetText( "/run/media/miniball/MiniballAnalysis/is562/config/MBSettings2018_CLX_DgfOffset.dat" );
	//check_source->SetOn();
	check_beamdump->SetOn();

	text_outfile->SetText( "/run/media/miniball/MiniballAnalysis/is562/rootfiles/106Sn_206Pb_xxx-yyy" );
	text_calfile->SetText( "/run/media/miniball/MiniballAnalysis/is562/config/is562-offline.cal" );
	text_cutfile->SetText( "/run/media/miniball/MiniballAnalysis/is562/config/cutfile-106Sn_206Pb_pos6.root:Bcut:Tcut" );
	text_srimdir->SetText( "/run/media/miniball/MiniballAnalysis/srim" );

	check_cdpad->SetOn();
	//check_singles->SetOn();
	//check_gamgam->SetOn();
	check_addback->SetOn();
	check_cutfile->SetOn();

	num_dop_zb->SetNumber( 50 );
	num_dop_zt->SetNumber( 82 );
	num_dop_ab->SetNumber( 106 );
	num_dop_at->SetNumber( 206 );
	num_dop_eb->SetNumber( 4400 );
	num_dop_ex->SetNumber( 1208 );
	num_dop_th->SetNumber( 4.2 );
	num_dop_id->SetNumber( 2.1 );
	num_dop_cd->SetNumber( 28.6 );
	num_dop_ro->SetNumber( 243.8 );
	num_dop_dl->SetNumber( 0.0007 );
	num_dop_sp->SetNumber( 26.2 );
	num_dop_bg->SetNumber( -0.461538 );
	num_dop_pd->SetNumber( 50. );


	////////////////////
	// Button presses //
	////////////////////
	
	but_open->Connect( "Clicked()", "MBSorter", this, "on_open_clicked()" );
	but_save->Connect( "Clicked()", "MBSorter", this, "on_save_clicked()" );
	but_add->Connect( "Clicked()", "MBSorter", this, "on_add_clicked()" );
	text_add_file->Connect( "ReturnPressed()", "MBSorter", this, "on_add_clicked()" );
	but_del->Connect( "Clicked()", "MBSorter", this, "on_del_clicked()" );
	but_rsync->Connect( "Clicked()", "MBSorter", this, "on_rsync_clicked()" );
	but_medroot->Connect( "Clicked()", "MBSorter", this, "on_medroot_clicked()" );
	but_aq4sort->Connect( "Clicked()", "MBSorter", this, "on_aq4sort_clicked()" );
	but_nesort->Connect( "Clicked()", "MBSorter", this, "on_nesort_clicked()" );
	but_build->Connect( "Clicked()", "MBSorter", this, "on_build_clicked()" );
	but_ana->Connect( "Clicked()", "MBSorter", this, "on_ana_clicked()" );
	but_mnt->Connect( "Clicked()", "MBSorter", this, "on_mnt_clicked()" );
	but_tdriv->Connect( "Clicked()", "MBSorter", this, "on_tdriv_clicked()" );

}

MBSorter::~MBSorter() {
	
	// Clean up main frame...
	main_frame->Cleanup();
	delete main_frame;

}

void MBSorter::on_open_clicked() {
	
	// Configure file dialog
	TGFileInfo fi;
	TString dir(".");
	fi.fIniDir = StrDup(dir);
	
	// Open a file dialog
	new TGFileDialog( gClient->GetRoot(), main_frame, kFDOpen, &fi );
	
	// Check if filename is null (on cancel, perhaps)
	if( fi.fFilename == NULL ) return;
	
	cout << "Opening setup from " << fi.fFilename << endl;
	
	LoadSetup( fi.fFilename );
	
	return;
	
}

void MBSorter::on_save_clicked() {
	
	// Configure file dialog
	TGFileInfo fi;
	TString dir(".");
	fi.fIniDir = StrDup(dir);
	
	// Open a file dialog
	new TGFileDialog( gClient->GetRoot(), main_frame, kFDSave, &fi );
	
	// Check if filename is null (on cancel, perhaps)
	if( fi.fFilename == NULL ) return;
	
	cout << "Saving setup to " << fi.fFilename << endl;
	
	SaveSetup( fi.fFilename );
	
	return;
	
}

void MBSorter::on_add_clicked() {
	
	// Slot to react to add file button
	TGString filename = text_add_file->GetText();
	
	run_list_box->AddEntrySort( filename, filelist.size() + 1 );
	run_list_box->Layout();

	filelist.push_back( filename.GetString() );
	filestatus.push_back( true );

}

void MBSorter::on_del_clicked() {
	
	// Cleanup the selected entries
	run_selected->Clear();
	
	// Slot to react to remove file button
	if( run_list_box->GetSelected() < 0 ) {

		cout << "\nNo runs selected" << endl;

	}

	else if( run_list_box->GetMultipleSelections() ) {
		
		run_list_box->GetSelectedEntries( run_selected );
		TIter next( run_selected );
		
		while( TGLBEntry *ent = (TGLBEntry*)next() ) {
		
			filestatus.at( ent->EntryId() - 1 ) = false;
			run_list_box->RemoveEntry( ent->EntryId() );

		}

	}
	
	else {
		
		filestatus.at( run_list_box->GetSelected() - 1 ) = false;
		run_list_box->RemoveEntry( -1 );
		
	}
	
	run_list_box->Layout();
	
}

void MBSorter::on_rsync_clicked() {
	
	// Slot to react to rsync button
	TString cmd = "nice rsync -avz ";
	cmd += "miniball@hpdaqpc:";
	cmd += text_daq_dir->GetText();
	cmd += "/";
	cmd += text_med_pre->GetText();
	cmd += "*.med ";
	cmd += text_local_dir->GetText();
	cmd += "/";
	
	cout << endl << cmd << endl << endl;
	gSystem->Exec( cmd );

}

void MBSorter::on_medroot_clicked() {
	
	// Slot to react to MedToRoot button
	TString filebase;
	TString rootfile;
	TString cmd;
	TString cmd_base = "MedToRoot ";

	if( check_source->IsOn() ) cmd_base += "-so ";
	if( check_beamdump->IsOn() ) cmd_base += "-bd ";

	cmd_base += " -fe ";
	cmd_base += convertInt( num_first_entry->GetIntNumber() );
	cmd_base += " -le ";
	cmd_base += convertInt( num_last_entry->GetIntNumber() );
	cmd_base += " -sf ";
	cmd_base += text_settings->GetText();
	
	for( unsigned int i = 0; i < filelist.size(); i++ ) {

		filebase = filelist.at( i );
		if( !filestatus.at( i ) ) continue;
		rootfile = text_local_dir->GetText();
		rootfile += "/";
		rootfile += filebase;
		if( check_source->IsOn() ) rootfile += "_Source.root";
		else rootfile += "_OnBeam.root";

		if( !gSystem->AccessPathName( rootfile ) ) {

			cout << "Skipping " << filebase;
			cout << " it's already been converted" << endl;
			continue;

		}

		cmd = cmd_base + " -mf ";
		cmd += text_local_dir->GetText();
		cmd += "/";
		cmd += filebase;
		cmd += ".med";

		cout << endl << cmd << endl << endl;
		gSystem->Exec( cmd );

	}

}

void MBSorter::on_build_clicked() {
	
	// Slot to react to TreeBuilder button
	TString filebase;
	TString rootfileout;
	TString files = "";
	bool answer = false;
	TString cmd = "TreeBuilder ";

	if( check_crex->IsOn() ) cmd += "-crex ";
	if( check_trex->IsOn() ) cmd += "-trex ";
	if( check_spede->IsOn() ) cmd += "-spede ";
	if( check_cdpad->IsOn() ) cmd += "-cdpad ";
	//if( check_ionch->IsOn() ) cmd += "-ionch ";
	cmd += "-ionch ";
	if( check_singles->IsOn() ) cmd += "-s ";
	if( check_gamgam->IsOn() ) cmd += "-gg ";
	if( check_addback->IsOn() ) cmd += "-addback ";
	if( check_reject->IsOn() ) cmd += "-reject ";
	if( check_verbose->IsOn() ) cmd += "-vl ";

	rootfileout = text_outfile->GetText();
	rootfileout += "_tree.root";

	if( !gSystem->AccessPathName( rootfileout ) ) {
		
		TString msg = "Are you sure you want to overwrite " + rootfileout + "?";
		new MyDialog( gClient->GetRoot(), main_frame, msg, answer );
		
		if( answer == false ) return;
		
	}

	cmd += "-c ";
	cmd += text_calfile->GetText();
	
	for( unsigned int i = 0; i < filelist.size(); i++ ) {

		filebase = filelist.at( i );
		if( !filestatus.at( i ) ) continue;

		files += text_local_dir->GetText();
		files += "/";
		files += filebase;
		if( check_source->IsOn() ) files += "_Source.root ";
		else files += "_OnBeam.root ";

	}

	cmd += " -i ";
	cmd += files;

	cmd += " -o ";
	cmd += text_outfile->GetText();
	cmd += "_tree.root";

	cout << endl << cmd << endl << endl;
	gSystem->Exec( cmd );
	
}

void MBSorter::on_nesort_clicked() {
	
	// Slot to react to TreeBuilder button
	TString filebase;
	TString rootfileout;
	TString files = "";
	bool answer = false;
	TString cmd = "NeSort ";
	
	rootfileout = text_outfile->GetText();
	rootfileout += "_nesort.root";
	
	if( !gSystem->AccessPathName( rootfileout ) ) {
		
		TString msg = "Are you sure you want to overwrite " + rootfileout + "?";
		new MyDialog( gClient->GetRoot(), main_frame, msg, answer );
		
		if( answer == false ) return;

	}
	
	cmd += "-c ";
	cmd += text_calfile->GetText();
	
	for( unsigned int i = 0; i < filelist.size(); i++ ) {
		
		filebase = filelist.at( i );
		if( !filestatus.at( i ) ) continue;
		
		files += text_local_dir->GetText();
		files += "/";
		files += filebase;
		if( check_source->IsOn() ) files += "_Source.root ";
		else files += "_OnBeam.root ";
		
	}
	
	cmd += " -i ";
	cmd += files;
	
	cmd += " -o ";
	cmd += text_outfile->GetText();
	cmd += "_nesort.root";
	
	cout << endl << cmd << endl << endl;
	gSystem->Exec( cmd );
	
}

void MBSorter::on_aq4sort_clicked() {
	
	// Slot to react to TreeBuilder button
	TString filebase;
	TString rootfileout;
	TString files = "";
	bool answer = false;
	TString cmd = "AQ4Sort ";
	
	rootfileout = text_outfile->GetText();
	rootfileout += "_aq4sort.root";
	
	if( !gSystem->AccessPathName( rootfileout ) ) {
		
		TString msg = "Are you sure you want to overwrite " + rootfileout + "?";
		new MyDialog( gClient->GetRoot(), main_frame, msg, answer );
		
		if( answer == false ) return;
		
	}
	
	cmd += "-c ";
	cmd += text_calfile->GetText();
	
	for( unsigned int i = 0; i < filelist.size(); i++ ) {
		
		filebase = filelist.at( i );
		if( !filestatus.at( i ) ) continue;
		
		files += text_local_dir->GetText();
		files += "/";
		files += filebase;
		if( check_source->IsOn() ) files += "_Source.root ";
		else files += "_OnBeam.root ";
		
	}
	
	cmd += " -i ";
	cmd += files;
	
	cmd += " -o ";
	cmd += text_outfile->GetText();
	cmd += "_aq4sort.root";
	
	cout << endl << cmd << endl << endl;
	gSystem->Exec( cmd );
	
}

void MBSorter::on_ana_clicked() {
	
	// Slot to react to Histograms button
	TString inputfile, outputfile;
	TString cmd;

	inputfile = text_outfile->GetText();
	inputfile += "_tree.root";
	outputfile = text_outfile->GetText();
	outputfile += "_hists.root";

	cmd = "CLXAna ";
	
	cmd += " -Zb ";
	cmd += convertInt( num_dop_zb->GetIntNumber() );
	cmd += " -Ab ";
	cmd += convertFloat( num_dop_ab->GetNumber() );
	cmd += " -Zt ";
	cmd += convertInt( num_dop_zt->GetIntNumber() );
	cmd += " -At ";
	cmd += convertFloat( num_dop_at->GetNumber() );
	cmd += " -Eb ";
	cmd += convertFloat( num_dop_eb->GetNumber() );
	cmd += " -Ex ";
	cmd += convertFloat( num_dop_ex->GetNumber() );
	cmd += " -thick ";
	cmd += convertFloat( num_dop_th->GetNumber() );
	cmd += " -depth ";
	cmd += convertFloat( num_dop_id->GetNumber() );
	cmd += " -cddist ";
	cmd += convertFloat( num_dop_cd->GetNumber() );
	cmd += " -cdoffset ";
	cmd += convertFloat( num_dop_ro->GetNumber() );
	cmd += " -deadlayer ";
	cmd += convertFloat( num_dop_dl->GetNumber() );
	cmd += " -spededist ";
	cmd += convertFloat( num_dop_sp->GetNumber() );
	cmd += " -bg_frac ";
	cmd += convertFloat( num_dop_bg->GetNumber() );
	cmd += " -srim ";
	cmd += text_srimdir->GetText();

	if( check_cutfile->IsOn() ) {

		cmd += " -cut ";
		cmd += text_cutfile->GetText();

	}

	cmd += " -i ";
	cmd += inputfile;
	cmd += " -o ";
	cmd += outputfile;

	cout << endl << cmd << endl << endl;
	gSystem->Exec( cmd );
	
}

void MBSorter::on_mnt_clicked() {
	
	// Slot to react to Histograms button
	TString inputfile, outputfile;
	TString cmd;

	inputfile = text_outfile->GetText();
	inputfile += "_tree.root";
	outputfile = text_outfile->GetText();
	outputfile += "_mnt.root";
	
	cmd = "MntAna -i " + inputfile + " -o " + outputfile;

	cout << endl << cmd << endl;
	gSystem->Exec( cmd );
	
}

void MBSorter::on_tdriv_clicked() {
	
	// Slot to react to Histograms button
	TString inputfile, outputfile;
	TString cmd;

	inputfile = text_outfile->GetText();
	inputfile += "_tree.root";
	outputfile = text_outfile->GetText();
	outputfile += "_tdriv.root";
	
	cmd = "TDRIVAna";

	cmd += " -Zb ";
	cmd += convertInt( num_dop_zb->GetIntNumber() );
	cmd += " -Ab ";
	cmd += convertFloat( num_dop_ab->GetNumber() );
	cmd += " -Zt ";
	cmd += convertInt( num_dop_zt->GetIntNumber() );
	cmd += " -At ";
	cmd += convertFloat( num_dop_at->GetNumber() );
	cmd += " -Eb ";
	cmd += convertFloat( num_dop_eb->GetNumber() );
	cmd += " -Ex ";
	cmd += convertFloat( num_dop_ex->GetNumber() );
	cmd += " -thick ";
	cmd += convertFloat( num_dop_th->GetNumber() );
	cmd += " -depth ";
	cmd += convertFloat( num_dop_id->GetNumber() );
	cmd += " -cddist ";
	cmd += convertFloat( num_dop_cd->GetNumber() );
	cmd += " -cdoffset ";
	cmd += convertFloat( num_dop_ro->GetNumber() );
	cmd += " -deadlayer ";
	cmd += convertFloat( num_dop_dl->GetNumber() );
	cmd += " -plunger ";
	cmd += convertFloat( num_dop_pd->GetNumber() );

	if( check_cutfile->IsOn() ) {

		cmd += " -cut ";
		cmd += text_cutfile->GetText();

	}

	cmd += " -i ";
	cmd += inputfile;
	cmd += " -o ";
	cmd += outputfile;


	cout << endl << cmd << endl << endl;
	gSystem->Exec( cmd );
	
}

void MBSorter::SaveSetup( string setupfile ) {

	TEnv *fSetup = new TEnv( setupfile.c_str() );

	fSetup->SetValue( "Zb", (double)num_dop_zb->GetIntNumber() );
	fSetup->SetValue( "Zt", (double)num_dop_zt->GetIntNumber() );
	fSetup->SetValue( "Ab", (double)num_dop_ab->GetNumber() );
	fSetup->SetValue( "At", (double)num_dop_at->GetNumber() );
	fSetup->SetValue( "Eb", (double)num_dop_eb->GetNumber() );
	fSetup->SetValue( "Ex", (double)num_dop_ex->GetNumber() );
	fSetup->SetValue( "thick", (double)num_dop_th->GetNumber() );
	fSetup->SetValue( "depth", (double)num_dop_id->GetNumber() );
	fSetup->SetValue( "cddist", (double)num_dop_cd->GetNumber() );
	fSetup->SetValue( "cdoffset", (double)num_dop_ro->GetNumber() );
	fSetup->SetValue( "deadlayer", (double)num_dop_dl->GetNumber() );
	fSetup->SetValue( "plunger", (double)num_dop_pd->GetNumber() );
	fSetup->SetValue( "bg_frac", (double)num_dop_bg->GetNumber() );

	fSetup->SetValue( "daq_dir", text_daq_dir->GetText() );
	fSetup->SetValue( "local_dir", text_local_dir->GetText() );
	fSetup->SetValue( "med_pre", text_med_pre->GetText() );
	fSetup->SetValue( "add_file", text_add_file->GetText() );
	fSetup->SetValue( "settings", text_settings->GetText() );
	fSetup->SetValue( "outfile", text_outfile->GetText() );
	fSetup->SetValue( "calfile", text_calfile->GetText() );
	fSetup->SetValue( "cutfile", text_cutfile->GetText() );
	fSetup->SetValue( "srimdir", text_srimdir->GetText() );
	
	string list_of_files = "";

	for( unsigned int i = 0; i < filelist.size(); i++ ) {

		if( !filestatus.at( i ) ) continue;

		list_of_files += filelist.at(i);
		list_of_files += " ";

	}

	fSetup->SetValue( "filelist", list_of_files.c_str() );
	
	fSetup->SetValue( "source", check_source->IsOn() );
	fSetup->SetValue( "beamdump", check_beamdump->IsOn() );
	fSetup->SetValue( "spede", check_spede->IsOn() );
	fSetup->SetValue( "crex", check_crex->IsOn() );
	fSetup->SetValue( "trex", check_trex->IsOn() );
	fSetup->SetValue( "cdpad", check_cdpad->IsOn() );
	fSetup->SetValue( "singles", check_singles->IsOn() );
	fSetup->SetValue( "gamgam", check_gamgam->IsOn() );
	fSetup->SetValue( "addback", check_addback->IsOn() );
	fSetup->SetValue( "reject", check_reject->IsOn() );
	fSetup->SetValue( "verbose", check_verbose->IsOn() );
	fSetup->SetValue( "usecut", check_cutfile->IsOn() );
	
	fSetup->WriteFile( setupfile.c_str() );
	
	return;

}

void MBSorter::LoadSetup( string setupfile ) {

	TEnv *fSetup = new TEnv( setupfile.c_str() );
	fSetup->ReadFile( setupfile.c_str(), kEnvLocal );

	num_dop_zb->SetNumber( fSetup->GetValue( "Zb", 0 ) );
	num_dop_zt->SetNumber( fSetup->GetValue( "Zt", 0 ) );
	num_dop_ab->SetNumber( fSetup->GetValue( "Ab", 0.0 ) );
	num_dop_at->SetNumber( fSetup->GetValue( "At", 0.0 ) );
	num_dop_eb->SetNumber( fSetup->GetValue( "Eb", 0.0 ) );
	num_dop_ex->SetNumber( fSetup->GetValue( "Ex", 0.0 ) );
	num_dop_th->SetNumber( fSetup->GetValue( "thick", 0.0 ) );
	num_dop_id->SetNumber( fSetup->GetValue( "depth", 0.0 ) );
	num_dop_cd->SetNumber( fSetup->GetValue( "cddist", 0.0 ) );
	num_dop_ro->SetNumber( fSetup->GetValue( "cdoffset", 0.0 ) );
	num_dop_dl->SetNumber( fSetup->GetValue( "deadlayer", 0.0 ) );
	num_dop_pd->SetNumber( fSetup->GetValue( "plunger", 0.0 ) );
	num_dop_bg->SetNumber( fSetup->GetValue( "bg_frac", 0.0 ) );
	
	text_daq_dir->SetText( fSetup->GetValue( "daq_dir", "/mbdata/miniball/isxxx" ) );
	text_local_dir->SetText( fSetup->GetValue( "local_dir", "./medfiles" ) );
	text_med_pre->SetText( fSetup->GetValue( "med_pre", "" ) );
	text_add_file->SetText( fSetup->GetValue( "add_file", "" ) );
	text_settings->SetText( fSetup->GetValue( "settings", "./config/MBSettings2018_CLX_DgfOffset.dat" ) );
	text_outfile->SetText( fSetup->GetValue( "outfile", "" ) );
	text_calfile->SetText( fSetup->GetValue( "calfile", "./config/is562-offline.cal" ) );
	text_cutfile->SetText( fSetup->GetValue( "cutfile", "./config/cutfile.root:Bcut:Tcut" ) );
	text_srimdir->SetText( fSetup->GetValue( "srimdir", "./srim" ) );
	
	string list_of_files = fSetup->GetValue( "filelist", "" );
	filelist.clear();
	filestatus.clear();
	run_list_box->RemoveAll();
	
	stringstream ss;
	string current_file;
	ss << list_of_files;
	ss >> current_file;
	
	while( !ss.eof() ) {
		
		filelist.push_back( current_file );
		filestatus.push_back( true );
		
		run_list_box->AddEntrySort( current_file.c_str(), filelist.size() );

		ss >> current_file;
		
	}
	
	run_list_box->Layout();
	
	check_source->SetOn( fSetup->GetValue( "source", false ) );
	check_beamdump->SetOn( fSetup->GetValue( "beamdump", true ) );
	check_spede->SetOn( fSetup->GetValue( "spede", false ) );
	check_crex->SetOn( fSetup->GetValue( "crex", false ) );
	check_trex->SetOn( fSetup->GetValue( "trex", false ) );
	check_cdpad->SetOn( fSetup->GetValue( "cdpad", true ) );
	check_singles->SetOn( fSetup->GetValue( "singles", false ) );
	check_gamgam->SetOn( fSetup->GetValue( "gamgam", false ) );
	check_addback->SetOn( fSetup->GetValue( "addback", true ) );
	check_reject->SetOn( fSetup->GetValue( "reject", false ) );
	check_verbose->SetOn( fSetup->GetValue( "verbose", false ) );
	check_cutfile->SetOn( fSetup->GetValue( "usecut", false ) );

	return;

}

MyDialog::MyDialog( const TGWindow *p, const TGWindow *main, TString msg, bool &ans ) {
	
	// Default answer is false
	answer = false;
	
	fDialog = new TGTransientFrame( p, main, 800, 100, kVerticalFrame );
	frame_button = new TGHorizontalFrame( fDialog, 240, 60 );
	
	dialog_msg = new TGLabel( fDialog, msg );
	fDialog->AddFrame( dialog_msg, new TGLayoutHints( kLHintsTop | kLHintsExpandY, 5, 5, 5, 5) );
	
	but_yes = new TGTextButton( frame_button, " &Yes ", 1 );
	but_yes->Connect( "Clicked()", "MyDialog", this, "SayYes()" );
	but_yes->Associate( fDialog );
	frame_button->AddFrame( but_yes, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4 ) );
	
	but_no = new TGTextButton( frame_button, " &No ", 1 );
	but_no->Connect( "Clicked()", "MyDialog", this, "SayNo()" );
	but_no->Associate( fDialog );
	frame_button->AddFrame( but_no, new TGLayoutHints( kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4 ) );
	
	frame_button->Resize( 100, but_no->GetDefaultHeight() + 4 );
	fDialog->AddFrame( frame_button, new TGLayoutHints( kLHintsBottom | kLHintsCenterX | kLHintsExpandY, 5, 5, 5, 5 ) );

	fDialog->MapSubwindows();
	fDialog->Resize( fDialog->GetDefaultSize() );
	
	fDialog->MapWindow();

	gClient->WaitFor( fDialog );

	ans = answer;
	
}

MyDialog::~MyDialog() {

	delete fDialog;
	delete but_yes;
	delete but_no;
	delete dialog_msg;
	delete frame_button;
	
	delete fDialog;
	
}

void MyDialog::SayYes() {
	
	answer = true;
	fDialog->SendCloseMessage();
	
}

void MyDialog::SayNo() {
	
	answer = false;
	fDialog->SendCloseMessage();
	
}

