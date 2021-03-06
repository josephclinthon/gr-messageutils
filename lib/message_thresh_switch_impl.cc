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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "message_thresh_switch_impl.h"

namespace gr {
  namespace messageutils {

    message_thresh_switch::sptr
    message_thresh_switch::make(size_t itemsize, std::string key, double thresh, bool debug)
    {
      return gnuradio::get_initial_sptr
        (new message_thresh_switch_impl(itemsize, key, thresh, debug));
    }


    message_thresh_switch_impl::message_thresh_switch_impl(size_t itemsize, std::string key, double thresh, bool debug)
      : gr::sync_block("message_thresh_switch",
              gr::io_signature::make(1, 1, itemsize),
              gr::io_signature::make2(2, 2, itemsize, itemsize)),
        d_itemsize(itemsize), d_key(key), d_thresh(thresh), d_debug(debug) 
    {
        message_port_register_in(pmt::mp("msg"));
        set_msg_handler(pmt::mp("msg"),
        boost::bind(&message_thresh_switch_impl::handle_msg, this, _1));
    }


    message_thresh_switch_impl::~message_thresh_switch_impl()
    {
    }


    void
    message_thresh_switch_impl::set_threshold(double thresh)
    {
        d_thresh = thresh;
    }

    void 
    message_thresh_switch_impl::handle_msg(pmt::pmt_t msg)
    {
        d_decode_msg = false;
        
        /* Chechk that received message contains metadata */
        if (pmt::is_pair(msg))      
        {
            d_meta = pmt::car(msg);
            d_vect = pmt::cdr(msg);
            
            if (!pmt::eq(d_meta, pmt::PMT_NIL) )
                d_decode_msg = true;
        }

        if (d_decode_msg)
        {
            /* Get a list of stream tags, find one that matches key, get value */
            pmt::pmt_t klist(pmt::dict_keys(d_meta));
            for(size_t i=0; i<pmt::length(klist); i++)
            {
                pmt::pmt_t k(pmt::nth(i, klist));
                pmt::pmt_t v(pmt::dict_ref(d_meta, k, pmt::PMT_NIL));

                if (pmt::eq(k,pmt::string_to_symbol(d_key)))
                {
                    d_value = pmt::to_double(v);
                    
                    if (d_value > d_thresh)
                        d_val_thresh_comp = VAL_ABOVE;
                    else if (d_value < d_thresh)
                        d_val_thresh_comp = VAL_BELOW;
                    else
                        d_val_thresh_comp = VAL_EQUAL;                    
                }     
            }
    
            /* Print Information if Debug */
            if (d_debug)
            {
                std::cout<<"Key: "<< d_key<<std::setw(20);
                std::cout<< std::fixed << std::setprecision(5);
                std::cout<<"Thresh: "<<d_thresh<<std::setw(20);
                std::cout<<"Value: "<<d_value<<std::setw(20);
                std::cout<<"\tTrigger: "<< ((d_val_thresh_comp == VAL_ABOVE) ? ("ABOVE") : ("BELOW"));
                std::cout<<std::endl;

            }
                       
         }




    }


    int
    message_thresh_switch_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const char *signal = (const char *) input_items[0];
        char *out_above = (char *) output_items[0];
        char *out_below = (char *) output_items[1];

   
        /* Output at either above or below depending on value and threshold */
        if (d_val_thresh_comp == VAL_ABOVE)
        {
            memcpy(out_above,signal,noutput_items*d_itemsize);
            memset(out_below, 0.0, noutput_items * output_signature()->sizeof_stream_item(0)); 
        }
        else if (d_val_thresh_comp == VAL_BELOW)
        {
            memcpy(out_below,signal,noutput_items*d_itemsize);
            memset(out_above, 0.0, noutput_items * output_signature()->sizeof_stream_item(0));     
        }
        /*
        else
        {
            if (d_above_if_equal)
            {
                memcpy(out_above,signal,noutput_items*d_itemsize);
                memset(out_below, 0.0, noutput_items * output_signature()->sizeof_stream_item(0)); 
            }
            else
            {
                memcpy(out_below,signal,noutput_items*d_itemsize);
                memset(out_above, 0.0, noutput_items * output_signature()->sizeof_stream_item(0)); 
            }

        }
        */


        return noutput_items;
    }

  } /* namespace messageutils */
} /* namespace gr */

