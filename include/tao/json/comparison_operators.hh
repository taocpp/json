// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_COMPARISON_OPERATORS_HH
#define TAOCPP_JSON_INCLUDE_COMPARISON_OPERATORS_HH

#include "value.hh"

namespace tao
{
   namespace json
   {
      template< template< typename ... > class Traits >
      bool operator== ( const basic_value< Traits > & lhs, const basic_value< Traits > & rhs ) noexcept
      {
         if ( lhs.type() == type::POINTER ) {
            if ( const auto * p = lhs.unsafe_get_pointer() ) {
               return * p == rhs;
            }
            else {
               return null == rhs;
            }
         }
         if ( lhs.type() != rhs.type() ) {
            if ( rhs.type() == type::POINTER ) {
               if ( const auto * p = rhs.unsafe_get_pointer() ) {
                  return lhs == * p;
               }
               else {
                  return lhs == null;
               }
            }
            if ( lhs.type() == type::SIGNED ) {
               if ( rhs.type() == type::UNSIGNED ) {
                  const auto v = lhs.unsafe_get_signed();
                  return ( v >= 0 ) && ( static_cast< std::uint64_t >( v ) == rhs.unsafe_get_unsigned() );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_signed() == rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::UNSIGNED ) {
               if ( rhs.type() == type::SIGNED ) {
                  const auto v = rhs.unsafe_get_signed();
                  return ( v >= 0 ) && ( lhs.unsafe_get_unsigned() == static_cast< std::uint64_t >( v ) );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_unsigned() == rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::DOUBLE ) {
               if ( rhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_double() == rhs.unsafe_get_signed();
               }
               if ( rhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_double() == rhs.unsafe_get_unsigned();
               }
            }
            return false;
         }
         switch ( lhs.type() ) {
            case type::NULL_:
               return true;
            case type::BOOL:
               return lhs.unsafe_get_bool() == rhs.unsafe_get_bool();
            case type::SIGNED:
               return lhs.unsafe_get_signed() == rhs.unsafe_get_signed();
            case type::UNSIGNED:
               return lhs.unsafe_get_unsigned() == rhs.unsafe_get_unsigned();
            case type::DOUBLE:
               return lhs.unsafe_get_double() == rhs.unsafe_get_double();
            case type::STRING:
               return lhs.unsafe_get_string() == rhs.unsafe_get_string();
            case type::ARRAY:
               return lhs.unsafe_get_array() == rhs.unsafe_get_array();
            case type::OBJECT:
               return lhs.unsafe_get_object() == rhs.unsafe_get_object();
            case type::POINTER:
               break;  // LCOV_EXCL_LINE
         }
         assert( false );  // LCOV_EXCL_LINE
      }

      template< template< typename ... > class Traits >
      bool operator< ( const basic_value< Traits > & lhs, const basic_value< Traits > & rhs ) noexcept
      {
         if ( lhs.type() == type::POINTER ) {
            if ( const auto * p = lhs.unsafe_get_pointer() ) {
               return * p < rhs;
            }
            else {
               return null < rhs;
            }
         }
         if ( lhs.type() != rhs.type() ) {
            if ( rhs.type() == type::POINTER ) {
               if ( const auto * p = rhs.unsafe_get_pointer() ) {
                  return lhs < * p;
               }
               else {
                  return lhs < null;
               }
            }
            if ( lhs.type() == type::SIGNED ) {
               if ( rhs.type() == type::UNSIGNED ) {
                  const auto v = lhs.unsafe_get_signed();
                  return ( v < 0 ) || ( static_cast< std::uint64_t >( v ) < rhs.unsafe_get_unsigned() );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_signed() < rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::UNSIGNED ) {
               if ( rhs.type() == type::SIGNED ) {
                  const auto v = rhs.unsafe_get_signed();
                  return ( v >= 0 ) && ( lhs.unsafe_get_unsigned() < static_cast< std::uint64_t >( v ) );
               }
               if ( rhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_unsigned() < rhs.unsafe_get_double();
               }
            }
            else if ( lhs.type() == type::DOUBLE ) {
               if ( rhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_double() < rhs.unsafe_get_signed();
               }
               if ( rhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_double() < rhs.unsafe_get_unsigned();
               }
            }
            return lhs.type() < rhs.type();
         }
         switch ( lhs.type() ) {
            case type::NULL_:
               return false;
            case type::BOOL:
               return lhs.unsafe_get_bool() < rhs.unsafe_get_bool();
            case type::SIGNED:
               return lhs.unsafe_get_signed() < rhs.unsafe_get_signed();
            case type::UNSIGNED:
               return lhs.unsafe_get_unsigned() < rhs.unsafe_get_unsigned();
            case type::DOUBLE:
               return lhs.unsafe_get_double() < rhs.unsafe_get_double();
            case type::STRING:
               return lhs.unsafe_get_string() < rhs.unsafe_get_string();
            case type::ARRAY:
               return lhs.unsafe_get_array() < rhs.unsafe_get_array();
            case type::OBJECT:
               return lhs.unsafe_get_object() < rhs.unsafe_get_object();
            case type::POINTER:
               break;  // LCOV_EXCL_LINE
         }
         assert( false );  // LCOV_EXCL_LINE
      }

   } // json

} // tao

#endif
