// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_TOTALLY_ORDERED_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_TOTALLY_ORDERED_HPP

#include <cstdint>

#include "../external/operators.hpp"

#include "../type.hpp"

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
            friend bool operator==( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p == rhs;
                  }
                  else {
                     return false;
                  }
               }
               return ( lhs.type() == E ) && ( lhs.T::template get< E >() == rhs );
            }

            friend bool operator<( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p < rhs;
                  }
                  else {
                     return true;
                  }
               }
               return ( lhs.type() < E ) || ( ( lhs.type() == E ) && ( lhs.T::template get< E >() < rhs ) );
            }

            friend bool operator>( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p > rhs;
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
            friend bool operator==( const T& lhs, null_t ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p == null;
                  }
                  else {
                     return true;
                  }
               }
               return lhs.type() == type::NULL_;
            }

            friend bool operator<( const T&, null_t ) noexcept
            {
               return false;
            }

            friend bool operator>( const T& lhs, null_t ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p > null;
                  }
                  else {
                     return false;
                  }
               }
               return lhs.type() > type::NULL_;
            }
         };

         template< typename T, typename U >
         struct totally_ordered< T, U, type::SIGNED >
            : operators::totally_ordered< T, U >
         {
            friend bool operator==( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p == rhs;
                  }
                  else {
                     return false;
                  }
               }
               if( lhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_signed() == rhs;
               }
               if( lhs.type() == type::UNSIGNED ) {
                  return ( rhs >= 0 ) && ( lhs.unsafe_get_unsigned() == static_cast< std::uint64_t >( rhs ) );
               }
               if( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() == rhs;
               }
               return false;
            }

            friend bool operator<( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p < rhs;
                  }
                  else {
                     return true;
                  }
               }
               if( lhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_signed() < rhs;
               }
               if( lhs.type() == type::UNSIGNED ) {
                  return ( rhs >= 0 ) && ( lhs.unsafe_get_unsigned() < static_cast< std::uint64_t >( rhs ) );
               }
               if( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() < rhs;
               }
               return lhs.type() < type::SIGNED;
            }

            friend bool operator>( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p > rhs;
                  }
                  else {
                     return false;
                  }
               }
               if( lhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_signed() > rhs;
               }
               if( lhs.type() == type::UNSIGNED ) {
                  return ( rhs < 0 ) || ( lhs.unsafe_get_unsigned() > static_cast< std::uint64_t >( rhs ) );
               }
               if( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() > rhs;
               }
               return lhs.type() > type::SIGNED;
            }
         };

         template< typename T, typename U >
         struct totally_ordered< T, U, type::UNSIGNED >
            : operators::totally_ordered< T, U >
         {
            friend bool operator==( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p == rhs;
                  }
                  else {
                     return false;
                  }
               }
               if( lhs.type() == type::SIGNED ) {
                  const auto v = lhs.unsafe_get_signed();
                  return ( v >= 0 ) && ( static_cast< std::uint64_t >( v ) == rhs );
               }
               if( lhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_unsigned() == rhs;
               }
               if( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() == rhs;
               }
               return false;
            }

            friend bool operator<( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p < rhs;
                  }
                  else {
                     return true;
                  }
               }
               if( lhs.type() == type::SIGNED ) {
                  const auto v = lhs.unsafe_get_signed();
                  return ( v < 0 ) || ( static_cast< std::uint64_t >( v ) < rhs );
               }
               if( lhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_unsigned() < rhs;
               }
               if( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() < rhs;
               }
               return lhs.type() < type::SIGNED;
            }

            friend bool operator>( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p > rhs;
                  }
                  else {
                     return false;
                  }
               }
               if( lhs.type() == type::SIGNED ) {
                  const auto v = lhs.unsafe_get_signed();
                  return ( v >= 0 ) && ( static_cast< std::uint64_t >( v ) > rhs );
               }
               if( lhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_unsigned() > rhs;
               }
               if( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() > rhs;
               }
               return lhs.type() > type::SIGNED;
            }
         };

         template< typename T, typename U >
         struct totally_ordered< T, U, type::DOUBLE >
            : operators::totally_ordered< T, U >
         {
            friend bool operator==( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p == rhs;
                  }
                  else {
                     return false;
                  }
               }
               if( lhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_signed() == rhs;
               }
               if( lhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_unsigned() == rhs;
               }
               if( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() == rhs;
               }
               return false;
            }

            friend bool operator<( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p < rhs;
                  }
                  else {
                     return true;
                  }
               }
               if( lhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_signed() < rhs;
               }
               if( lhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_unsigned() < rhs;
               }
               if( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() < rhs;
               }
               return lhs.type() < type::DOUBLE;
            }

            friend bool operator>( const T& lhs, const U& rhs ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p > rhs;
                  }
                  else {
                     return false;
                  }
               }
               if( lhs.type() == type::SIGNED ) {
                  return lhs.unsafe_get_signed() > rhs;
               }
               if( lhs.type() == type::UNSIGNED ) {
                  return lhs.unsafe_get_unsigned() > rhs;
               }
               if( lhs.type() == type::DOUBLE ) {
                  return lhs.unsafe_get_double() > rhs;
               }
               return lhs.type() > type::DOUBLE;
            }
         };

         template< typename T >
         struct totally_ordered< T, empty_array_t, type::ARRAY >
            : operators::totally_ordered< T, empty_array_t >
         {
            friend bool operator==( const T& lhs, empty_array_t rhs ) noexcept
            {
               return lhs == T( rhs );
            }

            friend bool operator<( const T& lhs, empty_array_t rhs ) noexcept
            {
               return lhs < T( rhs );
            }

            friend bool operator>( const T& lhs, empty_array_t rhs ) noexcept
            {
               return lhs > T( rhs );
            }
         };

         template< typename T >
         struct totally_ordered< T, empty_object_t, type::OBJECT >
            : operators::totally_ordered< T, empty_object_t >
         {
            friend bool operator==( const T& lhs, empty_object_t rhs ) noexcept
            {
               return lhs == T( rhs );
            }

            friend bool operator<( const T& lhs, empty_object_t rhs ) noexcept
            {
               return lhs < T( rhs );
            }

            friend bool operator>( const T& lhs, empty_object_t rhs ) noexcept
            {
               return lhs > T( rhs );
            }
         };

         template< typename T >
         struct totally_ordered< T, const T*, type::RAW_PTR >
            : operators::totally_ordered< T, const T* >
         {
            friend bool operator==( const T& lhs, const T* rhs ) noexcept
            {
               if( rhs == nullptr ) {
                  return lhs == null;
               }
               return lhs == *rhs;
            }

            friend bool operator<( const T& lhs, const T* rhs ) noexcept
            {
               if( rhs == nullptr ) {
                  return lhs < null;
               }
               return lhs < *rhs;
            }

            friend bool operator>( const T& lhs, const T* rhs ) noexcept
            {
               if( rhs == nullptr ) {
                  return lhs > null;
               }
               return lhs > *rhs;
            }
         };

         template< typename T >
         struct totally_ordered< T, T*, type::RAW_PTR >
            : operators::totally_ordered< T, T* >
         {
            friend bool operator==( const T& lhs, T* rhs ) noexcept
            {
               if( rhs == nullptr ) {
                  return lhs == null;
               }
               return lhs == *rhs;
            }

            friend bool operator<( const T& lhs, T* rhs ) noexcept
            {
               if( rhs == nullptr ) {
                  return lhs < null;
               }
               return lhs < *rhs;
            }

            friend bool operator>( const T& lhs, T* rhs ) noexcept
            {
               if( rhs == nullptr ) {
                  return lhs > null;
               }
               return lhs > *rhs;
            }
         };

         template< typename T >
         struct totally_ordered< T, std::nullptr_t, type::RAW_PTR >
            : operators::totally_ordered< T, std::nullptr_t >
         {
            friend bool operator==( const T& lhs, std::nullptr_t ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p == null;
                  }
                  else {
                     return true;
                  }
               }
               return lhs.is_null();
            }

            friend bool operator<( const T&, std::nullptr_t ) noexcept
            {
               return false;
            }

            friend bool operator>( const T& lhs, std::nullptr_t ) noexcept
            {
               if( lhs.type() == type::RAW_PTR ) {
                  if( const auto* p = lhs.unsafe_get_raw_ptr() ) {
                     return *p > null;
                  }
                  else {
                     return false;
                  }
               }
               return !lhs.is_null();
            }
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
