// Copyright (c) 2017-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_OPERATORS_HPP
#define TAO_JSON_OPERATORS_HPP

#include "basic_value.hpp"

#include "internal/format.hpp"

namespace tao
{
   namespace json
   {
      template< template< typename... > class TraitsL, template< typename... > class TraitsR >
      bool operator==( const basic_value< TraitsL >& lhs, const basic_value< TraitsR >& rhs ) noexcept
      {
         if( rhs.is_value_ptr() ) {
            return lhs == rhs.skip_value_ptr();
         }
         if( lhs.type() != rhs.type() ) {
            switch( lhs.type() ) {
               case type::VALUE_PTR:
                  return lhs.skip_value_ptr() == rhs;

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

            case type::VALUE_PTR:
               assert( lhs.type() != type::VALUE_PTR );
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

      template< template< typename... > class Traits, int = 1 >  // work-around for Visual C++
      bool operator==( const basic_value< Traits >& lhs, tao::internal::identity_t< basic_value< Traits > > rhs ) noexcept
      {
         return lhs == rhs;
      }

      template< template< typename... > class Traits, int = 2 >  // work-around for Visual C++
      bool operator==( tao::internal::identity_t< basic_value< Traits > > lhs, const basic_value< Traits >& rhs ) noexcept
      {
         return lhs == rhs;
      }

      template< template< typename... > class Traits, typename T >
      bool operator==( const basic_value< Traits >& lhs, const std::optional< T >& rhs ) noexcept
      {
         static_assert( noexcept( Traits< std::optional< T > >::equal( lhs, rhs ) ), "equal must be noexcept" );
         return Traits< std::optional< T > >::equal( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits >
      bool operator==( const std::optional< T >& lhs, const basic_value< Traits >& rhs ) noexcept
      {
         return rhs == lhs;
      }

      template< template< typename... > class Traits, typename T >
      auto operator==( const basic_value< Traits >& lhs, const T& rhs ) noexcept -> decltype( Traits< std::decay_t< T > >::equal( lhs, rhs ) )
      {
         using D = std::decay_t< T >;
         static_assert( noexcept( Traits< D >::equal( lhs, rhs ) ), "equal must be noexcept" );
         return Traits< D >::equal( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits >
      auto operator==( const T& lhs, const basic_value< Traits >& rhs ) noexcept -> decltype( Traits< std::decay_t< T > >::equal( rhs, lhs ) )
      {
         return rhs == lhs;
      }

      template< template< typename... > class TraitsL, template< typename... > class TraitsR >
      bool operator!=( const basic_value< TraitsL >& lhs, const basic_value< TraitsR >& rhs ) noexcept
      {
         return !( lhs == rhs );
      }

      template< template< typename... > class Traits, typename T >
      bool operator!=( const basic_value< Traits >& lhs, const std::optional< T >& rhs ) noexcept
      {
         return !( lhs == rhs );
      }

      template< typename T, template< typename... > class Traits >
      bool operator!=( const std::optional< T >& lhs, const basic_value< Traits >& rhs ) noexcept
      {
         return !( rhs == lhs );
      }

      template< template< typename... > class Traits, typename T >
      bool operator!=( const basic_value< Traits >& lhs, const T& rhs ) noexcept
      {
         return !( lhs == rhs );
      }

      template< typename T, template< typename... > class Traits >
      bool operator!=( const T& lhs, const basic_value< Traits >& rhs ) noexcept
      {
         return !( lhs == rhs );
      }

      template< template< typename... > class TraitsL, template< typename... > class TraitsR >
      bool operator<( const basic_value< TraitsL >& lhs, const basic_value< TraitsR >& rhs ) noexcept
      {
         if( rhs.is_value_ptr() ) {
            return lhs < rhs.skip_value_ptr();
         }
         if( lhs.type() != rhs.type() ) {
            switch( lhs.type() ) {
               case type::VALUE_PTR:
                  return lhs.skip_value_ptr() < rhs;

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

            case type::VALUE_PTR:
               assert( lhs.type() != type::VALUE_PTR );
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

      template< template< typename... > class Traits, int = 1 >  // work-around for Visual C++
      bool operator<( const basic_value< Traits >& lhs, tao::internal::identity_t< basic_value< Traits > > rhs ) noexcept
      {
         return lhs < rhs;
      }

      template< template< typename... > class Traits, int = 2 >  // work-around for Visual C++
      bool operator<( const tao::internal::identity_t< basic_value< Traits > > lhs, const basic_value< Traits >& rhs ) noexcept
      {
         return lhs < rhs;
      }

      template< template< typename... > class Traits, typename T >
      bool operator<( const basic_value< Traits >& lhs, const std::optional< T >& rhs ) noexcept
      {
         static_assert( noexcept( Traits< std::optional< T > >::less_than( lhs, rhs ) ), "less_than must be noexcept" );
         return Traits< std::optional< T > >::less_than( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits >
      bool operator<( const std::optional< T >& lhs, const basic_value< Traits >& rhs ) noexcept
      {
         static_assert( noexcept( Traits< std::optional< T > >::greater_than( rhs, lhs ) ), "greater_than must be noexcept" );
         return Traits< std::optional< T > >::greater_than( rhs, lhs );
      }

      template< template< typename... > class Traits, typename T >
      auto operator<( const basic_value< Traits >& lhs, const T& rhs ) noexcept -> decltype( Traits< std::decay_t< T > >::less_than( lhs, rhs ) )
      {
         using D = std::decay_t< T >;
         static_assert( noexcept( Traits< D >::less_than( lhs, rhs ) ), "less_than must be noexcept" );
         return Traits< D >::less_than( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits >
      auto operator<( const T& lhs, const basic_value< Traits >& rhs ) noexcept -> decltype( Traits< std::decay_t< T > >::greater_than( rhs, lhs ) )
      {
         using D = std::decay_t< T >;
         static_assert( noexcept( Traits< D >::greater_than( rhs, lhs ) ), "greater_than must be noexcept" );
         return Traits< D >::greater_than( rhs, lhs );
      }

      template< template< typename... > class TraitsL, template< typename... > class TraitsR >
      bool operator>( const basic_value< TraitsL >& lhs, const basic_value< TraitsR >& rhs ) noexcept
      {
         return rhs < lhs;
      }

      template< template< typename... > class Traits >
      bool operator>( const basic_value< Traits >& lhs, tao::internal::identity_t< basic_value< Traits > > rhs ) noexcept
      {
         return rhs < lhs;
      }

      template< template< typename... > class Traits >
      bool operator>( tao::internal::identity_t< basic_value< Traits > > lhs, const basic_value< Traits >& rhs ) noexcept
      {
         return rhs < lhs;
      }

      template< template< typename... > class Traits, typename T >
      bool operator>( const basic_value< Traits >& lhs, const std::optional< T >& rhs ) noexcept
      {
         static_assert( noexcept( Traits< std::optional< T > >::greater_than( lhs, rhs ) ), "greater_than must be noexcept" );
         return Traits< std::optional< T > >::greater_than( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits >
      bool operator>( const std::optional< T >& lhs, const basic_value< Traits >& rhs ) noexcept
      {
         static_assert( noexcept( Traits< std::optional< T > >::less_than( rhs, lhs ) ), "less_than must be noexcept" );
         return Traits< std::optional< T > >::less_than( rhs, lhs );
      }

      template< template< typename... > class Traits, typename T >
      auto operator>( const basic_value< Traits >& lhs, const T& rhs ) noexcept -> decltype( Traits< std::decay_t< T > >::greater_than( lhs, rhs ) )
      {
         using D = std::decay_t< T >;
         static_assert( noexcept( Traits< D >::greater_than( lhs, rhs ) ), "greater_than must be noexcept" );
         return Traits< D >::greater_than( lhs, rhs );
      }

      template< typename T, template< typename... > class Traits >
      auto operator>( const T& lhs, const basic_value< Traits >& rhs ) noexcept -> decltype( Traits< std::decay_t< T > >::less_than( rhs, lhs ) )
      {
         using D = std::decay_t< T >;
         static_assert( noexcept( Traits< D >::less_than( rhs, lhs ) ), "less_than must be noexcept" );
         return Traits< D >::less_than( rhs, lhs );
      }

      template< template< typename... > class TraitsL, template< typename... > class TraitsR >
      bool operator<=( const basic_value< TraitsL >& lhs, const basic_value< TraitsR >& rhs ) noexcept
      {
         return !( lhs > rhs );
      }

      template< template< typename... > class Traits, typename T >
      bool operator<=( const basic_value< Traits >& lhs, const std::optional< T >& rhs ) noexcept
      {
         return !( lhs > rhs );
      }

      template< typename T, template< typename... > class Traits >
      bool operator<=( const std::optional< T >& lhs, const basic_value< Traits >& rhs ) noexcept
      {
         return !( lhs > rhs );
      }

      template< template< typename... > class Traits, typename T >
      bool operator<=( const basic_value< Traits >& lhs, const T& rhs ) noexcept
      {
         return !( lhs > rhs );
      }

      template< typename T, template< typename... > class Traits >
      bool operator<=( const T& lhs, const basic_value< Traits >& rhs ) noexcept
      {
         return !( lhs > rhs );
      }

      template< template< typename... > class TraitsL, template< typename... > class TraitsR >
      bool operator>=( const basic_value< TraitsL >& lhs, const basic_value< TraitsR >& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

      template< template< typename... > class Traits, typename T >
      bool operator>=( const basic_value< Traits >& lhs, const std::optional< T >& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

      template< typename T, template< typename... > class Traits >
      bool operator>=( const std::optional< T >& lhs, const basic_value< Traits >& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

      template< template< typename... > class Traits, typename T >
      bool operator>=( const basic_value< Traits >& lhs, const T& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

      template< typename T, template< typename... > class Traits >
      bool operator>=( const T& lhs, const basic_value< Traits >& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

   }  // namespace json

}  // namespace tao

#endif
