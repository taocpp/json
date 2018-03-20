// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_INTERNAL_NUMBER_TRAITS_HPP
#define TAO_JSON_INTERNAL_NUMBER_TRAITS_HPP

#include <cstdint>

#include "../forward.hpp"
#include "../type.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         struct number_trait
         {
            template< template< typename... > class Traits, typename Base >
            static T as( const basic_value< Traits, Base >& v )
            {
               switch( v.type() ) {
                  case type::SIGNED:
                     return static_cast< T >( v.unsafe_get_signed() );
                     break;
                  case type::UNSIGNED:
                     return static_cast< T >( v.unsafe_get_unsigned() );
                     break;
                  case type::DOUBLE:
                     return static_cast< T >( v.unsafe_get_double() );
                     break;
                  default:
                     v.throw_invalid_json_type();
               }
               std::abort();  // LCOV_EXCL_LINE
            }
         };

         template< typename T >
         struct signed_trait
            : number_trait< T >
         {
            template< template< typename... > class, typename Parts >
            static T consume( Parts& parser )
            {
               return static_cast< T >( parser.number_signed() );
            }

            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& c, const T i )
            {
               c.number( std::int64_t( i ) );
            }

            template< template< typename... > class Traits, typename Base >
            static void assign( basic_value< Traits, Base >& v, const T i ) noexcept
            {
               v.unsafe_assign_signed( i );
            }

            template< template< typename... > class Traits, typename Base >
            static bool equal( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_value_ptr();
               switch( p->type() ) {
                  case type::SIGNED:
                     return p->unsafe_get_signed() == rhs;
                  case type::UNSIGNED:
                     return ( rhs >= 0 ) && ( p->unsafe_get_unsigned() == static_cast< std::uint64_t >( rhs ) );
                  case type::DOUBLE:
                     return p->unsafe_get_double() == rhs;
                  default:
                     return false;
               }
            }

            template< template< typename... > class Traits, typename Base >
            static bool less_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_value_ptr();
               switch( p->type() ) {
                  case type::SIGNED:
                     return p->unsafe_get_signed() < rhs;
                  case type::UNSIGNED:
                     return ( rhs >= 0 ) && ( p->unsafe_get_unsigned() < static_cast< std::uint64_t >( rhs ) );
                  case type::DOUBLE:
                     return p->unsafe_get_double() < rhs;
                  default:
                     return p->type() < type::SIGNED;
               }
            }

            template< template< typename... > class Traits, typename Base >
            static bool greater_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_value_ptr();
               switch( p->type() ) {
                  case type::SIGNED:
                     return p->unsafe_get_signed() > rhs;
                  case type::UNSIGNED:
                     return ( rhs < 0 ) || ( p->unsafe_get_unsigned() > static_cast< std::uint64_t >( rhs ) );
                  case type::DOUBLE:
                     return p->unsafe_get_double() > rhs;
                  default:
                     return p->type() > type::SIGNED;
               }
            }
         };

         template< typename T >
         struct unsigned_trait
            : number_trait< T >
         {
            template< template< typename... > class, typename Parts >
            static T consume( Parts& parser )
            {
               return static_cast< T >( parser.number_unsigned() );
            }

            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& c, const T i )
            {
               c.number( std::uint64_t( i ) );
            }

            template< template< typename... > class Traits, typename Base >
            static void assign( basic_value< Traits, Base >& v, const T i ) noexcept
            {
               v.unsafe_assign_unsigned( i );
            }

            template< template< typename... > class Traits, typename Base >
            static bool equal( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_value_ptr();
               switch( p->type() ) {
                  case type::SIGNED: {
                     const auto v = p->unsafe_get_signed();
                     return ( v >= 0 ) && ( static_cast< std::uint64_t >( v ) == rhs );
                  }
                  case type::UNSIGNED:
                     return p->unsafe_get_unsigned() == rhs;
                  case type::DOUBLE:
                     return p->unsafe_get_double() == rhs;
                  default:
                     return false;
               }
            }

            template< template< typename... > class Traits, typename Base >
            static bool less_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_value_ptr();
               switch( p->type() ) {
                  case type::SIGNED: {
                     const auto v = p->unsafe_get_signed();
                     return ( v < 0 ) || ( static_cast< std::uint64_t >( v ) < rhs );
                  }
                  case type::UNSIGNED:
                     return p->unsafe_get_unsigned() < rhs;
                  case type::DOUBLE:
                     return p->unsafe_get_double() < rhs;
                  default:
                     return p->type() < type::UNSIGNED;
               }
            }

            template< template< typename... > class Traits, typename Base >
            static bool greater_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_value_ptr();
               switch( p->type() ) {
                  case type::SIGNED: {
                     const auto v = p->unsafe_get_signed();
                     return ( v >= 0 ) && ( static_cast< std::uint64_t >( v ) > rhs );
                  }
                  case type::UNSIGNED:
                     return p->unsafe_get_unsigned() > rhs;
                  case type::DOUBLE:
                     return p->unsafe_get_double() > rhs;
                  default:
                     return p->type() > type::UNSIGNED;
               }
            }
         };

         template< typename T >
         struct float_trait
            : number_trait< T >
         {
            template< template< typename... > class, typename Parts >
            static T consume( Parts& parser )
            {
               return static_cast< T >( parser.number_double() );
            }

            template< template< typename... > class Traits, typename Consumer >
            static void produce( Consumer& c, const T f )
            {
               c.number( double( f ) );
            }

            template< template< typename... > class Traits, typename Base >
            static void assign( basic_value< Traits, Base >& v, const T f ) noexcept
            {
               v.unsafe_assign_double( f );
            }

            template< template< typename... > class Traits, typename Base >
            static bool equal( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_value_ptr();
               switch( p->type() ) {
                  case type::SIGNED:
                     return p->unsafe_get_signed() == rhs;
                  case type::UNSIGNED:
                     return p->unsafe_get_unsigned() == rhs;
                  case type::DOUBLE:
                     return p->unsafe_get_double() == rhs;
                  default:
                     return false;
               }
            }

            template< template< typename... > class Traits, typename Base >
            static bool less_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_value_ptr();
               switch( p->type() ) {
                  case type::SIGNED:
                     return p->unsafe_get_signed() < rhs;
                  case type::UNSIGNED:
                     return p->unsafe_get_unsigned() < rhs;
                  case type::DOUBLE:
                     return p->unsafe_get_double() < rhs;
                  default:
                     return p->type() < type::DOUBLE;
               }
            }

            template< template< typename... > class Traits, typename Base >
            static bool greater_than( const basic_value< Traits, Base >& lhs, const T rhs ) noexcept
            {
               const auto* p = lhs.skip_value_ptr();
               switch( p->type() ) {
                  case type::SIGNED:
                     return p->unsafe_get_signed() > rhs;
                  case type::UNSIGNED:
                     return p->unsafe_get_unsigned() > rhs;
                  case type::DOUBLE:
                     return p->unsafe_get_double() > rhs;
                  default:
                     return p->type() > type::DOUBLE;
               }
            }
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
