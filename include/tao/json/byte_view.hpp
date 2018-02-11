// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BYTE_VIEW_HPP
#define TAO_JSON_BYTE_VIEW_HPP

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

#include "internal/identity.hpp"

#include "external/byte.hpp"

namespace tao
{
   // modelled after C++17's basic_string_view
   template< class byteT >
   class basic_byte_view  // NOLINT
   {
   public:
      using value_type = byteT;
      using pointer = value_type*;
      using const_pointer = const value_type*;
      using reference = value_type&;
      using const_reference = const value_type&;
      using const_iterator = const value_type*;
      using iterator = const_iterator;
      using const_reverse_iterator = std::reverse_iterator< const_iterator >;
      using reverse_iterator = const_reverse_iterator;
      using size_type = std::size_t;
      using difference_type = ptrdiff_t;

      static constexpr size_type npos = size_type( -1 );

      constexpr basic_byte_view() noexcept
         : data_( nullptr ), size_( 0 )
      {
      }

      constexpr basic_byte_view( const byteT* str, size_type len ) noexcept
         : data_( str ), size_( len )
      {
      }

      constexpr basic_byte_view( const byteT* b, const byteT* e ) noexcept
         : data_( b ), size_( e - b )
      {
      }

      constexpr basic_byte_view( const std::vector< byteT >& str ) noexcept  // NOLINT
         : data_( str.data() ), size_( str.size() )
      {
      }

      constexpr basic_byte_view( const basic_byte_view& ) noexcept = default;
      /*constexpr*/ basic_byte_view& operator=( const basic_byte_view& ) noexcept = default;

      constexpr const_iterator begin() const noexcept
      {
         return data_;
      }

      constexpr const_iterator end() const noexcept
      {
         return begin() + size();
      }

      constexpr const_iterator cbegin() const noexcept
      {
         return begin();
      }

      constexpr const_iterator cend() const noexcept
      {
         return begin() + size();
      }

      constexpr const_reverse_iterator rbegin() const noexcept
      {
         return const_reverse_iterator( end() );
      }

      constexpr const_reverse_iterator rend() const noexcept
      {
         return const_reverse_iterator( begin() );
      }

      constexpr const_reverse_iterator crbegin() const noexcept
      {
         return const_reverse_iterator( end() );
      }

      constexpr const_reverse_iterator crend() const noexcept
      {
         return const_reverse_iterator( begin() );
      }

      constexpr size_type size() const noexcept
      {
         return size_;
      }

      constexpr size_type length() const noexcept
      {
         return size_;
      }

      constexpr size_type max_size() const noexcept
      {
         return npos - 1;
      }

      constexpr bool empty() const noexcept
      {
         return size_ == 0;
      }

      constexpr const_reference operator[]( size_type pos ) const noexcept
      {
         return data_[ pos ];
      }

      constexpr const_reference at( size_type pos ) const
      {
         return ( pos >= size() ) ? throw std::out_of_range( "index out of range in tao::basic_byte_view::at" ) : data_[ pos ];  // NOLINT
      }

      constexpr const_reference front() const noexcept
      {
         return data_[ 0 ];
      }

      constexpr const_reference back() const noexcept
      {
         return data_[ size() - 1 ];
      }

      constexpr const_pointer data() const noexcept
      {
         return data_;
      }

      /*constexpr*/ void remove_prefix( size_type n ) noexcept
      {
         data_ += n;
         size_ -= n;
      }

      /*constexpr*/ void remove_suffix( size_type n ) noexcept
      {
         size_ -= n;
      }

      /*constexpr*/ void swap( basic_byte_view& s ) noexcept
      {
         std::swap( data_, s.data_ );
         std::swap( size_, s.size_ );
      }

      size_type copy( byteT* s, size_type n, size_type pos = 0 ) const
      {
         if( pos > size() ) {
            throw std::out_of_range( "index out of range in tao::basic_byte_view::copy" );  // NOLINT
         }
         const size_type rlen = std::min( n, size() - pos );
         std::memcpy( s, data() + pos, rlen );
         return rlen;
      }

      constexpr basic_byte_view substr( size_type pos = 0, size_type n = npos ) const
      {
         return ( pos > size() ) ? throw std::out_of_range( "index out of range in tao::basic_byte_view::substr" ) : basic_byte_view( data() + pos, std::min( n, size() - pos ) );  // NOLINT
      }

      /*constexpr*/ int compare( basic_byte_view s ) const noexcept
      {
         const size_type rlen = std::min( size(), s.size() );
         const int result = std::memcmp( data(), s.data(), rlen );
         if( result == 0 && size() != s.size() ) {
            return ( size() < s.size() ) ? -1 : 1;
         }
         return result;
      }

      constexpr int compare( size_type pos1, size_type n1, basic_byte_view s ) const
      {
         return substr( pos1, n1 ).compare( s );
      }

      constexpr int compare( size_type pos1, size_type n1, basic_byte_view s, size_type pos2, size_type n2 ) const
      {
         return substr( pos1, n1 ).compare( s.substr( pos2, n2 ) );
      }

      constexpr int compare( const byteT* s ) const
      {
         return compare( basic_byte_view( s ) );
      }

      constexpr int compare( size_type pos1, size_type n1, const byteT* s, size_type n2 ) const
      {
         return substr( pos1, n1 ).compare( basic_byte_view( s, n2 ) );
      }

      /*constexpr*/ size_type find( basic_byte_view s, size_type pos = 0 ) const noexcept
      {
         if( pos > size() - s.size() ) {
            return npos;
         }
         const auto iter = std::search( begin() + pos, end(), s.begin(), s.end() );
         return iter == end() ? npos : iter - begin();
      }

      constexpr size_type find( byteT c, size_type pos = 0 ) const noexcept
      {
         return find( basic_byte_view( &c, 1 ), pos );
      }

      constexpr size_type find( const byteT* s, size_type pos, size_type n ) const noexcept
      {
         return find( basic_byte_view( s, n ), pos );
      }

      /*constexpr*/ size_type rfind( basic_byte_view s, size_type pos = npos ) const noexcept
      {
         if( pos > size() - s.size() ) {
            return npos;
         }
         const auto iter = std::find_end( begin() + pos, end(), s.begin(), s.end() );
         return iter == end() ? npos : iter - begin();
      }

      constexpr size_type rfind( byteT c, size_type pos = npos ) const noexcept
      {
         return rfind( basic_byte_view( &c, 1 ), pos );
      }

      constexpr size_type rfind( const byteT* s, size_type pos, size_type n ) const noexcept
      {
         return rfind( basic_byte_view( s, n ), pos );
      }

      /*constexpr*/ size_type find_first_of( basic_byte_view s, size_type pos = 0 ) const noexcept
      {
         if( pos > size() - 1 ) {
            return npos;
         }
         const auto iter = std::find_first_of( begin() + pos, end(), s.begin(), s.end() );
         return iter == end() ? npos : iter - begin();
      }

      constexpr size_type find_first_of( byteT c, size_type pos = 0 ) const noexcept
      {
         return find_first_of( basic_byte_view( &c, 1 ), pos );
      }

      constexpr size_type find_first_of( const byteT* s, size_type pos, size_type n ) const noexcept
      {
         return find_first_of( basic_byte_view( s, n ), pos );
      }

      /*constexpr*/ size_type find_last_of( basic_byte_view s, size_type pos = npos ) const noexcept
      {
         if( pos > size() - 1 ) {
            return npos;
         }
         const auto iter = std::find_first_of( rbegin() + pos, rend(), s.begin(), s.end() );
         return iter == rend() ? npos : iter.base() - 1 - begin();
      }

      constexpr size_type find_last_of( byteT c, size_type pos = npos ) const noexcept
      {
         return find_last_of( basic_byte_view( &c, 1 ), pos );
      }

      constexpr size_type find_last_of( const byteT* s, size_type pos, size_type n ) const noexcept
      {
         return find_last_of( basic_byte_view( s, n ), pos );
      }

      /*constexpr*/ size_type find_first_not_of( basic_byte_view s, size_type pos = 0 ) const noexcept
      {
         if( pos > size() - 1 ) {
            return npos;
         }
         const auto iter = std::find_if( begin() + pos, end(), [&]( char c ) {
            return std::find( s.begin(), s.end(), c ) == s.end();
         } );
         return iter == end() ? npos : iter - begin();
      }

      constexpr size_type find_first_not_of( byteT c, size_type pos = 0 ) const noexcept
      {
         return find_first_not_of( basic_byte_view( &c, 1 ), pos );
      }

      constexpr size_type find_first_not_of( const byteT* s, size_type pos, size_type n ) const noexcept
      {
         return find_first_not_of( basic_byte_view( s, n ), pos );
      }

      /*constexpr*/ size_type find_last_not_of( basic_byte_view s, size_type pos = npos ) const noexcept
      {
         if( pos > size() - 1 ) {
            return npos;
         }
         const auto iter = std::find_if( rbegin() + pos, rend(), [&]( char c ) {
            return std::find( s.begin(), s.end(), c ) == s.end();
         } );
         return iter == rend() ? npos : iter.base() - 1 - begin();
      }

      constexpr size_type find_last_not_of( byteT c, size_type pos = npos ) const noexcept
      {
         return find_last_not_of( basic_byte_view( &c, 1 ), pos );
      }

      constexpr size_type find_last_not_of( const byteT* s, size_type pos, size_type n ) const noexcept
      {
         return find_first_not_of( basic_byte_view( s, n ), pos );
      }

   private:
      const_pointer data_;
      size_type size_;  // NOLINT
   };

   template< class byteT >
   constexpr bool operator==( basic_byte_view< byteT > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) == 0;
   }

   template< class byteT, int = 1 >  // work-around for Visual C++
   constexpr bool operator==( basic_byte_view< byteT > x, internal::identity_t< basic_byte_view< byteT > > y ) noexcept
   {
      return x.compare( y ) == 0;
   }

   template< class byteT, int = 2 >  // work-around for Visual C++
   constexpr bool operator==( internal::identity_t< basic_byte_view< byteT > > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) == 0;
   }

   template< class byteT >
   constexpr bool operator!=( basic_byte_view< byteT > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) != 0;
   }

   template< class byteT, int = 1 >  // work-around for Visual C++
   constexpr bool operator!=( basic_byte_view< byteT > x, internal::identity_t< basic_byte_view< byteT > > y ) noexcept
   {
      return x.compare( y ) != 0;
   }

   template< class byteT, int = 2 >  // work-around for Visual C++
   constexpr bool operator!=( internal::identity_t< basic_byte_view< byteT > > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) != 0;
   }

   template< class byteT >
   constexpr bool operator<( basic_byte_view< byteT > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) < 0;
   }

   template< class byteT, int = 1 >  // work-around for Visual C++
   constexpr bool operator<( basic_byte_view< byteT > x, internal::identity_t< basic_byte_view< byteT > > y ) noexcept
   {
      return x.compare( y ) < 0;
   }

   template< class byteT, int = 2 >  // work-around for Visual C++
   constexpr bool operator<( internal::identity_t< basic_byte_view< byteT > > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) < 0;
   }

   template< class byteT >
   constexpr bool operator>( basic_byte_view< byteT > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) > 0;
   }

   template< class byteT, int = 1 >  // work-around for Visual C++
   constexpr bool operator>( basic_byte_view< byteT > x, internal::identity_t< basic_byte_view< byteT > > y ) noexcept
   {
      return x.compare( y ) > 0;
   }

   template< class byteT, int = 2 >  // work-around for Visual C++
   constexpr bool operator>( internal::identity_t< basic_byte_view< byteT > > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) > 0;
   }

   template< class byteT >
   constexpr bool operator<=( basic_byte_view< byteT > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) <= 0;
   }

   template< class byteT, int = 1 >  // work-around for Visual C++
   constexpr bool operator<=( basic_byte_view< byteT > x, internal::identity_t< basic_byte_view< byteT > > y ) noexcept
   {
      return x.compare( y ) <= 0;
   }

   template< class byteT, int = 2 >  // work-around for Visual C++
   constexpr bool operator<=( internal::identity_t< basic_byte_view< byteT > > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) <= 0;
   }

   template< class byteT >
   constexpr bool operator>=( basic_byte_view< byteT > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) >= 0;
   }

   template< class byteT, int = 1 >  // work-around for Visual C++
   constexpr bool operator>=( basic_byte_view< byteT > x, internal::identity_t< basic_byte_view< byteT > > y ) noexcept
   {
      return x.compare( y ) >= 0;
   }

   template< class byteT, int = 2 >  // work-around for Visual C++
   constexpr bool operator>=( internal::identity_t< basic_byte_view< byteT > > x, basic_byte_view< byteT > y ) noexcept
   {
      return x.compare( y ) >= 0;
   }

   using byte_view = basic_byte_view< tao::byte >;

}  // namespace tao

#endif
