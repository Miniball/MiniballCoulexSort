#include "typedefs.h"

/******************************************************************************/
/*!	\file		s_veshe.h
	\brief		GSI VME subevent header
	\author 	GSI
	\version	$Revision: 1.2 $       
	\date		$Date: 2009-01-20 14:27:30 $
*******************************************************************************/

struct _s_veshe 			/*! GSI VME subevent header */
{
	INTS4 l_dlen;			/*!< data length + 2 in words */
	INTS2 i_subtype;		/*!< subtype */ 
	INTS2 i_type;			/*!< type */ 
	CHARS h_control;		/*!< processor type */
	CHARS h_subcrate;		/*!< crate number */
	INTS2 i_procid; 		/*!< processor ID */
};

typedef struct _s_veshe s_veshe;	/*!< GSI VME subevent header */
