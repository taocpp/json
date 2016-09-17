// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_TEE_HH
#define TAOCPP_JSON_INCLUDE_SAX_TEE_HH

#include <utility>
#include <cstdint>
#include <string>
#include <type_traits>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         struct strip_reference_wrapper
         {
            using type = T;
         };

         template< typename T >
         struct strip_reference_wrapper< std::reference_wrapper< T > >
         {
            using type = T &;
         };

         template< typename T >
         using decay_and_strip_t = typename strip_reference_wrapper< typename std::decay< T >::type >::type;

      } // internal

      namespace sax
      {
         // SAX consumer that forwards to two nested consumers
         // TODO: generalize to multiple consumers?
         template< typename T1, typename T2 >
         class tee
         {
         private:
            T1 t1;
            T2 t2;

         public:
            template< typename U1, typename U2 >
            tee( U1 && u1, U2 && u2 )
              : t1( std::forward< U1 >( u1 ) ),
                t2( std::forward< U2 >( u2 ) )
            { }

            void null()
            {
               t1.null();
               t2.null();
            }

            void boolean( const bool v )
            {
               t1.boolean( v );
               t2.boolean( v );
            }

            void number( const std::int64_t v )
            {
               t1.number( v );
               t2.number( v );
            }

            void number( const std::uint64_t v )
            {
               t1.number( v );
               t2.number( v );
            }

            void number( const double v )
            {
               t1.number( v );
               t2.number( v );
            }

            void string( const std::string & v )
            {
               t1.string( v );
               t2.string( v );
            }

            void string( std::string && v )
            {
               t1.string( v );
               t2.string( std::move( v ) );
            }

            // array
            void begin_array()
            {
               t1.begin_array();
               t2.begin_array();
            }

            void element()
            {
               t1.element();
               t2.element();
            }

            void end_array()
            {
               t1.end_array();
               t2.end_array();
            }

            // object
            void begin_object()
            {
               t1.begin_object();
               t2.begin_object();
            }

            void key( const std::string & v )
            {
               t1.key( v );
               t2.key( v );
            }

            void key( std::string && v )
            {
               t1.key( v );
               t2.key( std::move( v ) );
            }

            void member()
            {
               t1.member();
               t2.member();
            }

            void end_object()
            {
               t1.end_object();
               t2.end_object();
            }
         };

         // make_tee
         template< typename ... T >
         tee< internal::decay_and_strip_t< T >... > make_tee( T && ... t )
         {
            return tee< internal::decay_and_strip_t< T > ... >( std::forward< T >( t ) ... );
         }

         // tie_tee
         template< typename ... T >
         tee< T & ... > tie_tee( T & ... t )
         {
            return tee< T & ... >( t ... );
         }

      } // sax

   } // json

} // tao

#endif
