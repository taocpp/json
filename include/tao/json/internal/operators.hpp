// The Art of C++ / Operators
// Copyright (c) 2013-2018 Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/operators/

#ifndef TAO_JSON_INTERNAL_OPERATORS_HPP
#define TAO_JSON_INTERNAL_OPERATORS_HPP

namespace tao
{
   namespace json
   {
      namespace internal
      {
         namespace operators
         {
            template< typename T, typename U = T >
            class equality_comparable
            {
               friend bool operator!=( const T& lhs, const U& rhs ) noexcept( noexcept( static_cast< bool >( lhs == rhs ) ) )
               {
                  return !static_cast< bool >( lhs == rhs );
               }

               friend bool operator==( const U& lhs, const T& rhs ) noexcept( noexcept( static_cast< bool >( rhs == lhs ) ) )
               {
                  return static_cast< bool >( rhs == lhs );
               }

               friend bool operator!=( const U& lhs, const T& rhs ) noexcept( noexcept( static_cast< bool >( rhs != lhs ) ) )
               {
                  return static_cast< bool >( rhs != lhs );
               }
            };

            template< typename T >
            class equality_comparable< T >
            {
               friend bool operator!=( const T& lhs, const T& rhs ) noexcept( noexcept( static_cast< bool >( lhs == rhs ) ) )
               {
                  return !static_cast< bool >( lhs == rhs );
               }
            };

            template< typename T, typename U = T >
            class less_than_comparable
            {
               friend bool operator<=( const T& lhs, const U& rhs ) noexcept( noexcept( static_cast< bool >( lhs > rhs ) ) )
               {
                  return !static_cast< bool >( lhs > rhs );
               }

               friend bool operator>=( const T& lhs, const U& rhs ) noexcept( noexcept( static_cast< bool >( lhs < rhs ) ) )
               {
                  return !static_cast< bool >( lhs < rhs );
               }

               friend bool operator<( const U& lhs, const T& rhs ) noexcept( noexcept( static_cast< bool >( rhs > lhs ) ) )
               {
                  return static_cast< bool >( rhs > lhs );
               }

               friend bool operator>( const U& lhs, const T& rhs ) noexcept( noexcept( static_cast< bool >( rhs < lhs ) ) )
               {
                  return static_cast< bool >( rhs < lhs );
               }

               friend bool operator<=( const U& lhs, const T& rhs ) noexcept( noexcept( static_cast< bool >( rhs >= lhs ) ) )
               {
                  return static_cast< bool >( rhs >= lhs );
               }

               friend bool operator>=( const U& lhs, const T& rhs ) noexcept( noexcept( static_cast< bool >( rhs <= lhs ) ) )
               {
                  return static_cast< bool >( rhs <= lhs );
               }
            };

            template< typename T >
            class less_than_comparable< T >
            {
               friend bool operator>( const T& lhs, const T& rhs ) noexcept( noexcept( static_cast< bool >( rhs < lhs ) ) )
               {
                  return static_cast< bool >( rhs < lhs );
               }

               friend bool operator<=( const T& lhs, const T& rhs ) noexcept( noexcept( static_cast< bool >( rhs < lhs ) ) )
               {
                  return !static_cast< bool >( rhs < lhs );
               }

               friend bool operator>=( const T& lhs, const T& rhs ) noexcept( noexcept( static_cast< bool >( lhs < rhs ) ) )
               {
                  return !static_cast< bool >( lhs < rhs );
               }
            };

            template< typename T, typename U = T >
            class totally_ordered
               : less_than_comparable< T, U >,
                 equality_comparable< T, U >
            {
            };

         }  // namespace operators

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif
