// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_OPERATORS_HPP
#define TAO_JSON_OPERATORS_HPP

#include "basic_value.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class TraitsL, typename BaseL, template< typename... > class TraitsR, typename BaseR >
      bool operator==( const basic_value< TraitsL, BaseL >& lhs, const basic_value< TraitsR, BaseR >& rhs ) noexcept
      {
         if( rhs.is_raw_ptr() ) {
            return lhs == *rhs.skip_raw_ptr();
         }
         if( lhs.type() != rhs.type() ) {
            switch( lhs.type() ) {
               case type::RAW_PTR:
                  return *lhs.skip_raw_ptr() == rhs;

               case type::SIGNED:
                  if( rhs.type() == type::UNSIGNED ) {
                     const auto v = lhs.unsafe_get_signed();
                     return ( v >= 0 ) && ( static_cast< std::uint64_t >( v ) == rhs.unsafe_get_unsigned() );
                  }
                  if( rhs.type() == type::DOUBLE ) {
                     return lhs.unsafe_get_signed() == rhs.unsafe_get_double();
                  }
                  break;

               case type::UNSIGNED:
                  if( rhs.type() == type::SIGNED ) {
                     const auto v = rhs.unsafe_get_signed();
                     return ( v >= 0 ) && ( lhs.unsafe_get_unsigned() == static_cast< std::uint64_t >( v ) );
                  }
                  if( rhs.type() == type::DOUBLE ) {
                     return lhs.unsafe_get_unsigned() == rhs.unsafe_get_double();
                  }
                  break;

               case type::DOUBLE:
                  if( rhs.type() == type::SIGNED ) {
                     return lhs.unsafe_get_double() == rhs.unsafe_get_signed();
                  }
                  if( rhs.type() == type::UNSIGNED ) {
                     return lhs.unsafe_get_double() == rhs.unsafe_get_unsigned();
                  }
                  break;

               case type::STRING:
                  if( rhs.type() == type::STRING_VIEW ) {
                     return lhs.unsafe_get_string() == rhs.unsafe_get_string_view();
                  }
                  break;

               case type::STRING_VIEW:
                  if( rhs.type() == type::STRING ) {
                     return lhs.unsafe_get_string_view() == rhs.unsafe_get_string();
                  }
                  break;

               case type::BINARY:
                  if( rhs.type() == type::BINARY_VIEW ) {
                     return lhs.unsafe_get_binary() == rhs.unsafe_get_binary_view();
                  }
                  break;

               case type::BINARY_VIEW:
                  if( rhs.type() == type::BINARY ) {
                     return lhs.unsafe_get_binary_view() == rhs.unsafe_get_binary();
                  }
                  break;

               case type::DISCARDED:
                  assert( lhs.type() != type::DISCARDED );
                  break;

               case type::DESTROYED:
                  assert( lhs.type() != type::DESTROYED );
                  break;

               case type::OPAQUE_PTR:
                  assert( lhs.type() != type::OPAQUE_PTR );
                  break;

               default:
                  break;
            }
            assert( rhs.type() != type::DISCARDED );
            assert( rhs.type() != type::DESTROYED );
            assert( rhs.type() != type::OPAQUE_PTR );
            return false;
         }

         switch( lhs.type() ) {
            case type::UNINITIALIZED:
               return true;

            // LCOV_EXCL_START
            case type::DISCARDED:
               assert( lhs.type() != type::DISCARDED );
               break;

            case type::DESTROYED:
               assert( lhs.type() != type::DESTROYED );
               break;
               // LCOV_EXCL_STOP

            case type::NULL_:
               return true;

            case type::BOOLEAN:
               return lhs.unsafe_get_boolean() == rhs.unsafe_get_boolean();

            case type::SIGNED:
               return lhs.unsafe_get_signed() == rhs.unsafe_get_signed();

            case type::UNSIGNED:
               return lhs.unsafe_get_unsigned() == rhs.unsafe_get_unsigned();

            case type::DOUBLE:
               return lhs.unsafe_get_double() == rhs.unsafe_get_double();

            case type::STRING:
               return lhs.unsafe_get_string() == rhs.unsafe_get_string();

            case type::STRING_VIEW:
               return lhs.unsafe_get_string_view() == rhs.unsafe_get_string_view();

            case type::BINARY:
               return lhs.unsafe_get_binary() == rhs.unsafe_get_binary();

            case type::BINARY_VIEW:
               return lhs.unsafe_get_binary_view() == rhs.unsafe_get_binary_view();

            case type::ARRAY:
               return lhs.unsafe_get_array() == rhs.unsafe_get_array();

            case type::OBJECT:
               return lhs.unsafe_get_object() == rhs.unsafe_get_object();

            case type::RAW_PTR:
               assert( lhs.type() != type::RAW_PTR );
               break;  // LCOV_EXCL_LINE

            case type::OPAQUE_PTR:
               assert( lhs.type() != type::OPAQUE_PTR );
               break;  // LCOV_EXCL_LINE
         }
         // LCOV_EXCL_START
         assert( false );
         return false;
         // LCOV_EXCL_STOP
      }

      template< template< typename... > class Traits, typename Base, int = 1 >  // work-around for Visual C++
      bool operator==( const basic_value< Traits, Base >& lhs, tao::internal::identity_t< basic_value< Traits, Base > > rhs ) noexcept
      {
         return lhs == rhs;
      }

      template< template< typename... > class Traits, typename Base, int = 2 >  // work-around for Visual C++
      bool operator==( tao::internal::identity_t< basic_value< Traits, Base > > lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         return lhs == rhs;
      }

      template< template< typename... > class Traits, typename Base, typename T >
      bool operator==( const basic_value< Traits, Base >& lhs, const tao::optional< T >& rhs ) noexcept
      {
         static_assert( noexcept( Traits< tao::optional< T > >::equal( lhs, rhs ) ), "equal must be noexcept" );
         return Traits< tao::optional< T > >::equal( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base >
      bool operator==( const tao::optional< T >& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         return rhs == lhs;
      }

      template< template< typename... > class Traits, typename Base, typename T, typename = decltype( Traits< typename std::decay< T >::type >::equal( std::declval< const basic_value< Traits, Base >& >(), std::declval< const T& >() ) ) >
      bool operator==( const basic_value< Traits, Base >& lhs, const T& rhs ) noexcept
      {
         using D = typename std::decay< T >::type;
         static_assert( noexcept( Traits< D >::equal( lhs, rhs ) ), "equal must be noexcept" );
         return Traits< D >::equal( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base, typename = decltype( Traits< typename std::decay< T >::type >::equal( std::declval< const basic_value< Traits, Base >& >(), std::declval< const T& >() ) ) >
      bool operator==( const T& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         return rhs == lhs;
      }

      template< template< typename... > class TraitsL, typename BaseL, template< typename... > class TraitsR, typename BaseR >
      bool operator!=( const basic_value< TraitsL, BaseL >& lhs, const basic_value< TraitsR, BaseR >& rhs ) noexcept
      {
         return !( lhs == rhs );
      }

      template< template< typename... > class Traits, typename Base, typename T >
      bool operator!=( const basic_value< Traits, Base >& lhs, const tao::optional< T >& rhs ) noexcept
      {
         return !( lhs == rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base >
      bool operator!=( const tao::optional< T >& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         return !( rhs == lhs );
      }

      template< template< typename... > class Traits, typename Base, typename T >
      bool operator!=( const basic_value< Traits, Base >& lhs, const T& rhs ) noexcept
      {
         return !( lhs == rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base >
      bool operator!=( const T& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         return !( lhs == rhs );
      }

      template< template< typename... > class TraitsL, typename BaseL, template< typename... > class TraitsR, typename BaseR >
      bool operator<( const basic_value< TraitsL, BaseL >& lhs, const basic_value< TraitsR, BaseR >& rhs ) noexcept
      {
         if( rhs.is_raw_ptr() ) {
            return lhs < *rhs.skip_raw_ptr();
         }
         if( lhs.type() != rhs.type() ) {
            switch( lhs.type() ) {
               case type::RAW_PTR:
                  return *lhs.skip_raw_ptr() < rhs;

               case type::SIGNED:
                  if( rhs.type() == type::UNSIGNED ) {
                     const auto v = lhs.unsafe_get_signed();
                     return ( v < 0 ) || ( static_cast< std::uint64_t >( v ) < rhs.unsafe_get_unsigned() );
                  }
                  if( rhs.type() == type::DOUBLE ) {
                     return lhs.unsafe_get_signed() < rhs.unsafe_get_double();
                  }
                  break;

               case type::UNSIGNED:
                  if( rhs.type() == type::SIGNED ) {
                     const auto v = rhs.unsafe_get_signed();
                     return ( v >= 0 ) && ( lhs.unsafe_get_unsigned() < static_cast< std::uint64_t >( v ) );
                  }
                  if( rhs.type() == type::DOUBLE ) {
                     return lhs.unsafe_get_unsigned() < rhs.unsafe_get_double();
                  }
                  break;

               case type::DOUBLE:
                  if( rhs.type() == type::SIGNED ) {
                     return lhs.unsafe_get_double() < rhs.unsafe_get_signed();
                  }
                  if( rhs.type() == type::UNSIGNED ) {
                     return lhs.unsafe_get_double() < rhs.unsafe_get_unsigned();
                  }
                  break;

               case type::STRING:
                  if( rhs.type() == type::STRING_VIEW ) {
                     return lhs.unsafe_get_string() < rhs.unsafe_get_string_view();
                  }
                  break;

               case type::STRING_VIEW:
                  if( rhs.type() == type::STRING ) {
                     return lhs.unsafe_get_string_view() < rhs.unsafe_get_string();
                  }
                  break;

               case type::BINARY:
                  if( rhs.type() == type::BINARY_VIEW ) {
                     return lhs.unsafe_get_binary() < rhs.unsafe_get_binary_view();
                  }
                  break;

               case type::BINARY_VIEW:
                  if( rhs.type() == type::BINARY ) {
                     return lhs.unsafe_get_binary_view() < rhs.unsafe_get_binary();
                  }
                  break;

               case type::DISCARDED:
                  assert( lhs.type() != type::DISCARDED );
                  break;

               case type::DESTROYED:
                  assert( lhs.type() != type::DESTROYED );
                  break;

               case type::OPAQUE_PTR:
                  assert( lhs.type() != type::OPAQUE_PTR );
                  break;

               default:
                  break;
            }
            assert( rhs.type() != type::DISCARDED );
            assert( rhs.type() != type::DESTROYED );
            assert( rhs.type() != type::OPAQUE_PTR );
            return lhs.type() < rhs.type();
         }

         switch( lhs.type() ) {
            case type::UNINITIALIZED:
               return false;

            // LCOV_EXCL_START
            case type::DISCARDED:
               assert( lhs.type() != type::DISCARDED );
               break;

            case type::DESTROYED:
               assert( lhs.type() != type::DESTROYED );
               break;
               // LCOV_EXCL_STOP

            case type::NULL_:
               return false;

            case type::BOOLEAN:
               return lhs.unsafe_get_boolean() < rhs.unsafe_get_boolean();

            case type::SIGNED:
               return lhs.unsafe_get_signed() < rhs.unsafe_get_signed();

            case type::UNSIGNED:
               return lhs.unsafe_get_unsigned() < rhs.unsafe_get_unsigned();

            case type::DOUBLE:
               return lhs.unsafe_get_double() < rhs.unsafe_get_double();

            case type::STRING:
               return lhs.unsafe_get_string() < rhs.unsafe_get_string();

            case type::STRING_VIEW:
               return lhs.unsafe_get_string_view() < rhs.unsafe_get_string_view();

            case type::BINARY:
               return lhs.unsafe_get_binary() < rhs.unsafe_get_binary();

            case type::BINARY_VIEW:
               return lhs.unsafe_get_binary_view() < rhs.unsafe_get_binary_view();

            case type::ARRAY:
               return lhs.unsafe_get_array() < rhs.unsafe_get_array();

            case type::OBJECT:
               return lhs.unsafe_get_object() < rhs.unsafe_get_object();

            case type::RAW_PTR:
               assert( lhs.type() != type::RAW_PTR );
               break;  // LCOV_EXCL_LINE

            case type::OPAQUE_PTR:
               assert( lhs.type() != type::OPAQUE_PTR );
               break;  // LCOV_EXCL_LINE
         }
         // LCOV_EXCL_START
         assert( false );
         return false;
         // LCOV_EXCL_STOP
      }

      template< template< typename... > class Traits, typename Base, int = 1 >  // work-around for Visual C++
      bool operator<( const basic_value< Traits, Base >& lhs, tao::internal::identity_t< basic_value< Traits, Base > > rhs ) noexcept
      {
         return lhs < rhs;
      }

      template< template< typename... > class Traits, typename Base, int = 2 >  // work-around for Visual C++
      bool operator<( const tao::internal::identity_t< basic_value< Traits, Base > > lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         return lhs < rhs;
      }

      template< template< typename... > class Traits, typename Base, typename T >
      bool operator<( const basic_value< Traits, Base >& lhs, const tao::optional< T >& rhs ) noexcept
      {
         static_assert( noexcept( Traits< tao::optional< T > >::less_than( lhs, rhs ) ), "less_than must be noexcept" );
         return Traits< tao::optional< T > >::less_than( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base >
      bool operator<( const tao::optional< T >& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         static_assert( noexcept( Traits< tao::optional< T > >::greater_than( rhs, lhs ) ), "greater_than must be noexcept" );
         return Traits< tao::optional< T > >::greater_than( rhs, lhs );
      }

      template< template< typename... > class Traits, typename Base, typename T, typename = decltype( Traits< typename std::decay< T >::type >::less_than( std::declval< const basic_value< Traits, Base >& >(), std::declval< const T& >() ) ) >
      bool operator<( const basic_value< Traits, Base >& lhs, const T& rhs ) noexcept
      {
         using D = typename std::decay< T >::type;
         static_assert( noexcept( Traits< D >::less_than( lhs, rhs ) ), "less_than must be noexcept" );
         return Traits< D >::less_than( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base, typename = decltype( Traits< typename std::decay< T >::type >::greater_than( std::declval< const basic_value< Traits, Base >& >(), std::declval< const T& >() ) ) >
      bool operator<( const T& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         using D = typename std::decay< T >::type;
         static_assert( noexcept( Traits< D >::greater_than( rhs, lhs ) ), "greater_than must be noexcept" );
         return Traits< D >::greater_than( rhs, lhs );
      }

      template< template< typename... > class TraitsL, typename BaseL, template< typename... > class TraitsR, typename BaseR >
      bool operator>( const basic_value< TraitsL, BaseL >& lhs, const basic_value< TraitsR, BaseR >& rhs ) noexcept
      {
         return rhs < lhs;
      }

      template< template< typename... > class Traits, typename Base >
      bool operator>( const basic_value< Traits, Base >& lhs, tao::internal::identity_t< basic_value< Traits, Base > > rhs ) noexcept
      {
         return rhs < lhs;
      }

      template< template< typename... > class Traits, typename Base >
      bool operator>( tao::internal::identity_t< basic_value< Traits, Base > > lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         return rhs < lhs;
      }

      template< template< typename... > class Traits, typename Base, typename T >
      bool operator>( const basic_value< Traits, Base >& lhs, const tao::optional< T >& rhs ) noexcept
      {
         static_assert( noexcept( Traits< tao::optional< T > >::greater_than( lhs, rhs ) ), "greater_than must be noexcept" );
         return Traits< tao::optional< T > >::greater_than( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base >
      bool operator>( const tao::optional< T >& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         static_assert( noexcept( Traits< tao::optional< T > >::less_than( rhs, lhs ) ), "less_than must be noexcept" );
         return Traits< tao::optional< T > >::less_than( rhs, lhs );
      }

      template< template< typename... > class Traits, typename Base, typename T, typename = decltype( Traits< typename std::decay< T >::type >::greater_than( std::declval< const basic_value< Traits, Base >& >(), std::declval< const T& >() ) ) >
      bool operator>( const basic_value< Traits, Base >& lhs, const T& rhs ) noexcept
      {
         using D = typename std::decay< T >::type;
         static_assert( noexcept( Traits< D >::greater_than( lhs, rhs ) ), "greater_than must be noexcept" );
         return Traits< D >::greater_than( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base, typename = decltype( Traits< typename std::decay< T >::type >::less_than( std::declval< const basic_value< Traits, Base >& >(), std::declval< const T& >() ) ) >
      bool operator>( const T& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         using D = typename std::decay< T >::type;
         static_assert( noexcept( Traits< D >::less_than( rhs, lhs ) ), "less_than must be noexcept" );
         return Traits< D >::less_than( rhs, lhs );
      }

      template< template< typename... > class TraitsL, typename BaseL, template< typename... > class TraitsR, typename BaseR >
      bool operator<=( const basic_value< TraitsL, BaseL >& lhs, const basic_value< TraitsR, BaseR >& rhs ) noexcept
      {
         return !( lhs > rhs );
      }

      template< template< typename... > class Traits, typename Base, typename T >
      bool operator<=( const basic_value< Traits, Base >& lhs, const tao::optional< T >& rhs ) noexcept
      {
         return !( lhs > rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base >
      bool operator<=( const tao::optional< T >& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         return !( lhs > rhs );
      }

      template< template< typename... > class Traits, typename Base, typename T >
      bool operator<=( const basic_value< Traits, Base >& lhs, const T& rhs ) noexcept
      {
         return !( lhs > rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base >
      bool operator<=( const T& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         return !( lhs > rhs );
      }

      template< template< typename... > class TraitsL, typename BaseL, template< typename... > class TraitsR, typename BaseR >
      bool operator>=( const basic_value< TraitsL, BaseL >& lhs, const basic_value< TraitsR, BaseR >& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

      template< template< typename... > class Traits, typename Base, typename T >
      bool operator>=( const basic_value< Traits, Base >& lhs, const tao::optional< T >& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base >
      bool operator>=( const tao::optional< T >& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

      template< template< typename... > class Traits, typename Base, typename T >
      bool operator>=( const basic_value< Traits, Base >& lhs, const T& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

      template< typename T, template< typename... > class Traits, typename Base >
      bool operator>=( const T& lhs, const basic_value< Traits, Base >& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

      template< template< typename... > class Traits, typename Base >
      basic_value< Traits, Base >& operator+=( basic_value< Traits, Base >& v, std::initializer_list< internal::pair< Traits, Base > >&& l )
      {
         v.insert( std::move( l ) );
         return v;
      }

      template< template< typename... > class Traits, typename Base >
      basic_value< Traits, Base >& operator+=( basic_value< Traits, Base >& v, const std::initializer_list< internal::pair< Traits, Base > >& l )
      {
         v.insert( l );
         return v;
      }

      template< template< typename... > class Traits, typename Base >
      basic_value< Traits, Base >& operator-=( basic_value< Traits, Base >& v, std::initializer_list< std::string > l )
      {
         auto& o = v.get_object();
         for( const auto& k : l ) {
            if( o.erase( k ) == 0 ) {
               throw std::runtime_error( "JSON object key not found: " + k );  // NOLINT
            }
         }
         return v;
      }

   }  // namespace json

}  // namespace tao

#endif
