#include "typedefs.h"
/******************************************************************************/
/*!	\file		s_filhe.h
	\brief		GSI file header
	\author 	GSI
	\version	$Revision: 1.3 $       
	\date		$Date: 2009-01-21 07:20:38 $
*******************************************************************************/

struct cv_string						/*! comment string */
{
	INTS2 string_l; 					/*!< length of comment */
	CHARS string[78]; 					/*!< comment string */
};



struct _s_filhe 						/*! GSI file header */
{
	INTS2 filhe_tlen;					/*!< total length of data in words */
	INTS2 filhe_dlen;					/*!< length of data w/o header */
	INTS2 filhe_subtype;				/*!< subtype = 1 */
	INTS2 filhe_type;					/*!< type = 2000 */	
	INTS2 filhe_frag;					/*!< fragment begin/end */
	INTS2 filhe_used;					/*!< length used */					
	INTS4 filhe_buf;					/*!< buffer number */
	INTS4 filhe_evt;					/*!< number of fragments */
	INTS4 filhe_current_i;				/*!< internal: used by unpack */
	INTS4 filhe_stime[2];				/*!< time stamp */
	INTS4 filhe_free[4];				/*!< free[0] = 1 -> swap */
	INTS2 filhe_label_l;				/*!< length of tape label string */
	CHARS filhe_label[30];				/*!< tape label */
	INTS2 filhe_file_l;					/*!< length of file name */
	CHARS filhe_file[86];				/*!< file name */
	INTS2 filhe_user_l;					/*!< length of user name */
	CHARS filhe_user[30];				/*!< user name */
	CHARS filhe_time[24];				/*!< date and time string */ 
	INTS2 filhe_run_l;					/*!< length of run id */
	CHARS filhe_run[66];				/*!< run id */
	INTS2 filhe_exp_l;					/*!< length of explanation */
	CHARS filhe_exp[66];				/*!< explanation */
	INTS4 filhe_lines;					/*!< # of comment lines */
	struct cv_string s_strings[30];		/*!< max 30 comment lines */
};

typedef struct _s_filhe s_filhe;	/*!< GSI file header */
