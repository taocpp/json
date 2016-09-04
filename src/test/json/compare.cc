// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

namespace tao
{
   namespace json
   {
      template< typename T, typename U >
      void test_lt( const T & a, const U & b )
      {
         TEST_ASSERT( a == a );
         TEST_ASSERT( ! ( a != a ) );
         TEST_ASSERT( ! ( a < a ) );
         TEST_ASSERT( ! ( a > a ) );
         TEST_ASSERT( a <= a );
         TEST_ASSERT( a >= a );

         TEST_ASSERT( b == b );
         TEST_ASSERT( ! ( b != b ) );
         TEST_ASSERT( ! ( b < b ) );
         TEST_ASSERT( ! ( b > b ) );
         TEST_ASSERT( b <= b );
         TEST_ASSERT( b >= b );

         TEST_ASSERT( ! ( a == b ) );
         TEST_ASSERT( ! ( b == a ) );
         TEST_ASSERT( a != b );
         TEST_ASSERT( b != a );

         TEST_ASSERT( a < b );
         TEST_ASSERT( b > a );
         TEST_ASSERT( ! ( a > b ) );
         TEST_ASSERT( ! ( b < a ) );

         TEST_ASSERT( a <= b );
         TEST_ASSERT( b >= a );
         TEST_ASSERT( ! ( a >= b ) );
         TEST_ASSERT( ! ( b <= a ) );
      }

      void test_null()
      {
         const value a;
         const value b( null );

         TEST_ASSERT( a.type() == type::NULL_ );
         TEST_ASSERT( b.type() == type::NULL_ );

         TEST_ASSERT( a == b );
      }

      void test_bool()
      {
         const value t( true );
         const value f( false );

         TEST_ASSERT( t.type() == type::BOOL );
         TEST_ASSERT( f.type() == type::BOOL );

         test_lt( f, t );
         test_lt( f, true );
      }

      template< typename N >
      void test_number()
      {
         const value a( N( 42 ) );
         const value b( N( 43 ) );

         test_lt( a, b );

         test_lt( N( 42 ), b );
         test_lt( 42, b );
         test_lt( 42u, b );

         test_lt( a, N( 43 ) );
         test_lt( a, 43 );
         test_lt( a, 43u );
      }

      void test_string()
      {
         const value a( "bar" );
         const value b( "foo" );

         TEST_ASSERT( a.type() == type::STRING );
         TEST_ASSERT( b.type() == type::STRING );

         test_lt( a, b );
         test_lt( a, "foo" );
         test_lt( a, std::string( "foo" ) );
      }

      void test_array()
      {
         const value a = from_string( "[ 1, 2 ]" );
         const value b = from_string( "[ 1, 3 ]" );
         const value c = from_string( "[ 2, 1 ]" );

         TEST_ASSERT( a.type() == type::ARRAY );
         TEST_ASSERT( b.type() == type::ARRAY );
         TEST_ASSERT( c.type() == type::ARRAY );

         test_lt( a, b );
         test_lt( a, c );
         test_lt( b, c );
      }

      void test_object()
      {
         const value a = from_string( "{ \"a\" : null }" );
         const value b = from_string( "{ \"a\" : true }" );
         const value c = from_string( "{ \"c\" : null }" );
         const value d = from_string( "{ \"d\" : 1, \"e\" : 2 }" );
         const value e = from_string( "{ \"d\" : 1, \"e\" : 3 }" );
         const value f = from_string( "{ \"d\" : 2, \"e\" : 3 }" );
         const value g = from_string( "{ \"z\" : 2, \"e\" : 3 }" );

         TEST_ASSERT( a.type() == type::OBJECT );
         TEST_ASSERT( b.type() == type::OBJECT );
         TEST_ASSERT( c.type() == type::OBJECT );
         TEST_ASSERT( d.type() == type::OBJECT );
         TEST_ASSERT( e.type() == type::OBJECT );
         TEST_ASSERT( f.type() == type::OBJECT );
         TEST_ASSERT( g.type() == type::OBJECT );

         test_lt( a, b );
         test_lt( a, c );
         test_lt( a, d );
         test_lt( a, e );
         test_lt( a, f );
         test_lt( a, g );
         test_lt( b, c );
         test_lt( b, d );
         test_lt( b, e );
         test_lt( b, f );
         test_lt( b, g );
         test_lt( c, d );
         test_lt( c, e );
         test_lt( c, f );
         test_lt( c, g );
         test_lt( d, e );
         test_lt( d, f );
         test_lt( d, g );
         test_lt( e, f );
         test_lt( e, g );
         test_lt( f, g );
      }

      void test_mixed()
      {
         const value n;
         const value b = from_string( "true" );
         const value i = from_string( "-42" );
         const value u = from_string( "42" );
         const value d = from_string( "43.0" );
         const value s = from_string( "\"string\"" );
         const value a = from_string( "[]" );
         const value o = from_string( "{}" );
         const value p = &u;
         const value pn = nullptr;

         TEST_ASSERT( n.type() == type::NULL_ );
         TEST_ASSERT( b.type() == type::BOOL );
         TEST_ASSERT( i.type() == type::SIGNED );
         TEST_ASSERT( u.type() == type::UNSIGNED );
         TEST_ASSERT( d.type() == type::DOUBLE );
         TEST_ASSERT( s.type() == type::STRING );
         TEST_ASSERT( a.type() == type::ARRAY );
         TEST_ASSERT( o.type() == type::OBJECT );
         TEST_ASSERT( p.type() == type::POINTER );
         TEST_ASSERT( pn.type() == type::POINTER );

         test_lt( n, b );
         test_lt( n, i );
         test_lt( n, u );
         test_lt( n, d );
         test_lt( n, s );
         test_lt( n, a );
         test_lt( n, o );
         test_lt( n, p );

         test_lt( nullptr, b );
         test_lt( nullptr, i );
         test_lt( nullptr, u );
         test_lt( nullptr, d );
         test_lt( nullptr, s );
         test_lt( nullptr, a );
         test_lt( nullptr, o );
         test_lt( nullptr, p );

         test_lt( &n, b );
         test_lt( &n, i );
         test_lt( &n, u );
         test_lt( &n, d );
         test_lt( &n, s );
         test_lt( &n, a );
         test_lt( &n, o );
         test_lt( &n, p );

         test_lt( n, &b );
         test_lt( n, &i );
         test_lt( n, &u );
         test_lt( n, &d );
         test_lt( n, &s );
         test_lt( n, &a );
         test_lt( n, &o );
         test_lt( n, &p );

         test_lt( pn, b );
         test_lt( pn, i );
         test_lt( pn, u );
         test_lt( pn, d );
         test_lt( pn, s );
         test_lt( pn, a );
         test_lt( pn, o );
         test_lt( pn, p );

         test_lt( n, true );
         test_lt( n, -42 );
         test_lt( n, 42u );
         test_lt( n, 43.0 );
         test_lt( n, "string" );
         test_lt( n, std::string( "string" ) );
         test_lt( n, empty_array );
         test_lt( n, empty_object );
         test_lt( n, &u );

         test_lt( b, i );
         test_lt( b, u );
         test_lt( b, d );
         test_lt( b, s );
         test_lt( b, a );
         test_lt( b, o );
         test_lt( b, p );

         test_lt( &b, i );
         test_lt( &b, u );
         test_lt( &b, d );
         test_lt( &b, s );
         test_lt( &b, a );
         test_lt( &b, o );
         test_lt( &b, p );

         test_lt( b, &i );
         test_lt( b, &u );
         test_lt( b, &d );
         test_lt( b, &s );
         test_lt( b, &a );
         test_lt( b, &o );
         test_lt( b, &p );

         test_lt( i, s );
         test_lt( i, a );
         test_lt( i, o );

         test_lt( &i, s );
         test_lt( &i, a );
         test_lt( &i, o );

         test_lt( i, &s );
         test_lt( i, &a );
         test_lt( i, &o );

         test_lt( u, s );
         test_lt( u, a );
         test_lt( u, o );

         test_lt( &u, s );
         test_lt( &u, a );
         test_lt( &u, o );

         test_lt( u, &s );
         test_lt( u, &a );
         test_lt( u, &o );

         test_lt( s, a );
         test_lt( s, o );

         test_lt( &s, a );
         test_lt( &s, o );

         test_lt( s, &a );
         test_lt( s, &o );

         test_lt( a, o );

         test_lt( &a, o );

         test_lt( a, &o );
      }

      void unit_test()
      {
         test_null();
         test_bool();
         test_number< signed char >();
         test_number< unsigned char >();
         test_number< signed short >();
         test_number< unsigned short >();
         test_number< signed int >();
         test_number< unsigned int >();
         test_number< signed long >();
         test_number< signed long long >();
         test_number< int64_t >();
         test_number< double >();
         test_string();
         test_array();
         test_object();
         test_mixed();
      }

   } // json

} // tao

#include "main.hh"
