#ifndef C_STYLE_PROTOS
#include <Rtypes.h>
#define XC	extern "C"		/*!< ROOT/C==: extern "C", C: n/a */
#else
#define XC
#endif

/******************************************************************************/
/*! \file			mbsio_protos.h
	\brief			C interface to mbsio calls
	\details		Defines protoypes of mbsio calls to be used with C and C++
	\author 		R. Lutter
	\version		$Revision: 1.11 $       
	\date           $Date: 2009-01-21 07:20:38 $
*******************************************************************************/

XC	char * bto_get_short(short * out, char * in, int cnt, int bytord);
XC	char * bto_get_int32(int * out, char * in, int cnt, int bytord);
XC	char * bto_get_string(char * out, char * in, int cnt, int bytord);
XC	char * bto_put_int32(int * out, char * in, int cnt, int bytord);

XC	void mbs_set_sevt_minwc(MBSDataIO * mbs, int wc);
XC	unsigned int mbs_next_sevent_raw(MBSDataIO * mbs);
XC	unsigned int mbs_next_sevent(MBSDataIO * mbs);
XC	unsigned int mbs_next_sheader(MBSDataIO * mbs);
XC	unsigned int mbs_get_sevent_subtype(MBSDataIO * mbs);
XC	int mbs_get_sevent_serial(MBSDataIO * mbs);
XC	int mbs_get_sevent_wc(MBSDataIO * mbs);
XC	unsigned short * mbs_get_sevent_dataptr(MBSDataIO *);
XC	unsigned int mbs_next_sdata_raw(MBSDataIO * mbs);
XC	unsigned int mbs_next_sdata(MBSDataIO * mbs);
XC	void mbs_pass_errors(const char * errbuf);
XC	MBSDataIO * mbs_open_file(char * device, char * connection , int bufsiz, FILE * out);
XC	boolean mbs_close_file(MBSDataIO * mbs);
XC	void mbs_free_dbase(MBSDataIO * mbs);
XC	unsigned int mbs_next_event(MBSDataIO * mbs);
XC	int mbs_get_event_trigger(MBSDataIO * mbs);
XC	int mbs_pass_sevent(MBSDataIO * mbs, unsigned short * data);
XC	boolean mbs_show(MBSDataIO * mbs, const char * elem, FILE * out);
XC	boolean mbs_set_show(MBSDataIO * mbs, const char * elem, int redu, FILE * out);
XC	boolean mbs_show_stat(MBSDataIO * mbs, FILE * out);
XC	boolean mbs_set_stat(MBSDataIO * mbs, int redu, FILE * out);
XC	boolean mbs_set_stream(MBSDataIO * mbs, int nstr, int sld);
XC	void mbs_set_dump(MBSDataIO * mbs, int);
XC	boolean mbs_open_log(const char * log);
XC	boolean mbs_open_lmd(const char * lmd);
XC	void mbs_close_lmd();
XC	boolean mbs_open_med(const char * med);
XC	void mbs_close_med();
XC	boolean mbs_is_running(MBSDataIO *mbs);

XC	boolean _mbs_check_dbase(MBSDataIO * mbs);
XC	void _mbs_output_error();
XC	int _mbs_connect_to_server(char * host, unsigned int stype);
XC	MBSServerInfo * _mbs_read_server_info(int fildes, MBSServerInfo * info);
XC	void _mbs_init_hit(MBSBufferElem *tlist);
XC	void _mbs_init_triggers();
XC	void _mbs_init_pool(MBSDataIO * mbs);
XC	unsigned int _mbs_next_buffer(MBSDataIO * mbs);
XC	boolean _mbs_check_active(MBSDataIO * mbs);
XC	void _mbs_disconnect_from_server(int fildes, unsigned int stype);
XC	void _mbs_output_log();
XC	void _mbs_free_pool(MBSDataIO * mbs);
XC	MBSBufferPool * _mbs_find_subseq_buffer(MBSDataIO * mbs);
XC	unsigned int _mbs_read_buffer(MBSDataIO * mbs);
XC	boolean _mbs_check_sequence(MBSDataIO *mbs, unsigned int type);
XC	boolean _mbs_check_buffer_empty(MBSDataIO *mbs);
XC	MBSBufferPool * _mbs_get_pool_pointer(MBSDataIO * mbs);
XC	int _mbs_request_stream(int fildes);
XC	int _mbs_read_stream(int fildes, char * buf, MBSServerInfo * info);
XC	void _mbs_dump_buffer(MBSDataIO * mbs);
XC	unsigned int _mbs_convert_data(MBSDataIO * mbs);
XC	void _mbs_store_bufno(MBSDataIO * mbs);
XC	void _mbs_store_time_stamp(MBSDataIO * mbs);
XC	unsigned int _mbs_next_med_event(MBSDataIO * mbs);
XC	unsigned int _mbs_next_lmd_event(MBSDataIO * mbs);
XC	MBSBufferElem *_mbs_check_type(unsigned int btype, MBSBufferElem * ltdescr, MBSBufferElem * tlist);
XC	void _mbs_set_run_flag(MBSDataIO * mbs, boolean flag);

XC	void _mbs_show_fheader(MBSDataIO * mbs, FILE * out);
XC	void _mbs_show_bheader();
XC	void _mbs_show_evhe_10_1();
XC	void _mbs_show_sev_10_1();
XC	void _mbs_show_sev_short();
XC	void _mbs_show_sev_long();
XC	void _mbs_show_sev_raw();
XC	void _mbs_copy_fheader();
XC	void _mbs_convert_bheader();
XC	void _mbs_convert_eheader();
XC	void _mbs_convert_sheader();
XC	void _mbs_type_error();
XC	unsigned int *_mbs_unpack_sev_10_1();
XC	unsigned int *_mbs_unpack_sev_short();
XC	unsigned int *_mbs_unpack_sev_long();
XC	unsigned int *_mbs_unpack_sev_raw();
	
XC	void _mbs_show_sev_9000_1();
XC	void _mbs_show_sev_9000_2();
XC	unsigned int *_mbs_unpack_sev_9000_X();

