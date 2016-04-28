// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_TOTALLY_ORDERED_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_TOTALLY_ORDERED_HH

#include "../external/operators.hpp"

#include "../type.hh"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         // helper class to generate optimized operators

         template< typename T, typename U, type E >
         struct totally_ordered
            : operators::totally_ordered< T, U >
         {
            friend bool operator==( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p == rhs;
                  }
                  else {
                     return false;
                  }
               }
               return ( lhs.type() == E ) && ( lhs.T::template get< E >() == rhs );
            }

            friend bool operator<( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p < rhs;
                  }
                  else {
                     return true;
                  }
               }
               return ( lhs.type() < E ) || ( ( lhs.type() == E ) && ( lhs.T::template get< E >() < rhs ) );
            }

            friend bool operator>( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p > rhs;
                  }
                  else {
                     return false;
                  }
               }
               return ( lhs.type() > E ) || ( ( lhs.type() == E ) && ( lhs.T::template get< E >() > rhs ) );
            }
         };


         template< typename T >
         struct totally_ordered< T, null_t, type::NULL_ >
            : operators::totally_ordered< T, null_t >
         {
            friend bool operator==( const T & lhs, null_t ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p == null;
                  }
                  else {
                     return true;
                  }
               }
               return lhs.type() == type::NULL_;
            }

            friend bool operator<( const T &, null_t ) noexcept
            {
               return false;
            }

            friend bool operator>( const T & lhs, null_t ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p > null;
                  }
                  else {
                     return false;
                  }
               }
               return lhs.type() > type::NULL_;
            }
         };

         template< typename T, typename U >
         struct totally_ordered< T, U, type::SIGNED_INTEGER >
            : operators::totally_ordered< T, U >
         {
            friend bool operator==( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p == rhs;
                  }
                  else {
                     return false;
                  }
               }
               if ( lhs.type() == type::SIGNED_INTEGER ) {
                  return lhs.unsafe_get_signed_integer() == rhs;
               }
               if ( lhs.type() == type::UNSIGNED_INTEGER ) {
                  return ( rhs >= 0 ) && ( lhs.unsafe_get_unsigned_integer() == static_cast< uint64_t >( rhs ) );
               }
               if ( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() == rhs;
               }
               return false;
            }

            friend bool operator<( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p < rhs;
                  }
                  else {
                     return true;
                  }
               }
               if ( lhs.type() == type::SIGNED_INTEGER ) {
                  return lhs.unsafe_get_signed_integer() < rhs;
               }
               if ( lhs.type() == type::UNSIGNED_INTEGER ) {
                  return ( rhs >= 0 ) && ( lhs.unsafe_get_unsigned_integer() < static_cast< uint64_t >( rhs ) );
               }
               if ( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() < rhs;
               }
               return lhs.type() < type::SIGNED_INTEGER;
            }

            friend bool operator>( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p > rhs;
                  }
                  else {
                     return false;
                  }
               }
               if ( lhs.type() == type::SIGNED_INTEGER ) {
                  return lhs.unsafe_get_signed_integer() > rhs;
               }
               if ( lhs.type() == type::UNSIGNED_INTEGER ) {
                  return ( rhs < 0 ) || ( lhs.unsafe_get_unsigned_integer() > static_cast< uint64_t >( rhs ) );
               }
               if ( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() > rhs;
               }
               return lhs.type() > type::SIGNED_INTEGER;
            }
         };

         template< typename T, typename U >
         struct totally_ordered< T, U, type::UNSIGNED_INTEGER >
            : operators::totally_ordered< T, U >
         {
            friend bool operator==( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p == rhs;
                  }
                  else {
                     return false;
                  }
               }
               if ( lhs.type() == type::SIGNED_INTEGER ) {
                  const auto v = lhs.unsafe_get_signed_integer();
                  return ( v >= 0 ) && ( static_cast< uint64_t >( v ) == rhs );
               }
               if ( lhs.type() == type::UNSIGNED_INTEGER ) {
                  return lhs.unsafe_get_unsigned_integer() == rhs;
               }
               if ( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() == rhs;
               }
               return false;
            }

            friend bool operator<( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p < rhs;
                  }
                  else {
                     return true;
                  }
               }
               if ( lhs.type() == type::SIGNED_INTEGER ) {
                  const auto v = lhs.unsafe_get_signed_integer();
                  return ( v < 0 ) || ( static_cast< uint64_t >( v ) < rhs );
               }
               if ( lhs.type() == type::UNSIGNED_INTEGER ) {
                  return lhs.unsafe_get_unsigned_integer() < rhs;
               }
               if ( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() < rhs;
               }
               return lhs.type() < type::SIGNED_INTEGER;
            }

            friend bool operator>( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p > rhs;
                  }
                  else {
                     return false;
                  }
               }
               if ( lhs.type() == type::SIGNED_INTEGER ) {
                  const auto v = lhs.unsafe_get_signed_integer();
                  return ( v >= 0 ) && ( static_cast< uint64_t >( v ) > rhs );
               }
               if ( lhs.type() == type::UNSIGNED_INTEGER ) {
                  return lhs.unsafe_get_unsigned_integer() > rhs;
               }
               if ( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() > rhs;
               }
               return lhs.type() > type::SIGNED_INTEGER;
            }
         };

         template< typename T, typename U >
         struct totally_ordered< T, U, type::DOUBLE >
            : operators::totally_ordered< T, U >
         {
            friend bool operator==( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p == rhs;
                  }
                  else {
                     return false;
                  }
               }
               if ( lhs.type() == type::SIGNED_INTEGER ) {
                  return lhs.unsafe_get_signed_integer() == rhs;
               }
               if ( lhs.type() == type::UNSIGNED_INTEGER ) {
                  return lhs.unsafe_get_unsigned_integer() == rhs;
               }
               if ( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() == rhs;
               }
               return false;
            }

            friend bool operator<( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p < rhs;
                  }
                  else {
                     return true;
                  }
               }
               if ( lhs.type() == type::SIGNED_INTEGER ) {
                  return lhs.unsafe_get_signed_integer() < rhs;
               }
               if ( lhs.type() == type::UNSIGNED_INTEGER ) {
                  return lhs.unsafe_get_unsigned_integer() < rhs;
               }
               if ( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() < rhs;
               }
               return lhs.type() < type::DOUBLE;
            }

            friend bool operator>( const T & lhs, const U & rhs ) noexcept
            {
               if ( lhs.type() == type::POINTER ) {
                  if ( const auto * p = lhs.unsafe_get_pointer() ) {
                     return * p > rhs;
                  }
                  else {
                     return false;
                  }
               }
               if ( lhs.type() == type::SIGNED_INTEGER ) {
                  return lhs.unsafe_get_signed_integer() > rhs;
               }
               if ( lhs.type() == type::UNSIGNED_INTEGER ) {
                  return lhs.unsafe_get_unsigned_integer() > rhs;
               }
               if ( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() > rhs;
               }
               return lhs.type() > type::DOUBLE;
            }
         };

      } // internal

   } // json

} // tao

#endif
