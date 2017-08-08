#ifndef __MBSio_h__
#define __MBSio_h__

/******************************************************************************/
/*! \file			mbsio.h
	\brief			Structs for MBS I/O
	\details		Defines structs to read MBS data streams
	$Author: Marabou $
	$Mail:			<a href=mailto:rudi.lutter@physik.uni-muenchen.de>R. Lutter</a>$
	$Revision: 1.25 $
	$Date: 2010-12-15 09:07:47 $
*******************************************************************************/

#include "typedefs.h"
#include "s_bufhe.h"
#include "s_evhe.h"
#include "s_filhe.h"
#include "s_vehe.h"
#include "s_veshe.h"

#define MBS_ID_WORD				"%MBS_RAW_DATA%"			/*!< magic id */
#define MBS_SIZEOF_DATA_W		0x4000						/*!< length of data (words) */
#define MBS_SIZEOF_DATA_B		0x8000						/*!< length of data (bytes) */

#define MBS_RTYPE_ERROR			0xffffffff					/*!< error */

#define MBS_CTYPE_FILE			0x1 						/*!< connection: file */
#define MBS_CTYPE_SYNC			0x2 						/*!< connection: synchronous via transport manager */
#define MBS_CTYPE_ASYNC 		0x4 						/*!< connection: asynchronous via stream server */
#define MBS_CTYPE_REMOTE		0x8 						/*!< connection: remote tape */
#define MBS_CTYPE_FILE_LMD		(MBS_CTYPE_FILE | 0x10) 	/*!< connection: file, format is LMD */
#define MBS_CTYPE_FILE_MED		(MBS_CTYPE_FILE | 0x20) 	/*!< connection: file, format is MED */

#define MBS_TY_FHEADER			0x1 						/*!< type: file header */
#define MBS_X_FHEADER			0 							/*!< index: file header */

#define MBS_TY_BUFFER			0x2 						/*!< type: buffer */
#define MBS_X_BUFFER			1 							/*!< index: buffer */
#define MBS_BTYPE_FHEADER		0x000107d0					/*!< file header [1,2000] */
#define MBS_BTYPE_VME			0x0001000a					/*!< vme event [1,10] */
#define MBS_BTYPE_EOF			0							/*!< end of file */
#define MBS_BTYPE_ERROR			0xffffffff					/*!< error */
#define MBS_BTYPE_ABORT			0xfffffffe					/*!< abort */
#define MBS_BTYPE_RAW			0xfffffffd					/*!< raw mode */

#define MBS_TY_EVENT			0x4 						/*!< type: event */
#define MBS_X_EVENT				2 							/*!< index: event */
#define MBS_ETYPE_VME			0x0001000a					/*!< vme event [1,10] */
#define MBS_ETYPE_EOF			MBS_BTYPE_EOF				/*!< end of file */
#define MBS_ETYPE_ERROR			MBS_BTYPE_ERROR				/*!< error */
#define MBS_ETYPE_ABORT			MBS_BTYPE_ABORT				/*!< abort */
#define MBS_ETYPE_RAW			MBS_BTYPE_RAW				/*!< raw mode */
#define MBS_ETYPE_WAIT			0xfffffffc					/*!< wait for write complete */
#define MBS_ETYPE_EOW			0xaffec0c0					/*!< end of wait loop */
#define MBS_ETYPE_START			0xfffffffb					/*!< start event */
#define MBS_ETYPE_STOP			0xfffffffa					/*!< stop event */

#define MBS_TY_SUBEVENT				0x8							/*!< type: subevent */
#define MBS_X_SUBEVENT				3							/*!< index: subevent */
#define MBS_STYPE_CAMAC_1			0x0001000a					/*!< subevent [1,10]: camac (chn,data) */
#define MBS_STYPE_CAMAC_WO_ID_1		0x000b000a					/*!< subevent [11,10]: camac w/o chn number */
#define MBS_STYPE_CAMAC_MULT_MOD	0x000c000a					/*!< subevent [12,10]: camac multi module */
#define MBS_STYPE_CAMAC_2			0x000d000a					/*!< subevent [13,10]: camac, data stored in hit buffer */
#define MBS_STYPE_CAMAC_WO_ID_2		0x000e000a					/*!< subevent [14,10]: camac, data stored in hit buffer */
#define MBS_STYPE_CAMAC_RAW 		0x000F000a					/*!< subevent [15,10]: camac, raw format */
#define MBS_STYPE_CAMAC_DGF_1		0x0015000a					/*!< subevent [21,10]: camac, dgf, accumulate histos directly */
#define MBS_STYPE_CAMAC_DGF_2		0x0016000a					/*!< subevent [22,10]: camac, dgf, data stored in hit buffer */
#define MBS_STYPE_CAMAC_DGF_3		0x0017000a					/*!< subevent [23,10]: camac, dgf, data stored in hit buffer */
#define MBS_STYPE_CAMAC_SILENA_1	0x001f000a					/*!< subevent [31,10]: camac, silena 4418 */
#define MBS_STYPE_CAMAC_SILENA_2	0x0020000a					/*!< subevent [32,10]: camac, silena 4418, data stored in hit buffer */
#define MBS_STYPE_VME_CAEN_V556_1	0x0021000a					/*!< subevent [33,10]: vme, caen v556 */
#define MBS_STYPE_VME_CAEN_V556_2	0x0022000a					/*!< subevent [34,10]: vme, caen v556, data stored in hit buffer */
#define MBS_STYPE_VME_CAEN_V556_3	0x0023000a					/*!< subevent [35,10]: vme, caen v556, data stored in hit buffer */
#define MBS_STYPE_VME_CAEN_1		0x0029000a					/*!< subevent [41,10]: vme, caen v875, v775 */
#define MBS_STYPE_VME_CAEN_2		0x002a000a					/*!< subevent [42,10]: vme, caen v875, v775, data stored in hit buffer */
#define MBS_STYPE_VME_CAEN_3		0x002b000a					/*!< subevent [43,10]: vme, caen v875, v775, data stored in hit buffer */
#define MBS_STYPE_VME_CAEN_V1X90_1	0x002d000a					/*!< subevent [45,10]: vme, caen v1190, v1290 */
#define MBS_STYPE_VME_CAEN_V1X90_2	0x002e000a					/*!< subevent [46,10]: vme, caen v1190, v1290, data stored in hit buffer */
#define MBS_STYPE_VME_CAEN_V1X90_3	0x002f000a					/*!< subevent [47,10]: vme, caen v1190, v1290, data stored in hit buffer */
#define MBS_STYPE_VME_SIS_1			0x0033000a					/*!< subevent [51,10]: vme, struck/sis */
#define MBS_STYPE_VME_SIS_2			0x0034000a					/*!< subevent [52,10]: vme, struck/sis, data stored in hit buffer */
#define MBS_STYPE_VME_SIS_3			0x0035000a					/*!< subevent [53,10]: vme, struck/sis, data stored in hit buffer */
#define MBS_STYPE_VME_SIS_3300		0x0036000a					/*!< subevent [54,10]: vme, struck/sis 3300, data stored in hit buffer */
#define MBS_STYPE_VME_SIS_3302_1	0x0038000a					/*!< subevent [55,10]: vme, struck/sis 3302 */
#define MBS_STYPE_VME_SIS_3302_2	0x0038000a					/*!< subevent [56,10]: vme, struck/sis 3302, data stored in hit buffer */
#define MBS_STYPE_VME_SIS_3302_3	0x0039000a					/*!< subevent [57,10]: vme, struck/sis 3302, data stored in hit buffer */
#define MBS_STYPE_CAMAC_CPTM		0x003d000a					/*!< subevent [61,10]: camac, cptm Cologne */
#define MBS_STYPE_DATA_SHORT		0x0040000a					/*!< subevent [64,10]: universal data container, short integer, 2 bytes */
#define MBS_STYPE_DATA_INT			0x0041000a					/*!< subevent [65,10]: universal data container, integer, 4 bytes */
#define MBS_STYPE_DATA_FLOAT		0x0042000a					/*!< subevent [66,10]: universal data container, float/double */
#define MBS_STYPE_HITBUF_1			0x0047000a					/*!< subevent [71,10]: hit buffer */
#define MBS_STYPE_VME_MADC_1		0x0051000a					/*!< subevent [81,10]: vme, mesytec madc32 */
#define MBS_STYPE_VME_MADC_2		0x0052000a					/*!< subevent [82,10]: vme, mesytec madc32, data stored in hit buffer */
#define MBS_STYPE_VME_MADC_3		0x0053000a					/*!< subevent [83,10]: vme, mesytec madc32, data stored in hit buffer */
#define MBS_STYPE_VME_SIS_3820_1	0x005b000a					/*!< subevent [91,10]: vme, sis 3820 scaler */
#define MBS_STYPE_VME_SIS_3820_2	0x005c000a					/*!< subevent [92,10]: vme, sis 3820 scaler, data stored in hit buffer */
#define MBS_STYPE_VME_SIS_3820_3	0x005d000a					/*!< subevent [93,10]:vme, sis 3820 scaler, data stored in hit buffer */
#define MBS_STYPE_TIME_STAMP		0x00012328					/*!< subevent [1,9000]: time stamp */
#define MBS_STYPE_DEAD_TIME 		0x00022328					/*!< subevent [2,9000]: dead time */
#define MBS_STYPE_DUMMY 	 		0x006f006f					/*!< subevent [111,111]: dummy */
#define MBS_STYPE_EOE				MBS_BTYPE_EOF				/*!< end of event */
#define MBS_STYPE_ERROR				MBS_BTYPE_ERROR				/*!< error */
#define MBS_STYPE_ABORT				MBS_BTYPE_ABORT				/*!< abort */
#define MBS_STYPE_RAW		 		MBS_BTYPE_RAW				/*!< raw mode */

#define MBS_TY_STAT				0x10						 	/*!< type: statistics */
#define MBS_X_STAT				4						 		/*!< index: statistics */

#define MBS_N_BELEMS			5								/*!< number if buffer elements */
#define MBS_N_TRIGGERS			16								/*!< number of triggers */
#define MBS_N_BUFFERS			5								/*!< number of buffers in buffer ring */

#define MBS_L_STR				256 							/*!< string length */
#define MBS_L_NAME				64								/*!< lenght of a name string */

#define MBS_ODD_NOF_PARAMS		1								/*!< indicates an odd number of params */

typedef int boolean;											/*!< type: boolean */

struct _MBSShowElem  		/*! structure to describe how to show a buffer element */
{
	FILE *out;				/*!< stream to send output to */
	int redu;				/*!< reduction */
};

typedef struct _MBSShowElem MBSShowElem; /*!< mbsio: show a buffer element */

struct _MBSBufferElem 		/*! structure to describe how to process a buffer element */
{
	unsigned int type;		/*!< element type */
	char *descr;			/*!< description */
	int hsize;				/*!< header size (bytes) */
	int hit;				/*!< # of hits */
	int (*unpack)();		/*!< function to unpack element */
	int (*show)();			/*!< function to to show element */
	int (*convert)();		/*!< function to convert element data */
};

typedef struct _MBSBufferElem MBSBufferElem;	 /*!< mbsio: process a buffer element */

struct _MBSServerInfo 			/*! server info from MBS */
{
	boolean is_swapped; 		/*!< true, if data swapped */
	int buf_size; 				/*!< buffer size */
	int buf_p_stream; 			/*!< buffers per stream */
	int nof_streams; 			/*!< number of streams */
};

typedef struct _MBSServerInfo MBSServerInfo; 		/*!< mbsio: server info from MBS */

struct _MBSBufferPool			/*! establish a buffer pool */
{
	int bufno_mbs;				/*!< MBS buffer number */
	char * data;				/*!< address of data */
};

typedef struct _MBSBufferPool MBSBufferPool;	/*!< mbsio: establish a buffer pool */

struct _MBSDataIO							/*! structure to describe MBS i/o */
{
	char id[16];							/*!< internal struct id: %MBS_RAW_DATA% */
	FILE *input;							/*!< stream descr (fopen/fread) */
	int fileno;								/*!< channel # (open/read) */
	int filepos;							/*!< file position */
	char device[MBS_L_STR];					/*!< name of input dev */
	char host[MBS_L_STR];					/*!< host name */
	unsigned int connection;				/*!< device type, MBS_DTYPE_xxxx */
	boolean running;						/*!< trigger 14 found - running */
	MBSBufferElem *buftype;					/*!< buffer typeE_xxxx */
	int byte_order;							/*!< byte ordering */
	MBSShowElem show_elems[MBS_N_BELEMS];	/*!< buffer elements to be shown automatically */
	int bufsiz;								/*!< buffer sizeE_xxxx */
	MBSServerInfo *server_info;				/*!< info block for server access */
	int max_streams;						/*!< max # of streams to process */
	int slow_down;							/*!< # of secs to wait after each stream */
	int nof_streams;						/*!< # of streams processed so far */
	int nof_buffers;						/*!< # of buffers */
	int nof_events;							/*!< # of events */
	int cur_bufno;							/*!< buffer number */
	int cur_bufno_stream;					/*!< buffer number within current stream */
	int bufno_mbs;							/*!< buffer number as given by MBS */
	boolean buf_to_be_dumped;				/*!< if N>0 every Nth buffer will be dumped */
	unsigned long long buf_ts;				/*!< buffer time stamp (vms format) */
	unsigned long long buf_ts_start;		/*!< time stamp of first buffer */
	char *hdr_data;							/*!< file header data */
	MBSBufferPool buf_pool[MBS_N_BUFFERS];	/*!< buffer pool */
	MBSBufferPool * poolpt;					/*!< pointer to current buffer in pool */
	char *bufpt;							/*!< pointer to current data */
	boolean buf_valid;						/*!< TRUE if buffer data valid */
	int buf_oo_phase;						/*!< buffer out of phase */
	MBSBufferElem *evttype;					/*!< event type */
	int evtsiz;								/*!< event size (bytes) */
	char *evtpt;							/*!< ptr to current event in buffer */
	int evtno;								/*!< current event number within buffer */
	int evtno_mbs;							/*!< event number as given by MBS */
	char *evt_data;							/*!< copy of event data (original, byte-swapped if necessary) */
	MBSBufferElem *sevttype;				/*!< subevent type */
	int sevtsiz;							/*!< subevent size (bytes) */
	char *sevtpt;							/*!< ptr to original subevent in evt_data */
	int sevtno;								/*!< subevent number within event */
	int nof_sevents;						/*!< # of subevents */
	int sevt_id;							/*!< current subevent id */
	unsigned int sevt_otype;				/*!< original subevent [subtype,type] */
	int sevt_minwc;							/*!< min # of data words */
	int sevt_wc;							/*!< # of data words */
	char *sevt_data;						/*!< subevent data (unpacked) */
};

typedef struct _MBSDataIO MBSDataIO;	/*!< mbsio: main struct to keep track of i/o */

struct _MBSCamacData			/*! camac data word */
{
	unsigned short data;		/*!< data word */
	short subaddr;				/*!< camac subaddr A(x)+1 */
};

typedef struct _MBSCamacData MBSCamacData;		/*!< mbsio: camac data word (subevent type [10,1]) */

struct _MBSCamacDataSW		/*! camac data word */
{
	unsigned short data;		/*!< data word */
	short subaddr;				/*!< camac subaddr A(x)+1 */
};

typedef struct _MBSCamacDataSW MBSCamacDataSW;	/*!< mbsio: camac data word (subevent type [10,1]) */

#endif
