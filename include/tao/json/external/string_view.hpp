// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EXTERNAL_STRING_VIEW_HPP
#define TAO_JSON_EXTERNAL_STRING_VIEW_HPP

#ifndef TAO_JSON_USE_STD_STRING_VIEW
#if( __cplusplus >= 201703L )
#define TAO_JSON_USE_STD_STRING_VIEW
#elif defined( _MSC_VER ) && ( _MSC_VER >= 1910L )
#if defined( _MSC_LANG ) && ( _MSC_LANG >= 201703L )
#define TAO_JSON_USE_STD_STRING_VIEW
#endif
#endif
#endif

#ifndef TAO_JSON_USE_STD_STRING_VIEW
#ifndef TAO_JSON_USE_TS_STRING_VIEW
#if( __cplusplus >= 201402L ) && defined( __GNUC__ ) && ( ( __GNUC__ > 4 ) || ( __GNUC__ == 4 ) && ( __GNUC_MINOR__ >= 9 ) )
#define TAO_JSON_USE_TS_STRING_VIEW
#endif
#endif
#endif

#if defined( TAO_JSON_USE_STD_STRING_VIEW )
#include <string_view>
#elif defined( TAO_JSON_USE_TS_STRING_VIEW )
#include <experimental/string_view>
#else
#include "../internal/identity.hpp"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <string>
#include <type_traits>
#endif

namespace tao
{

#if defined( TAO_JSON_USE_STD_STRING_VIEW )
   using std::basic_string_view;
#elif defined( TAO_JSON_USE_TS_STRING_VIEW )
   using std::experimental::basic_string_view;
#else

   // this is C++17's std::string_view
   // * based on N4659, except:
   // * additional ctor for basic_string to compensate for missing basic_string::operator basic_string_view
   // * additional noexcept specifiers have been added in a few places
   // * dropped some constexpr specifiers which are not possible in C++11
   // * user-defined literals have suffix '_sv' instead of 'sv'
   // * inefficient implementation of operator<<
   // * no std::hash-specializations
   // * implementations of find-methods is based on N3921
   template< class charT, class traits = std::char_traits< charT > >
   class basic_string_view
   {
   public:
      // types
      using traits_type = traits;
      using value_type = charT;
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

      // 24.4.2.1, construction and assignment
      constexpr basic_string_view() noexcept
         : data_( nullptr ), size_( 0 )
      {
      }

      constexpr basic_string_view( const charT* str )  // TODO: noexcept?
         : data_( str ), size_( traits::length( str ) )
      {
      }

      constexpr basic_string_view( const charT* str, size_type len ) noexcept
         : data_( str ), size_( len )
      {
      }

      // this is a work-around for missing operator basic_string_view<charT,traits> in std::string
      constexpr basic_string_view( const std::basic_string< charT, traits >& str ) noexcept
         : data_( str.data() ), size_( str.size() )
      {
      }

      constexpr basic_string_view( const basic_string_view& ) noexcept = default;
      /*constexpr*/ basic_string_view& operator=( const basic_string_view& ) noexcept = default;

      // 24.4.2.2, iterator support
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

      // 24.4.2.3, capacity
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

      // 24.4.2.4, element access
      constexpr const_reference operator[]( size_type pos ) const noexcept
      {
         return data_[ pos ];
      }

      constexpr const_reference at( size_type pos ) const
      {
         return ( pos >= size() ) ? throw std::out_of_range( "index out of range in tao::basic_string_view::at" ) : data_[ pos ];
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

      // 24.4.2.5, modifiers
      /*constexpr*/ void remove_prefix( size_type n ) noexcept
      {
         data_ += n;
         size_ -= n;
      }

      /*constexpr*/ void remove_suffix( size_type n ) noexcept
      {
         size_ -= n;
      }

      /*constexpr*/ void swap( basic_string_view& s ) noexcept
      {
         std::swap( data_, s.data_ );
         std::swap( size_, s.size_ );
      }

      // 24.4.2.6, string operations
      size_type copy( charT* s, size_type n, size_type pos = 0 ) const
      {
         if( pos > size() ) {
            throw std::out_of_range( "index out of range in tao::basic_string_view::copy" );
         }
         const size_type rlen = std::min( n, size() - pos );
         traits::copy( s, data() + pos, rlen );
         return rlen;
      }

      constexpr basic_string_view substr( size_type pos = 0, size_type n = npos ) const
      {
         return ( pos > size() ) ? throw std::out_of_range( "index out of range in tao::basic_string_view::substr" ) : basic_string_view( data() + pos, std::min( n, size() - pos ) );
      }

      /*constexpr*/ int compare( basic_string_view s ) const noexcept
      {
         const size_type rlen = std::min( size(), s.size() );
         const int result = traits::compare( data(), s.data(), rlen );
         if( result == 0 && size() != s.size() ) {
            return ( size() < s.size() ) ? -1 : 1;
         }
         return result;
      }

      constexpr int compare( size_type pos1, size_type n1, basic_string_view s ) const
      {
         return substr( pos1, n1 ).compare( s );
      }

      constexpr int compare( size_type pos1, size_type n1, basic_string_view s, size_type pos2, size_type n2 ) const
      {
         return substr( pos1, n1 ).compare( s.substr( pos2, n2 ) );
      }

      constexpr int compare( const charT* s ) const
      {
         return compare( basic_string_view( s ) );
      }

      constexpr int compare( size_type pos1, size_type n1, const charT* s ) const
      {
         return substr( pos1, n1 ).compare( basic_string_view( s ) );
      }

      constexpr int compare( size_type pos1, size_type n1, const charT* s, size_type n2 ) const
      {
         return substr( pos1, n1 ).compare( basic_string_view( s, n2 ) );
      }

      /*constexpr*/ size_type find( basic_string_view s, size_type pos = 0 ) const noexcept
      {
         if( pos > size() - s.size() ) {
            return npos;
         }
         const auto iter = std::search( begin() + pos, end(), s.begin(), s.end() );
         return iter == end() ? npos : iter - begin();
      }

      constexpr size_type find( charT c, size_type pos = 0 ) const noexcept
      {
         return find( basic_string_view( &c, 1 ), pos );
      }

      constexpr size_type find( const charT* s, size_type pos, size_type n ) const noexcept
      {
         return find( basic_string_view( s, n ), pos );
      }

      constexpr size_type find( const charT* s, size_type pos = 0 ) const
      {
         return find( basic_string_view( s ), pos );
      }

      /*constexpr*/ size_type rfind( basic_string_view s, size_type pos = npos ) const noexcept
      {
         if( pos > size() - s.size() ) {
            return npos;
         }
         const auto iter = std::find_end( begin() + pos, end(), s.begin(), s.end() );
         return iter == end() ? npos : iter - begin();
      }

      constexpr size_type rfind( charT c, size_type pos = npos ) const noexcept
      {
         return rfind( basic_string_view( &c, 1 ), pos );
      }

      constexpr size_type rfind( const charT* s, size_type pos, size_type n ) const noexcept
      {
         return rfind( basic_string_view( s, n ), pos );
      }

      constexpr size_type rfind( const charT* s, size_type pos = npos ) const
      {
         return rfind( basic_string_view( s ), pos );
      }

      /*constexpr*/ size_type find_first_of( basic_string_view s, size_type pos = 0 ) const noexcept
      {
         if( pos > size() - 1 ) {
            return npos;
         }
         const auto iter = std::find_first_of( begin() + pos, end(), s.begin(), s.end() );
         return iter == end() ? npos : iter - begin();
      }

      constexpr size_type find_first_of( charT c, size_type pos = 0 ) const noexcept
      {
         return find_first_of( basic_string_view( &c, 1 ), pos );
      }

      constexpr size_type find_first_of( const charT* s, size_type pos, size_type n ) const noexcept
      {
         return find_first_of( basic_string_view( s, n ), pos );
      }

      constexpr size_type find_first_of( const charT* s, size_type pos = 0 ) const
      {
         return find_first_of( basic_string_view( s ), pos );
      }

      /*constexpr*/ size_type find_last_of( basic_string_view s, size_type pos = npos ) const noexcept
      {
         if( pos > size() - 1 ) {
            return npos;
         }
         const auto iter = std::find_first_of( rbegin() + pos, rend(), s.begin(), s.end() );
         return iter == rend() ? npos : iter.base() - 1 - begin();
      }

      constexpr size_type find_last_of( charT c, size_type pos = npos ) const noexcept
      {
         return find_last_of( basic_string_view( &c, 1 ), pos );
      }

      constexpr size_type find_last_of( const charT* s, size_type pos, size_type n ) const noexcept
      {
         return find_last_of( basic_string_view( s, n ), pos );
      }

      constexpr size_type find_last_of( const charT* s, size_type pos = npos ) const
      {
         return find_last_of( basic_string_view( s ), pos );
      }

      /*constexpr*/ size_type find_first_not_of( basic_string_view s, size_type pos = 0 ) const noexcept
      {
         if( pos > size() - 1 ) {
            return npos;
         }
         const auto iter = std::find_if( begin() + pos, end(), [&]( char c ) {
            return std::find( s.begin(), s.end(), c ) == s.end();
         } );
         return iter == end() ? npos : iter - begin();
      }

      constexpr size_type find_first_not_of( charT c, size_type pos = 0 ) const noexcept
      {
         return find_first_not_of( basic_string_view( &c, 1 ), pos );
      }

      constexpr size_type find_first_not_of( const charT* s, size_type pos, size_type n ) const noexcept
      {
         return find_first_not_of( basic_string_view( s, n ), pos );
      }

      constexpr size_type find_first_not_of( const charT* s, size_type pos = 0 ) const
      {
         return find_first_not_of( basic_string_view( s ), pos );
      }

      /*constexpr*/ size_type find_last_not_of( basic_string_view s, size_type pos = npos ) const noexcept
      {
         if( pos > size() - 1 ) {
            return npos;
         }
         const auto iter = std::find_if( rbegin() + pos, rend(), [&]( char c ) {
            return std::find( s.begin(), s.end(), c ) == s.end();
         } );
         return iter == rend() ? npos : iter.base() - 1 - begin();
      }

      constexpr size_type find_last_not_of( charT c, size_type pos = npos ) const noexcept
      {
         return find_last_not_of( basic_string_view( &c, 1 ), pos );
      }

      constexpr size_type find_last_not_of( const charT* s, size_type pos, size_type n ) const noexcept
      {
         return find_first_not_of( basic_string_view( s, n ), pos );
      }

      constexpr size_type find_last_not_of( const charT* s, size_type pos = npos ) const
      {
         return find_last_not_of( basic_string_view( s ), pos );
      }

   private:
      const_pointer data_;
      size_type size_;
   };

   // 24.4.3, non-member comparison functions
   template< class charT, class traits >
   constexpr bool operator==( basic_string_view< charT, traits > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) == 0;
   }

   template< class charT, class traits, int = 1 >  // work-around for Visual C++
   constexpr bool operator==( basic_string_view< charT, traits > x, internal::identity_t< basic_string_view< charT, traits > > y ) noexcept
   {
      return x.compare( y ) == 0;
   }

   template< class charT, class traits, int = 2 >  // work-around for Visual C++
   constexpr bool operator==( internal::identity_t< basic_string_view< charT, traits > > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) == 0;
   }

   template< class charT, class traits >
   constexpr bool operator!=( basic_string_view< charT, traits > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) != 0;
   }

   template< class charT, class traits, int = 1 >  // work-around for Visual C++
   constexpr bool operator!=( basic_string_view< charT, traits > x, internal::identity_t< basic_string_view< charT, traits > > y ) noexcept
   {
      return x.compare( y ) != 0;
   }

   template< class charT, class traits, int = 2 >  // work-around for Visual C++
   constexpr bool operator!=( internal::identity_t< basic_string_view< charT, traits > > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) != 0;
   }

   template< class charT, class traits >
   constexpr bool operator<( basic_string_view< charT, traits > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) < 0;
   }

   template< class charT, class traits, int = 1 >  // work-around for Visual C++
   constexpr bool operator<( basic_string_view< charT, traits > x, internal::identity_t< basic_string_view< charT, traits > > y ) noexcept
   {
      return x.compare( y ) < 0;
   }

   template< class charT, class traits, int = 2 >  // work-around for Visual C++
   constexpr bool operator<( internal::identity_t< basic_string_view< charT, traits > > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) < 0;
   }

   template< class charT, class traits >
   constexpr bool operator>( basic_string_view< charT, traits > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) > 0;
   }

   template< class charT, class traits, int = 1 >  // work-around for Visual C++
   constexpr bool operator>( basic_string_view< charT, traits > x, internal::identity_t< basic_string_view< charT, traits > > y ) noexcept
   {
      return x.compare( y ) > 0;
   }

   template< class charT, class traits, int = 2 >  // work-around for Visual C++
   constexpr bool operator>( internal::identity_t< basic_string_view< charT, traits > > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) > 0;
   }

   template< class charT, class traits >
   constexpr bool operator<=( basic_string_view< charT, traits > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) <= 0;
   }

   template< class charT, class traits, int = 1 >  // work-around for Visual C++
   constexpr bool operator<=( basic_string_view< charT, traits > x, internal::identity_t< basic_string_view< charT, traits > > y ) noexcept
   {
      return x.compare( y ) <= 0;
   }

   template< class charT, class traits, int = 2 >  // work-around for Visual C++
   constexpr bool operator<=( internal::identity_t< basic_string_view< charT, traits > > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) <= 0;
   }

   template< class charT, class traits >
   constexpr bool operator>=( basic_string_view< charT, traits > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) >= 0;
   }

   template< class charT, class traits, int = 1 >  // work-around for Visual C++
   constexpr bool operator>=( basic_string_view< charT, traits > x, internal::identity_t< basic_string_view< charT, traits > > y ) noexcept
   {
      return x.compare( y ) >= 0;
   }

   template< class charT, class traits, int = 2 >  // work-around for Visual C++
   constexpr bool operator>=( internal::identity_t< basic_string_view< charT, traits > > x, basic_string_view< charT, traits > y ) noexcept
   {
      return x.compare( y ) >= 0;
   }

   // 24.4.4, inserters and extractors
   template< class charT, class traits >
   std::basic_ostream< charT, traits >& operator<<( std::basic_ostream< charT, traits >& os, basic_string_view< charT, traits > str )
   {
      // TODO: This is easy, but also inefficient.
      return os << std::string( str.begin(), str.end() );
   }

      // TODO: 24.4.5, hash support

#endif

   using string_view = basic_string_view< char >;
   using u16string_view = basic_string_view< char16_t >;
   using u32string_view = basic_string_view< char32_t >;
   using wstring_view = basic_string_view< wchar_t >;

   inline namespace literals
   {
      inline namespace string_view_literals
      {
         // 24.4.6, suffix for basic_string_view literals
         // NOTE: We are not the real STL, we use '_sv' instead of 'sv' as the suffix
         constexpr string_view operator"" _sv( const char* str, std::size_t len ) noexcept
         {
            return string_view( str, len );
         }

         constexpr u16string_view operator"" _sv( const char16_t* str, std::size_t len ) noexcept
         {
            return u16string_view( str, len );
         }

         constexpr u32string_view operator"" _sv( const char32_t* str, std::size_t len ) noexcept
         {
            return u32string_view( str, len );
         }

         constexpr wstring_view operator"" _sv( const wchar_t* str, std::size_t len ) noexcept
         {
            return wstring_view( str, len );
         }

      }  // namespace string_view_literals

   }  // namespace literals

}  // namespace tao

#endif
