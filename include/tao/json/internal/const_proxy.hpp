// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_CONST_PROXY_HPP
#define TAOCPP_JSON_INCLUDE_INTERNAL_CONST_PROXY_HPP

#include <cstdint>
#include <type_traits>
#include <utility>

#include "../external/byte.hpp"
#include "../external/operators.hpp"
#include "../external/optional.hpp"
#include "../external/string_view.hpp"

#include "../byte_view.hpp"
#include "../data.hpp"
#include "../pointer.hpp"
#include "../type.hpp"

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename, typename, typename = void >
         struct has_as : std::false_type
         {
         };

         template< typename T, typename V >
         struct has_as< T, V, decltype( T::as( std::declval< const V& >() ), void() ) > : std::true_type
         {
         };

         template< template< typename... > class Traits >
         class const_proxy
         {
         private:
            using binary_t = data::binary_t;
            using array_t = data::array_t;
            using object_t = data::object_t;

            const data& v_;

         public:
            explicit const_proxy( const data& v ) noexcept
               : v_( v )
            {
            }

            operator const data&() const noexcept
            {
               return v_;
            }

            json::type type() const noexcept
            {
               return v_.type();
            }

            explicit operator bool() const noexcept
            {
               return v_.empty();
            }

            bool is_null() const noexcept
            {
               return v_.is_null();
            }

            bool is_boolean() const noexcept
            {
               return v_.is_boolean();
            }

            bool is_signed() const noexcept
            {
               return v_.is_signed();
            }

            bool is_unsigned() const noexcept
            {
               return v_.is_unsigned();
            }

            bool is_integer() const noexcept
            {
               return is_integer();
            }

            bool is_double() const noexcept
            {
               return v_.is_double();
            }

            bool is_number() const noexcept
            {
               return v_.is_number();
            }

            bool is_string() const noexcept
            {
               return v_.is_string();
            }

            bool is_string_view() const noexcept
            {
               return v_.is_string_view();
            }

            bool is_string_type() const noexcept
            {
               return v_.is_string_type();
            }

            bool is_binary() const noexcept
            {
               return v_.is_binary();
            }

            bool is_binary_view() const noexcept
            {
               return v_.is_binary_view();
            }

            bool is_binary_type() const noexcept
            {
               return v_.is_binary_type();
            }

            bool is_array() const noexcept
            {
               return v_.is_array();
            }

            bool is_object() const noexcept
            {
               return v_.is_object();
            }

            bool is_raw_ptr() const noexcept
            {
               return v_.is_raw_ptr();
            }

            bool unsafe_get_boolean() const noexcept
            {
               return v_.unsafe_get_boolean();
            }

            std::int64_t unsafe_get_signed() const noexcept
            {
               return v_.unsafe_get_signed();
            }

            std::uint64_t unsafe_get_unsigned() const noexcept
            {
               return v_.unsafe_get_unsigned();
            }

            double unsafe_get_double() const noexcept
            {
               return v_.unsafe_get_double();
            }

            const std::string& unsafe_get_string() const noexcept
            {
               return v_.unsafe_get_string();
            }

            tao::string_view unsafe_get_string_view() const noexcept
            {
               return v_.unsafe_get_string_view();
            }

            const binary_t& unsafe_get_binary() const noexcept
            {
               return v_.unsafe_get_binary();
            }

            tao::byte_view unsafe_get_binary_view() const noexcept
            {
               return v_.unsafe_get_binary_view();
            }

            const array_t& unsafe_get_array() const noexcept
            {
               return v_.unsafe_get_array();
            }

            const object_t& unsafe_get_object() const noexcept
            {
               return v_.unsafe_get_object();
            }

            const data* unsafe_get_raw_ptr() const noexcept
            {
               return v_.unsafe_get_raw_ptr();
            }

            template< json::type E >
            decltype( v_.unsafe_get< E >() ) unsafe_get() const noexcept
            {
               return v_.unsafe_get< E >();
            }

            bool get_boolean() const
            {
               return v_.get_boolean();
            }

            std::int64_t get_signed() const
            {
               return v_.get_signed();
            }

            std::uint64_t get_unsigned() const
            {
               return v_.get_unsigned();
            }

            double get_double() const
            {
               return v_.get_double();
            }

            const std::string& get_string() const
            {
               return v_.get_string();
            }

            tao::string_view get_string_view() const
            {
               return v_.get_string_view();
            }

            const binary_t& get_binary() const
            {
               return v_.get_binary();
            }

            tao::byte_view get_binary_view() const
            {
               return v_.get_binary_view();
            }

            const array_t& get_array() const
            {
               return v_.get_array();
            }

            const object_t& get_object() const
            {
               return v_.get_object();
            }

            const data* get_raw_ptr() const
            {
               return v_.get_raw_ptr();
            }

            template< json::type E >
            decltype( v_.get< E >() ) get() const noexcept
            {
               return v_.get< E >();
            }

            const data* skip_raw_ptr() const noexcept
            {
               return v_.skip_raw_ptr();
            }

            const data* unsafe_find( const std::string& key ) const noexcept
            {
               return v_.unsafe_find( key );
            }

            template< typename T >
            const data* unsafe_find( const T& key ) const noexcept
            {
               return v_.unsafe_find( key );
            }

            const data* find( const std::string& key ) const
            {
               return v_.find( key );
            }

            template< typename T >
            const data* find( const T& key ) const
            {
               return v_.find( key );
            }

            const_proxy unsafe_at( const std::size_t index ) const noexcept
            {
               return const_proxy( v_.unsafe_at( index ) );
            }

            const_proxy unsafe_at( const std::string& key ) const noexcept
            {
               return const_proxy( v_.unsafe_at( key ) );
            }

            const_proxy at( const std::size_t index ) const
            {
               return const_proxy( v_.at( index ) );
            }

            const_proxy at( const std::string& key ) const
            {
               return const_proxy( v_.at( key ) );
            }

            const_proxy at( const pointer& k ) const
            {
               return const_proxy( v_.at( k ) );
            }

            const_proxy operator[]( const std::size_t index ) const noexcept
            {
               return const_proxy( v_[ index ] );
            }

            bool empty() const noexcept
            {
               return v_.empty();
            }

            template< typename T >
            void extract( T& v ) const
            {
               Traits< typename std::decay< T >::type >::extract( *this, v );
            }

            template< typename T >
            typename std::enable_if< internal::has_as< Traits< T >, const_proxy >::value, T >::type as() const
            {
               return Traits< T >::as( *this );
            }

            template< typename T >
            typename std::enable_if< !internal::has_as< Traits< T >, const_proxy >::value, T >::type as() const
            {
               T nrv;
               this->extract( nrv );
               return nrv;
            }

            template< typename T >
            tao::optional< T > optional() const
            {
               if( is_null() ) {
                  return tao::nullopt;
               }
               else {
                  return as< T >();
               }
            }

            template< typename T, typename K >
            tao::optional< T > optional( const K& key ) const
            {
               if( const auto* p = find( key ) ) {
                  return tao::nullopt;
               }
               else {
                  return const_proxy( *p ).as< T >();
               }
            }

            template< typename T >
            tao::optional< T > optional( const std::string& key ) const
            {
               return optional< T, std::string >( key );
            }
         };

         template< template< typename... > class TraitsL, template< typename... > class TraitsR >
         bool operator==( const_proxy< TraitsL > lhs, const_proxy< TraitsR > rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) == static_cast< const data& >( rhs );
         }

         template< template< typename... > class TraitsL, template< typename... > class TraitsR >
         bool operator<( const_proxy< TraitsL > lhs, const_proxy< TraitsR > rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) < static_cast< const data& >( rhs );
         }

         template< template< typename... > class TraitsL, typename R >
         bool operator==( const_proxy< TraitsL > lhs, R&& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) == std::forward< R >( rhs );
         }

         template< template< typename... > class TraitsL, typename R >
         bool operator<( const_proxy< TraitsL > lhs, R&& rhs ) noexcept
         {
            return static_cast< const data& >( lhs ) < std::forward< R >( rhs );
         }

         template< typename L, template< typename... > class TraitsR >
         bool operator==( L&& lhs, const_proxy< TraitsR > rhs ) noexcept
         {
            return std::forward< L >( lhs ) == static_cast< const data& >( rhs );
         }

         template< typename L, template< typename... > class TraitsR >
         bool operator<( L&& lhs, const_proxy< TraitsR > rhs ) noexcept
         {
            return std::forward< L >( lhs ) < static_cast< const data& >( rhs );
         }

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
