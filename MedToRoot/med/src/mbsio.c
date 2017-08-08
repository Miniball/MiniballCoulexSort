/******************************************************************************/
/*!	\file		mbsio.c
	\brief		MBS raw data input
	\details	Procedures to read MBS data from disk or tcp socket
	$Author: Marabou $
	$Mail:		<a href=mailto:rudi.lutter@physik.uni-muenchen.de>R. Lutter</a>$
	$Revision: 1.49 $
	$Date: 2010-12-15 09:07:47 $
*****************************************************************************/

/* include files needed by mbsio */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include "byte_order.h"

#define TRUE	1			/*!< boolean TRUE */
#define FALSE	0			/*!< boolean FALSE */

#include "mbsio.h"

/* include files needed by m.muench's tcp package */

#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define C_STYLE_PROTOS		/*!< use C-style prototyping when in a C environment */
#include "mbsio_protos.h"

/* if you want to have remote tape access */

extern int errno;

char loc_errbuf[MBS_L_STR]; 		/*!< where to store error messages */
char *rem_errbuf = NULL;			/*!< ptr to remote error buffer */

char loc_logbuf[MBS_L_STR]; 		/*!< where to store log data */
char log_file[MBS_L_STR]; 			/*!< name of log file */
FILE * log_out = NULL; 				/*!< log file stream */

char med_file[MBS_L_STR]; 			/*!< name of MED file */
FILE * med_out = NULL; 				/*!< MED file stream */

char lmd_file[MBS_L_STR];			/*!< name of LMD file */
FILE * lmd_out = NULL; 				/*!< LMD file stream */

int total = 0;


static MBSBufferElem buffer_types[] = {
				{	MBS_BTYPE_FHEADER,		// [subtype,type]
					"File header",			// description
					sizeof(s_filhe),		// size
					0,						// hits
					NULL,					// proc to unpack
					(void *) _mbs_show_fheader,		// proc to show data
					(void *) _mbs_copy_fheader		// proc to convert data
				},
				{	MBS_BTYPE_VME,
					"VME buffer",
					sizeof(s_bufhe),
					0,
					NULL,
					(void *) _mbs_show_bheader,
					(void *) _mbs_convert_bheader
				},
				{0, "", 0,  0,  NULL,   NULL,   NULL}
			};

static MBSBufferElem event_types[] = {
				{	MBS_ETYPE_VME,
					"Standard MBS event",
					sizeof(s_vehe),
					0,
					NULL,
					(void *) _mbs_show_evhe_10_1,
					(void *) _mbs_convert_eheader
				},
				{0, "", 0,  0,  NULL,   NULL,   NULL}
			};

static MBSBufferElem sevent_types[] = {
				{	MBS_STYPE_CAMAC_1,
					"Chn + Data",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_10_1,
					(void *) _mbs_show_sev_10_1,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_CAMAC_WO_ID_1,
					"Data w/o Chn",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_CAMAC_2,
					"Chn + Data",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_10_1,
					(void *) _mbs_show_sev_10_1,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_CAMAC_WO_ID_2,
					"Data w/o Chn",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_TIME_STAMP,
					"Time stamp",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_9000_X,
					(void *) _mbs_show_sev_9000_1,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_DEAD_TIME,
					"Dead time",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_9000_X,
					(void *) _mbs_show_sev_9000_2,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_CAMAC_DGF_1,
					"XIA DGF-4C (1)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_CAMAC_DGF_2,
					"XIA DGF-4C (2)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_CAMAC_DGF_3,
					"XIA DGF-4C (3, time stamp)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_CAMAC_SILENA_1,
					"Silena 4418 (1, zero suppr)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_CAMAC_SILENA_2,
					"Silena 4418 (2, zero suppr)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_CAEN_V556_1,
					"Caen VME ADCs V556 (1)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_CAEN_V556_2,
					"Caen VME ADCs V556 (2)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_CAEN_V556_3,
					"Caen VME ADCs V556 (3)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_CAEN_1,
					"Caen VME ADCs/TDCs (1)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_CAEN_2,
					"Caen VME ADCs/TDCs (2)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_CAEN_3,
					"Caen VME ADCs/TDCs (3)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_CAEN_V1X90_1,
					"Caen VME TDCs V1X90 (1)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_CAEN_V1X90_2,
					"Caen VME TDCs V1X90 (2)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_CAEN_V1X90_3,
					"Caen VME TDCs V1X90 (3)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_SIS_1,
					"SIS VME modules (1)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_SIS_2,
					"SIS VME modules (2)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_SIS_3,
					"SIS VME modules (3)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_SIS_3300,
					"SIS 3300 VME modules",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_long,
					(void *) _mbs_show_sev_long,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_SIS_3302_1,
					"SIS 3302 VME modules (1)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_long,
					(void *) _mbs_show_sev_long,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_SIS_3302_2,
					"SIS 3302 VME modules (2)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_long,
					(void *) _mbs_show_sev_long,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_SIS_3302_3,
					"SIS 3302 VME modules (3)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_long,
					(void *) _mbs_show_sev_long,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_HITBUF_1,
					"Hitbuffer (1)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_MADC_1,
					"Mesytec MADC modules (1)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_MADC_2,
					"Mesytec MADC modules (2)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_VME_MADC_3,
					"Mesytec MADC modules (3)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_DATA_SHORT,
					"Plain data (16 bit)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_DATA_INT,
					"Plain data (32 bit)",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_short,
					(void *) _mbs_show_sev_short,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_DUMMY,
					"MBS dummy",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_10_1,
					(void *) _mbs_show_sev_10_1,
					(void *) _mbs_convert_sheader
				},
				{	MBS_STYPE_RAW,
					"Raw data",
					sizeof(s_veshe),
					0,
					(void *) _mbs_unpack_sev_raw,
					(void *) _mbs_show_sev_raw,
					(void *) _mbs_convert_sheader
				},
				{0, "", 0,  0,  NULL,   NULL,   NULL}
			};

static MBSBufferElem * sevent_type_raw = NULL;

static MBSBufferElem triggers[MBS_N_TRIGGERS];

static MBSBufferElem buffer_type_error =
				{	MBS_BTYPE_ERROR,
					"Illegal buffer type",
					0,
					0,
					(void *) _mbs_type_error,
					(void *) _mbs_type_error,
					(void *) _mbs_type_error
				};

#if 0
static MBSBufferElem event_type_error =
				{	MBS_ETYPE_ERROR,
					"Illegal event type",
					0,
					0,
					(void *) _mbs_type_error,
					(void *) _mbs_type_error,
					(void *) _mbs_type_error
				};

static MBSBufferElem sevent_type_error =
				{	MBS_STYPE_ERROR,
					"Illegal subevent type",
					0,
					0,
					(void *) _mbs_type_error,
					(void *) _mbs_type_error,
					(void *) _mbs_type_error
				};
#endif

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Open a MBS data file
	\details	Opens an input stream to read mbs data from
	\sa 		mbs_close_file()
	\param[in]	device			-- name of input device
	\param[in]	connection		-- mode of connection: f(ile), s(ynchronous), or a(synchronous)
	\param[in]	bufsiz			-- buffer size, default = 16 kbytes
	\param[in]	out				-- stream to send header info to (NULL = don't output header)
	\retval 	mbs 			-- pointer to MBS database or NULL
*/
/*-------------------------------------------------------------------------------------------*/

MBSDataIO *mbs_open_file(char *device, char *connection, int bufsiz, FILE *out) {

	FILE *input;
	MBSDataIO *mbs;
	char cmode;
	char *dev;
	char *ext;
	int fno;
	int bps;
	unsigned int ctype;
	unsigned int buffer_type;
	char * xp;

	MBSBufferElem *tlist;

	char host[MBS_L_STR];
	MBSServerInfo server_info;

	if (sevent_type_raw == NULL) {
		tlist = sevent_types;
		while (tlist->type != 0) {
			if (tlist->type == MBS_STYPE_RAW) {
				sevent_type_raw = tlist;
				break;
			}
		tlist++;
		}
	}

	cmode = tolower(*connection);

	input = NULL;
	fno = 0;

	if (cmode == 'f') {
		ctype = MBS_CTYPE_FILE;
		dev = strchr(device, ':');
		if (dev != NULL) {
			ctype |= MBS_CTYPE_REMOTE;
			*dev++ = '\0';
			strcpy(host, device);
		} else {
			dev = device;
			host[0] = '\0';
		}

		if (ctype & MBS_CTYPE_REMOTE) {
			sprintf(loc_errbuf, "?NO_REM-[mbs_open_file]- %s: No REMOTE tape access", device);
			_mbs_output_error(NULL);
			return(NULL);
		} else {
			if (strcmp(device, "-") == 0) {
				input = stdin;
				device = "<stdin>";
			} else if ((input = fopen(device, "r")) == NULL) {
				sprintf(loc_errbuf, "?SYSERR-[mbs_open_file]- %s: %s (%d)",
													device, strerror(errno), errno);
				_mbs_output_error(NULL);
				return(NULL);
			}
			xp = device;
			ext = NULL;
			while (strchr(xp, '.') != NULL) {
				ext = strchr(xp, '.');
				xp = ext + 1;
			}
			if (ext != NULL) {
				if (strcmp(ext, ".med") == 0) ctype = MBS_CTYPE_FILE_MED;
				else if (strcmp(ext, ".lmd") == 0) ctype = MBS_CTYPE_FILE_LMD;
			}
			fno = fileno(input);
		}
	} else if (cmode == 's') {
		ctype = MBS_CTYPE_SYNC;
	} else if (cmode == 'a') {
		ctype = MBS_CTYPE_ASYNC;
	} else {
		sprintf(loc_errbuf
,
				"?ILLMOD-[mbs_open_file]- %s: Illegal I/O mode \"%s\"",
							device, connection);
		_mbs_output_error(NULL);
		return(NULL);
	}

	bps = 1;
	if (ctype & (MBS_CTYPE_SYNC | MBS_CTYPE_ASYNC)) {
		strcpy(host, device);
		fno = _mbs_connect_to_server(host, ctype);
		if (fno == -1) {
			_mbs_output_error(NULL);
			return(NULL);
		}
		if (_mbs_read_server_info(fno, &server_info) == NULL) {
			_mbs_output_error(NULL);
			return(NULL);
		}
		bufsiz = server_info.buf_size;
		bps = server_info.buf_p_stream;
		input = fdopen(fno, "r");
	}

	bufsiz = (bufsiz <= 0) ? MBS_SIZEOF_DATA_B : bufsiz;

	mbs = (MBSDataIO *) calloc(1, sizeof(MBSDataIO));
	if (mbs == NULL)
	{
		sprintf(loc_errbuf, "?ALLOC-[mbs_open_file]- %s: Can't allocate data structure",
															device);
		_mbs_output_error(NULL);
		return(NULL);
	}

	strcpy((char *) mbs->id, (char *) MBS_ID_WORD);
	mbs->input = input;
	mbs->fileno = fno;
	mbs->filepos = -1;
	mbs->cur_bufno_stream = bps;
	strcpy((char *) mbs->device, (char *) device);
	strcpy((char *) mbs->host, (char *) host);
	mbs->connection = ctype;
	mbs->bufsiz = bufsiz;
	mbs->bufno_mbs = -1;
	mbs->evtno_mbs = -1;
	mbs->buf_to_be_dumped = 0;

	if ((ctype & MBS_CTYPE_FILE) == MBS_CTYPE_FILE) {
		mbs->server_info = NULL;
	} else {
		mbs->server_info = calloc(1, sizeof(MBSServerInfo));
		memcpy(mbs->server_info, &server_info, sizeof(MBSServerInfo));
	}

	mbs->hdr_data = calloc(1, bufsiz);
	if (mbs->hdr_data == NULL)
	{
		sprintf(loc_errbuf, "?ALLOC-[mbs_open_file]- %s: Can't allocate internal buffer", device);
		_mbs_output_error(NULL);
		return(NULL);
	}

	if (out) {
		mbs->show_elems[MBS_X_FHEADER].out = out;
		mbs->show_elems[MBS_X_FHEADER].redu = 1;
	}

	_mbs_init_hit(buffer_types);
	_mbs_init_hit(event_types);
	_mbs_init_hit(sevent_types);
	_mbs_init_triggers();

	_mbs_init_pool(mbs);

	mbs->buf_valid = FALSE;

	if ((ctype & MBS_CTYPE_FILE_LMD) == MBS_CTYPE_FILE_LMD) {
		buffer_type = _mbs_next_buffer(mbs);
		if (buffer_type == MBS_BTYPE_ERROR || buffer_type == MBS_BTYPE_ABORT) return(NULL);
		if (buffer_type == MBS_BTYPE_FHEADER) mbs->buf_valid = FALSE;
	}

	return(mbs);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Close MBS data file
	\details	Close i/o stream and free memory
	\sa 		mbs_open_file()
	\param[in]	mbs 	-- ptr as returned by mbs_open_file()
	\return 	TRUE/FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean mbs_close_file(MBSDataIO *mbs) {

	if (!_mbs_check_active(mbs)) return(FALSE);

	if (mbs->connection & MBS_CTYPE_FILE) {
		fclose(mbs->input);
	} else {
		_mbs_disconnect_from_server(mbs->fileno, mbs->connection);
		free(mbs->server_info);
	}

	if (log_out != NULL) {
		sprintf(loc_logbuf,
			"[mbs_close_file]- %s: %d buffer(s), %d out of phase, %d event(s)",
					mbs->device, mbs->nof_buffers, mbs->buf_oo_phase, mbs->nof_events);
		_mbs_output_log(mbs);
		fclose(log_out);
	}

	mbs->fileno = -1;		/* indicate "file inactive" */
	return(TRUE);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Free data base
	\details	Free memory allocated so far
	\param[in] 	mbs 	-- ptr as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void mbs_free_dbase(MBSDataIO * mbs) {

	if (mbs != NULL) {
		free(mbs->hdr_data);
		free(mbs->evt_data);
		free(mbs->sevt_data);
		_mbs_free_pool(mbs);
		free(mbs);
	}
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		[internal] Setup access to next buffer
	\details	Requests next buffer either from data source or from buffer pool
	\param[in] 	mbs 	-- ptr to MBS database as returned by mbs_open_file()
	\retval 	type	-- buffer type: MBS_BTYPE_xxxx
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int _mbs_next_buffer(MBSDataIO *mbs) {

	register MBSBufferPool * bpp;
	int sc;
	unsigned int buffer_type;
	s_bufhe * bh;

	void (*s)();

	if (!_mbs_check_active(mbs)) return(MBS_BTYPE_ABORT);

	bpp = NULL;
	buffer_type = 0;
	while (bpp == NULL) {
		bpp = _mbs_find_subseq_buffer(mbs);
		if (bpp == NULL) {
			buffer_type = _mbs_read_buffer(mbs);
			if (buffer_type == MBS_BTYPE_VME) {
				if (_mbs_check_buffer_empty(mbs)) {
					if (log_out != NULL) {
						sprintf(loc_errbuf,
				"%%KPALIV-[_mbs_next_buffer]- %s: received KEEP ALIVE buffer", mbs->device);
						_mbs_output_error(mbs);
					}
					(mbs->poolpt)->bufno_mbs = -1;
					continue;
				} else if (!_mbs_check_sequence(mbs, MBS_TY_BUFFER)) {
					mbs->buf_oo_phase++;
					if (log_out != NULL) {
						sprintf(loc_errbuf,
				"?ILLSEQ-[_mbs_next_buffer]- %s: illegal buffer sequence - last=%d <> this=%d",
							mbs->device, mbs->bufno_mbs, (mbs->poolpt)->bufno_mbs);
						_mbs_output_error(mbs);
					}
				} else {
					bh = (s_bufhe *) (mbs->poolpt)->data;
					mbs->bufno_mbs = bh->l_buf;
					bpp = mbs->poolpt;
				}
			} else if (buffer_type == MBS_BTYPE_FHEADER) {
				break;
			} else {
				return(buffer_type);
			}
		} else {
			mbs->poolpt = bpp;
			mbs->bufpt = bpp->data;
		}
	}

	(mbs->poolpt)->bufno_mbs = -1;

	mbs->buf_valid = TRUE;
	mbs->evtno = 0;
	mbs->evtpt = NULL;
	mbs->sevtno = 0;
	mbs->sevtpt = NULL;
	mbs->sevt_wc = 0;

	if (buffer_type == MBS_BTYPE_FHEADER) {
		mbs->nof_events = 0;

		sc = mbs->show_elems[MBS_X_FHEADER].redu;
		if (sc > 0) {
			s = (void *) (mbs->buftype)->show;			// show file header
			if (s != NULL) (*s)(mbs, mbs->show_elems[MBS_X_FHEADER].out);
		}
	} else {
		sc = mbs->show_elems[MBS_X_BUFFER].redu;
		if (sc > 0 && ((mbs->nof_buffers % sc) == 0)) {
			s = (void *) (mbs->buftype)->show;			// show buffer header
			if (s != NULL) (*s)(mbs, mbs->show_elems[MBS_X_BUFFER].out);
		}

		sc = mbs->show_elems[MBS_X_STAT].redu;
		if (sc > 0 && ((mbs->nof_buffers % sc) == 0))
				mbs_show_stat(mbs, mbs->show_elems[MBS_X_STAT].out);
	}

	return(buffer_type);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		[internal] Read next buffer
	\details	Reads one buffer and determines buffer type
	\param[in] 	mbs 	-- ptr to MBS database as returned by mbs_open_file()
	\retval 	type	-- buffer type: MBS_BTYPE_xxxx
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int _mbs_read_buffer(MBSDataIO *mbs) {

	register MBSBufferPool * bpp;
	int bytes;
	int bytes_read;
	unsigned int buffer_type;


	if (!_mbs_check_active(mbs)) return(MBS_BTYPE_ABORT);

	bytes = mbs->bufsiz;

	bpp = _mbs_get_pool_pointer(mbs);
	if (bpp == NULL) return(MBS_BTYPE_ABORT);
	mbs->poolpt = bpp;

	bytes_read = 0;
	if (mbs->connection & MBS_CTYPE_FILE) {
		if (mbs->connection & MBS_CTYPE_REMOTE) {
			sprintf(loc_errbuf, "?NO_REM-[_mbs_read_buffer]- %s: No REMOTE tape access",
																mbs->device);
			_mbs_output_error(mbs);
			return(MBS_BTYPE_ABORT);
		} else {
			bytes_read = read(mbs->fileno, bpp->data, bytes);
		}
		mbs->cur_bufno_stream = 1;
		mbs->bufpt = bpp->data;
	} else {
		if ((mbs->max_streams > 0) && (mbs->nof_streams > mbs->max_streams)) {
			sprintf(loc_errbuf, "%%MXNSTR-[_mbs_read_buffer]- Max # of streams read: %d", mbs->max_streams);
			_mbs_output_error(mbs);
			mbs_close_file(mbs);
			return(MBS_BTYPE_EOF);
		} else if (mbs->connection & MBS_CTYPE_ASYNC) {
			if ((mbs->slow_down > 0) && (mbs->nof_streams > 0)) sleep(mbs->slow_down);
			_mbs_request_stream(mbs->fileno);
			bytes_read = _mbs_read_stream(mbs->fileno, bpp->data, mbs->server_info);
			mbs->cur_bufno_stream = 1;
			mbs->bufpt = bpp->data;
			mbs->nof_streams++;
		} else if (mbs->connection & MBS_CTYPE_SYNC) {
			bytes_read = _mbs_read_stream(mbs->fileno, bpp->data, mbs->server_info);
			mbs->cur_bufno_stream = 1;
			mbs->bufpt = bpp->data;
			mbs->nof_streams++;
		}
	}

	if (bytes_read == 0) return(MBS_BTYPE_EOF);

	if (bytes_read == -1)
	{
		sprintf(loc_errbuf, "?INPERR-[_mbs_read_buffer]- %s (buf %d): %s (%d)",
							mbs->device, mbs->cur_bufno, strerror(errno), errno);
		_mbs_output_error(mbs);
		return(MBS_BTYPE_ABORT);
	}

	mbs->nof_buffers++;

	if (bytes_read != bytes)
	{
		sprintf(loc_errbuf,
		"%%INPERR-[_mbs_read_buffer]- %s (buf %d): Short byte count %d (should be %d)",
								mbs->device, mbs->cur_bufno, bytes_read, bytes);
		_mbs_output_error(mbs);
	}

	if (mbs->buf_to_be_dumped > 0 && mbs->nof_buffers % mbs->buf_to_be_dumped == 0) _mbs_dump_buffer(mbs);

	buffer_type = _mbs_convert_data(mbs);
	if (buffer_type == MBS_BTYPE_ERROR || buffer_type == MBS_BTYPE_ABORT) return(buffer_type);

	_mbs_store_bufno(mbs);
	_mbs_store_time_stamp(mbs);

	return(buffer_type);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Setup access to next event
	\details	Sets ptrs to next event struct
	\param[in] 	mbs 	-- ptr to MBS database as returned by mbs_open_file()
	\retval 	type	-- event type: MBS_ETYPE_xxxx
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int mbs_next_event(MBSDataIO *mbs) {

	if (!_mbs_check_active(mbs)) return(MBS_ETYPE_ABORT);

	if ((mbs->connection & MBS_CTYPE_FILE_MED) == MBS_CTYPE_FILE_MED)	return(_mbs_next_med_event(mbs));
	else																return(_mbs_next_lmd_event(mbs));
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Return event trigger
	\details	Returns trigger number assigned to current event
	\param[in] 	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	trigger 	-- event trigger
*/
/*-------------------------------------------------------------------------------------------*/

int mbs_get_event_trigger(MBSDataIO *mbs) {

	return(((s_vehe *) mbs->evt_data)->i_trigger);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		[internal] Setup access to next event
	\details	Sets ptrs to next event struct
				(expects original LMD data from tcp or file)
	\param[in] 	mbs 	-- ptr to MBS database as returned by mbs_open_file()
	\retval 	type	-- event type: MBS_ETYPE_xxxx
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int _mbs_next_lmd_event(MBSDataIO *mbs) {

	s_bufhe *bh;
	s_evhe *eh;
	s_vehe *vh;
	unsigned int bo;
	unsigned int btype, etype;
	int evl, evlsv;
	int frag1, frag2;
	int sc;
	unsigned int (*s)();

	unsigned char eHdr[sizeof(s_vehe)];
	int ehs;

	bh = (s_bufhe *) mbs->bufpt;
	bo = mbs->byte_order;

	if (mbs->evtpt == NULL || mbs->evtno == bh->l_evt) {
		if (!mbs->buf_valid) {
			btype = _mbs_next_buffer(mbs);
			if (btype == MBS_BTYPE_ERROR || btype == MBS_BTYPE_ABORT || btype == MBS_BTYPE_EOF) return(btype);
			bo = mbs->byte_order;
			bh = (s_bufhe *) mbs->bufpt;
			mbs->cur_bufno = bh->l_buf;
		}

		if (bh->h_begin == 1) {
			sprintf(loc_errbuf,
			"?EVTERR-[_mbs_next_lmd_event]- %s (buf %d, evt 1): Illegal event spanning - unexp frag on top",
						mbs->device, mbs->cur_bufno);
			_mbs_output_error(mbs);
			mbs->buf_valid = FALSE;
			return(MBS_ETYPE_ERROR);
		}
		mbs->evtpt = mbs->bufpt + sizeof(s_bufhe);
		mbs->evtno = 1;
	} else if (mbs->evtno == 1 && bh->h_begin == 1) {
		mbs->evtpt = mbs->bufpt + sizeof(s_bufhe);
		eh = (s_evhe *) mbs->evtpt;
		bto_get_int32(&evl, (char *) &eh->l_dlen, 1, bo);
		mbs->evtpt += evl * sizeof(unsigned short) + sizeof(s_evhe);
		mbs->evtno++;
	} else {
		eh = (s_evhe *) mbs->evt_data;
		mbs->evtpt += eh->l_dlen * sizeof(unsigned short) + sizeof(s_evhe);
		mbs->evtno++;
	}
	eh = (s_evhe *) mbs->evtpt;
	mbs->buf_valid = FALSE;

	if (mbs->evtno == bh->l_evt && bh->h_end == 1) {
		evl = bh->l_free[1];
		bto_get_int32(&frag1, (char *) &eh->l_dlen, 1, bo);
	} else {
		bto_get_int32(&evl, (char *) &eh->l_dlen, 1, bo);
		frag1 = evl;
	}

	evlsv = evl;
	evl = evl * sizeof(unsigned short) + sizeof(s_evhe);
	frag1 = frag1 * sizeof(unsigned short) + sizeof(s_evhe);

	if (evl <= mbs->evtsiz) {
		memset(mbs->evt_data, 0, mbs->evtsiz);
	} else {
		if (mbs->evt_data != NULL) free(mbs->evt_data);
		mbs->evt_data = calloc(1, evl);
		memset(mbs->evt_data, 0, evl);
	}
	mbs->evtsiz = evl;
 	memcpy(mbs->evt_data, mbs->evtpt, frag1);

	eh = (s_evhe *) mbs->evt_data;
	bto_get_int32((int *) &etype, (char *) &eh->i_subtype, 1, bo);

	mbs->evttype = _mbs_check_type(etype, mbs->evttype, event_types);
	etype = (mbs->evttype)->type;
	if (etype == MBS_ETYPE_ERROR || etype == MBS_ETYPE_ABORT) {
		sprintf(loc_errbuf,
		"?EVTERR-[_mbs_next_lmd_event]- %s (buf %d, evt %d): Not a legal event type - %#x",
					mbs->device, mbs->cur_bufno, mbs->evtno, etype);
		_mbs_output_error(mbs);
		return(etype);
	}

	memcpy(eHdr, mbs->evt_data, sizeof(s_vehe)); 	/* save header ###unswapped### */

	s = (void *) (mbs->evttype)->convert;
	(*s)(mbs);

	frag2 = 0;

	while (frag1 < evl) {
		btype = _mbs_next_buffer(mbs);
		if (btype == MBS_BTYPE_ERROR || btype == MBS_BTYPE_ERROR || btype == MBS_BTYPE_EOF) return(btype);
		bh = (s_bufhe *) mbs->bufpt;
		mbs->cur_bufno = bh->l_buf;

		if (bh->h_begin != 1) {
			sprintf(loc_errbuf,
			"?EVTERR-[_mbs_next_lmd_event]- %s (buf %d, evt %d): Illegal event spanning - 2nd frag missing",
						mbs->device, mbs->cur_bufno, mbs->evtno);
			_mbs_output_error(mbs);
			return(MBS_ETYPE_ABORT);
		}

		mbs->evtpt = mbs->bufpt + sizeof(s_bufhe);
		mbs->evtno = 1;
		eh = (s_evhe *) mbs->evtpt;
		bto_get_int32(&frag2, (char *) &eh->l_dlen, 1, bo);
		frag2 *= sizeof(unsigned short);

		memcpy(mbs->evt_data + frag1, mbs->evtpt + sizeof(s_evhe), frag2);
		mbs->evtno = 1;
		frag1 += frag2;
	}

	if (frag1 != evl) {
		sprintf(loc_errbuf,
		"?EVTERR-[_mbs_next_lmd_event]- %s (buf %d, evt %d): Illegal event fragmentation",
					mbs->device, mbs->cur_bufno, mbs->evtno);
		_mbs_output_error(mbs);
		return(MBS_ETYPE_ABORT);
	}

	mbs->nof_events++;
	(mbs->evttype)->hit++;
	mbs->sevtpt = NULL;
	mbs->sevtno = 0;
	mbs->sevt_wc = 0;

	if (!_mbs_check_sequence(mbs, MBS_TY_EVENT)) {
		sprintf(loc_errbuf,
		"?ILLSEQ-[_mbs_next_lmd_event]- %s (buf %d): Illegal event sequence - last=%d <> this=%d",
						mbs->device, mbs->evtno_mbs,
						((s_vehe *) mbs->evt_data)->l_count,
						mbs->evtno_mbs);
			_mbs_output_error(mbs);
		return(MBS_ETYPE_ERROR);
	}
	mbs->evtno_mbs = ((s_vehe *) mbs->evt_data)->l_count;

	if ((mbs->evttype)->type == MBS_ETYPE_VME) {
		vh = (s_vehe *) mbs->evt_data;
		triggers[vh->i_trigger].hit++;
		if (vh->i_trigger == 14) {
			_mbs_set_run_flag(mbs, TRUE);
			etype = MBS_ETYPE_START;
		} else if (vh->i_trigger == 15)	{
			_mbs_set_run_flag(mbs, FALSE);
			etype = MBS_ETYPE_STOP;
		}
	}

	sc = mbs->show_elems[MBS_X_EVENT].redu;
	if (sc > 0 && ((mbs->nof_events % sc) == 0)) {
		s = (void *) (mbs->evttype)->show;
		if (s != NULL) (*s)(mbs, mbs->show_elems[MBS_X_EVENT].out);
	}

	if (med_out) {
		eh = (s_evhe *) eHdr;
		bto_get_int32(&eh->l_dlen, (char *) &evlsv, 1, bo);
		ehs = sizeof(s_vehe);
		fwrite(eHdr, 1, ehs, med_out);								/* write event header - take unswapped data */
		fwrite((char *) mbs->evt_data + ehs, 1, mbs->evtsiz - ehs, med_out);	/* write subevent data unswapped */
	}

	return(etype);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		[internal] Setup access to next event
	\details	Sets ptrs to next event struct
				(expects MED data from tcp or file)
	\param[in] 	mbs 	-- ptr to MBS database as returned by mbs_open_file()
	\retval 	type	-- event type: MBS_ETYPE_xxxx
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int _mbs_next_med_event(MBSDataIO *mbs) {

	s_evhe *eh;
	s_vehe *vh;
	unsigned int bo;
	unsigned int etype;
	int evl;
	int sc;
	unsigned int (*s)();
	int bytes_read, bytes_requested;

	unsigned char eHdr[sizeof(s_evhe)];

/**************************************************/
/*	use FIXED byte ordering                       */
	mbs->byte_order = BYTE_ORDER_REV;
/**************************************************/

	bo = mbs->byte_order;

	mbs->filepos = lseek(mbs->fileno, (off_t) 0, SEEK_CUR);

	bytes_read = read(mbs->fileno, eHdr, sizeof(s_evhe));

	if (bytes_read == 0) {
		return(MBS_ETYPE_EOF);
	} else if (bytes_read == -1) {
		sprintf(loc_errbuf, "?INPERR-[_mbs_next_med_event]- %s (evt %d): %s (%d)",
													mbs->device, mbs->evtno, strerror(errno), errno);
		_mbs_output_error(mbs);
		return(MBS_ETYPE_ABORT);
	} else if (bytes_read != sizeof(s_evhe)) {
		lseek(mbs->fileno, mbs->filepos, SEEK_SET);
		return(MBS_ETYPE_WAIT);
	}

	total += bytes_read;

	eh = (s_evhe *) eHdr;
	bto_get_int32(&evl, (char *) &eh->l_dlen, 1, bo);

	evl = evl * sizeof(unsigned short) + sizeof(s_evhe);

	if (evl <= mbs->evtsiz) {
		memset(mbs->evt_data, 0, mbs->evtsiz);
	} else {
		if (mbs->evt_data != NULL) free(mbs->evt_data);
		mbs->evt_data = calloc(1, evl);
		memset(mbs->evt_data, 0, evl);
	}

	mbs->evtsiz = evl;

	memcpy(mbs->evt_data, eHdr, sizeof(s_evhe));

	bytes_requested = evl - sizeof(s_evhe);

	bytes_read = read(mbs->fileno, mbs->evt_data + sizeof(s_evhe), bytes_requested);
	if (bytes_read != bytes_requested) {
		lseek(mbs->fileno, mbs->filepos, SEEK_SET);
		return(MBS_ETYPE_WAIT);
	}

	total += bytes_read;

	eh = (s_evhe *) mbs->evt_data;
	bto_get_int32((int *) &etype, (char *) &eh->i_subtype, 1, bo);

	mbs->evttype = _mbs_check_type(etype, mbs->evttype, event_types);

	etype = (mbs->evttype)->type;
	if (etype == MBS_ETYPE_ERROR || etype == MBS_ETYPE_ABORT) {
		sprintf(loc_errbuf,
		"?EVTERR-[_mbs_next_med_event]- %s (evt %d): Not a legal event type - %#x",
														mbs->device, mbs->evtno, etype);
		_mbs_output_error(mbs);
		return(etype);
	}

	s = (void *) (mbs->evttype)->convert;
	(*s)(mbs);

	mbs->nof_events++;
	(mbs->evttype)->hit++;
	mbs->sevtpt = NULL;
	mbs->sevtno = 0;
	mbs->sevt_wc = 0;

	mbs->evtno_mbs = ((s_vehe *) mbs->evt_data)->l_count;

	if ((mbs->evttype)->type == MBS_ETYPE_VME) {
		vh = (s_vehe *) mbs->evt_data;
		triggers[vh->i_trigger].hit++;
		if (vh->i_trigger == 14) {
			_mbs_set_run_flag(mbs, TRUE);
			etype = MBS_ETYPE_START;
		} else if (vh->i_trigger == 15)	{
			_mbs_set_run_flag(mbs, FALSE);
			etype = MBS_ETYPE_STOP;
		}
	}

	sc = mbs->show_elems[MBS_X_EVENT].redu;
	if (sc > 0 && ((mbs->nof_events % sc) == 0)) {
		s = (void *) (mbs->evttype)->show;
		if (s != NULL) (*s)(mbs, mbs->show_elems[MBS_X_EVENT].out);
	}

	return(etype);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Setup next subevent
	\details	Sets ptrs to next subevent struct and decodes header
	\param[in] 	mbs 	-- ptr to MBS database as returned by mbs_open_file()
	\retval 	type	-- subevent type: MBS_STYPE_xxxx
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int mbs_next_sheader(MBSDataIO *mbs) {

	s_evhe *eh;
	s_evhe *sh;
	unsigned int bo;
	unsigned int stype;

	if (!_mbs_check_active(mbs)) return(MBS_STYPE_ABORT);

	eh = (s_evhe *) mbs->evt_data;
	bo = mbs->byte_order;

	if (mbs->sevtpt == NULL) {
		mbs->sevtpt = mbs->evt_data + (mbs->evttype)->hsize;
	} else {
		sh = (s_evhe *) mbs->sevtpt;
		mbs->sevtpt += sh->l_dlen * sizeof(unsigned short) + sizeof(s_evhe);
	}

	if (mbs->sevtpt >= (mbs->evt_data + mbs->evtsiz)) return(MBS_STYPE_EOE);

	mbs->sevtno++;

	_mbs_convert_sheader(mbs);
	mbs->sevttype = _mbs_check_type(mbs->sevt_otype, mbs->sevttype, sevent_types);
	stype = (mbs->sevttype)->type;

	return(stype);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Return subevent subtype
	\param[in] 	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	subtype 	-- subevent subtype
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int mbs_get_sevent_subtype(MBSDataIO *mbs) {

	return(((mbs->sevttype)->type >> 16) & 0xFFFF);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Return subevent serial number
	\details	Returns serial number of current subevent
	\param[in] 	 mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	serial		-- subevent serial
*/
/*-------------------------------------------------------------------------------------------*/

int mbs_get_sevent_serial(MBSDataIO *mbs) {

	return(mbs->sevt_id);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Return subevent word count
	\details	Returns word count of current subevent
	\param[in] 	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	wc			-- subevent wc
*/
/*-------------------------------------------------------------------------------------------*/

int mbs_get_sevent_wc(MBSDataIO *mbs) {

	return(mbs->sevt_wc);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Return pointer to subevent data
	\param[in] 	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	data		-- ptr to subevent data
*/
/*-------------------------------------------------------------------------------------------*/

unsigned short * mbs_get_sevent_dataptr(MBSDataIO *mbs) {

	return((unsigned short *) mbs->sevt_data);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Get next subevent
	\details	Converts next subevent. Needs mbs_next_sheader() to be called first
	\param[in] 	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	type		-- subevent type: MBS_STYPE_xxxx
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int mbs_next_sdata(MBSDataIO *mbs) {

	int sc;
	unsigned int (*s)();
	unsigned int stype;
	unsigned int stp, sstp;

	(mbs->sevttype)->hit++;
	mbs->sevt_wc = 0;

	s = (void *) (mbs->sevttype)->unpack;
	(*s)(mbs);

	sc = mbs->show_elems[MBS_X_SUBEVENT].redu;
	if (sc > 0 && ((mbs->nof_events % sc) == 0)) {
		s = (void *) (mbs->sevttype)->show;			// show subevent
		if (s != NULL) (*s)(mbs, mbs->show_elems[MBS_X_SUBEVENT].out);
	}

	stype = (mbs->sevttype)->type;
	if (stype == MBS_STYPE_ERROR) {
		sstp = (mbs->sevt_otype >> 16) & 0xffff;
		stp = mbs->sevt_otype & 0xffff;
		sprintf(loc_errbuf,
		"?EVTERR-[mbs_next_sdata]- %s (buf %d, evt %d, sevt %d): Not a legal subevent type - [%d, %d]",
					mbs->device, mbs->cur_bufno, mbs->evtno, mbs->sevtno, stp, sstp);
		_mbs_output_error(mbs);
	}
	return(stype);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Get next subevent
	\details	Reads next subevent in raw mode. Needs mbs_next_sheader() to be called first
	\param[in] 	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	type		-- subevent type: MBS_STYPE_xxxx
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int mbs_next_sdata_raw(MBSDataIO *mbs) {

	mbs->sevttype = sevent_type_raw;
	return(mbs_next_sdata(mbs));
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Decode next subevent
	\details	Decodes next subevent. Retunrs error if subevent type unknown.
	\param[in] 	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	type		-- subevent type: MBS_STYPE_xxxx
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int mbs_next_sevent(MBSDataIO *mbs) {

	unsigned int stype;
	unsigned int stp, sstp;

	stype = mbs_next_sheader(mbs);
	if (stype == MBS_STYPE_ABORT || stype == MBS_STYPE_EOE) return(stype);

	if (stype == MBS_STYPE_ERROR) {
		sstp = (mbs->sevt_otype >> 16) & 0xffff;
		stp = mbs->sevt_otype & 0xffff;
		sprintf(loc_errbuf,
		"?EVTERR-[mbs_next_sevent]- %s (buf %d, evt %d, sevt %d): Not a legal subevent type - [%d, %d]",
					mbs->device, mbs->cur_bufno, mbs->evtno, mbs->sevtno, stp, sstp);
		_mbs_output_error(mbs);
		return(stype);
	}

	return(mbs_next_sdata(mbs));
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Get next subevent in raw mode
	\param[in] 	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	type		-- subevent type: MBS_STYPE_xxxx
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int mbs_next_sevent_raw(MBSDataIO *mbs) {

	unsigned int stype;

	stype = mbs_next_sheader(mbs);
	if (stype == MBS_STYPE_ABORT || stype == MBS_STYPE_EOE) return(stype);
	mbs->sevttype = sevent_type_raw;
	return(mbs_next_sdata(mbs));
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Pass subevent data to user
	\details	Passes a subevent vector to the user
				Subevent data are converted to a "stretched vector" padded with zeros
	\param[in] 	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[out] data 		-- array to hold subevent data (filled with zeros)
	\retval 	wc			-- length of subevent vector
*/
/*-------------------------------------------------------------------------------------------*/

int mbs_pass_sevent(MBSDataIO *mbs, unsigned short *data) {

	if (!_mbs_check_active(mbs)) return(-1);

	memcpy(data, mbs->sevt_data, mbs->sevt_wc * sizeof(unsigned short));
	return(mbs->sevt_wc);
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Define min subevent length
	\details	Sets the minimal length of a subevent
	\param[in] 	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	wc			-- minimal word count
*/
/*-------------------------------------------------------------------------------------------*/

void mbs_set_sevt_minwc(MBSDataIO *mbs, int wc) {

	mbs->sevt_minwc = wc;
}

/*-------------------------------------------------------------------------------------------*/
/*! \brief		Check run flag
	\param[in] 	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean mbs_is_running(MBSDataIO *mbs) {

	return(mbs->running);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Define statistics to be shown
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	redu		-- reduction: show statistics every redu-th buffer only
	\param[in]	out 		-- stream to send output to (NULL = stdout)
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean mbs_set_stat(MBSDataIO *mbs, int redu, FILE *out) {

	register FILE *old;

	if (!_mbs_check_dbase(mbs)) return(FALSE);

	old = mbs->show_elems[MBS_X_STAT].out;
	if (old != NULL && old != stdout && old != stderr) fclose(old);
	mbs->show_elems[MBS_X_STAT].out = out;
	mbs->show_elems[MBS_X_STAT].redu = redu;
	return(TRUE);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Show event statistics
	\details	Outputs header/data information
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	out 		-- stream to send output to (NULL = stdout)
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean mbs_show_stat(MBSDataIO *mbs, FILE *out) {

	MBSBufferElem *tp;
	register int i;
	char *x1;
	char x2[30];
	unsigned int itype, isubtype;

	if (!_mbs_check_dbase(mbs)) return(FALSE);

	if (out == NULL) out = stdout;

	fprintf(out, "\n==============================================================================\n");
	fprintf(out, "  EVENT STATISTICS: %s", mbs->device);
	fprintf(out, "\n==============================================================================\n");

	fprintf(out, "  Buffers processed    : %d\n", mbs->nof_buffers);
	fprintf(out, "  Buffers out of phase : %d\n", mbs->buf_oo_phase);
	fprintf(out, "  Events read          : %d\n", mbs->nof_events);
	fprintf(out, "------------------------------------------------------------------------------\n");

	fprintf(out, "  Profile              :\n");
	tp = buffer_types;
	x1 = "Buffers   : ";
	while (tp->type) {
		if (tp->hit) {
			itype = tp->type & 0xffff;
			isubtype = (tp->type >> 16) & 0xffff;
			sprintf(x2, "[%d,%d]", itype, isubtype);
			fprintf(out, "             %-12s%-25s%-12s%8d\n",
										x1, tp->descr, x2, tp->hit);
			x1 = "";
		}
		tp++;
	}
	tp = event_types;
	x1 = "Events    : ";
	while (tp->type) {
		if (tp->hit) {
			itype = tp->type & 0xffff;
			isubtype = (tp->type >> 16) & 0xffff;
			sprintf(x2, "[%d,%d]", itype, isubtype);
			fprintf(out, "             %-12s%-25s%-12s%8d\n",
										x1, tp->descr, x2, tp->hit);
			x1 = "";
		}
		tp++;
	}
	tp = triggers;
	x1 = "Triggers  : ";
	for (i = 0; i < MBS_N_TRIGGERS; i++, tp++) {
		if (tp->hit) {
			fprintf(out, "             %-12s%-25s%-12s%8d\n", x1, tp->descr, "", tp->hit);
			x1 = "";
		}
	}
	tp = sevent_types;
	x1 = "Subevents : ";
	while (tp->type) {
		if (tp->hit) {
			itype = tp->type & 0xffff;
			isubtype = (tp->type >> 16) & 0xffff;
			sprintf(x2, "[%d,%d]", itype, isubtype);
			fprintf(out, "             %-12s%-25s%-12s%8d\n",
										x1, tp->descr, x2, tp->hit);
			x1 = "";
		}
		tp++;
	}
	fprintf(out, "==============================================================================\n");
	fprintf(out, "\n");
	return(TRUE);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Show buffer element
	\details	Outputs header/data information
	\param[in]	mbs 			-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	show_elem		-- buffer element to be shown
	\param[in]	out 			-- stream to send output to (NULL = stdout)
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean mbs_show(MBSDataIO *mbs, const char *show_elem, FILE *out) {

	void (*s)();

	if (!_mbs_check_dbase(mbs)) return(FALSE);

	if (out == NULL) out = stdout;

	switch (toupper(*show_elem)) {
		case 'F':	_mbs_show_fheader(mbs, out); return(TRUE);
		case 'B':	s = (void *) (mbs->buftype)->show; break;
		case 'E':	s = (void *) (mbs->evttype)->show; break;
		case 'S':	s = (void *) (mbs->sevttype)->show; break;
		default:
			sprintf(loc_errbuf,
				"?ILLFMT-[_mbs_show]- %s: Illegal show specs - %s",
													mbs->device, show_elem);
			_mbs_output_error(mbs);
			return(FALSE);
	}
	if (s != NULL) (*s)(mbs, out);
	return(TRUE);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Define buffer elements to be shown
	\details	Defines buffer elements to be shown automatically
	\param[in]	mbs 			-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	show_elems		-- buffer element to be shown
	\param[in]	redu			-- reduction: show every redu-th element only
	\param[in]	out 			-- stream to send output to (NULL = stdout)
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean mbs_set_show(MBSDataIO *mbs, const char *show_elems, int redu, FILE *out) {

	register FILE *old;
	register char c;
	register int x;

	if (!_mbs_check_dbase(mbs)) return(FALSE);

	while ((c = toupper(*show_elems++))) {
		switch (c) {
			case 'F':	x = MBS_X_FHEADER; break;
			case 'B':	x = MBS_X_BUFFER; break;
			case 'E':	x = MBS_X_EVENT; break;
			case 'S':	x = MBS_X_SUBEVENT; break;
			default:	x = -1; break;
		}
		if (x != -1) {
			old = mbs->show_elems[x].out;
			if (old != NULL && old != stdout && old != stderr) fclose(old);
			mbs->show_elems[x].out = out;
			mbs->show_elems[x].redu = redu;
		}
	}
	return(TRUE);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Define buffers to be dumped
	\details	Defines every Nth buffer to be dumped to file
	\param[in]	mbs 			-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	count			-- dump count
*/
/*-------------------------------------------------------------------------------------------*/

void mbs_set_dump(MBSDataIO *mbs, int count) {

	mbs->buf_to_be_dumped = count;
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Define stream profile
	\details	Defines how to process stream data
	\param[in]	mbs 			-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	nstreams		-- max # of streams to be processed (0 = ad infinitum)
	\param[in]	slow_down		-- # of secs to wait after each stream (0 = don't wait)
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean mbs_set_stream(MBSDataIO *mbs, int nstreams, int slow_down) {

	if (!_mbs_check_active(mbs)) return(FALSE);
	if ((mbs->connection & (MBS_CTYPE_ASYNC | MBS_CTYPE_SYNC)) != 0) {
		mbs->max_streams = nstreams;
		mbs->slow_down = slow_down;
	} else {
		sprintf(loc_errbuf, "?NCSERV-[mbs_set_stream]- Not connected to MBS server");
		_mbs_output_error(mbs);
		return(FALSE);
	}
	return(TRUE);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Open a file for (error) logging
	\param[in]	logfile 		-- log file, will be opened in append mode
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean mbs_open_log(const char *logfile) {

	FILE * f;

	if ((f = fopen(logfile, "a")) == NULL) {
		sprintf(loc_errbuf, "?SYSERR-[mbs_open_log]- %s (%d)", strerror(errno), errno);
		_mbs_output_error(NULL);
		return(FALSE);
	}
	strcpy(log_file, logfile);
	log_out = f;
	return(TRUE);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Open a file for output
	\details	Opens a file to store MED event data
	\sa 		mbs_close_med()
	\param[in]	medfile     	-- name of med file
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean mbs_open_med(const char *medfile) {

	FILE * f;

	if ((f = fopen(medfile, "w")) == NULL) {
		sprintf(loc_errbuf, "?SYSERR-[mbs_open_med]- %s (%d)", strerror(errno), errno);
		_mbs_output_error(NULL);
		return(FALSE);
	}
	strcpy(med_file, medfile);
	med_out = f;
	return(TRUE);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Close med file
	\sa 		mbs_open_med()
*/
/*-------------------------------------------------------------------------------------------*/

void mbs_close_med() {

	if (med_out) fclose(med_out);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Open a file for output
	\details	Opens a file to store LMD event data
	\sa 		mbs_close_lmd()
	\param[in]	lmdfile     	-- name of lmd file
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean mbs_open_lmd(const char *lmdfile) {

	FILE * f;

	if ((f = fopen(lmdfile, "w")) == NULL) {
		sprintf(loc_errbuf, "?SYSERR-[mbs_open_lmd]- %s (%d)", strerror(errno), errno);
		_mbs_output_error(NULL);
		return(FALSE);
	}
	strcpy(lmd_file, lmdfile);
	lmd_out = f;
	return(TRUE);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Close lmd file
	\sa 		mbs_open_lmd()
*/
/*-------------------------------------------------------------------------------------------*/

void mbs_close_lmd() {

	if (lmd_out) fclose(lmd_out);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		Define an external buffer for error messages
	\param[in]	errbuf  -- buffer to store error messages (NULL = output errors to stderr)
*/
/*-------------------------------------------------------------------------------------------*/

void mbs_pass_errors(const char * errbuf) {

	rem_errbuf = (char *) errbuf;
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Output file header data
	\details	Decodes file header data and outputs it to stream
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	out 		-- stream to send output to
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_show_fheader(MBSDataIO *mbs, FILE *out) {

	register int i;
	s_filhe *fh;
	char text[MBS_L_STR];

	char *mbs_xfht();

	fh = (s_filhe *) mbs->hdr_data;

	if (out == NULL) out = stdout;

	fprintf(out, "\n==============================================================================\n");
	fprintf(out, "  FILE HEADER: %s, buf# %d", mbs->device, mbs->nof_buffers);
	fprintf(out, "\n==============================================================================\n");

	fprintf(out, "  Header length        : %d bytes\n", sizeof(s_filhe));
	fprintf(out, "  Length w/o header    : %d words\n", fh->filhe_dlen);
	fprintf(out, "  Type                 : File header [%d,%d]\n",
										fh->filhe_type, fh->filhe_subtype);
	fprintf(out, "  Fragment indicator   : ");
	if (fh->filhe_frag == 0x0) fprintf(out, "none\n");
	else if (fh->filhe_frag == 0x001) fprintf(out, "fragment at BEGIN of buffer\n");
	else if (fh->filhe_frag == 0x100) fprintf(out, "fragment at END of buffer\n");
	else fprintf(out, "fragments at BEGIN & END of buffer\n");

	fprintf(out, "  Used length of data  : %d words\n", fh->filhe_used);
	fprintf(out, "  Buffer number        : %d\n", fh->filhe_buf);
	fprintf(out, "  # of buffer elements : %d\n", fh->filhe_evt);

	fprintf(out, "  Tape label           : %s\n",
			mbs_xfht(text, fh->filhe_label_l, fh->filhe_label));
	fprintf(out, "  File name            : %s\n",
			mbs_xfht(text, fh->filhe_file_l, fh->filhe_file));
	fprintf(out, "  User name            : %s\n",
			mbs_xfht(text, fh->filhe_user_l, fh->filhe_user));
	fprintf(out, "  Creation date        : %s\n",
			mbs_xfht(text, 24, fh->filhe_time));
	fprintf(out, "  Run ID               : %s\n",
			mbs_xfht(text, fh->filhe_run_l, fh->filhe_run));
	fprintf(out, "  Experiment           : %s\n",
			mbs_xfht(text, fh->filhe_exp_l, fh->filhe_exp));
	fprintf(out, "------------------------------------------------------------------------------\n");

	if (fh->filhe_lines) {
		fprintf(out, "  Comments             :\n");
		for (i = 0; i < fh->filhe_lines; i++) {
			fprintf(out, "         %s\n",
				mbs_xfht(text, fh->s_strings[i].string_l,\
				                fh->s_strings[i].string));
		}
	}
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Output buffer header data
	\details	Decodes buffer header data and outputs it to stream
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	out 		-- stream to send output to
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_show_bheader(MBSDataIO *mbs, FILE *out) {

	s_bufhe *bh;

	if (out == NULL) out = stdout;

	bh = (s_bufhe *) (mbs->poolpt)->data;

	fprintf(out, "\n==============================================================================\n");
	fprintf(out, "  BUFFER HEADER: %s, buf# %d (%d)", mbs->device, mbs->nof_buffers, mbs->cur_bufno);
	fprintf(out, "\n==============================================================================\n");

	fprintf(out, "  Header length        : %d bytes\n", sizeof(s_bufhe));
	fprintf(out, "  Length w/o header    : %d words\n", bh->l_dlen);
	fprintf(out, "  Type                 : %s [%d,%d]\n",
							(mbs->buftype)->descr, bh->i_type, bh->i_subtype);

	fprintf(out, "  Fragment indicator   : ");
	if (bh->h_begin == 0) {
		if (bh->h_end == 0) fprintf(out, "none\n");
		else fprintf(out, "fragment at END of buffer\n");
	} else {
		if (bh->h_end == 0) fprintf(out, "fragment at BEGIN of buffer\n");
		else fprintf(out, "fragments at BEGIN & END of buffer\n");
	}

	fprintf(out, "  Used length of data  : %d words\n", bh->i_used);
	fprintf(out, "  Buffer number        : %d\n", bh->l_buf);
	fprintf(out, "  # of buffer elements : %d\n", bh->l_evt);
	fprintf(out, "  Time stamp           : %lld +%lld\n", mbs->buf_ts, mbs->buf_ts - mbs->buf_ts_start);
	fprintf(out, "  Length of last event : %d\n", bh->l_free[1]);
	fprintf(out, "------------------------------------------------------------------------------\n");
}

char *mbs_xfht(char *t, int l, char *s)

{
	memset(t, 0, MBS_L_STR);
	strncpy(t, s, l);
	return(t);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Output event header
	\details	Decodes event header data and outputs it to stream
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	out 		-- stream to send output to
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_show_evhe_10_1(MBSDataIO *mbs, FILE *out) {

	s_vehe *eh;

	if (out == NULL) out = stdout;

	eh = (s_vehe *) mbs->evt_data;

	fprintf(out, "\n==============================================================================\n");
	fprintf(out, "  EVENT HEADER: %s, buf# %d (%d), evt# %d", mbs->device, mbs->nof_buffers, mbs->cur_bufno, mbs->evtno);
	fprintf(out, "\n==============================================================================\n");

	fprintf(out, "  Header length        : %d bytes\n", sizeof(s_vehe));
	fprintf(out, "  Data length          : %d words\n", eh->l_dlen);
	fprintf(out, "  Type                 : %s [%d,%d]\n",
							(mbs->evttype)->descr, eh->i_type, eh->i_subtype);
	fprintf(out, "  Trigger              : %d\n", eh->i_trigger);
	fprintf(out, "  Event counter        : %d\n", eh->l_count);
	fprintf(out, "------------------------------------------------------------------------------\n");
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Output subevent data
	\details	Decodes subevent data (type [10,1] and outputs it to stream
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	out 		-- stream to send output to
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_show_sev_10_1(MBSDataIO *mbs, FILE *out) {

	register int i;
	int n;
	s_veshe *sh;
	unsigned short *dp;

	if (out == NULL) out = stdout;

	sh = (s_veshe *) mbs->sevtpt;

	fprintf(out, "\n==============================================================================\n");
	fprintf(out, "  SUBEVENT HEADER: %s, buf# %d (%d), evt# %d, sevt# %d",
					mbs->device, mbs->nof_buffers, mbs->cur_bufno, mbs->evtno, mbs->sevtno);
	fprintf(out, "\n==============================================================================\n");

	fprintf(out, "  Header length        : %d bytes\n", sizeof(s_veshe));
	fprintf(out, "  Data length          : %d words\n", sh->l_dlen);
	fprintf(out, "  Type                 : %s [%d,%d]\n",
							(mbs->sevttype)->descr, sh->i_type, sh->i_subtype);
	fprintf(out, "  Control              : %d\n", sh->h_control);
	fprintf(out, "  Subcrate             : %d\n", sh->h_subcrate);
	fprintf(out, "  Subevent/Proc ID     : %d\n", sh->i_procid);
	fprintf(out, "  # of data words      : %d\n", mbs->sevt_wc);
	fprintf(out, "------------------------------------------------------------------------------\n");
	fprintf(out, "  SUBEVENT DATA:");

	dp = (unsigned short *) mbs->sevt_data;
	n = 0;
	for (i = 0; i < mbs->sevtsiz; i++, dp++) {
		if (*dp) {
			if ((n % 5) == 0) fprintf(out, "\n");
			fprintf(out, "%6d:%8d", i, *dp);
			n++;
		}
	}
	fprintf(out, "\n------------------------------------------------------------------------------\n");
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Unpack subevent
	\details	Unpacks subevent data of type [10,1] (CAMAC)
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	sevtptr		-- pointer to subevent data
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int *_mbs_unpack_sev_10_1(MBSDataIO *mbs) {

	register int i;
	s_veshe *sh;
	char *idp, *odp;
	unsigned short *sdp;
	MBSCamacDataSW *cp;
	int wc;
	int maxaddr, addrspace;

	if (mbs->sevt_wc > 0) return((unsigned int *) mbs->sevt_data);

	sh = (s_veshe *) mbs->sevtpt;
	wc = (sh->l_dlen
			- (sizeof(s_veshe) - sizeof(s_evhe)) / sizeof(unsigned short)) / 2;
	mbs->sevt_wc = wc;
	idp = mbs->sevtpt + sizeof(s_veshe);
	bto_get_short((short *) idp, idp, wc * 2, mbs->byte_order);

	cp = (MBSCamacDataSW *) idp;
	maxaddr = mbs->sevt_minwc;
	for (i = 0; i < wc; i++, cp++) {
		if (cp->subaddr > maxaddr) maxaddr = cp->subaddr;
	}

	addrspace = maxaddr + 1;

	if (addrspace <= mbs->sevtsiz) {
		if (mbs->sevt_data == NULL) mbs->sevt_data = calloc(addrspace, sizeof(unsigned short));
		else memset(mbs->sevt_data, 0, mbs->sevtsiz * sizeof(unsigned short));
	} else {
		if (mbs->sevt_data != NULL) free(mbs->sevt_data);
		mbs->sevt_data = calloc(addrspace, sizeof(unsigned short));
		mbs->sevtsiz = addrspace;
	}

	cp = (MBSCamacDataSW *) idp;
	for (i = 0; i < wc; i++, cp++) {
		odp = mbs->sevt_data
			+ cp->subaddr * sizeof(unsigned short);
		sdp = (unsigned short *) odp;
		*sdp = cp->data;
	}
	return((unsigned int *) mbs->sevt_data);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Output subevent data
	\details	Decodes subevent data (16 bit) and outputs it to stream
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	out 		-- stream to send output to
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_show_sev_short(MBSDataIO *mbs, FILE *out) {

	register int i;
	s_veshe *sh;
	unsigned short *dp;

	if (out == NULL) out = stdout;

	sh = (s_veshe *) mbs->sevtpt;

	fprintf(out, "\n==============================================================================\n");
	fprintf(out, "  SUBEVENT HEADER: %s, buf# %d (%d), evt# %d, sevt# %d",
					mbs->device, mbs->nof_buffers, mbs->cur_bufno, mbs->evtno, mbs->sevtno);
	fprintf(out, "\n==============================================================================\n");

	fprintf(out, "  Header length        : %d bytes\n", sizeof(s_veshe));
	fprintf(out, "  Data length          : %d words\n", sh->l_dlen);
	fprintf(out, "  Type                 : %s [%d,%d]\n",
							(mbs->sevttype)->descr, sh->i_type, sh->i_subtype);
	fprintf(out, "  Control              : %d\n", sh->h_control);
	fprintf(out, "  Subcrate             : %d\n", sh->h_subcrate);
	fprintf(out, "  Subevent/Proc ID     : %d\n", sh->i_procid);
	fprintf(out, "  # of data words      : %d\n", mbs->sevt_wc);
	fprintf(out, "------------------------------------------------------------------------------\n");
	fprintf(out, "  SUBEVENT DATA:");

	dp = (unsigned short *) mbs->sevt_data;
	for (i = 0; i < mbs->sevt_wc; i++, dp++) {
		if ((i % 5) == 0) fprintf(out, "\n%6d: ", i + 1);
		fprintf(out, "%8d", *dp);
	}
	fprintf(out, "\n------------------------------------------------------------------------------\n");
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Output subevent data
	\details	Decodes subevent data (32 bit) and outputs it to stream
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	out 		-- stream to send output to
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_show_sev_long(MBSDataIO *mbs, FILE *out) {

	register int i;
	s_veshe *sh;
	unsigned int *dp;

	if (out == NULL) out = stdout;

	sh = (s_veshe *) mbs->sevtpt;

	fprintf(out, "\n==============================================================================\n");
	fprintf(out, "  SUBEVENT HEADER: %s, buf# %d (%d), evt# %d, sevt# %d",
					mbs->device, mbs->nof_buffers, mbs->cur_bufno, mbs->evtno, mbs->sevtno);
	fprintf(out, "\n==============================================================================\n");

	fprintf(out, "  Header length        : %d bytes\n", sizeof(s_veshe));
	fprintf(out, "  Data length          : %d words\n", sh->l_dlen);
	fprintf(out, "  Type                 : %s [%d,%d]\n",
							(mbs->sevttype)->descr, sh->i_type, sh->i_subtype);
	fprintf(out, "  Control              : %d\n", sh->h_control);
	fprintf(out, "  Subcrate             : %d\n", sh->h_subcrate);
	fprintf(out, "  Subevent/Proc ID     : %d\n", sh->i_procid);
	fprintf(out, "  # of data words      : %d\n", mbs->sevt_wc);
	fprintf(out, "------------------------------------------------------------------------------\n");
	fprintf(out, "  SUBEVENT DATA:");

	dp = (unsigned int *) mbs->sevt_data;
	for (i = 0; i < mbs->sevt_wc; i++, dp++) {
		if ((i % 5) == 0) fprintf(out, "\n%10d: ", i + 1);
		fprintf(out, "%8d", *dp);
	}
	fprintf(out, "\n------------------------------------------------------------------------------\n");
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Unpack subevent
	\details	Unpacks subevent data (16 bit) (CAMAC)
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	sevtptr		-- pointer to subevent data
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int *_mbs_unpack_sev_short(MBSDataIO *mbs) {

	s_veshe *sh;
	char *dp;
	int wc;
	int mwc;

	if (mbs->sevt_wc > 0) return((unsigned int *) mbs->sevt_data);

	sh = (s_veshe *) mbs->sevtpt;
	wc = (sh->l_dlen - (sizeof(s_veshe) - sizeof(s_evhe)) / sizeof(unsigned short));
	mbs->sevt_wc = wc;
	dp = mbs->sevtpt + sizeof(s_veshe);
	bto_get_short((short *) dp, dp, wc, mbs->byte_order);

	mwc = mbs->sevt_minwc;
	if (mwc < wc) mwc = wc;
	if (mwc <= mbs->sevtsiz) {
		if (mbs->sevt_data == NULL) mbs->sevt_data = calloc(mwc, sizeof(unsigned short));
		else memset(mbs->sevt_data, 0, mbs->sevtsiz * sizeof(unsigned short));
	} else {
		if (mbs->sevt_data != NULL) free(mbs->sevt_data);
		mbs->sevt_data = calloc(mwc, sizeof(unsigned short));
		mbs->sevtsiz = mwc;
	}

	memcpy(mbs->sevt_data, dp, wc * sizeof(unsigned short));

	return((unsigned int *) mbs->sevt_data);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Unpack subevent
	\details	Unpacks subevent data (32 bit) (CAMAC)
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	sevtptr		-- pointer to subevent data
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int *_mbs_unpack_sev_long(MBSDataIO *mbs) {

	s_veshe *sh;
	char *dp;
	int wc;
	int mwc;

	if (mbs->sevt_wc > 0) return((unsigned int *) mbs->sevt_data);

	sh = (s_veshe *) mbs->sevtpt;
	wc = (sh->l_dlen - (sizeof(s_veshe) - sizeof(s_evhe)) / sizeof(unsigned short));
	mbs->sevt_wc = wc;
	dp = mbs->sevtpt + sizeof(s_veshe);
	bto_get_int32((int *) dp, dp, wc/2, mbs->byte_order);

	mwc = mbs->sevt_minwc;
	if (mwc < wc) mwc = wc;
	if (mwc <= mbs->sevtsiz) {
		if (mbs->sevt_data == NULL) mbs->sevt_data = calloc(mwc, sizeof(unsigned short));
		else memset(mbs->sevt_data, 0, mbs->sevtsiz * sizeof(unsigned short));
	} else {
		if (mbs->sevt_data != NULL) free(mbs->sevt_data);
		mbs->sevt_data = calloc(mwc, sizeof(unsigned short));
		mbs->sevtsiz = mwc;
	}

	memcpy(mbs->sevt_data, dp, wc * sizeof(unsigned short));

	return((unsigned int *) mbs->sevt_data);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Unpack subevent
	\details	Unpacks raw data. No byte ordering will be done.
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	sevtptr		-- pointer to subevent data
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int *_mbs_unpack_sev_raw(MBSDataIO *mbs) {

	s_veshe *sh;
	char *dp;
	int wc;
	int mwc;

	if (mbs->sevt_wc > 0) return((unsigned int *) mbs->sevt_data);

	sh = (s_veshe *) mbs->sevtpt;
	wc = (sh->l_dlen - (sizeof(s_veshe) - sizeof(s_evhe)) / sizeof(unsigned short));
	mbs->sevt_wc = wc;
	dp = mbs->sevtpt + sizeof(s_veshe);

	mwc = mbs->sevt_minwc;
	if (mwc < wc) mwc = wc;
	if (mwc <= mbs->sevtsiz) {
		if (mbs->sevt_data == NULL) mbs->sevt_data = calloc(mwc, sizeof(unsigned short));
		else memset(mbs->sevt_data, 0, mbs->sevtsiz * sizeof(unsigned short));
	} else {
		if (mbs->sevt_data != NULL) free(mbs->sevt_data);
		mbs->sevt_data = calloc(mwc, sizeof(unsigned short));
		mbs->sevtsiz = mwc;
	}

	memcpy(mbs->sevt_data, dp, wc * sizeof(unsigned short));

	return((unsigned int *) mbs->sevt_data);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Output subevent data
	\details	Decodes subevent data (type [9000,1], TimeStamp) and outputs it to stream
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	out 		-- stream to send output to
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_show_sev_9000_1(MBSDataIO *mbs, FILE *out) {

	s_veshe *sh;
	unsigned int *dp;
	char tstr[100];
	unsigned int sec;

	if (out == NULL) out = stdout;

	sh = (s_veshe *) mbs->sevtpt;

	dp = (unsigned int *) mbs->sevt_data;

	fprintf(out, "\n==============================================================================\n");
	fprintf(out, "  SUBEVENT HEADER: %s, buf# %d (%d), evt# %d, sevt# %d",
					mbs->device, mbs->nof_buffers, mbs->cur_bufno, mbs->evtno, mbs->sevtno);
	fprintf(out, "\n==============================================================================\n");

	fprintf(out, "  Header length        : %d bytes\n", sizeof(s_veshe));
	fprintf(out, "  Data length          : %d words\n", sh->l_dlen);
	fprintf(out, "  Type                 : %s [%d,%d]\n",
							(mbs->sevttype)->descr, sh->i_type, sh->i_subtype);
	fprintf(out, "  Clock resolution     : %d nsecs\n", *dp++);
	sec = *dp++;
	fprintf(out, "  Seconds              : %d\n", sec);
	fprintf(out, "  Nano seconds         : %d\n", *dp++);
	strftime(tstr, 100, "%e-%b-%Y %H:%M:%S", localtime((const time_t *) &sec));
	fprintf(out, "  Time stamp           : %s\n", tstr);
	fprintf(out, "------------------------------------------------------------------------------\n");
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Unpack subevent
	\details	Unpack subevent (type [9000,X] - X=1:TimeStamp, X=2:DeadTime)
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	sevtptr		-- pointer to subevent data
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int *_mbs_unpack_sev_9000_X(MBSDataIO *mbs) {

	s_veshe *sh;
	char *dp;
	int wc, wc2;

	if (mbs->sevt_wc > 0) return((unsigned int *) mbs->sevt_data);

	sh = (s_veshe *) mbs->sevtpt;
	wc = (sh->l_dlen
			- (sizeof(s_veshe) - sizeof(s_evhe)) / sizeof(unsigned short)) / 2;
	wc2 = wc * 2;
	mbs->sevt_wc = wc2;
	dp = mbs->sevtpt + sizeof(s_veshe);
	bto_get_int32((int *) dp, dp, wc, mbs->byte_order);

	if (wc2 <= mbs->sevtsiz) {
		if (mbs->sevt_data == NULL) mbs->sevt_data = calloc(wc, sizeof(unsigned short));
		else memset(mbs->sevt_data, 0, mbs->sevtsiz * sizeof(unsigned short));
	} else {
		if (mbs->sevt_data != NULL) free(mbs->sevt_data);
		mbs->sevt_data = calloc(wc, sizeof(unsigned int));
		mbs->sevtsiz = wc2;
	}

	memcpy(mbs->sevt_data, dp, wc * sizeof(unsigned int));

	return((unsigned int *) mbs->sevt_data);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Output subevent data
	\details	Decodes subevent data (type [9000,2], DeadTime) and outputs it to stream
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	out 		-- stream to send output to
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_show_sev_9000_2(MBSDataIO *mbs, FILE *out) {

	s_veshe *sh;
	unsigned int *dp;
	char tstr[100];
	int sec;
	int dtevc;
	int scacon;
	float dtime;

	if (out == NULL) out = stdout;

	sh = (s_veshe *) mbs->sevtpt;

	dp = (unsigned int *) mbs->sevt_data;

	fprintf(out, "\n==============================================================================\n");
	fprintf(out, "  SUBEVENT HEADER: %s, buf# %d (%d), evt# %d, sevt# %d",
					mbs->device, mbs->nof_buffers, mbs->cur_bufno, mbs->evtno, mbs->sevtno);
	fprintf(out, "\n==============================================================================\n");

	fprintf(out, "  Header length        : %d bytes\n", sizeof(s_veshe));
	fprintf(out, "  Data length          : %d words\n", sh->l_dlen);
	fprintf(out, "  Type                 : %s [%d,%d]\n",
							(mbs->sevttype)->descr, sh->i_type, sh->i_subtype);
	fprintf(out, "  Clock resolution     : %d nsecs\n", *dp++);
	sec = *dp++;
	fprintf(out, "  Seconds              : %d\n", sec);
	fprintf(out, "  Nano seconds         : %d\n", *dp++);
	strftime(tstr, 100, "%e-%b-%Y %H:%M:%S", localtime((const time_t *) &sec));
	fprintf(out, "  Time stamp           : %s\n", tstr);
	fprintf(out, "  Events since start   : %d\n", *dp++);
	dtevc = *dp++;
	fprintf(out, "  Events during interv : %d\n", dtevc);
	scacon = *dp++;
	fprintf(out, "  Scaler contents      : %d\n", scacon);
	dtime = (1. - (float) dtevc / (float) scacon) * 100.;
	fprintf(out, "  Dead time            : %4.2f %%\n", dtime);
	fprintf(out, "------------------------------------------------------------------------------\n");
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Output subevent data
	\details	Decodes subevent data and outputs it to stream
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	out 		-- stream to send output to
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_show_sev_raw(MBSDataIO *mbs, FILE *out) {

	register int i;
	s_veshe *sh;
	unsigned short *dp;

	if (out == NULL) out = stdout;

	sh = (s_veshe *) mbs->sevtpt;

	fprintf(out, "\n==============================================================================\n");
	fprintf(out, "  SUBEVENT HEADER: %s, buf# %d (%d), evt# %d, sevt# %d",
					mbs->device, mbs->nof_buffers, mbs->cur_bufno, mbs->evtno, mbs->sevtno);
	fprintf(out, "\n==============================================================================\n");

	fprintf(out, "  Header length        : %d bytes\n", sizeof(s_veshe));
	fprintf(out, "  Data length          : %d words\n", sh->l_dlen);
	fprintf(out, "  Type                 : %s [%d,%d]\n",
							(mbs->sevttype)->descr, sh->i_type, sh->i_subtype);
	fprintf(out, "  Control              : %d\n", sh->h_control);
	fprintf(out, "  Subcrate             : %d\n", sh->h_subcrate);
	fprintf(out, "  Subevent/Proc ID     : %d\n", sh->i_procid);
	fprintf(out, "  # of data words      : %d\n", mbs->sevt_wc);
	fprintf(out, "------------------------------------------------------------------------------\n");
	fprintf(out, "  SUBEVENT DATA:");

	dp = (unsigned short *) mbs->sevt_data;
	for (i = 0; i < mbs->sevt_wc; i++, dp++) {
		if ((i % 5) == 0) fprintf(out, "\n%6d: ", i + 1);
		fprintf(out, "%8d", *dp);
	}
	fprintf(out, "\n------------------------------------------------------------------------------\n");
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal]  Test and convert data
	\details	Tests byte ordering and converts data
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	type		-- buffer type
*/
/*-------------------------------------------------------------------------------------------*/

unsigned int _mbs_convert_data(MBSDataIO *mbs) {

	unsigned int bo_tag;
	unsigned int byte_order;
	unsigned int btype;
	s_bufhe *bh;

	unsigned int (*s)();

	MBSBufferElem *_mbs_check_type();

	bh = (s_bufhe *) (mbs->poolpt)->data;

	bo_tag = bh->l_free[0];
	if (bo_tag == 0 && mbs->nof_buffers == 1) {
		byte_order = BYTE_ORDER_1_TO_1;
		bto_get_int32((int *) &btype, (char *) &bh->i_subtype, 1, byte_order);
		if (btype != MBS_BTYPE_FHEADER) {
			byte_order = BYTE_ORDER_REV;
			bto_get_int32((int *) &btype, (char *) &bh->i_subtype, 1, byte_order);
			if (btype != MBS_BTYPE_FHEADER) {
				sprintf(loc_errbuf,
"?ILLFMT-[_mbs_convert_data]- %s (buf %d): Can't determine byte ordering - %#x",
									mbs->device, mbs->nof_buffers, bh->l_free[0]);
				_mbs_output_error(mbs);
				mbs->buftype = &buffer_type_error;
				return(MBS_BTYPE_ABORT);
			}
		}
	} else if (bo_tag == 1) {
		byte_order = BYTE_ORDER_1_TO_1;
	} else {
		bto_get_int32((int *) &bo_tag, (char *) &bh->l_free[0], 1, BYTE_ORDER_REV);
		if (bo_tag == 1) {
			byte_order = BYTE_ORDER_REV;
		} else {
			sprintf(loc_errbuf,
	"?ILLFMT-[_mbs_convert_data]- %s (buf %d): Can't determine byte ordering - %#x",
										mbs->device, mbs->nof_buffers, bh->l_free[0]);
			_mbs_output_error(mbs);
			mbs->buftype = &buffer_type_error;
			return(MBS_BTYPE_ABORT);
		}
	}

	mbs->byte_order = byte_order;

	bto_get_int32((int *) &btype, (char *) &bh->i_subtype, 1, byte_order);
	mbs->buftype = _mbs_check_type(btype, mbs->buftype, buffer_types);
	btype = (mbs->buftype)->type;
	if (btype == MBS_BTYPE_ERROR || btype == MBS_BTYPE_ABORT) {
		sprintf(loc_errbuf,
		"?ILLFMT-[_mbs_convert_data]- %s (buf %d): Not a legal buffer type - %#x",
										mbs->device, mbs->nof_buffers, btype);
		_mbs_output_error(mbs);
		mbs->buftype = &buffer_type_error;
		return(MBS_BTYPE_ABORT);
	}

	s = (void *) (mbs->buftype)->convert;
	(*s)(mbs);

	(mbs->buftype)->hit++;

	return((mbs->buftype)->type);
}

void _mbs_type_error(MBSDataIO *mbs) {}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal]  Initialize hit count
	\param[in]	tlist		-- list of buffer types
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_init_hit(MBSBufferElem *tlist) {

	while (tlist->type != 0) {
		tlist->hit = 0;
		tlist++;
	}
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal]  Initialize trigger list
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_init_triggers() {

	register int i;
	MBSBufferElem * tp;

	tp = triggers;
	for (i = 0; i < MBS_N_TRIGGERS; i++, tp++) {
		tp->descr = calloc(MBS_L_NAME, sizeof(char));
		sprintf(tp->descr, "Trig %d", i);
		tp->hit = 0;
	}
	strcat(triggers[14].descr, " (Start)");
	strcat(triggers[15].descr, " (Stop)");
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal]  Check buffer type
	\param[in]	btype		-- buffer type to be tested
	\param[in]	ltdescr 	-- ptr to previous type descr
	\param[in]	tlist		-- list of types
	\retval 	btpt		-- ptr to type descriptor
*/
/*-------------------------------------------------------------------------------------------*/

MBSBufferElem *_mbs_check_type(unsigned int btype, MBSBufferElem *ltdescr, MBSBufferElem *tlist) {

	unsigned int t;
	if (ltdescr != NULL && (btype == ltdescr->type)) return(ltdescr);

	while (tlist->type != 0) {
		t = tlist->type;
		if (t == btype) return(tlist);
		tlist++;
	}
	return(&buffer_type_error);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal]  Check if data in sequence
	\details	Tests if subsequent buffers/events have increasing numbers
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in] 	type        -- type: buffer or event
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean _mbs_check_sequence(MBSDataIO *mbs, unsigned int type) {

	s_bufhe *bh;
	s_vehe *eh;

	if (type == MBS_TY_BUFFER) {
		bh = (s_bufhe *) (mbs->poolpt)->data;
		if ((mbs->bufno_mbs > 0) && (bh->l_buf != mbs->bufno_mbs + 1)) return(FALSE);
	} else if (type == MBS_TY_EVENT) {
		eh = (s_vehe *) mbs->evt_data;
		if ((mbs->evtno_mbs >= 0) && (eh->l_count != mbs->evtno_mbs + 1)) return(FALSE);
	}
	return(TRUE);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal]  Copy file header (swapped)
	\details	Copies file header data from input buffer to hdr_data
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_copy_fheader(MBSDataIO *mbs) {

	register int i;
	register char *ipnt;
	s_filhe *bh, *fh;
	unsigned int bo;
	struct cv_string *cmt;

	bh = (s_filhe *) (mbs->poolpt)->data;
	fh = (s_filhe *) mbs->hdr_data;
	bo = mbs->byte_order;

	if (bo == BYTE_ORDER_1_TO_1) {
		memcpy(mbs->hdr_data, (mbs->poolpt)->data, mbs->bufsiz);
	} else {
		bto_get_short(&fh->filhe_tlen, (char *) bh, 6, bo);
		ipnt = bto_get_short(&fh->filhe_label_l, (char *) &bh->filhe_label_l, 1, bo);
		ipnt = bto_get_string(fh->filhe_label, ipnt, 30, bo);
		ipnt = bto_get_short(&fh->filhe_file_l, ipnt, 1, bo);
		ipnt = bto_get_string(fh->filhe_file, ipnt, 86, bo);
		ipnt = bto_get_short(&fh->filhe_user_l, ipnt, 1, bo);
		ipnt = bto_get_string(fh->filhe_user, ipnt, 30, bo);
		ipnt = bto_get_string(fh->filhe_time, ipnt, 24, bo);
		ipnt = bto_get_short(&fh->filhe_run_l, ipnt, 1, bo);
		ipnt = bto_get_string(fh->filhe_run, ipnt, 66, bo);
		ipnt = bto_get_short(&fh->filhe_exp_l, ipnt, 1, bo);
		ipnt = bto_get_string(fh->filhe_exp, ipnt, 66, bo);
		ipnt = bto_get_int32(&fh->filhe_lines, ipnt, 1, bo);
		cmt = fh->s_strings;
		for (i = 0; i < fh->filhe_lines; i++, cmt++) {
			ipnt = bto_get_short(&cmt->string_l, ipnt, 1, bo);
			ipnt = bto_get_string(cmt->string, ipnt, 78, bo);
		}
	}
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal]  Convert buffer header
	\details	Converts header data according to byte order
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_convert_bheader(MBSDataIO *mbs) {

	register char *ipnt;
	unsigned int bo;
	s_bufhe *bh;

	bo = mbs->byte_order;
	if (bo == BYTE_ORDER_1_TO_1) return;

	bh = (s_bufhe *) (mbs->poolpt)->data;

	ipnt = bto_get_int32(&bh->l_dlen, (char *) &bh->l_dlen, 1, bo);
	ipnt = bto_get_short(&bh->i_subtype, ipnt, 2, bo);
	ipnt = bto_get_short((short *) &bh->h_begin, ipnt, 1, bo);
	ipnt = bto_get_short(&bh->i_used, ipnt, 1, bo);
	ipnt = bto_get_int32(&bh->l_buf, ipnt, 9, bo);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal]  Convert event header
	\details	Converts header data according to byte order
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_convert_eheader(MBSDataIO *mbs) {

	register char *ipnt;
	unsigned int bo;
	s_vehe *eh;

	bo = mbs->byte_order;
	if (bo == BYTE_ORDER_1_TO_1) return;

	eh = (s_vehe *) mbs->evt_data;

	ipnt = bto_get_int32(&eh->l_dlen, (char *) &eh->l_dlen, 1, bo);
	ipnt = bto_get_short(&eh->i_subtype, ipnt, 2, bo);
	ipnt = bto_get_short(&eh->i_trigger, ipnt, 2, bo);
	ipnt = bto_get_int32(&eh->l_count, ipnt, 1, bo);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal]  Convert subevent header
	\details	Converts header data according to byte order
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_convert_sheader(MBSDataIO *mbs) {

	register char *ipnt;
	unsigned int bo;
	s_veshe *seh;

	bo = mbs->byte_order;
	if (bo == BYTE_ORDER_1_TO_1) return;

	seh = (s_veshe *) mbs->sevtpt;

	ipnt = bto_get_int32(&seh->l_dlen, (char *) &seh->l_dlen, 1, bo);
	ipnt = bto_get_short(&seh->i_subtype, ipnt, 2, bo);
	ipnt = bto_get_short((short *) &seh->h_control, ipnt, 2, bo);
	mbs->sevt_otype = (seh->i_subtype << 16) | seh->i_type;
	mbs->sevt_id = seh->i_procid;
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Check if a given mbs struct is active
	\details	Tests magic word of mbs database
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean _mbs_check_active(MBSDataIO *mbs) {

	if (mbs == NULL || mbs->fileno == -1) {
		sprintf(loc_errbuf, "?MBSNAC-[mbs_check_active]- MBSIO not active");
		_mbs_output_error(mbs);
		return(FALSE);
	} else if (strncmp(mbs->id, MBS_ID_WORD, 15) != 0) {
		sprintf(loc_errbuf, "?NMBSIO-[mbs_check_active]- Not a MBSIO struct");
		_mbs_output_error(mbs);
		return(FALSE);
	} else {
		return(TRUE);
	}
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Check if a given mbs struct has valid data
	\details	Tests magic word of mbs database
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean _mbs_check_dbase(MBSDataIO *mbs) {

	if (mbs == NULL) {
		sprintf(loc_errbuf, "?MBSDNV-[mbs_check_dbase]- data base not valid");
		_mbs_output_error(mbs);
		return(FALSE);
	} else if (strncmp(mbs->id, MBS_ID_WORD, 15) != 0) {
		sprintf(loc_errbuf, "?NMBSIO-[mbs_check_dbase]- Not a MBSIO struct");
		_mbs_output_error(mbs);
		return(FALSE);
	} else {
		return(TRUE);
	}
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Check if buffer is empty
	\details	Checks field 'used data' in buffer header
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\return 	TRUE or FALSE
*/
/*-------------------------------------------------------------------------------------------*/

boolean _mbs_check_buffer_empty(MBSDataIO *mbs) {

	s_bufhe * bh;
	bh = (s_bufhe *) (mbs->poolpt)->data;
	return((int) (bh->i_used == 0));
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Output error message
	\details	Outputs an error message, either to stderr or to rem_errbuf
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_output_error(MBSDataIO *mbs) {
/*________________________________________________________[C PRIVATE FUNCTION]
//////////////////////////////////////////////////////////////////////////////
// Name:           _mbs_output_error
// Purpose:        Output error message
// Arguments:      MBSDataIO * mbs  -- pointer to mbs struct
// Results:        --
// Exceptions:
// Description:    Outputs an error message, either to stderr or to rem_errbuf.
// Keywords:
/////////////////////////////////////////////////////////////////////////// */

	char datestr[MBS_L_STR];
	char fposstr[MBS_L_STR];
	time_t now;
	off_t filepos;

	if (mbs) filepos = mbs->filepos; else filepos = -1;

	if (filepos >= 0) {
		sprintf(fposstr, " (@ filepos = %d)", (int) filepos);
		strcat(loc_errbuf, fposstr);
	}
	if (rem_errbuf == NULL) {
		fprintf(stderr, "%s\n", loc_errbuf);
	} else {
		strcpy(rem_errbuf, loc_errbuf);
	}
	if (log_out) {
		time(&now);
		strftime(datestr, MBS_L_STR, "%e-%b-%Y %H:%M:%S", localtime(&now));
		fprintf(log_out, "%-18s: %s\n", datestr, loc_errbuf);
	}
	if (mbs && mbs->buf_to_be_dumped != 0)_mbs_dump_buffer(mbs);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Output message to logfile
	\details	Outputs a message to log channel
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_output_log(MBSDataIO *mbs) {

	char datestr[MBS_L_STR];
	char fposstr[MBS_L_STR];
	time_t now;
	off_t filepos;

	if (mbs) filepos = mbs->filepos; else filepos = -1;

	if (filepos >= 0) {
		sprintf(fposstr, " (@ filepos = %d)", (int) filepos);
		strcat(loc_logbuf, fposstr);
	}
	if (log_out) {
		now = time(NULL);
		strftime(datestr, MBS_L_STR, "%e-%b-%Y %H:%M:%S", localtime(&now));
		fprintf(log_out, "%-18s: %s\n", datestr, loc_logbuf);
	}
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Connect to MBS server
	\param[in]	host			-- host name to connect to
	\param[in]	server_type 	-- server type (MBS_CTYPE_xxxx)
	\retval 	fildes			-- file descriptor or -1 if error
*/
/*-------------------------------------------------------------------------------------------*/

int _mbs_connect_to_server(char * host, unsigned int server_type) {

	int s;
	int len;
	struct sockaddr_in sa;
	struct hostent *hp;
	struct servent *sp;
	int port=0;

	s = 0;
	len = 0;

	hp = NULL;
	sp = NULL;

	switch (server_type) {
		case MBS_CTYPE_SYNC:
			port = 6000;
			break;
		case MBS_CTYPE_ASYNC:
			port = 6002;
			break;
		default:
			sprintf(loc_errbuf,
				"?UKSRVTP-[mbs_connect_to_server]- Unknown server type: %#x", server_type);
			return(-1);
	}

	if ((hp = gethostbyname(host)) == NULL) {
		sprintf(loc_errbuf, "?UKHOSTN-[mbs_connect_to_server]- Host name unknown: %s", host);
		return(-1);
	}
	memcpy(&sa.sin_addr, hp->h_addr, hp->h_length);
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons(port);
	if((s = socket(hp->h_addrtype, SOCK_STREAM, 0)) < 0) {
		sprintf(loc_errbuf, "?SYSERR-[mbs_connect_to_server]- %s (%d), socket=%d", strerror(errno), errno, port);
		return(-1);
	}
	if(connect(s, (struct sockaddr *) &sa, sizeof sa) < 0) {
		sprintf(loc_errbuf, "?SYSERR-[mbs_connect_to_server]- %s (%d), socket=%d", strerror(errno), errno, port);
		return(-1);
	}
	return(s);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Connect to MBS server
	\param[in]	fildes			-- file descriptor for current connection
	\param[in]	info			-- info block where to put data
	\retval 	info			-- info block
*/
/*-------------------------------------------------------------------------------------------*/

MBSServerInfo * _mbs_read_server_info(int fildes, MBSServerInfo *info) {

	int infoWord;
	int swInfoWord;

	errno = 0;

	read(fildes, &infoWord, sizeof(infoWord));
	if (infoWord == 1) {
		info->is_swapped = FALSE;
	} else {
		bto_get_int32((int *) &swInfoWord, (char *) &infoWord, 1, BYTE_ORDER_REV);
		if (swInfoWord == 1) {
			info->is_swapped = TRUE;
		} else {
			sprintf(loc_errbuf,
				"?ILINFB-[mbs_read_server_info]- Illegal info data (version mismatch?)");
			return(NULL);
		}
	}

	read(fildes, &infoWord, sizeof(infoWord));
	if (info->is_swapped) {
		bto_get_int32((int *) &swInfoWord, (char *) &infoWord, 1, BYTE_ORDER_REV);
		info->buf_size = swInfoWord;
	} else {
		info->buf_size = infoWord;
	}

	read(fildes, &infoWord, sizeof(infoWord));
	if (info->is_swapped) {
		bto_get_int32((int *) &swInfoWord, (char *) &infoWord, 1, BYTE_ORDER_REV);
		info->buf_p_stream = swInfoWord;
	} else {
		info->buf_p_stream = infoWord;
	}

	read(fildes, &infoWord, sizeof(infoWord));
	if (info->is_swapped) {
		bto_get_int32((int *) &swInfoWord, (char *) &infoWord, 1, BYTE_ORDER_REV);
		info->nof_streams = swInfoWord;
	} else {
		info->nof_streams = infoWord;
	}

	printf("mbsio: ServerInfo >> buffer size: %d, bufs per stream: %d, number of streams: %d\n",
			info->buf_size,
			info->buf_p_stream,
			info->nof_streams);
	if (errno != 0) {
		sprintf(loc_errbuf, "?SYSERR-[mbs_read_server_info]- %s (%d)", strerror(errno), errno);
		return(NULL);
	}
	return(info);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Read data from remote stream
	\details	Reads a buffer from remote stream
	\param[in]	fildes			-- file descriptor for current connection
	\param[in]	buf 			-- ptr to output buffer
	\param[in]	info			-- server info block
	\retval 	bytes_read		-- number of bytes read by this request
*/
/*-------------------------------------------------------------------------------------------*/

int _mbs_read_stream(int fildes, char * buf, MBSServerInfo *info) {

	int nof_read, nof_tot_read;

	nof_read = 0;
	nof_tot_read = 0;

	while (nof_tot_read < info->buf_size &&
			(nof_read = read(fildes, buf + nof_tot_read, info->buf_size - nof_tot_read)) > 0) {
		nof_tot_read += nof_read;
	}

	if (nof_read < 0) return(nof_read);
	if (lmd_out) fwrite(buf, 1, nof_tot_read, lmd_out);
	return(nof_tot_read);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Request a stream from server
	\details	Sends a request to server
	\param[in]	fildes			-- file descriptor for current connection
	\retval 	sts 			-- status
*/
/*-------------------------------------------------------------------------------------------*/

int _mbs_request_stream(int fildes) {

	return(write(fildes, "            ", 12));
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Disconnect from MBS server
	\param[in]	fildes			-- file descriptor for current connection
	\param[in]	server_type 	-- type of connection
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_disconnect_from_server(int fildes, unsigned int server_type) {

	switch (server_type) {
		case MBS_CTYPE_SYNC:
			break;
		case MBS_CTYPE_ASYNC:
			_mbs_request_stream(fildes);
			break;
	}
	shutdown(fildes, 2);
	close(fildes);
}


/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Initialize buffer pool
	\details	Initializes buffer numbers and data pointers of buffer pool
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_init_pool(MBSDataIO * mbs) {

	register int i;
	register MBSBufferPool * bpp;

	bpp = mbs->buf_pool;
	for (i = 0; i < MBS_N_BUFFERS; i++, bpp++) {
		bpp->bufno_mbs = -1;
		bpp->data = NULL;
	}
	mbs->poolpt = NULL;
	mbs->buf_oo_phase = 0;
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Free memory allocated for the buffer pool
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_free_pool(MBSDataIO * mbs) {

	register int i;
	register MBSBufferPool * bpp;

	bpp = mbs->buf_pool;
	for (i = 0; i < MBS_N_BUFFERS; i++, bpp++) {
		if (bpp->data != NULL) free(bpp->data);
	}
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Find an empty place in the buffer pool
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	bpp 		-- pointer to empty buffer
*/
/*-------------------------------------------------------------------------------------------*/

MBSBufferPool * _mbs_get_pool_pointer(MBSDataIO * mbs) {

	register int i;
	register MBSBufferPool * bpp;

	bpp = mbs->buf_pool;
	for (i = 0; i < MBS_N_BUFFERS; i++, bpp++) {
		if (bpp->bufno_mbs == -1) {
			if (bpp->data == NULL) {
				bpp->data = calloc(1, mbs->bufsiz);
				if (bpp->data == NULL)
				{
					sprintf(loc_errbuf,
						"?ALLOC-[_mbs_get_pool_pointer]- %s: Can't allocate internal buffer",
														mbs->device);
					_mbs_output_error(mbs);
					return(NULL);
				}
			}
			return(bpp);
		}
	}
	sprintf(loc_errbuf,
		"?NOBUF-[_mbs_get_pool_pointer]- %s: Can't find empty slot in buffer pool",
														mbs->device);
	_mbs_output_error(mbs);
	return(NULL);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Store current buffer number in pool header
	\details	Extracts buffer number from MBS data and stores it in pool header
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_store_bufno(MBSDataIO * mbs) {

	MBSBufferPool * bpp;

	bpp = mbs->poolpt;
	bpp->bufno_mbs = ((s_bufhe *) bpp->data)->l_buf;
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Store time stamp
	\details	Extracts buffer time stamp and stores is in MBS database
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_store_time_stamp(MBSDataIO * mbs) {

	s_bufhe *bh;
	bh = (s_bufhe *) (mbs->poolpt)->data;

	mbs->buf_ts = ((long long) bh->l_time[0]) << 32 | ((long long) bh->l_time[1]);
	if (mbs->buf_ts_start == 0) mbs->buf_ts_start = mbs->buf_ts;
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Search for next buffer in the pool
	\details	Inspects the buffer pool to find a buffer with the right buffer number = last+1
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\retval 	bpp 		-- pointer to selected buffer
*/
/*-------------------------------------------------------------------------------------------*/

MBSBufferPool * _mbs_find_subseq_buffer(MBSDataIO * mbs) {

	register int i;
	register MBSBufferPool * bpp;
	int bufno;

	if ((bufno = mbs->bufno_mbs) <= 0) return(NULL);
	bufno++;

	bpp = mbs->buf_pool;
	for (i = 0; i < MBS_N_BUFFERS; i++, bpp++) {
		if (bpp->bufno_mbs == bufno) return(bpp);
	}
	return(NULL);
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Dump buffer data
	\details	Opens a file named "XXX_buf_NNN.dmp"
				where XXX is the device and NNN is the buffer number.
				Dumps bufsiz data to file.
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_dump_buffer(MBSDataIO * mbs) {

	register FILE * f;
	char fname[MBS_L_STR];

	char datestr[MBS_L_STR];
	time_t now;

	if (mbs->bufpt != NULL) {
		sprintf(fname, "%s_buf_%d.dmp", mbs->device, mbs->cur_bufno);
		if ((f = fopen(fname, "w")) != NULL) {
			sprintf(loc_logbuf,
				"%%BUFDMP-[_mbs_dump_buffer]- Dumping buffer #%d/%d (%d bytes) to file \"%s\"",
								mbs->bufno_mbs, mbs->cur_bufno, mbs->bufsiz, fname);
			_mbs_output_log(mbs);
			fwrite(mbs->bufpt, mbs->bufsiz, 1, f);
			fclose(f);
		}
	} else {
		sprintf(loc_errbuf,
			"?NO_BUF-[_mbs_dump_buffer]- Can't dump buffer - no buf addr present (buffer #%d/%d, %d bytes)",
								mbs->bufno_mbs, mbs->cur_bufno, mbs->bufsiz);
		if (rem_errbuf == NULL) {
			fprintf(stderr, "%s\n", loc_errbuf);
		} else {
			strcpy(rem_errbuf, loc_errbuf);
		}
		if (log_out) {
			time(&now);
			strftime(datestr, MBS_L_STR, "%e-%b-%Y %H:%M:%S", localtime(&now));
			fprintf(log_out, "%-18s: %s\n", datestr, loc_errbuf);
		}
	}
}

/*-------------------------------------------------------------------------------------------*/
/*!	\brief		[internal] Turn on/off run flag
	\param[in]	mbs 		-- ptr to MBS database as returned by mbs_open_file()
	\param[in]	flag		-- enable/disable run
*/
/*-------------------------------------------------------------------------------------------*/

void _mbs_set_run_flag(MBSDataIO * mbs, boolean flag) {

	mbs->running = flag;
}

