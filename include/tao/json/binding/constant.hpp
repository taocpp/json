// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_CONSTANT_HPP
#define TAO_JSON_BINDING_CONSTANT_HPP

#include <cstdint>
#include <iomanip>
#include <string>

#include "../base_message_extension.hpp"
#include "../external/pegtl/internal/pegtl_string.hpp"
#include "../internal/escape.hpp"
#include "../internal/format.hpp"

#include "element.hpp"
#include "member.hpp"

namespace tao
{
   namespace json
   {
      namespace binding
      {
         template< bool V >
         struct element_b
         {
            template< typename C >
            static bool read( const C& /*unused*/ )
            {
               return V;
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static void as( const basic_value< Traits, Base >& v, C& /*unused*/ )
            {
               const auto t = v.template as< bool >();
               if( t != V ) {
                  throw std::runtime_error( json::internal::format( std::boolalpha, "boolean mismatch, expected ", V, " parsed ", t, json::base_message_extension( v.base() ) ) );  // NOLINT
               }
            }

            template< template< typename... > class Traits = traits, typename Consumer, typename C >
            static void produce( Consumer& consumer, const C& /*unused*/ )
            {
               consumer.boolean( V );
            }

            template< template< typename... > class Traits = traits, typename Producer, typename C >
            static void consume( Producer& parser, C& /*unused*/ )
            {
               const auto t = parser.boolean();
               if( t != V ) {
                  parser.throw_parse_error( json::internal::format( std::boolalpha, "boolean mismatch, expected ", V, " parsed ", t ) );
               }
            }
         };

         template< std::int64_t V >
         struct element_i
         {
            template< typename C >
            static std::int64_t read( const C& /*unused*/ )
            {
               return V;
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static void as( const basic_value< Traits, Base >& v, C& /*unused*/ )
            {
               const auto t = v.template as< std::int64_t >();
               if( t != V ) {
                  throw std::runtime_error( json::internal::format( "signed integer mismatch, expected ", V, " parsed ", t, json::base_message_extension( v.base() ) ) );  // NOLINT
               }
            }

            template< template< typename... > class Traits = traits, typename Consumer, typename C >
            static void produce( Consumer& consumer, const C& /*unused*/ )
            {
               consumer.number( V );
            }

            template< template< typename... > class Traits = traits, typename Producer, typename C >
            static void consume( Producer& parser, C& /*unused*/ )
            {
               const auto t = parser.number_signed();
               if( t != V ) {
                  parser.throw_parse_error( json::internal::format( "signed integer mismatch, expected ", V, " parsed ", t ) );
               }
            }
         };

         template< std::uint64_t V >
         struct element_u
         {
            template< typename C >
            static std::uint64_t read( const C& /*unused*/ )
            {
               return V;
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static void as( const basic_value< Traits, Base >& v, C& /*unused*/ )
            {
               const auto t = v.template as< std::uint64_t >();
               if( t != V ) {
                  throw std::runtime_error( json::internal::format( "unsigned integer mismatch, expected ", V, " parsed ", t, json::base_message_extension( v.base() ) ) );  // NOLINT
               }
            }

            template< template< typename... > class Traits = traits, typename Consumer, typename C >
            static void produce( Consumer& consumer, const C& /*unused*/ )
            {
               consumer.number( V );
            }

            template< template< typename... > class Traits = traits, typename Producer, typename C >
            static void consume( Producer& parser, C& /*unused*/ )
            {
               const auto t = parser.number_unsigned();
               if( t != V ) {
                  parser.throw_parse_error( json::internal::format( "unsigned integer mismatch, expected ", V, " parsed ", t ) );
               }
            }
         };

         template< char... Cs >
         using string = json_pegtl::string< Cs... >;

         template< typename S >
         struct element_s;

         template< char... Cs >
         struct element_s< json_pegtl::string< Cs... > >
         {
            template< typename C >
            static tao::string_view read( const C& /*unused*/ )
            {
               static const char s[] = { Cs..., 0 };
               return tao::string_view( s, sizeof...( Cs ) );
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static void as( const basic_value< Traits, Base >& v, C& /*unused*/ )
            {
               static const char s[] = { Cs..., 0 };
               const auto sv = v.template as< tao::string_view >();
               if( sv != tao::string_view( s, sizeof...( Cs ) ) ) {
                  throw std::runtime_error( json::internal::format( "string mismatch, expected \"", json::internal::escape( sv ), "\" parsed \"", json::internal::escape( tao::string_view( s, sizeof...( Cs ) ) ), '"', json::base_message_extension( v.base() ) ) );  // NOLINT
               }
            }

            template< template< typename... > class Traits = traits, typename Consumer, typename T >
            static void produce( Consumer& consumer, const T& /*unused*/ )
            {
               static const char s[] = { Cs..., 0 };
               consumer.string( tao::string_view( s, sizeof...( Cs ) ) );
            }

            template< template< typename... > class Traits = traits, typename Producer, typename T >
            static void consume( Producer& parser, T& /*unused*/ )
            {
               static const char s[] = { Cs..., 0 };
               const auto t = parser.string();
               if( t != s ) {
                  parser.throw_parse_error( json::internal::format( "string mismatch, expected \"", json::internal::escape( s ), "\" parsed \"", json::internal::escape( t ), '"' ) );
               }
            }
         };

         template< member_kind R, typename K, bool B >
         struct member_b
            : public element_b< B >,
              public member_key< R, K >
         {
         };

         template< member_kind R, typename K, std::int64_t V >
         struct member_i
            : public element_i< V >,
              public member_key< R, K >
         {
         };

         template< member_kind R, typename K, std::uint64_t V >
         struct member_u
            : public element_u< V >,
              public member_key< R, K >
         {
         };

         template< member_kind R, typename K, typename S >
         struct member_s
            : public element_s< S >,
              public member_key< R, K >
         {
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
