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
	main_frame->SetEditable();
	
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
	comp_frame_2 = new TGCompositeFrame( centre_frame, 800, 90, kHorizontalFrame | kFixedWidth );
	comp_frame_2->SetName( "comp_frame_2" );
	centre_frame->AddFrame( comp_frame_2, new TGLayoutHints( kLHintsExpandX ) );
	
	// Horizontal separator 2
	fHorizontal_2 = new TGHorizontal3DLine( centre_frame, 800, 8 );
	fHorizontal_2->SetName( "fHorizontal_2" );
	centre_frame->AddFrame( fHorizontal_2, new TGLayoutHints( kLHintsLeft | kLHintsTop, 2, 2, 2, 2 ) );
	
	// Centre frame 3 - CLXAna
	comp_frame_3 = new TGCompositeFrame( centre_frame, 800, 90, kHorizontalFrame | kFixedWidth );
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
	
	// Sub frame 11 - Analysis boxes
	sub_frame_11 = new TGVerticalFrame( comp_frame_3, 700, 90 );
	sub_frame_11->SetName( "sub_frame_11" );
	comp_frame_3->AddFrame( sub_frame_11, new TGLayoutHints( kLHintsLeft ) );
	
	// Sub frame 12 - Configuration file
	sub_frame_12 = new TGHorizontalFrame( sub_frame_11, 700, 45 );
	sub_frame_12->SetName( "sub_frame_12" );
	sub_frame_11->AddFrame( sub_frame_12, new TGLayoutHints( kLHintsTop | kLHintsExpandX ) );
	
	// Sub frame 13 - Kinematic cut file
	sub_frame_13 = new TGHorizontalFrame( sub_frame_11, 700, 45 );
	sub_frame_13->SetName( "sub_frame_13" );
	sub_frame_11->AddFrame( sub_frame_13, new TGLayoutHints( kLHintsBottom | kLHintsExpandX ) );
	
	// Sub frame 14 - Analysis check buttons
	sub_frame_14 = new TGHorizontalFrame( sub_frame_11, 700, 45 );
	sub_frame_14->SetName( "sub_frame_14" );
	sub_frame_11->AddFrame( sub_frame_14, new TGLayoutHints( kLHintsBottom | kLHintsExpandX ) );
	
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
	
	// Last entry to convert from med to root
	lab_last_entry = new TGLabel( sub_frame_6, "Last entry" );
	lab_last_entry->SetTextJustify( 36 );
	lab_last_entry->SetMargins( 0, 0, 0, 0 );
	lab_last_entry->SetWrapLength( -1 );
	lab_last_entry->Resize( 80, lab_last_entry->GetDefaultHeight() );
	sub_frame_6->AddFrame( lab_last_entry,
						  new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );
	
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
	
	// Configuration file
	lab_config = new TGLabel( sub_frame_12, "Configuration file" );
	lab_config->SetTextJustify( 36 );
	lab_config->SetMargins( 0, 0, 0, 0 );
	lab_config->SetWrapLength( -1 );
	lab_config->Resize( 80, lab_config->GetDefaultHeight() );
	sub_frame_12->AddFrame( lab_config,
						  new TGLayoutHints( kLHintsLeft | kFixedWidth, 2, 2, 2, 2 ) );
	
	// Kinematic cut file
	lab_cutfile = new TGLabel( sub_frame_13, "Kinematic cuts file" );
	lab_cutfile->SetTextJustify( 36 );
	lab_cutfile->SetMargins( 0, 0, 0, 0 );
	lab_cutfile->SetWrapLength( -1 );
	lab_cutfile->Resize( 80, lab_cutfile->GetDefaultHeight() );
	sub_frame_13->AddFrame( lab_cutfile,
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
	
	// Units - Ex
	lab_unit_ex = new TGLabel( dop_frame_2, "keV/u" );
	lab_unit_ex->SetTextJustify( 36 );
	lab_unit_ex->SetMargins( 0, 0, 0, 0 );
	lab_unit_ex->SetWrapLength( -1 );
	lab_unit_ex->Resize( 40, lab_unit_ex->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_ex,
						  new TGLayoutHints( kLHintsTop, 2, 2, 5, 5 ) );
	
	// Doppler - Ex
	lab_dop_ex = new TGLabel( dop_frame_0, "E_x =" );
	lab_dop_ex->SetTextJustify( kTextRight );
	lab_dop_ex->SetMargins( 0, 0, 0, 0 );
	lab_dop_ex->SetWrapLength( -1 );
	lab_dop_ex->Resize( 40, lab_dop_ex->GetDefaultHeight() );
	dop_frame_0->AddFrame( lab_dop_ex,
						  new TGLayoutHints( kLHintsTop | kLHintsRight, 2, 2, 5, 5 ) );
	
	// Units - Eb
	lab_unit_eb = new TGLabel( dop_frame_2, "keV/u" );
	lab_unit_eb->SetTextJustify( 36 );
	lab_unit_eb->SetMargins( 0, 0, 0, 0 );
	lab_unit_eb->SetWrapLength( -1 );
	lab_unit_eb->Resize( 40, lab_unit_eb->GetDefaultHeight() );
	dop_frame_2->AddFrame( lab_unit_eb,
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
	lab_unit_th = new TGLabel( dop_frame_2, "µg/cm^2" );
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
	lab_unit_id = new TGLabel( dop_frame_2, "µg/cm^2" );
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
	lab_unit_dl = new TGLabel( dop_frame_2, "µm" );
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
	
	
	/////////////////////
	// Create list box //
	/////////////////////
	
	// Run files
	run_list_box = new TGListBox( left_frame, -1, kSunkenFrame );
	run_list_box->SetName( "run_list_box" );
	run_list_box->Resize( 250, 286 );
	left_frame->AddFrame( run_list_box, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	
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
	
	// Configuration file
	text_config = new TGTextEntry( sub_frame_12 );
	text_config->SetMaxLength( 4096 );
	text_config->SetAlignment( kTextLeft );
	text_config->Resize( 370, text_config->GetDefaultHeight() );
	sub_frame_12->AddFrame( text_config, new TGLayoutHints( kLHintsRight | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	// Kinematic cut file
	text_cutfile = new TGTextEntry( sub_frame_13 );
	text_cutfile->SetMaxLength( 4096 );
	text_cutfile->SetAlignment( kTextLeft );
	text_cutfile->Resize( 370, text_cutfile->GetDefaultHeight() );
	sub_frame_13->AddFrame( text_cutfile, new TGLayoutHints( kLHintsRight | kLHintsExpandX, 2, 2, 2, 2 ) );
	
	
	//////////////////////////
	// Create check buttons //
	//////////////////////////
	
	check_source = new TGCheckButton( sub_frame_6, "Source run" );
	sub_frame_6->AddFrame( check_source, new TGLayoutHints( kLHintsRight, 2, 2, 2, 2 ) );
	
	check_spede = new TGCheckButton( sub_frame_10, "SPEDE" );
	sub_frame_10->AddFrame( check_spede, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_crex = new TGCheckButton( sub_frame_10, "C-REX" );
	sub_frame_10->AddFrame( check_crex, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_cdpad = new TGCheckButton( sub_frame_10, "CD+PAD" );
	sub_frame_10->AddFrame( check_cdpad, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_ionch = new TGCheckButton( sub_frame_10, "Ion. Ch." );
	sub_frame_10->AddFrame( check_ionch, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_singles = new TGCheckButton( sub_frame_10, "Singles" );
	sub_frame_10->AddFrame( check_singles, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_gamgam = new TGCheckButton( sub_frame_10, "Gamma-Gamma" );
	sub_frame_10->AddFrame( check_gamgam, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_addback = new TGCheckButton( sub_frame_10, "Addback" );
	sub_frame_10->AddFrame( check_addback, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_verbose = new TGCheckButton( sub_frame_10, "Verbose" );
	sub_frame_10->AddFrame( check_verbose, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	check_config = new TGCheckButton( sub_frame_14, "Use config file" );
	sub_frame_14->AddFrame( check_config, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	
	///////////////////////////
	// Create number entries //
	///////////////////////////
	
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
		TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		1, 300 );
	dop_frame_1->AddFrame( num_dop_ab, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_at = new TGNumberEntry( dop_frame_1, 206, 4, -1,
		TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		1, 300 );
	dop_frame_1->AddFrame( num_dop_at, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_eb = new TGNumberEntry( dop_frame_1, 5000, 5, -1,
		TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMin,
		1, 999999 );
	dop_frame_1->AddFrame( num_dop_eb, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_ex = new TGNumberEntry( dop_frame_1, 1206, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMin,
		1, 999999 );
	dop_frame_1->AddFrame( num_dop_ex, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_th = new TGNumberEntry( dop_frame_1, 2000, 5, -1,
		TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMin,
		1, 999999 );
	dop_frame_1->AddFrame( num_dop_th, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_id = new TGNumberEntry( dop_frame_1, 1000, 5, -1,
		TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMin,
		1, 999999 );
	dop_frame_1->AddFrame( num_dop_id, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_cd = new TGNumberEntry( dop_frame_1, 21.1, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		1, 100 );
	dop_frame_1->AddFrame( num_dop_cd, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_ro = new TGNumberEntry( dop_frame_1, 242.6, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax,
		-999, 999 );
	dop_frame_1->AddFrame( num_dop_ro, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_dl = new TGNumberEntry( dop_frame_1, 0.7, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax,
		0, 10 );
	dop_frame_1->AddFrame( num_dop_dl, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	num_dop_sp = new TGNumberEntry( dop_frame_1, 26.2, 5, -1,
		TGNumberFormat::kNESReal, TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax,
		1, 100 );
	dop_frame_1->AddFrame( num_dop_sp, new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2 ) );
	
	
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
	comp_frame_0->AddFrame( but_rsync,
				new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	// MedToRoot
	but_medroot = new TGTextButton( comp_frame_1, "Convert to ROOT", -1, TGTextButton::GetDefaultGC()(),
								   TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_medroot->SetTextJustify( 36 );
	but_medroot->SetMargins( 0, 0, 0, 0 );
	but_medroot->SetWrapLength( -1 );
	but_medroot->Resize( 50, 56 );
	comp_frame_1->AddFrame( but_medroot,
				new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	// TreeBuilder
	but_build = new TGTextButton( comp_frame_2, "Build tree", -1, TGTextButton::GetDefaultGC()(),
								 TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_build->SetTextJustify( 36 );
	but_build->SetMargins( 0, 0, 0, 0 );
	but_build->SetWrapLength( -1 );
	but_build->Resize( 50, 56 );
	comp_frame_2->AddFrame( but_build,
				new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	// CLXAna
	but_ana = new TGTextButton( comp_frame_3, "Coulex analysis", -1, TGTextButton::GetDefaultGC()(),
							   TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_ana->SetTextJustify( 36 );
	but_ana->SetMargins( 0, 0, 0, 0 );
	but_ana->SetWrapLength( -1 );
	but_ana->Resize( 50, 56 );
	comp_frame_3->AddFrame( but_ana,
				new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );

	// MntAna
	but_mnt = new TGTextButton( comp_frame_3, "MNT analysis", -1, TGTextButton::GetDefaultGC()(),
							   TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_mnt->SetTextJustify( 36 );
	but_mnt->SetMargins( 0, 0, 0, 0 );
	but_mnt->SetWrapLength( -1 );
	but_mnt->Resize( 50, 56 );
	comp_frame_3->AddFrame( but_mnt,
				new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );
	
	// TDRIVAna
	but_tdriv = new TGTextButton( comp_frame_3, "TDRIV analysis", -1, TGTextButton::GetDefaultGC()(),
							   TGTextButton::GetDefaultFontStruct(), kRaisedFrame );
	but_tdriv->SetTextJustify( 36 );
	but_tdriv->SetMargins( 0, 0, 0, 0 );
	but_tdriv->SetWrapLength( -1 );
	but_tdriv->Resize( 50, 56 );
	comp_frame_3->AddFrame( but_tdriv,
				new TGLayoutHints( kLHintsRight | kLHintsExpandY, 2, 2, 2, 2 ) );
	
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
	main_frame->SetWindowName( "MBSorter" );
	
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
	
	// terminate ROOT session when window is closed
	main_frame->Connect( "CloseWindow()", "TApplication", gApplication, "Terminate()" );
	main_frame->DontCallClose();

	
	//////////////
	// Defaults //
	//////////////
	
	text_daq_dir->SetText( "/mbdata/miniball/is562-171009" );
	text_local_dir->SetText( "/media/MinballBackup/is562/medfiles" );
	text_med_pre->SetText( "108Sn" );

	text_add_file->SetText( "108Sn_206Pb_pos6_laser_on_053" );

	text_settings->SetText( "/media/MinballBackup/is562/config/MBSettings2017_CLX.dat" );
	check_source->SetOn();

	text_outfile->SetText( "/media/MinballBackup/is562/rootfiles/108Sn_206Pb_laser_on_xxx-yyy" );
	text_calfile->SetText( "/media/MinballBackup/is562/config/calibration-online-is546.dat" );
	text_config->SetText( "/media/MinballBackup/is562/config/config-is562.dat" );
	text_config->SetText( "/media/MinballBackup/is562/config/cutfile-is562.root" );

	check_cdpad->SetOn();
	//check_singles->SetOn();
	//check_gamgam->SetOn();
	check_addback->SetOn();
	//check_config->SetOn();

	num_dop_zb->SetNumber( 50 );
	num_dop_zt->SetNumber( 82 );
	num_dop_ab->SetNumber( 108 );
	num_dop_at->SetNumber( 208 );
	num_dop_eb->SetNumber( 4500 );
	num_dop_ex->SetNumber( 1206 );
	num_dop_th->SetNumber( 4700 );
	num_dop_id->SetNumber( 2350 );
	num_dop_cd->SetNumber( 21.1 );
	num_dop_ro->SetNumber( 242.6 );
	num_dop_dl->SetNumber( 0.7 );
	num_dop_sp->SetNumber( 26.2 );

	////////////////////
	// Button presses //
	////////////////////
	
	but_add->Connect( "Clicked()", "MBSorter", this, "on_add_clicked()" );
	text_add_file->Connect( "ReturnPressed()", "MBSorter", this, "on_add_clicked()" );
	but_del->Connect( "Clicked()", "MBSorter", this, "on_del_clicked()" );
	but_rsync->Connect( "Clicked()", "MBSorter", this, "on_rsync_clicked()" );
	but_medroot->Connect( "Clicked()", "MBSorter", this, "on_medroot_clicked()" );
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

void MBSorter::on_add_clicked() {
	
	// Slot to react to add file button
	TGString filename = text_add_file->GetText();
	
	run_list_box->AddEntrySort( filename, filelist.size() + 1 );
	run_list_box->Layout();

	filelist.push_back( filename.GetString() );
	filestatus.push_back( true );

}

void MBSorter::on_del_clicked() {
	
	// Slot to react to remove file button
	if( run_list_box->GetSelected() < 0 ) {

		cout << "No runs selected" << endl;

	}

	else {

		filestatus.at( run_list_box->GetSelected() - 1 ) = false;
		run_list_box->RemoveEntry( -1 );

	}

	run_list_box->Layout();
	
}

void MBSorter::on_rsync_clicked() {
	
	// Slot to react to rsync button
	TString cmd = "rsync -avz ";
	cmd += "miniball@hpdaqpc:";
	cmd += text_daq_dir->GetText();
	cmd += "/";
	cmd += text_med_pre->GetText();
	cmd += "*.med ";
	cmd += text_local_dir->GetText();
	cmd += "/";
	
	gSystem->Exec( cmd );

}

void MBSorter::on_medroot_clicked() {
	
	// Slot to react to MedToRoot button
	TString filebase;
	TString rootfile;
	TString cmd;
	TString cmd_base = "MedToRoot ";

	if( check_source->IsOn() ) cmd_base += "-so ";

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
		rootfile += "_Source.root";

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

		cout << cmd << endl;
		gSystem->Exec( cmd );

	}

}

void MBSorter::on_build_clicked() {
	
	// Slot to react to TreeBuilder button
	TString filebase;
	TString rootfileout;
	TString files = "";
	string answer;
	TString cmd = "TreeBuilder ";

	if( check_crex->IsOn() ) cmd += "-crex ";
	if( check_spede->IsOn() ) cmd += "-spede ";
	if( check_cdpad->IsOn() ) cmd += "-cdpad ";
	if( check_ionch->IsOn() ) cmd += "-ionch ";
	if( check_singles->IsOn() ) cmd += "-s ";
	if( check_gamgam->IsOn() ) cmd += "-gg ";
	if( check_addback->IsOn() ) cmd += "-addback ";
	if( check_verbose->IsOn() ) cmd += "-vl ";

	rootfileout = text_outfile->GetText();
	rootfileout += "_tree.root";

	if( !gSystem->AccessPathName( rootfileout ) ) {

		cout << "Are you sure you want to overwrite " << rootfileout << "? (Y/n): ";
		cin >> answer;
		
		if( answer != "Y" && answer != "y" ) return;

	}

	cmd += "-c ";
	cmd += text_calfile->GetText();
	
	for( unsigned int i = 0; i < filelist.size(); i++ ) {

		filebase = filelist.at( i );
		if( !filestatus.at( i ) ) continue;

		files += text_local_dir->GetText();
		files += "/";
		files += filebase;
		files += "_Source.root ";

	}

	cmd += " -i ";
	cmd += files;

	cmd += " -o ";
	cmd += text_outfile->GetText();
	cmd += "_tree.root";

	cout << cmd << endl;
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

	if( check_config->IsOn() ) {

		cmd += " -c ";
		cmd += text_config->GetText();

	}

	else {

		cmd += " -Zb ";
		cmd += convertInt( num_dop_zb->GetIntNumber() );
		cmd += " -Ab ";
		cmd += convertInt( num_dop_ab->GetIntNumber() );
		cmd += " -Zt ";
		cmd += convertInt( num_dop_zt->GetIntNumber() );
		cmd += " -At ";
		cmd += convertInt( num_dop_at->GetIntNumber() );
		cmd += " -Eb ";
		cmd += convertInt( num_dop_eb->GetIntNumber() );
		cmd += " -Ex ";
		cmd += convertInt( num_dop_ex->GetIntNumber() );
		cmd += " -thick ";
		cmd += convertInt( num_dop_th->GetIntNumber() );
		cmd += " -depth ";
		cmd += convertInt( num_dop_id->GetIntNumber() );
		cmd += " -cddist ";
		cmd += convertInt( num_dop_cd->GetIntNumber() );
		cmd += " -cdoffset ";
		cmd += convertInt( num_dop_ro->GetIntNumber() );
		cmd += " -cdoffset ";
		cmd += convertInt( num_dop_ro->GetIntNumber() );
		cmd += " -deadlayer ";
		cmd += convertInt( num_dop_dl->GetIntNumber() );
		cmd += " -spededist ";
		cmd += convertInt( num_dop_sp->GetIntNumber() );

	}

	if( check_cutfile->IsOn() ) {

		cmd += " -cut ";
		cmd += text_cutfile->GetText();

	}

	cmd += " -i ";
	cmd += inputfile;
	cmd += " -o ";
	cmd += outputfile;

	cout << cmd << endl;
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

	cout << cmd << endl;
	gSystem->Exec( cmd );
	
}

void MBSorter::on_tdriv_clicked() {
	
	// Slot to react to Histograms button
	TString inputfile, outputfile;
	TString cmd;

	inputfile = text_outfile->GetText();
	inputfile += "_tree.root";
	outputfile = text_outfile->GetText();
	outputfile += "_mnt.root";
	
	cmd = "TDRIVAna -i " + inputfile + " -o " + outputfile;

	cout << cmd << endl;
	gSystem->Exec( cmd );
	
}

