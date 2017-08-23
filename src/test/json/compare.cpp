// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/from_string.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      template< typename T, typename U >
      void test_lt( const T& a, const U& b )
      {
         TEST_ASSERT( a == a );
         TEST_ASSERT( !( a != a ) );
         TEST_ASSERT( !( a < a ) );
         TEST_ASSERT( !( a > a ) );
         TEST_ASSERT( a <= a );
         TEST_ASSERT( a >= a );

         TEST_ASSERT( !( a == b ) );
         TEST_ASSERT( !( b == a ) );
         TEST_ASSERT( a != b );
         TEST_ASSERT( b != a );

         TEST_ASSERT( a < b );
         TEST_ASSERT( b > a );
         TEST_ASSERT( !( a > b ) );
         TEST_ASSERT( !( b < a ) );

         TEST_ASSERT( a <= b );
         TEST_ASSERT( b >= a );
         TEST_ASSERT( !( a >= b ) );
         TEST_ASSERT( !( b <= a ) );
      }

      template< typename T >
      void test_lt( const std::nullptr_t& a, const T& b )
      {
         TEST_ASSERT( !( a == b ) );
         TEST_ASSERT( !( b == a ) );
         TEST_ASSERT( a != b );
         TEST_ASSERT( b != a );

         TEST_ASSERT( a < b );
         TEST_ASSERT( b > a );
         TEST_ASSERT( !( a > b ) );
         TEST_ASSERT( !( b < a ) );

         TEST_ASSERT( a <= b );
         TEST_ASSERT( b >= a );
         TEST_ASSERT( !( a >= b ) );
         TEST_ASSERT( !( b <= a ) );
      }

      void test_null()
      {
         const custom_value a = null;
         const custom_value b( null );
         const custom_value* p = nullptr;

         const custom_value pa( &a );
         const custom_value pb( &b );
         const custom_value pz( nullptr );

         TEST_ASSERT( a.type() == type::NULL_ );
         TEST_ASSERT( b.type() == type::NULL_ );

         TEST_ASSERT( a == a );
         TEST_ASSERT( a == b );
         TEST_ASSERT( a == p );
         TEST_ASSERT( a == pa );
         TEST_ASSERT( a == pb );
         TEST_ASSERT( a == pz );
         TEST_ASSERT( a == null );
         TEST_ASSERT( a == nullptr );

         TEST_ASSERT( pa == a );
         TEST_ASSERT( pa == b );
         TEST_ASSERT( pa == p );
         TEST_ASSERT( pa == pa );
         TEST_ASSERT( pa == pb );
         TEST_ASSERT( pa == pz );
         TEST_ASSERT( pa == null );
         TEST_ASSERT( pa == nullptr );

         TEST_ASSERT( pz == a );
         TEST_ASSERT( pz == b );
         TEST_ASSERT( pz == p );
         TEST_ASSERT( pz == pa );
         TEST_ASSERT( pz == pb );
         TEST_ASSERT( pz == pz );
         TEST_ASSERT( pz == null );
         TEST_ASSERT( pz == nullptr );

         TEST_ASSERT( p == a );
         TEST_ASSERT( p == b );
         TEST_ASSERT( p == pa );
         TEST_ASSERT( p == pb );
         TEST_ASSERT( p == pz );

         TEST_ASSERT( !( a < a ) );
         TEST_ASSERT( !( a < b ) );
         TEST_ASSERT( !( a < p ) );
         TEST_ASSERT( !( a < pa ) );
         TEST_ASSERT( !( a < pb ) );
         TEST_ASSERT( !( a < pz ) );
         TEST_ASSERT( !( a < null ) );
         TEST_ASSERT( !( a < nullptr ) );

         TEST_ASSERT( !( pa < a ) );
         TEST_ASSERT( !( pa < b ) );
         TEST_ASSERT( !( pa < p ) );
         TEST_ASSERT( !( pa < pa ) );
         TEST_ASSERT( !( pa < pb ) );
         TEST_ASSERT( !( pa < pz ) );
         TEST_ASSERT( !( pa < null ) );
         TEST_ASSERT( !( pa < nullptr ) );

         TEST_ASSERT( !( pz < a ) );
         TEST_ASSERT( !( pz < b ) );
         TEST_ASSERT( !( pz < p ) );
         TEST_ASSERT( !( pz < pa ) );
         TEST_ASSERT( !( pz < pb ) );
         TEST_ASSERT( !( pz < pz ) );
         TEST_ASSERT( !( pz < null ) );
         TEST_ASSERT( !( pz < nullptr ) );

         TEST_ASSERT( !( p < a ) );
         TEST_ASSERT( !( p < b ) );
         TEST_ASSERT( !( p < pa ) );
         TEST_ASSERT( !( p < pb ) );
         TEST_ASSERT( !( p < pz ) );

         TEST_ASSERT( !( a > a ) );
         TEST_ASSERT( !( a > b ) );
         TEST_ASSERT( !( a > p ) );
         TEST_ASSERT( !( a > pa ) );
         TEST_ASSERT( !( a > pb ) );
         TEST_ASSERT( !( a > pz ) );
         TEST_ASSERT( !( a > null ) );
         TEST_ASSERT( !( a > nullptr ) );

         TEST_ASSERT( !( pa > a ) );
         TEST_ASSERT( !( pa > b ) );
         TEST_ASSERT( !( pa > p ) );
         TEST_ASSERT( !( pa > pa ) );
         TEST_ASSERT( !( pa > pb ) );
         TEST_ASSERT( !( pa > pz ) );
         TEST_ASSERT( !( pa > null ) );
         TEST_ASSERT( !( pa > nullptr ) );

         TEST_ASSERT( !( pz > a ) );
         TEST_ASSERT( !( pz > b ) );
         TEST_ASSERT( !( pz > p ) );
         TEST_ASSERT( !( pz > pa ) );
         TEST_ASSERT( !( pz > pb ) );
         TEST_ASSERT( !( pz > pz ) );
         TEST_ASSERT( !( pz > null ) );
         TEST_ASSERT( !( pz > nullptr ) );

         TEST_ASSERT( !( p > a ) );
         TEST_ASSERT( !( p > b ) );
         TEST_ASSERT( !( p > pa ) );
         TEST_ASSERT( !( p > pb ) );
         TEST_ASSERT( !( p > pz ) );

         TEST_ASSERT( a <= a );
         TEST_ASSERT( a <= b );
         TEST_ASSERT( a <= p );
         TEST_ASSERT( a <= pa );
         TEST_ASSERT( a <= pb );
         TEST_ASSERT( a <= pz );
         TEST_ASSERT( a <= null );
         TEST_ASSERT( a <= nullptr );

         TEST_ASSERT( pa <= a );
         TEST_ASSERT( pa <= b );
         TEST_ASSERT( pa <= p );
         TEST_ASSERT( pa <= pa );
         TEST_ASSERT( pa <= pb );
         TEST_ASSERT( pa <= pz );
         TEST_ASSERT( pa <= null );
         TEST_ASSERT( pa <= nullptr );

         TEST_ASSERT( pz <= a );
         TEST_ASSERT( pz <= b );
         TEST_ASSERT( pz <= p );
         TEST_ASSERT( pz <= pa );
         TEST_ASSERT( pz <= pb );
         TEST_ASSERT( pz <= pz );
         TEST_ASSERT( pz <= null );
         TEST_ASSERT( pz <= nullptr );

         TEST_ASSERT( p <= a );
         TEST_ASSERT( p <= b );
         TEST_ASSERT( p <= pa );
         TEST_ASSERT( p <= pb );
         TEST_ASSERT( p <= pz );

         TEST_ASSERT( a >= a );
         TEST_ASSERT( a >= b );
         TEST_ASSERT( a >= p );
         TEST_ASSERT( a >= pa );
         TEST_ASSERT( a >= pb );
         TEST_ASSERT( a >= pz );
         TEST_ASSERT( a >= null );
         TEST_ASSERT( a >= nullptr );

         TEST_ASSERT( pa >= a );
         TEST_ASSERT( pa >= b );
         TEST_ASSERT( pa >= p );
         TEST_ASSERT( pa >= pa );
         TEST_ASSERT( pa >= pb );
         TEST_ASSERT( pa >= pz );
         TEST_ASSERT( pa >= null );
         TEST_ASSERT( pa >= nullptr );

         TEST_ASSERT( pz >= a );
         TEST_ASSERT( pz >= b );
         TEST_ASSERT( pz >= p );
         TEST_ASSERT( pz >= pa );
         TEST_ASSERT( pz >= pb );
         TEST_ASSERT( pz >= pz );
         TEST_ASSERT( pz >= null );
         TEST_ASSERT( pz >= nullptr );

         TEST_ASSERT( p >= a );
         TEST_ASSERT( p >= b );
         TEST_ASSERT( p >= pa );
         TEST_ASSERT( p >= pb );
         TEST_ASSERT( p >= pz );
      }

      void test_bool()
      {
         const custom_value t( true );
         const custom_value f( false );

         const custom_value pt( &t );
         const custom_value pf( &f );

         TEST_ASSERT( t.type() == type::BOOLEAN );
         TEST_ASSERT( f.type() == type::BOOLEAN );

         test_lt( f, t );
         test_lt( f, pt );
         test_lt( f, true );

         test_lt( pf, t );
         test_lt( pf, pt );
         test_lt( pf, true );
      }

      template< typename N >
      void test_number()
      {
         const custom_value a( N( 42 ) );
         const custom_value b( N( 43 ) );

         const custom_value pa( &a );
         const custom_value pb( &b );

         test_lt( a, b );
         test_lt( pa, b );
         test_lt( a, pb );
         test_lt( pa, pb );

         test_lt( N( 42 ), b );
         test_lt( 42, b );
         test_lt( 42u, b );
         test_lt( 42.0, b );
         test_lt( custom_value( 42 ), b );
         test_lt( custom_value( 42u ), b );
         test_lt( custom_value( 42.0 ), b );

         test_lt( N( 42 ), pb );
         test_lt( 42, pb );
         test_lt( 42u, pb );
         test_lt( 42.0, pb );
         test_lt( custom_value( 42 ), pb );
         test_lt( custom_value( 42u ), pb );
         test_lt( custom_value( 42.0 ), pb );

         test_lt( a, N( 43 ) );
         test_lt( a, 43 );
         test_lt( a, 43u );
         test_lt( a, 43.0 );
         test_lt( a, custom_value( 43 ) );
         test_lt( a, custom_value( 43u ) );
         test_lt( a, custom_value( 43.0 ) );

         test_lt( pa, N( 43 ) );
         test_lt( pa, 43 );
         test_lt( pa, 43u );
         test_lt( pa, 43.0 );
         test_lt( pa, custom_value( 43 ) );
         test_lt( pa, custom_value( 43u ) );
         test_lt( pa, custom_value( 43.0 ) );
      }

      void test_string()
      {
         const custom_value a( "bar" );
         const custom_value b( "foo" );

         const custom_value pa( &a );
         const custom_value pb( &b );

         TEST_ASSERT( a.type() == type::STRING );
         TEST_ASSERT( b.type() == type::STRING );

         test_lt( a, b );
         test_lt( a, pb );
         test_lt( a, "foo" );
         test_lt( a, std::string( "foo" ) );

         test_lt( pa, b );
         test_lt( pa, pb );
         test_lt( pa, "foo" );
         test_lt( pa, std::string( "foo" ) );
      }

      void test_array()
      {
         const custom_value a = from_string( "[ 1, 2 ]" );
         const custom_value b = from_string( "[ 1, 3 ]" );
         const custom_value c = from_string( "[ 2, 1 ]" );

         TEST_ASSERT( a.type() == type::ARRAY );
         TEST_ASSERT( b.type() == type::ARRAY );
         TEST_ASSERT( c.type() == type::ARRAY );

         test_lt( a, b );
         test_lt( a, c );
         test_lt( b, c );
      }

      void test_object()
      {
         const custom_value a = from_string( "{ \"a\" : null }" );
         const custom_value b = from_string( "{ \"a\" : true }" );
         const custom_value c = from_string( "{ \"c\" : null }" );
         const custom_value d = from_string( "{ \"d\" : 1, \"e\" : 2 }" );
         const custom_value e = from_string( "{ \"d\" : 1, \"e\" : 3 }" );
         const custom_value f = from_string( "{ \"d\" : 2, \"e\" : 3 }" );
         const custom_value g = from_string( "{ \"z\" : 2, \"e\" : 3 }" );

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
         const custom_value n = from_string( "null" );
         const custom_value b = from_string( "true" );
         const custom_value i = from_string( "-42" );
         const custom_value u = from_string( "42" );
         const custom_value d = from_string( "43.0" );
         const custom_value s = from_string( "\"string\"" );
         const custom_value a = from_string( "[]" );
         const custom_value o = from_string( "{}" );

         const custom_value pz = nullptr;
         const custom_value pn = &n;
         const custom_value pb = &b;
         const custom_value pi = &i;
         const custom_value pu = &u;
         const custom_value pd = &d;
         const custom_value ps = &s;
         const custom_value pa = &a;
         const custom_value po = &o;

         TEST_ASSERT( n.type() == type::NULL_ );
         TEST_ASSERT( b.type() == type::BOOLEAN );
         TEST_ASSERT( i.type() == type::SIGNED );
         TEST_ASSERT( u.type() == type::UNSIGNED );
         TEST_ASSERT( d.type() == type::DOUBLE );
         TEST_ASSERT( s.type() == type::STRING );
         TEST_ASSERT( a.type() == type::ARRAY );
         TEST_ASSERT( o.type() == type::OBJECT );

         TEST_ASSERT( pz.type() == type::RAW_PTR );
         TEST_ASSERT( pn.type() == type::RAW_PTR );
         TEST_ASSERT( pb.type() == type::RAW_PTR );
         TEST_ASSERT( pi.type() == type::RAW_PTR );
         TEST_ASSERT( pu.type() == type::RAW_PTR );
         TEST_ASSERT( pd.type() == type::RAW_PTR );
         TEST_ASSERT( ps.type() == type::RAW_PTR );
         TEST_ASSERT( pa.type() == type::RAW_PTR );
         TEST_ASSERT( po.type() == type::RAW_PTR );

         test_lt( pz, b );
         test_lt( pz, i );
         test_lt( pz, u );
         test_lt( pz, d );
         test_lt( pz, s );
         test_lt( pz, a );
         test_lt( pz, o );

         test_lt( pz, pb );
         test_lt( pz, pi );
         test_lt( pz, pu );
         test_lt( pz, pd );
         test_lt( pz, ps );
         test_lt( pz, pa );
         test_lt( pz, po );

         test_lt( n, b );
         test_lt( n, i );
         test_lt( n, u );
         test_lt( n, d );
         test_lt( n, s );
         test_lt( n, a );
         test_lt( n, o );

         test_lt( n, pb );
         test_lt( n, pi );
         test_lt( n, pu );
         test_lt( n, pd );
         test_lt( n, ps );
         test_lt( n, pa );
         test_lt( n, po );

         test_lt( pn, pb );
         test_lt( pn, pi );
         test_lt( pn, pu );
         test_lt( pn, pd );
         test_lt( pn, ps );
         test_lt( pn, pa );
         test_lt( pn, po );

         test_lt( nullptr, b );
         test_lt( nullptr, i );
         test_lt( nullptr, u );
         test_lt( nullptr, d );
         test_lt( nullptr, s );
         test_lt( nullptr, a );
         test_lt( nullptr, o );

         test_lt( nullptr, pb );
         test_lt( nullptr, pi );
         test_lt( nullptr, pu );
         test_lt( nullptr, pd );
         test_lt( nullptr, ps );
         test_lt( nullptr, pa );
         test_lt( nullptr, po );

         test_lt( &n, b );
         test_lt( &n, i );
         test_lt( &n, u );
         test_lt( &n, d );
         test_lt( &n, s );
         test_lt( &n, a );
         test_lt( &n, o );

         test_lt( &n, pb );
         test_lt( &n, pi );
         test_lt( &n, pu );
         test_lt( &n, pd );
         test_lt( &n, ps );
         test_lt( &n, pa );
         test_lt( &n, po );

         test_lt( n, &b );
         test_lt( n, &i );
         test_lt( n, &u );
         test_lt( n, &d );
         test_lt( n, &s );
         test_lt( n, &a );
         test_lt( n, &o );

         test_lt( n, &pb );
         test_lt( n, &pi );
         test_lt( n, &pu );
         test_lt( n, &pd );
         test_lt( n, &ps );
         test_lt( n, &pa );
         test_lt( n, &po );

         test_lt( pn, b );
         test_lt( pn, i );
         test_lt( pn, u );
         test_lt( pn, d );
         test_lt( pn, s );
         test_lt( pn, a );
         test_lt( pn, o );

         test_lt( pn, pb );
         test_lt( pn, pi );
         test_lt( pn, pu );
         test_lt( pn, pd );
         test_lt( pn, ps );
         test_lt( pn, pa );
         test_lt( pn, po );

         test_lt( pn, &pb );
         test_lt( pn, &pi );
         test_lt( pn, &pu );
         test_lt( pn, &pd );
         test_lt( pn, &ps );
         test_lt( pn, &pa );
         test_lt( pn, &po );

         test_lt( n, true );
         test_lt( n, -42 );
         test_lt( n, 42u );
         test_lt( n, 43.0 );
         test_lt( n, "string" );
         test_lt( n, std::string( "string" ) );
         test_lt( n, empty_array );
         test_lt( n, empty_object );
         test_lt( n, &u );

         test_lt( pn, true );
         test_lt( pn, -42 );
         test_lt( pn, 42u );
         test_lt( pn, 43.0 );
         test_lt( pn, "string" );
         test_lt( pn, std::string( "string" ) );
         test_lt( pn, empty_array );
         test_lt( pn, empty_object );
         test_lt( pn, &u );

         test_lt( pz, true );
         test_lt( pz, -42 );
         test_lt( pz, 42u );
         test_lt( pz, 43.0 );
         test_lt( pz, "string" );
         test_lt( pz, std::string( "string" ) );
         test_lt( pz, empty_array );
         test_lt( pz, empty_object );
         test_lt( pz, &u );

         test_lt( b, i );
         test_lt( b, u );
         test_lt( b, d );
         test_lt( b, s );
         test_lt( b, a );
         test_lt( b, o );

         test_lt( b, pi );
         test_lt( b, pu );
         test_lt( b, pd );
         test_lt( b, ps );
         test_lt( b, pa );
         test_lt( b, po );

         test_lt( &b, i );
         test_lt( &b, u );
         test_lt( &b, d );
         test_lt( &b, s );
         test_lt( &b, a );
         test_lt( &b, o );

         test_lt( b, &i );
         test_lt( b, &u );
         test_lt( b, &d );
         test_lt( b, &s );
         test_lt( b, &a );
         test_lt( b, &o );

         test_lt( i, s );
         test_lt( i, a );
         test_lt( i, o );

         test_lt( i, ps );
         test_lt( i, pa );
         test_lt( i, po );

         test_lt( pi, s );
         test_lt( pi, a );
         test_lt( pi, o );

         test_lt( pi, ps );
         test_lt( pi, pa );
         test_lt( pi, po );

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

   }  // namespace json

}  // namespace tao

#include "main.hpp"
