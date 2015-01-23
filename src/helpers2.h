/* $Id: helpers2.h 161 2010-04-02 20:06:51Z oh2gve $
 *
 * Copyright 2005, 2006, 2007, 2008, 2009 Tapio Sokura
 * Copyright 2007, 2008, 2009 Heikki Hannikainen
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
 * \file helpers2.h
 * \brief Declarations of helper functions for helpers.c.
 * \author Tapio Aaltonen
*/

#ifndef HELPERS2_H
#define HELPERS2_H


#include "fap.h"


/// Max amount of path elements.
#define MAX_DIGIS 8

/// Nautical miles per hour to kilometers per hour.
#define KNOT_TO_KMH 1.852

/// Miles per hour to kilometers per hour.
#define MPH_TO_KMH 1.609344

/// Kilometers per hour to meters per second.
#define KMH_TO_MS 10.0/36.0

/// Miles per hout to meters per second.
#define MPH_TO_MS (MPH_TO_KMH * KMH_TO_MS)

/// Feets to meters.
#define FT_TO_M 0.3048

/// Fahrenheit to celcius degrees.
#define FAHRENHEIT_TO_CELCIUS(x) ((x-32.0)/1.8)

/// Hundredths of an inch to millimeters
#define HINCH_TO_MM 0.254



/// Linked list item.
typedef struct fapint_llist_item
{
	/// Data stored in the element.
	char* text;
	/// Next element in list.
	struct fapint_llist_item* next;
} fapint_llist_item_t;


/// Cleanup function for linked list.
void fapint_clear_llist(fapint_llist_item_t* list);



/// Return position resolution in meters based on the number of minute decimal digits.
/** Also accepts negative numbers, i.e. -1 for 10 minute resolution and -2
 *  for 1 degree resolution. Calculation is based on latitude so it is worst
 *  case (resolution in longitude gets better as you get closer to the
 *  poles).
*/
double fapint_get_pos_resolution(int const minute_digit_count);


/// Parse station symbol from GPSxyz or SPCxyz -formatted destination callsign.
int fapint_parse_symbol_from_dst_callsign(fap_packet_t* packet);


/// Maps destination callsign symbol chars to APRS symbol.
/**
 * Contains a list of mappings from GPSxyz (or SPCxyz) to APRS symbols.
 * Overlay characters (z) are not handled here.
 * \param input Two chars from destination callsign.
 * \param output Two chars ([0] and [1]) of aprs symbol.
*/
int fapint_symbol_from_dst_type(char input[2], char* output);


/// Returns 0 if string contains non-numeric chars, 1 if not.
int fapint_is_number(char const* input);


/// Returns 1 if given date is valid. */
int fapint_check_date(unsigned int year, unsigned int month, unsigned int day);


/// Get coordinate from nmea fields.
/**
 * Field 2 is used to decide between latitude and longitude. If
 * pos_resolution is not set, this will set it too.
 * \param packet Results are saved here.
 * \param field1 The (dd)dmm.m(mmm..) string.
 * \param field2 The north/south or east/west indicator.
 * \return Zero for failure, non-zero for success.
*/
int fapint_get_nmea_latlon(fap_packet_t* packet, char* field1, char* field2);


/// Set all fields of given wx report to NULL.
void fapint_init_wx_report(fap_wx_report_t* wx_report);


#endif // HELPERS2_H
