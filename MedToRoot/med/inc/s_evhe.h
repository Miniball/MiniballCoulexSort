#include "typedefs.h"

/******************************************************************************/
/*!	\file		s_evhe.h
	\brief		GSI VME General event header
	\author 	GSI
	\version	$Revision: 1.2 $       
	\date		$Date: 2009-01-20 14:27:30 $
*******************************************************************************/

struct _s_evhe			/*! GSI VME General event header */
{
	INTS4 l_dlen;		/*!< length of data in words */
	INTS2 i_subtype;	/*!< subtype */
	INTS2 i_type;		/*!< type */
};

typedef struct _s_evhe s_evhe;	/*!< GSI VME General event headerader */
