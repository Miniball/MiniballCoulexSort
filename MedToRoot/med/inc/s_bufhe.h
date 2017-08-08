#include "typedefs.h"

/******************************************************************************/
/*! \file			s_bufhe.h
	\brief			GSI buffer header
	\author 		GSI
	\version		$Revision: 1.2 $       
	\date           $Date: 2009-01-20 14:27:30 $
*******************************************************************************/

struct _s_bufhe 			/*! GSI buffer header */
{
	INTS4  l_dlen;			/*!< length of data field in words */
	INTS2  i_subtype;		/*!< subtype */ 
	INTS2  i_type;			/*!< type */
	CHARS  h_begin; 		/*!< fragment at end of buffer */
	CHARS  h_end;	 		/*!< fragment at begin of buffer */
	INTS2  i_used;			/*!< used length of data field in words */
	INTS4  l_buf;			/*!< current buffer number */
	INTS4  l_evt;			/*!< number of fragments */
	INTS4  l_current_i;		/*!< index, temporarily used */
	INTS4  l_time[2];		/*!< time stamp */
	INTS4  l_free[4];		/*!< [0] byte order tag, [1] length of last event, [2,3] free */
};

typedef struct _s_bufhe s_bufhe;	/*!< GSI buffer header */
