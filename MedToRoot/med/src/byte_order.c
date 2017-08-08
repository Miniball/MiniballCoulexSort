/******************************************************************************/
/*!	\file		byte_order.c
	\brief		Bytewise data access
	\details	Provides functions to access data of different type
				with given byte ordering
	\author 	R. Lutter
	\version	$Revision: 1.7 $       
	\date		$Date: 2009-01-20 14:27:30 $
*****************************************************************************/

static char sccs_id[] = "%W%";

#include <string.h>
#include "byte_order.h"

/*! \brief		return \a cnt data of type \a short
	\sa 		bto_get_int32(), bto_get_string()
	\param[out] out			-- where to output short data
	\param[in]	in;			-- where to read bytes from
	\param[in]	cnt;		-- number of shorts to read
	\param[in]	bytord	 	-- byte ordering
	\return 	pointer to next input
*/

char *bto_get_short(short *out, char *in, int cnt, int bytord)
{
	int i;
	union x {
		short s;
		char b[2];
	} x;

	switch (bytord)
	{
		case BYTE_ORDER_LSW:
		case BYTE_ORDER_1_TO_1:	for (i = 0; i < cnt; i++)
								{
									x.b[0] = *in++;
									x.b[1] = *in++;
									*out++ = x.s;
								}
								break;

		case BYTE_ORDER_REV:
		case BYTE_ORDER_BSW:	for (i = 0; i < cnt; i++)
								{
									x.b[1] = *in++;
									x.b[0] = *in++;
									*out++ = x.s;
								}
								break;
	}
	return(in);
}

/*! \brief		return \a cnt data of type \a int
	\sa 		bto_get_short(), bto_get_string()
	\param[out] out			-- where to output int data
	\param[in]	in			-- where to read bytes from
	\param[in]	cnt			-- number of shorts to read
	\param[in]	bytord 	-- byte ordering
	\return 	pointer to next input
*/

char *bto_get_int32(int *out, char *in, int cnt, int bytord) {

	int i;
	union x {
		int l;
		char b[4];
	} x;

	switch (bytord)
	{
		case BYTE_ORDER_1_TO_1:	for (i = 0; i < cnt; i++)
								{
									x.b[0] = *in++;
									x.b[1] = *in++;
									x.b[2] = *in++;
									x.b[3] = *in++;
									*out++ = x.l;
								}
								break;

		case BYTE_ORDER_BSW:	for (i = 0; i < cnt; i++)
								{
									x.b[1] = *in++;
									x.b[0] = *in++;
									x.b[3] = *in++;
									x.b[2] = *in++;
									*out++ = x.l;
								}
								break;

		case BYTE_ORDER_LSW:	for (i = 0; i < cnt; i++)
								{
									x.b[2] = *in++;
									x.b[3] = *in++;
									x.b[0] = *in++;
									x.b[1] = *in++;
									*out++ = x.l;
								}
								break;

		case BYTE_ORDER_REV:	for (i = 0; i < cnt; i++)
								{
									x.b[3] = *in++;
									x.b[2] = *in++;
									x.b[1] = *in++;
									x.b[0] = *in++;
									*out++ = x.l;
								}
								break;
	}
	return(in);
}

/*! \brief		return \a cnt data of type \a char*
	\sa 		bto_get_short(), bto_get_int32()
	\param[out] out			-- where to output string data
	\param[in]	in			-- where to read bytes from
	\param[in]	cnt			-- number of shorts to read
	\param[in]	bytord  	-- byte ordering
	\return 	pointer to next input
*/

char *bto_get_string(char *out, char *in, int cnt, int bytord) {

	int i;
	char b;

	switch (bytord)
	{
		case BYTE_ORDER_REV:
		case BYTE_ORDER_1_TO_1:	if (cnt <= 0)
								{
									strcpy(out, in);
									return(in + strlen(in));
								} else {
									memcpy(out, in, cnt);
									return(in + cnt);
								}

		case BYTE_ORDER_LSW:
		case BYTE_ORDER_BSW:	if (cnt > 0)
								{
									for (i = 0; i < cnt; i += 2)
									{
										*out++ = *(in + 1);
										*out++ = *in;
										in += 2;
									}
									return(in);
								} else {
									for (;;)
									{
										b = *(in + 1);
										*out++ = b;
										if (b == '\0') return(in + 1);
										b = *in;
										*out++ = b;
										in += 2;
										if (b == '\0') return(in);
									}
								}
	}
	return(in); 		/* will not be reached */
}
