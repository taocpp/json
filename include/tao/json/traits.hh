// Copyright (c) 2016 Dr. Colin Hirsch
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_TRAITS_HH
#define TAOCPP_JSON_INCLUDE_TRAITS_HH

#include "value.hh"

namespace tao
{
  namespace json
  {
     template<>
     struct traits< std::nullptr_t >
     {
        static void assign( value & v, std::nullptr_t )
        {
           v.m_type = json::type::NULL_;
        }
     };

     template<>
     struct traits< bool >
     {
        static void assign( value & v, const bool b )
        {
           v.m_union.b = b;
           v.m_type = json::type::BOOL_;
        }
     };

     template<>
     struct traits< signed char >
     {
        static void assign( value & v, const signed char i )
        {
           v.m_union.i = i;
           v.m_type = json::type::INT64;
        }
     };

     template<>
     struct traits< unsigned char >
     {
        static void assign( value & v, const unsigned char i )
        {
           v.m_union.i = i;
           v.m_type = json::type::INT64;
        }
     };

     template<>
     struct traits< signed short >
     {
        static void assign( value & v, const signed short i )
        {
           v.m_union.i = i;
           v.m_type = json::type::INT64;
        }
     };

     template<>
     struct traits< unsigned short >
     {
        static void assign( value & v, const unsigned short i )
        {
           v.m_union.i = i;
           v.m_type = json::type::INT64;
        }
     };

     template<>
     struct traits< signed int >
     {
        static void assign( value & v, const signed int i )
        {
           v.m_union.i = i;
           v.m_type = json::type::INT64;
        }
     };

     template<>
     struct traits< unsigned int >
     {
        static void assign( value & v, const unsigned int i )
        {
           v.m_union.i = i;
           v.m_type = json::type::INT64;
        }
     };

     template<>
     struct traits< signed long >
     {
        static void assign( value & v, const signed long i )
        {
           v.m_union.i = i;
           v.m_type = json::type::INT64;
        }
     };

     template<>
     struct traits< signed long long >
     {
        static void assign( value & v, const signed long long i )
        {
           v.m_union.i = i;
           v.m_type = json::type::INT64;
        }
     };

     template<>
     struct traits< double >
     {
        static void assign( value & v, const double d )
        {
          if ( ! std::isfinite( d ) ) {
             throw std::runtime_error( "non-finite double value illegal for json" );
          }
           v.m_union.d = d;
           v.m_type = json::type::DOUBLE;
        }
     };

     template<>
     struct traits< empty_array_t >
     {
        static void assign( value & v, empty_array_t )
        {
           new ( & v.m_union.a ) std::vector< value >();
           v.m_type = json::type::ARRAY;
        }
     };

     template<>
     struct traits< empty_object_t >
     {
        static void assign( value & v, empty_object_t )
        {
           new ( & v.m_union.o ) std::map< std::string, value >();
           v.m_type = json::type::OBJECT;
        }
     };

     template<>
     struct traits< std::string >
     {
        template< typename T >
        static void assign( value & v, T && s )
        {
           new ( & v.m_union.s ) std::string( std::forward< T >( s ) );
           v.m_type = json::type::STRING;
        }
     };

     template<>
     struct traits< const char * >
     {
        static void assign( value & v, const char * s )
        {
           new ( & v.m_union.s ) std::string( s );
           v.m_type = json::type::STRING;
        }
     };

     template<>
     struct traits< std::vector< value > >
     {
        template< typename T >
        static void assign( value & v, T && a )
        {
           v.unsafe_emplace_array( std::forward< T >( a ) );
        }
     };

     template<>
     struct traits< std::map< std::string, value > >
     {
        template< typename T >
        static void assign( value & v, T && o )
        {
           v.unsafe_emplace_object( std::forward< T >( o ) );
        }
     };

     template<>
     struct traits< std::reference_wrapper< const value > >
     {
        template< typename T >
        static void assign( value & v, T && r )
        {
           v.m_union.p = &r;
           v.m_type = json::type::REFERENCE;
        }
     };

   } // json

} // tao

#endif // TAOCPP_JSON_INCLUDE_TRAITS_HH
