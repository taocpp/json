// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/from_string.hpp>
#include <tao/json/self_contained.hpp>
#include <tao/json/to_stream.hpp>
#include <tao/json/to_string.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      struct point
      {
         double x = 1.0;
         double y = 2.0;
      };

      template< template< typename... > class Traits, typename Consumer >
      struct array_t
      {
         Consumer* c_;

         explicit array_t( Consumer& c ) noexcept
            : c_( &c )
         {
            c_->begin_array();
         }

         array_t( const array_t& ) = delete;

         array_t( array_t&& r ) noexcept
            : c_( r.c_ )
         {
            r.c_ = nullptr;
         }

         ~array_t()
         {
            if( c_ )
               c_->end_array();
         }

         void operator=( const array_t& ) = delete;
         void operator=( array_t&& ) = delete;

         template< typename T >
         array_t& push_back( const T& v )
         {
            assert( c_ );
            Traits< T >::template produce< Traits >( *c_, v );
            c_->element();
            return *this;
         }
      };

      template< template< typename... > class Traits, typename Consumer >
      struct object_t
      {
         Consumer* c_;

         explicit object_t( Consumer& c ) noexcept
            : c_( &c )
         {
            c_->begin_object();
         }

         object_t( const object_t& ) = delete;

         object_t( object_t&& r ) noexcept
            : c_( r.c_ )
         {
            r.c_ = nullptr;
         }

         ~object_t()
         {
            if( c_ )
               c_->end_object();
         }

         void operator=( const object_t& ) = delete;
         void operator=( object_t&& ) = delete;

         template< typename T >
         object_t& insert( const std::string& k, const T& v )
         {
            assert( c_ );
            c_->key( k );
            Traits< T >::template produce< Traits >( *c_, v );
            c_->member();
            return *this;
         }
      };

      template< template< typename... > class Traits, typename Consumer >
      array_t< Traits, Consumer > array( Consumer& c ) noexcept
      {
         return array_t< Traits, Consumer >( c );
      }

      template< template< typename... > class Traits, typename Consumer >
      object_t< Traits, Consumer > object( Consumer& c ) noexcept
      {
         return object_t< Traits, Consumer >( c );
      }

      template<>
      struct traits< point >
      {
         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& consumer, const point& data )
         {
            auto a = array< Traits >( consumer );
            a.push_back( data.x );
            a.push_back( data.y );
         }
      };

      struct employee
      {
         std::string name = "Isidor";
         std::string position = "CEO";
         std::uint64_t income = 42;
      };

      template<>
      struct traits< employee >
      {
         template< template< typename... > class Traits, typename Consumer >
         static void produce( Consumer& consumer, const employee& data )
         {
            auto o = object< Traits >( consumer );
            o.insert( "name", data.name );
            if( !data.position.empty() ) {
               o.insert( "position", data.position );
            }
            o.insert( "income", data.income );
         }
      };

      template<>
      struct traits< const employee* >
      {
         static void assign( value& v, const employee* e )
         {
            v.unsafe_assign_opaque( e );
         }
      };

      namespace events
      {
         template< template< typename... > class Traits = traits, typename Consumer, typename T >
         void from_other( Consumer& c, T&& t )
         {
            Traits< typename std::decay< T >::type >::template produce< Traits >( c, std::forward< T >( t ) );
         }

      }  // namespace events

      template< template< typename... > class Traits = traits, typename T >
      void other_to_stream( std::ostream& os, const T& t )
      {
         events::to_stream consumer( os );
         events::from_other< Traits >( consumer, t );
      }

      template< template< typename... > class Traits = traits, typename T >
      void other_to_stream( std::ostream& os, const T& t, const std::size_t indent )
      {
         events::to_pretty_stream consumer( os, indent );
         events::from_other< Traits >( consumer, t );
      }

      template< template< typename... > class Traits = traits, typename... Ts >
      std::string other_to_string( Ts&&... ts )
      {
         std::ostringstream oss;
         other_to_stream< Traits >( oss, std::forward< Ts >( ts )... );
         return oss.str();
      }

      void unit_test()
      {
         const employee e{};

         value v1 = {
            { "account", 1 },
            { "employee", &e }
         };

         const auto s1 = to_string( from_string( to_string( v1 ) ) );

         value v2 = v1;

         make_self_contained( v2 );

         const auto s2 = to_string( from_string( to_string( v2 ) ) );

         TEST_ASSERT( s1 == s2 );

         value v3 = v2;

         TEST_ASSERT( v2 == v3 );

         const auto s3 = other_to_string( e );

         TEST_ASSERT( s3 == "{\"name\":\"Isidor\",\"position\":\"CEO\",\"income\":42}" );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
