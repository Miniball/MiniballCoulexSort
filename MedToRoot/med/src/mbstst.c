#define C_STYLE_PROTOS

#include <stdio.h>
#include "byte_order.h"
#include "mbsio.h"
#include "mbsio_protos.h"

int main(int argc, char *argv[])

{
	register int i, j;
	MBSDataIO *mbs;
	int nevts;
	unsigned int etype, stype;
	unsigned short data[8192];
	int swc;
	char mode[20];
	char show[20];
	int scd;
	int raw;
	int dump;
	
	if (argc == 8) {
		strcpy(mode, argv[2]);
		nevts = atoi(argv[3]);
		strcpy(show, argv[4]);
		scd = atoi(argv[5]);
		raw = atoi(argv[6]);
		dump = atoi(argv[7]);
	} else if (argc == 7) {
		strcpy(mode, argv[2]);
		nevts = atoi(argv[3]);
		strcpy(show, argv[4]);
		scd = atoi(argv[5]);
		raw = atoi(argv[6]);
		dump = 0;
	} else if (argc == 6) {
		strcpy(mode, argv[2]);
		nevts = atoi(argv[3]);
		strcpy(show, argv[4]);
		scd = atoi(argv[5]);
		raw = 0;
		dump = 0;
	} else if (argc == 4) {
		strcpy(mode, argv[2]);
		nevts = atoi(argv[3]);
		strcpy(show, "ES");
		scd = 1;
		raw = 0;
		dump = 0;
	} else if (argc == 3) {
		strcpy(mode, argv[2]);
		nevts = 1000000000;
		strcpy(show, "ES");
		scd = 1;
		raw = 0;
		dump = 0;
	} else if (argc == 2) {
		strcpy(mode, "file");
		nevts = 0x7fffffff;
		strcpy(show, "ES");
		scd = 1;
		raw = 0;
		dump = 0;
	} else {
		fprintf(stderr, "\nmbstst: Wrong number of args\n\n");
		fprintf(stderr, "        Usage: mbstst input mode nevts show scd raw dump\n\n");
		fprintf(stderr, "                      input:  data source to read data from\n");
		fprintf(stderr, "                      mode :  xfer mode: S = TCP/IP synchronous (default)\n");
		fprintf(stderr, "                                         A = TCP/IP asynchronous\n");
		fprintf(stderr, "                                         F = from file (ext .lmd or .med\n");
		fprintf(stderr, "                      nevts:  number of events to be collected (default: 0 = unlimited)\n");
		fprintf(stderr, "                      show :  buffer items to be shown (default: \"ES\")\n");
		fprintf(stderr, "                                         F = file header\n");
		fprintf(stderr, "                                         B = buffer header\n");
		fprintf(stderr, "                                         E = event header\n");
		fprintf(stderr, "                                         S = subevent header + data\n");
		fprintf(stderr, "                      scd  :  scale down: show evenry Nth item only (default: 1 = off)\n");
		fprintf(stderr, "                      raw  :  raw mode on/off (default: 0 = off)\n");
		fprintf(stderr, "                      dump :  dump every Nth buffer to file (default: 0 = none)\n\n");
		exit(1);
	}

	printf("mbstst:\n");
	printf("  File/Host: %s\n", argv[1]);
	printf("  Mode     : %s\n", mode);
	printf("  Nevts    : %d\n", nevts);
	printf("  Show     : %s\n", show);
	printf("  Scd      : %d\n", scd);
	printf("  Raw      : %d\n", raw);
	printf("  Dump     : %d\n", dump);

	mbs = mbs_open_file(argv[1], mode, 0, stdout);	// open lmd file
	if (mbs == NULL) exit(1);

	mbs_set_show(mbs, show, scd, stdout);
	mbs_set_stat(mbs, 1, stdout);
	mbs_set_dump(mbs, dump);
	mbs_open_log("mbstst.log");

	if (nevts == 0) nevts = 1000000000;
	for (i = 0; i < nevts; i++) {			// decode events
		etype = mbs_next_event(mbs);
		if (etype == MBS_ETYPE_EOF) {
			printf("\nmbstst: [EOF] End of file\n");
			break;
		} else if (etype == MBS_ETYPE_ERROR) {
			printf("mbstst: Exit from event loop after ERROR\n");
			break;
		} else if (etype == MBS_ETYPE_ABORT) {
			printf("mbstst: Aborting\n");
			break;
		} else {
			for (;;) {
				stype = raw ? mbs_next_sevent_raw(mbs) : mbs_next_sevent(mbs);
				if (stype == MBS_STYPE_EOE) {
					printf("mbstst: [EOE] end of event\n");
					break;
				}
				if (stype != MBS_STYPE_ERROR) {
					swc = mbs_pass_sevent(mbs, data); // copy data to local array
				}
			}
		}
	}
/*	mbs_close_file(mbs); */
	mbs_show_stat(mbs, stdout);
	exit(0);
}
