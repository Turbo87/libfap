/* $Id: helpers2.c 172 2010-05-06 09:50:16Z oh2gve $
 *
 * Copyright 2005, 2006, 2007, 2008, 2009, 2010 Tapio Sokura
 * Copyright 2007, 2008, 2009, 2010 Heikki Hannikainen
 *
 * Perl-to-C modifications
 * Copyright 2009, 2010 Tapio Aaltonen
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
 * \file helpers2.c
 * \brief Implementations of helper functions for helpers.c.
 * \author Tapio Aaltonen
*/


#include "helpers2.h"
#include "fap.h"
#include "regs.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>
#include <math.h>



void fapint_clear_llist(fapint_llist_item_t* list)
{
	fapint_llist_item_t* current_elem = list, *tmp_elem;

	while ( current_elem != NULL )
	{
		if ( current_elem->text ) free(current_elem->text);
		tmp_elem = current_elem->next;
		free(current_elem);
		current_elem = tmp_elem;
	}
}



double fapint_get_pos_resolution(int const minute_digit_count)
{
	return KNOT_TO_KMH * 1000 * pow(10, -1*minute_digit_count);
}



int fapint_parse_symbol_from_dst_callsign(fap_packet_t* packet)
{
	int len;
	char leftover_str[3];
	char type;

	char numberid_str[3];
	int numberid;

	char dst_type[2];
	char overlay;
	char code[2];
	char tmp_2b[2];

	unsigned int const matchcount = 3;
	regmatch_t matches[matchcount];


	/* Check parameters. */
	if ( !packet || !packet->dst_callsign )
	{
	  return 0;
	}

	/* Check if destination callsign seems to contain symbol info. */
	if ( regexec(&fapint_regex_nmea_dst, packet->dst_callsign, matchcount, (regmatch_t*)&matches, 0) == 0 )
	{
		/* Save symbol-containing part. */
		len = matches[2].rm_eo - matches[2].rm_so;
		memset(leftover_str, 0, 3);
		memcpy(leftover_str, packet->dst_callsign+matches[2].rm_so, len);

		type = leftover_str[0];

		if ( len == 3 )
		{
			if ( type == 'C' || type == 'E' )
			{
				/* Get id. */
				memset(numberid_str, 0, 3);
				numberid_str[0] = leftover_str[1];
				numberid_str[1] = leftover_str[2];
				numberid = atoi(numberid_str);
				
				/* Save symbol, if id is valid. */
				if ( numberid > 0 && numberid < 95 )
				{
					packet->symbol_code = numberid + 32;
					if ( type == 'C' )
					{
						packet->symbol_table = '/';
					}
					else
					{
						packet->symbol_table = '\\';
					}
					return 1;
				}
				else
				{
					/* Invalid id. */
					return 0;
				}
			}
			else
			{
				/* Secondary symbol table, with overlay. Check first that we really are in the secondary symbol table. */
				dst_type[0] = leftover_str[0];
				dst_type[1] = leftover_str[1];
				overlay = leftover_str[2];
				if ( (type == 'O' || type == 'A' || type == 'N' ||
						type == 'D' || type == 'S' || type == 'Q') &&
					  isalnum(overlay) )
				{
					if ( fapint_symbol_from_dst_type(dst_type, code) )
					{
						packet->symbol_table = overlay;
						packet->symbol_code = code[1];
						return 1;
					}
					else
					{
						/* Could not map into APRS symbol. */
						return 0;
					}
				}
				else
				{
					/* Propably not in first symbol table. */
					return 0;
				}
			}
		}
		else
		{
			/* Primary or secondary symbol table, no overlay. */
			tmp_2b[0] = leftover_str[0];
			tmp_2b[1] = leftover_str[1];
			if ( fapint_symbol_from_dst_type(tmp_2b, code) )
			{
				packet->symbol_table = code[0];
				packet->symbol_code = code[1];
				return 1;
			}
			else
			{
				/* Could not map into APRS symbol. */
				return 0;
			}
		}
	}
	else
	{
		return 0;
	}

	/* Failsafe catch-all. */
	return 0;
}



int fapint_symbol_from_dst_type(char input[2], char* output)
{
	switch ( input[0] )
	{
		case 'B':
		case 'O':
			if ( input[0] == 'B' )
			{
				output[0] = '/';
			}
			else
			{
				output[0] = '\\';
			}
			switch ( input[1] )
			{
				case 'B':
					output[1] = '!';
					return 1;
				case 'C':
					output[1] = '"';
					return 1;
				case 'D':
					output[1] = '#';
					return 1;
				case 'E':
					output[1] = '$';
					return 1;
				case 'F':
					output[1] = '%';
					return 1;
				case 'G':
					output[1] = '&';
					return 1;
				case 'H':
					output[1] = '\'';
					return 1;
				case 'I':
					output[1] = '(';
					return 1;
				case 'J':
					output[1] = ')';
					return 1;
				case 'K':
					output[1] = '*';
					return 1;
				case 'L':
					output[1] = '+';
					return 1;
				case 'M':
					output[1] = ',';
					return 1;
				case 'N':
					output[1] = '-';
					return 1;
				case 'O':
					output[1] = '.';
					return 1;
				case 'P':
					output[1] = '/';
					return 1;
			}
			return 0;
		case 'P':
		case 'A':
			if ( input[0] == 'P' )
			{
				output[0] = '/';
			}
			else
			{
				output[0] = '\\';
			}
			if ( isdigit(input[1]) || isupper(input[1]) )
			{
				output[1] = input[1];
				return 1;
			}
			return 0;
		case 'M':
		case 'N':
			if ( input[0] == 'M' )
			{
				output[0] = '/';
			}
			else
			{
				output[0] = '\\';
			}
			switch ( input[1] )
			{
				case 'R':
					output[1] = ':';
					return 1;
				case 'S':
					output[1] = ';';
					return 1;
				case 'T':
					output[1] = '<';
					return 1;
				case 'U':
					output[1] = '=';
					return 1;
				case 'V':
					output[1] = '>';
					return 1;
				case 'W':
					output[1] = '?';
					return 1;
				case 'X':
					output[1] = '@';
					return 1;
			}
			return 0;
		case 'H':
		case 'D':
			if ( input[0] == 'H' )
			{
				output[0] = '/';
			}
			else
			{
				output[0] = '\\';
			}
			switch ( input[1] )
			{
				case 'S':
					output[1] = '[';
					return 1;
				case 'T':
					output[1] = '\\';
					return 1;
				case 'U':
					output[1] = ']';
					return 1;
				case 'V':
					output[1] = '^';
					return 1;
				case 'W':
					output[1] = '_';
					return 1;
				case 'X':
					output[1] = '`';
					return 1;
			}
			return 0;
		case 'L':
		case 'S':
			if ( input[0] == 'L' )
			{
				output[0] = '/';
			}
			else
			{
				output[0] = '\\';
			}
			if ( isupper(input[1]) )
			{
				output[1] = tolower(input[1]);
				return 1;
			}
			return 0;
		case 'J':
		case 'Q':
			if ( input[0] == 'J' )
			{
				output[0] = '/';
			}
			else
			{
				output[0] = '\\';
			}
			switch ( input[1] )
			{
				case '1':
					output[1] = '{';
					return 1;
				case '2':
					output[1] = '|';
					return 1;
				case '3':
					output[1] = '}';
					return 1;
				case '4':
					output[1] = '~';
					return 1;
			}
			return 0;
	}
	
	return 0;
}



int fapint_is_number(char const* input)
{
	int i;

	if ( !input ) return 0;

	for ( i = 0; i < strlen(input); ++i )
	{
		if ( !isdigit(input[i]) || ( i==0 && (input[i]=='-' || input[i]=='+') ) ) return 0;
	}

	return 1;
}



int fapint_check_date(unsigned int year, unsigned int month, unsigned int day)
{
	return year < 10000 && month <= 12 && day <= 31;
}



int fapint_get_nmea_latlon(fap_packet_t* packet, char* field1, char* field2)
{
	double value;
	char direction;

	char* tmp_str;
	unsigned int tmp_us;
	int len;
	
	unsigned int matchcount = 4;
	regmatch_t matches[matchcount];
	

	/* Check params. */
	if ( !packet || !field1 || !field2 )
	{
		return 0;
	}
	
	/* Check and get sign. */
	if ( regexec(&fapint_regex_nmea_flag, field2, matchcount, (regmatch_t*)&matches, 0) == 0 )
	{
		direction = field2[matches[1].rm_so];
	}
	else
	{
		packet->error_code = malloc(sizeof(fap_error_code_t));
		if ( packet->error_code ) *packet->error_code = fapNMEA_INV_SIGN;
		return 0;
	}
	
	/* Be leninent on what to accept, anything goes as long as degrees
	   has 1-3 digits, minutes has 2 digits and there is at least one
	   decimal minute. */
	if ( regexec(&fapint_regex_nmea_coord, field1, matchcount, (regmatch_t*)&matches, 0) == 0 )
	{
		len = matches[1].rm_eo - matches[1].rm_so;
		tmp_str = malloc(len+1);
		if ( !tmp_str ) return 0;
		memcpy(tmp_str, field1+matches[1].rm_so, len);
		tmp_str[len] = 0;
		tmp_us = atoi(tmp_str);
		free(tmp_str);
		
		len = matches[2].rm_eo - matches[2].rm_so;
		tmp_str = malloc(len+1);
		if ( !tmp_str ) return 0;
		memcpy(tmp_str, field1+matches[2].rm_so, len);
		tmp_str[len] = 0;
		value = atof(tmp_str);
		free(tmp_str);
		
		len = matches[3].rm_eo - matches[3].rm_so;
		
		value = tmp_us + value/60;
		if ( !packet->pos_resolution )
		{
			packet->pos_resolution = malloc(sizeof(double));
			if ( !packet->pos_resolution ) return 0;
			*packet->pos_resolution = fapint_get_pos_resolution(len);
		}
	}
	else
	{
		packet->error_code = malloc(sizeof(fap_error_code_t));
		if ( packet->error_code ) *packet->error_code = fapNMEA_INV_CVAL;
		return 0;
	}
	
	/* Apply sign and save. */
	switch ( toupper(direction) )
	{
		case 'E':
		case 'W':
			if ( value > 179.999999 )
			{
				packet->error_code = malloc(sizeof(fap_error_code_t));
				if ( packet->error_code ) *packet->error_code = fapNMEA_LARGE_EW;
				return 0;
			}
			if ( toupper(direction) == 'W' )
			{
				value *= -1;
			}
			packet->longitude = malloc(sizeof(double));
			if ( !packet->longitude ) return 0;
			*packet->longitude = value;
			return 1;
		case 'N':
		case 'S':
			if ( value > 89.999999 )
			{
				packet->error_code = malloc(sizeof(fap_error_code_t));
				if ( packet->error_code ) *packet->error_code = fapNMEA_LARGE_NS;
				return 0;
			}
			if ( toupper(direction) == 'S' )
			{
				value *= -1;
			}
			packet->latitude = malloc(sizeof(double));
			if ( !packet->latitude ) return 0;
			*packet->latitude = value;
			return 1;
	}
		
	return 0;
}



void fapint_init_wx_report(fap_wx_report_t* wx_report)
{
	wx_report->wind_gust = NULL;
	wx_report->wind_dir = NULL;
	wx_report->wind_speed = NULL;
	wx_report->temp = NULL;
	wx_report->temp_in = NULL;
	wx_report->rain_1h = NULL;
	wx_report->rain_24h = NULL;
	wx_report->rain_midnight = NULL;
	wx_report->humidity = NULL;
	wx_report->humidity_in = NULL;
	wx_report->pressure = NULL;
	wx_report->luminosity = NULL;
	wx_report->snow_24h = NULL;
	wx_report->soft = NULL;
}
