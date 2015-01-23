/* $Id: smoketest.c 121 2009-10-07 11:17:07Z oh2gve $
 *
 * This file contains an automatic smoke tester for libfap.
 * 
 * Copyright 2009 Tapio Aaltonen
 *
 * This file is part of libfap.
 *
 * Libfap may be copied only under the terms of either the Artistic License
 * or the GNU General Public License, which may be found in the libfap
 * source kit.
 *
 * APRS is a registered trademark of APRS Software and Bob Bruninga, WB4APR.
*/

/**
 * \file smoketest.c
 * \brief Automatic smoke tester for the library.
 * \author Tapio Aaltonen
*/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "fap.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


/* KISS frame control bytes. */
#define FEND 0xc0
#define FESC 0xdb
#define TFEND 0xdc
#define TFESC 0xdd


/* Operation buffer. */
#define BUFFER_LEN 512
char buffer[BUFFER_LEN];



// Prints all available fields and their values to stdout.
void print_packet(fap_packet_t* packet)
{
	int i;

	if ( packet->error_code ) printf("error_code: %d\n", *packet->error_code);
	if ( packet->error_message ) printf("error_message: %s\n", packet->error_message);
	if ( packet->type ) printf("type: %d\n", *packet->type);
	
	if ( packet->orig_packet ) printf("orig_packet: %s\n", packet->orig_packet);
	printf("orig_packet_len: %d\n", packet->orig_packet_len);
	
	if ( packet->header ) printf("header: %s\n", packet->header);
	if ( packet->body ) printf("body: %s\n", packet->body);
	printf("body_len: %d\n", packet->body_len);
	if ( packet->src_callsign ) printf("src_callsign: %s\n", packet->src_callsign);
	if ( packet->dst_callsign ) printf("dst_callsign: %s\n", packet->dst_callsign);
	if ( packet->path )
	{
		printf("path:");
		for ( i = 0; i < packet->path_len; ++i )
		{
			printf(" %s", packet->path[i]);
		}
		printf("\n");
	}
	printf("path_len: %d\n", packet->path_len);
	
	if ( packet->latitude ) printf("latitude: %f\n", *packet->latitude);
	if ( packet->longitude ) printf("longitude: %f\n", *packet->longitude);
	if ( packet->pos_resolution ) printf("pos_resolution: %f\n", *packet->pos_resolution);
	if ( packet->pos_ambiguity ) printf("pos_ambiguity: %d\n", *packet->pos_ambiguity);
	if ( packet->dao_datum_byte ) printf("dao_datum_byte: %c\n", packet->dao_datum_byte);
	
	if ( packet->altitude ) printf("altitude: %d\n", *packet->altitude);
	if ( packet->course ) printf("course: %d\n", *packet->course);
	if ( packet->speed ) printf("speed: %d\n", *packet->speed);

	if ( packet->symbol_table ) printf("symbol_table: %c\n", packet->symbol_table);
	if ( packet->symbol_code ) printf("symbol_code: %c\n", packet->symbol_code);

	if ( packet->messaging ) printf("messaging: %d\n", *packet->messaging);
	if ( packet->destination ) printf("destination: %s\n", packet->destination);
	if ( packet->message ) printf("message: %s\n", packet->message);
	if ( packet->message_ack ) printf("message_ack: %s\n", packet->message_ack);
	if ( packet->message_id ) printf("message_id: %s\n", packet->message_id);
	if ( packet->comment ) printf("comment: %s\n", packet->comment);
	printf("comment_len: %d\n", packet->comment_len);
	
	if ( packet->object_or_item_name ) printf("object_or_item_name: %s\n", packet->object_or_item_name);
	if ( packet->alive ) printf("alive: %d\n", *packet->alive);
	
	if ( packet->gps_fix_status ) printf("gps_fix_status: %d\n", *packet->gps_fix_status);
	if ( packet->radio_range ) printf("radio_range: %d\n", *packet->radio_range);
	if ( packet->phg ) printf("phg: %s\n", packet->phg);
	if ( packet->timestamp ) printf("timestamp: %d (%s)\n", (unsigned int)*packet->timestamp, ctime(packet->timestamp));
	if ( packet->nmea_checksum_ok ) printf("nmea_checksum_ok: %d\n", *packet->nmea_checksum_ok);
	
	if ( packet->wx_report )
	{
		printf("wx_report:");
		printf("\n");
	}
	
	if ( packet->telemetry )
	{
		printf("telemetry:");
		printf("\n");
	}

	if ( packet->messagebits ) printf("messagebits: %s (%s)\n", packet->messagebits, fap_mice_mbits_to_message(packet->messagebits));
	if ( packet->status ) printf("status: %s\n", packet->status);
	printf("status_len: %d\n", packet->status_len);
	
	if ( packet->capabilities )
	{
		printf("capabilities:");
		printf("\n");
	}
	printf("capabilites_len: %d\n", packet->capabilities_len);
}



// Runs tests fap_parseaprs().
int test_parser()
{
	fap_packet_t* packet;
	int retval = 1;
	
	memset(buffer, 0, BUFFER_LEN);
	sprintf(buffer, "OH2GVE-15>APRS,TCPIP*:!//H*{TPI.L{0Ctest");
	printf("########################################\n");
	printf("Testing parser with packet (%s) ... ", buffer);
	packet = fap_parseaprs(buffer, strlen(buffer), 0);
	if ( packet->error_code )
	{
		printf("failed: %s\n", fap_explain_error(*packet->error_code));
		retval = 0;
	}
	else printf("success\n");
	print_packet(packet);
	printf("########################################\n");
	fap_free(packet);
	
	memset(buffer, 0, BUFFER_LEN);
	sprintf(buffer, "OH3RBE-1>APND0Z,WIDE1-1:)ACUTA!6130.32N/02348.81E+ 24h Emergency Ward ACUTA +358 3 5657 0023");
	printf("########################################\n");
	printf("Testing parser with packet (%s) ... ", buffer);
	packet = fap_parseaprs(buffer, strlen(buffer), 0);
	if ( packet->error_code )
	{
		printf("failed: %s\n", fap_explain_error(*packet->error_code));
		retval = 0;
	}
	else printf("success\n");
	print_packet(packet);
	printf("########################################\n");
	fap_free(packet);
	
	memset(buffer, 0, BUFFER_LEN);
	sprintf(buffer, "OH3RDH>APNU19,OH3RBE-3*:!6059.33NU02530.82E#PHG5330 [Hollola KP20SX UIDIGI 1.9B3] 2");
	printf("########################################\n");
	printf("Testing parser with packet (%s) ... ", buffer);
	packet = fap_parseaprs(buffer, strlen(buffer), 0);
	if ( packet->error_code )
	{
		printf("failed: %s\n", fap_explain_error(*packet->error_code));
		retval = 0;
	}
	else printf("success\n");
	print_packet(packet);
	printf("########################################\n");
	fap_free(packet);
	
	memset(buffer, 0, BUFFER_LEN);
	sprintf(buffer, "OH3RBE>APD225,OH3RBE-1*,WIDE1*:}OH3KV>APU25N,TCPIP*,OH3RBE*:=6055.91N/02335.96E#RX-igate KOIJARVI  OH3KV {UIV32}");
	printf("########################################\n");
	printf("Testing parser with packet (%s) ... ", buffer);
	packet = fap_parseaprs(buffer, strlen(buffer), 0);
	if ( packet->error_code )
	{
		printf("failed: %s\n", fap_explain_error(*packet->error_code));
		retval = 0;
	}
	else printf("success\n");
	print_packet(packet);
	printf("########################################\n");
	fap_free(packet);
	
	memset(buffer, 0, BUFFER_LEN);
	sprintf(buffer, "OH5LEP-9>V0SS62,OH2RDU*,OH3RBE-3*:`4V.l!e>/AZO-592");
	printf("########################################\n");
	printf("Testing parser with packet (%s) ... ", buffer);
	packet = fap_parseaprs(buffer, strlen(buffer), 0);
	if ( packet->error_code )
	{
		printf("failed: %s\n", fap_explain_error(*packet->error_code));
		retval = 0;
	}
	else printf("success\n");
	print_packet(packet);
	printf("########################################\n");
	fap_free(packet);
	
	memset(buffer, 0, BUFFER_LEN);
	sprintf(buffer, "OH3RUX>BEACON,OH3RBE-1*,TRACE2-1:!6213.36N/02404.22E#PHG9500/R,W,T OH3RUX Virrat");
	printf("########################################\n");
	printf("Testing parser with packet (%s) ... ", buffer);
	packet = fap_parseaprs(buffer, strlen(buffer), 1);
	if ( packet->error_code )
	{
		printf("failed: %s\n", fap_explain_error(*packet->error_code));
		retval = 0;
	}
	else printf("success\n");
	print_packet(packet);
	printf("########################################\n");
	fap_free(packet);
	
	return retval;
}



// Runs tests for fap_kiss_to_tnc2().
int test_converters(char const* filename)
{
	FILE* infile;
	
	int retval = 1, badness = 0, i = 0;

	unsigned int out_len = BUFFER_LEN, tnc_id = 666;
	unsigned int kissframe_len = BUFFER_LEN;
	char outbuffer[out_len];

	char orig_data[BUFFER_LEN];
	unsigned int orig_data_len;
	
	
	printf("Reading test data from %s:", filename);
	memset(buffer, 0, BUFFER_LEN);
	infile = fopen(filename, "r");
	while ( !feof(infile) )
	{
		buffer[i] = getc(infile);
		orig_data[i] = buffer[i];
		if ( !feof(infile) )
		{
			printf(" %2hhx", buffer[i]);
			++i;
		}
		else
		{
			break;
		}
	}
	orig_data_len = i;
	fclose(infile);
	printf("\n");

	
	printf("########################################\n");
	printf("Testing kiss_to_tnc2() with a %d-byte input ... ", i);
	memset(outbuffer, 0, out_len);
	if ( fap_kiss_to_tnc2(buffer, i, (char*)&outbuffer, &out_len, &tnc_id) )
	{
		printf("success.\n");
	}
	else
	{
		printf("failure.\n");
		retval = 0;
	}
	printf("tnc2frame=(%s) len=(%d) tnc_id=(%d)\n", outbuffer, out_len, tnc_id);
	if ( !retval ) return 0;
	printf("########################################\n");

	
	printf("########################################\n");
	printf("Testing tnc2_to_kiss() by trying to convert output of previous test back to KISS frame ... ");
	memset(buffer, 0, BUFFER_LEN);
	if ( fap_tnc2_to_kiss(outbuffer, out_len, tnc_id, buffer, &kissframe_len) )
	{
		printf("success.\n");
	}
	else
	{
		printf("failure.\n");
		retval = 0;
	}
	printf("Comparing output to original test data:");
	for ( i = 0; i < orig_data_len; ++i )
	{
		if ( orig_data[i] != buffer[i] )
		{
			printf("!%2hhx%2hhx", orig_data[i], buffer[i]);
			badness = 1;
		}
		else
		{
			printf(" %2hhx%2hhx", orig_data[i], buffer[i]);
		}
	}
	printf("\n");
	if ( badness || kissframe_len != orig_data_len )
	{
		printf("Differences found, test failed!\n");
		retval = 0;
	}
	printf("Original test data had %d bytes, tnc2_to_kiss() returned %d bytes.\n", orig_data_len, kissframe_len);
	return retval;
}



int main(int argc, char* argv[])
{
	int retval;
	char const* kiss_datafile_name = "smoketest.data";
	FILE* tmp;
	
	fap_init();

	printf("Testing the parser.\n");
	retval = test_parser();
	if ( (tmp = fopen(kiss_datafile_name, "r")) != NULL )
	{
		fclose(tmp);
		printf("Found test data file, engaging converter tests.\n");
		retval = test_converters(kiss_datafile_name) && retval;
	}
		
	fap_cleanup();
	
	if ( !retval ) return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
