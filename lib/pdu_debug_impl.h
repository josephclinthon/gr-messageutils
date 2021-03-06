/* -*- c++ -*- */
/* 
 * Copyright 2014 Adam Gannon.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_MESSAGEUTILS_PDU_DEBUG_IMPL_H
#define INCLUDED_MESSAGEUTILS_PDU_DEBUG_IMPL_H

#include <messageutils/pdu_debug.h>

namespace gr {
  namespace messageutils {

    class pdu_debug_impl : public pdu_debug
    {
			private:
				size_t 												d_type;
				bool 													d_print_ascii;
				bool 													d_reverse_ascii;
				bool 													d_print_hex;
				bool 													d_meta_only;
    		bool 													d_display;



      	gr::thread::mutex 						d_mutex;
      	std::vector<pmt::pmt_t> 			d_messages;


     public:
      pdu_debug_impl(size_t type, bool print_ascii, bool reverse_ascii, bool print_hex, bool meta_only, bool display);
      ~pdu_debug_impl();

			void print_one_byte(uint8_t d);
      void print_pdu(pmt::pmt_t pdu);
      uint8_t reverse(uint8_t x);
      int num_messages();
      pmt::pmt_t get_message(int i);

      
    


    };

  } // namespace messageutils
} // namespace gr

#endif /* INCLUDED_MESSAGEUTILS_PDU_DEBUG_IMPL_H */

