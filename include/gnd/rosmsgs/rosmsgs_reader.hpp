/*
 * msgreader.hpp
 *
 *  Created on: 2014/06/24
 *      Author: tyamada
 */

#ifndef MSGREADER_HPP_
#define MSGREADER_HPP_

#include <std_msgs/Header.h>
#include <boost/shared_ptr.hpp>

#include "gnd/gnd-lib-error.h"

namespace gnd {
	namespace rosmsgs {

		template< class M >
		class MessageReader {
		// type definition
		public:
			typedef M msg_t;
			typedef M const const_msg_t;
			typedef boost::shared_ptr< M > msg_sharedptr;
			typedef boost::shared_ptr< M const> const_msg_sharedptr;

		// constructor, destructor
		public:
			MessageReader() {
				msgs_ = 0;
				length_ = 0;
				header_ = 0;
			}
			MessageReader( long l ) {
				msgs_ = 0;
				length_ = 0;
				header_ = 0;
				allocate(l);
			}
			~MessageReader(){
				deallocate();
			}

		// msg data
		protected:
			// ring buffer
			msg_t *msgs_;
			int length_;
			int header_;

		//  allocate, deallocate
		public:
			int is_allocated() {
				return msgs_ != 0 && length_ > 0;
			}

			int allocate( int l ) {
				gnd_assert( is_allocated(), -1, "buffer is already allocated");
				msgs_ = new M[l];
				if( msgs_ ) length_ = l;
				header_ = 0;
				return msgs_ != 0;
			}

			int deallocate() {
				gnd_error( !is_allocated(), -1, "buffer is not allocated");
				delete[] msgs_;
				length_ = 0;
				header_ = 0;

				return 0;
			}

		// seek
		public:
			int copy_latest( msg_t* m ) {
				*m = msgs_[header_];
				return 0;
			}

			virtual int copy_at_time( msg_t* dest, ros::Time *query ){
				return -1;
			}

			virtual int copy_next( msg_t* dest,  uint32_t query){
				return -1;
			}
			virtual int copy_next( msg_t* dest, std_msgs::Header *query){
				return this->copy_next(dest, query->seq);
			}


		// callback for ROS topic subscribe
		public:
			// callback for ROS topic subscription
			public:
				/**
				 * \brief copy from ROS topic (callback for ROS topic subscription)
				 * \memo usage: MessageReader<foo> foo_reader;
				 *              ros::NodeHandle nodehandle;
				 *              nodehandle.subscribe("topic", 10, &MessageReader<foo>::rosmsg_read, foo_reader.reader_pointer());
				 */
			void rosmsg_read(const boost::shared_ptr< M const>& m ){
				gnd_assert_void( !is_allocated() , "null buffer" );
				header_ = (header_ + 1) % length_;
				msgs_[header_] = *m;
			}
			MessageReader<M>* reader_pointer() {
				return this;
			}
		};
	}
}


#endif /* MSGREADER_HPP_ */
