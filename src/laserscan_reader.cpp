/*
 * laserscan_reader.hpp
 *
 *  Created on: 2014/06/24
 *      Author: tyamada
 */

#include "gnd/rosmsgs/laserscan_reader.hpp"
#include "gnd/gnd-lib-error.h"


int gnd::rosmsgs::LaserScanReader::copy_at_time( msg_t *dest, ros::Time *query ) {
	{ // ---> assertion
		gnd_assert(!dest, -1, "invalid null argument");
		gnd_assert(!query, -1, "invalid null argument");
		gnd_assert(!is_allocated(), -1, "buffer null");
	} // <--- assertion

	{ // ---> operation
		int i, j;

		// ---> seek scan of nearest query time
		if( msgs_[header_].header.stamp < *query ) {
			// query time is after last scan timestamp
			i = header_;
		}
		else {
			ros::Duration diff1, diff2;
			for( j = 1; j < length_; j++ ) {
				i = (header_ - j + length_) % length_;
				if( msgs_[i].header.stamp < *query ) {
					break;
				}
			}
			diff1 = *query - msgs_[i].header.stamp;
			i = (header_ - j - 1 + length_) % length_;
			diff2 = msgs_[i].header.stamp - *query;
			i = diff1 < diff2 ? (header_ - j + length_) % length_ : (header_ - j - 1 + length_) % length_;
		}
		// <--- seek scan of nearest query time

		// copy
		*dest = msgs_[i];
	} // <--- operation

	return 0;
}


int gnd::rosmsgs::LaserScanReader::copy_next( msg_t *dest, uint32_t query ) {
	int ret = 0;
	{ // ---> assertion
		gnd_assert(!dest, -1, "invalid null argument");
		gnd_assert(!is_allocated(), -1, "buffer null");
	} // <--- assertion

	{ // ---> operation
		int i, j;

		// ---> seek scan of nearest query time
		if( msgs_[header_].header.seq <= query ) {
			// query time is after last scan timestamp
			i = header_;
			ret = 1;
		}
		else {
			for( j = 1; j < length_; j++ ) {
				i = (header_ - j + length_) % length_;
				if( msgs_[i].header.seq <= query ) {
					break;
				}
			}
			i = (header_ - j - 1 + length_) % length_;
		}
		// <--- seek scan of nearest query time

		// copy
		*dest = msgs_[i];
	} // <--- operation

	return ret;
}
