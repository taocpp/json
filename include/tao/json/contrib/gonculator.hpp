// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_GONCULATOR_HPP
#define TAO_JSON_CONTRIB_GONCULATOR_HPP

#include <cassert>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>

#include "../external/pegtl.hpp"

namespace tao
{
   namespace json
   {
      struct minus_goncule_t
      {
         explicit constexpr minus_goncule_t( int /*unused*/ ) {}
      };

      struct star_goncule_t
      {
         explicit constexpr star_goncule_t( int /*unused*/ ) {}
      };

      struct other_goncule_t
      {
         explicit constexpr other_goncule_t( int /*unused*/ ) {}
      };

      constexpr minus_goncule_t minus_goncule{ 0 };        // NOLINT
      constexpr star_goncule_t star_goncule{ 0 };          // NOLINT
      constexpr other_goncule_t other_goncule{ 0 };        // NOLINT

      class goncule
      {
      public:
         goncule( const minus_goncule_t t )
            : m_goncule( t )
         {
         }

         goncule( const star_goncule_t t )
            : m_goncule( t )
         {
         }

         goncule( const other_goncule_t t )
            : m_goncule( t )
         {
         }

         explicit goncule( const std::int64_t i )
            : m_goncule( i )
         {
         }

         explicit goncule( std::string&& s )
            : m_goncule( std::move( s ) )
         {
         }

         explicit goncule( const std::string& s )
            : m_goncule( s )
         {
         }

         enum kind : std::uint8_t
         {
            index = 0,
            name = 1,
            minus = 2,
            star = 3,
            other = 4
         };

         kind type() const noexcept
         {
            return static_cast< kind >( m_goncule.index() );
         }

         bool is_index() const noexcept
         {
            return type() == index;
         }

         bool is_name() const noexcept
         {
            return type() == name;
         }

         bool is_minus() const noexcept
         {
            return type() == minus;
         }

         bool is_star() const noexcept
         {
            return type() == star;
         }

         bool is_other() const noexcept
         {
            return type() == other;
         }

         using signed_t = std::int64_t;
         using unsigned_t = std::uint64_t;

         std::int64_t get_index() const
         {
            return std::get< static_cast< std::size_t >( index ) >( m_goncule );
         }

         const std::string& get_name() const
         {
            return std::get< static_cast< std::size_t >( name ) >( m_goncule );
         }

         void set_index( const std::int64_t i ) noexcept
         {
            m_goncule = i;
         }

         void set_name( std::string&& s ) noexcept
         {
            m_goncule = std::move( s );
         }

         void set_name( const std::string& s )
         {
            m_goncule = s;
         }

         const auto& variant() const noexcept
         {
            return m_goncule;
         }

      private:
         std::variant< std::int64_t, std::string, minus_goncule_t, star_goncule_t, other_goncule_t > m_goncule;
      };

      inline bool operator<( const goncule& l, const goncule& r )
      {
         if( l.type() < r.type() ) {
            return true;
         }
         if( l.type() > r.type() ) {
            return false;
         }
         switch( l.type() ) {
            case goncule::name:
               return l.get_name() < r.get_name();
            case goncule::index:
               return l.get_index() < r.get_index();
            default:
               return false;
         }
         return false;
      }

      inline bool operator==( const goncule& l, const goncule& r )
      {
         return l.variant() == r.variant();
      }

      inline bool operator!=( const goncule& l, const goncule& r )
      {
         return l.variant() != r.variant();
      }

      namespace internal
      {
         // clang-format off
         struct gonculator_dot : json_pegtl::one< '.' > {};
         struct gonculator_star : json_pegtl::one< '*' > {};
         struct gonculator_minus : json_pegtl::one< '-' > {};
         struct gonculator_index : json_pegtl::plus< json_pegtl::digit > {};
         struct gonculator_optional : json_pegtl::star< json_pegtl::digit > {};
         struct gonculator_first : json_pegtl::ranges< 'a', 'z', 'A', 'Z', '_' > {};
         struct gonculator_other : json_pegtl::ranges< 'a', 'z', 'A', 'Z', '0', '9', '-', '-', '_' > {};
         struct gonculator_name : json_pegtl::seq< gonculator_first, json_pegtl::star< gonculator_other > > {};
         struct gonculator_choice : json_pegtl::if_then_else< gonculator_minus, gonculator_optional, gonculator_index > {};
         // TODO: gonculator_quoted
         struct gonculator_goncule : json_pegtl::sor< gonculator_name, gonculator_star, gonculator_choice > {};
         struct gonculator_rule : json_pegtl::list_must< gonculator_goncule, gonculator_dot > {};
         struct gonculator_grammar : json_pegtl::must< gonculator_rule, json_pegtl::eof > {};
         // clang-format on

         // TODO: Possible extensions for the future:
         // - A leading dot is 'relative/alternative resolution'.
         // - Two dots in the middle mean are like '//' in XPath.

         template< typename Rule >
         struct gonculator_action
            : public json_pegtl::nothing< Rule >
         {
         };

         template<>
         struct gonculator_action< gonculator_star >
         {
            static void apply0( std::vector< goncule >& v )
            {
               v.emplace_back( star_goncule );
            }
         };

         template<>
         struct gonculator_action< gonculator_name >
         {
            template< typename Input >
            static void apply( const Input& in, std::vector< goncule >& v )
            {
               v.emplace_back( in.string() );
            }
         };

         template<>
         struct gonculator_action< gonculator_choice >
         {
            template< typename Input >
            static void apply( const Input& in, std::vector< goncule >& v )
            {
               assert( !in.empty() );

               if( ( in.peek_char() != '-' ) || ( in.size() > 1 ) ) {
                  v.emplace_back( std::stol( in.string() ) );
               }
               else {
                  v.emplace_back( minus_goncule );
               }
            }
         };

         void gonculator_parse( std::vector< goncule >& v, const std::string_view& s )
         {
            json_pegtl::memory_input< json_pegtl::tracking_mode::lazy, json_pegtl::eol::lf_crlf, const char* > in( s, "gonculator_parse" );
            json_pegtl::parse< gonculator_grammar, gonculator_action >( in, v );
         }

         template< typename T >
         inline std::size_t make_unsigned_size( const T t )
         {
            static_assert( std::is_signed_v< T > );
            return static_cast< std::size_t >( static_cast< std::make_signed_t< std::size_t > >( t ) );
         }

         template< typename T, typename I >
         T& gonculator_at( T* v, const I& begin, const I& end )
         {
            for( auto i = begin; i != end; ++i ) {
               switch( i->type() ) {
                  case goncule::name:
                     v = &v->at( i->get_name() );
                     break;
                  case goncule::index:
                     if( const auto n = i->get_index(); n >= 0 ) {
                        v = &v->get_array().at( n );  // TODO: Exception.
                     }
                     else {
                        v = &v->unsafe_get_array().at( v->get_array().size() + make_unsigned_size( n ) );  // TODO: Exception.
                     }
                     break;
                  case goncule::minus:
                     v = &v->unsafe_get_array().at( v->get_array().size() + make_unsigned_size( -1 ) );  // TOOD: Exception.
                     break;
                  default:
                     throw std::runtime_error( "invalid goncule type for at()" );  // NOLINT
               }
            }
            return *v;
         }

         template< typename T, typename I >
         T* gonculator_find( T* v, const I& begin, const I& end )
         {
            for( auto i = begin; v && ( i != end ); ++i ) {
               switch( i->type() ) {
                  case goncule::name:
                     v = &v->at( i->get_name() );
                     break;
                  case goncule::index:
                     if( const auto n = i->get_index(); n >= 0 ) {
                        v = v->get_array().find( n );  // TODO: Exception.
                     }
                     else {
                        v = v->unsafe_get_array().find( v->get_array().size() + make_unsigned_size( n ) );  // TODO: Exception.
                     }
                     break;
                  case goncule::minus:
                     v = v->unsafe_get_array().find( v->get_array().size() + make_unsigned_size( -1 ) );  // TOOD: Exception.
                     break;
                  default:
                     throw std::runtime_error( "invalid goncule type for find()" );  // NOLINT
               }
            }
            return v;
         }

      }  // namespace internal

      struct gonculator
         : public std::vector< goncule >
      {
         gonculator() noexcept = default;
         gonculator( const gonculator& ) = default;
         gonculator( gonculator&& p ) noexcept = default;

         explicit gonculator( const std::string_view& s )
         {
            internal::gonculator_parse( vector(), s );
         }

         gonculator( const std::initializer_list< goncule >& l )
            : std::vector< goncule >( l )
         {
         }

         ~gonculator() = default;

         gonculator& operator=( const gonculator& ) = default;
         gonculator& operator=( gonculator&& p ) = default;

         gonculator& operator=( const std::string& v )
         {
            clear();
            internal::gonculator_parse( vector(), v );
            return *this;
         }

         gonculator& operator=( const std::initializer_list< goncule >& l )
         {
            vector() = l;
            return *this;
         }

         std::vector< goncule >& vector() noexcept
         {
            return static_cast< std::vector< goncule >& >( *this );
         }

         const std::vector< goncule >& vector() const noexcept
         {
            return static_cast< const std::vector< goncule >& >( *this );
         }
      };

      inline bool operator==( const gonculator& lhs, const gonculator& rhs ) noexcept
      {
         return lhs.vector() == rhs.vector();
      }

      inline bool operator<( const gonculator& lhs, const gonculator& rhs ) noexcept
      {
         return lhs.vector() < rhs.vector();
      }

      inline bool operator!=( const gonculator& lhs, const gonculator& rhs ) noexcept
      {
         return !( lhs == rhs );
      }

      inline bool operator>( const gonculator& lhs, const gonculator& rhs ) noexcept
      {
         return rhs < lhs;
      }

      inline bool operator<=( const gonculator& lhs, const gonculator& rhs ) noexcept
      {
         return !( rhs < lhs );
      }

      inline bool operator>=( const gonculator& lhs, const gonculator& rhs ) noexcept
      {
         return !( lhs < rhs );
      }

      inline gonculator& operator+=( gonculator& lhs, const std::string& rhs )
      {
         lhs.emplace_back( rhs );
         return lhs;
      }

      inline gonculator& operator+=( gonculator& lhs, std::string&& rhs )
      {
         lhs.emplace_back( std::move( rhs ) );
         return lhs;
      }

      inline gonculator& operator+=( gonculator& lhs, const std::int64_t rhs )
      {
         lhs.emplace_back( rhs );
         return lhs;
      }

      inline gonculator& operator+=( gonculator& lhs, const minus_goncule_t rhs )
      {
         lhs.emplace_back( rhs );
         return lhs;
      }

      inline gonculator& operator+=( gonculator& lhs, const star_goncule_t rhs )
      {
         lhs.emplace_back( rhs );
         return lhs;
      }

      inline gonculator& operator+=( gonculator& lhs, const other_goncule_t rhs )
      {
         lhs.emplace_back( rhs );
         return lhs;
      }

      inline gonculator operator+( const gonculator& p, const std::string& v )
      {
         gonculator nrv( p );
         nrv += v;
         return nrv;
      }

      inline gonculator operator+( const gonculator& p, std::string&& v )
      {
         gonculator nrv( p );
         nrv += std::move( v );
         return nrv;
      }

      inline gonculator operator+( const gonculator& p, const std::int64_t i )
      {
         gonculator nrv( p );
         nrv += i;
         return nrv;
      }

      inline gonculator operator+( const gonculator& p, const minus_goncule_t t )
      {
         gonculator nrv( p );
         nrv += t;
         return nrv;
      }

      inline gonculator operator+( const gonculator& p, const star_goncule_t t )
      {
         gonculator nrv( p );
         nrv += t;
         return nrv;
      }

      inline gonculator operator+( const gonculator& p, const other_goncule_t t )
      {
         gonculator nrv( p );
         nrv += t;
         return nrv;
      }

      inline namespace literals
      {
         inline gonculator operator"" _gonculator( const char* data, const std::size_t size )
         {
            return gonculator( { data, size } );
         }

      }  // namespace literals

   }  // namespace json

}  // namespace tao

#endif
