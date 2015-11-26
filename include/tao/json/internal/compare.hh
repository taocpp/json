// Copyright (c) 2015 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_COMPARE_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_COMPARE_HH

namespace tao
{
   namespace json
   {
      inline bool operator< ( const value & l, const value & r );
      inline bool operator== ( const value & l, const value & r );

      namespace internal
      {
         inline bool less_value( const value & l, const value & r )
         {
            switch ( l.type() ) {
               case type::NULL_:
                  return false;
               case type::BOOL_:
                  return l.unsafe_bool() < r.unsafe_bool();
               case type::INT64:
                  return l.unsafe_int64() < r.unsafe_int64();
               case type::DOUBLE:
                  return l.unsafe_double() < r.unsafe_double();
               case type::STRING:
                  return l.unsafe_string() < r.unsafe_string();
               case type::ARRAY:
                  return l.unsafe_array() < r.unsafe_array();
               case type::OBJECT:
                  return l.unsafe_object() < r.unsafe_object();
            }
            assert( false );
         }

         inline bool equal_value( const value & l, const value & r )
         {
            switch ( l.type() ) {
               case type::NULL_:
                  return true;
               case type::BOOL_:
                  return l.unsafe_bool() == r.unsafe_bool();
               case type::INT64:
                  return l.unsafe_int64() == r.unsafe_int64();
               case type::DOUBLE:
                  return l.unsafe_double() == r.unsafe_double();
               case type::STRING:
                  return l.unsafe_string() == r.unsafe_string();
               case type::ARRAY:
                  return l.unsafe_array() == r.unsafe_array();
               case type::OBJECT:
                  return l.unsafe_object() == r.unsafe_object();
            }
            assert( false );
         }

      } // internal

   } // json

} // tao

#endif
