/*
 * laserscan_reader.hpp
 *
 *  Created on: 2014/06/23
 *      Author: tyamada
 */

#ifndef LASERSCAN_READER_HPP_
#define LASERSCAN_READER_HPP_

#include "gnd/rosmsgs/rosmsgs_reader.hpp"
#include "sensor_msgs/LaserScan.h"


namespace gnd {
	namespace rosmsgs {
		class LaserScanReader :
			public MessageReader< sensor_msgs::LaserScan > {

		public:
			virtual int copy_at_time( msg_t *dest, ros::Time *query );
			virtual int copy_next( msg_t *dest, uint32_t query);

		};
	}
}

//void (gnd::rosmsgs::LaserScanReader::*)(const LaserScanConstPtr&, int)




#endif /* LASERSCAN_READER_HPP_ */
