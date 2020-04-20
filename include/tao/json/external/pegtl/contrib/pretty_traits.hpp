// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_JSON_PEGTL_CONTRIB_PRETTY_TRAITS_HPP
#define TAO_JSON_PEGTL_CONTRIB_PRETTY_TRAITS_HPP

#include <iostream>
#include <string_view>

#include "../ascii.hpp"
#include "../config.hpp"
#include "../rules.hpp"

#include "../internal/dependent_false.hpp"

#include "forward.hpp"

#include "internal/escape.hpp"

namespace TAO_JSON_PEGTL_NAMESPACE
{
   template< typename... Rules >
   struct pretty_sub_traits;

   template< typename Rule >
   struct pretty_sub_traits< Rule >
   {
      static void print( std::ostream& os, const bool paren, const std::string_view /*unused*/ = " " )
      {
         pretty_traits< Rule, typename Rule::rule_t >::print( os, paren );
      }
   };

   template< typename Rule, typename... Rules >
   struct pretty_sub_traits< Rule, Rules... >
   {
      static void print( std::ostream& os, const bool paren, const std::string_view sv = " " )
      {
         if( paren ) {
            os << "( ";
         }
         pretty_traits< Rule, typename Rule::rule_t >::print( os, true );
         ( ( os << sv, pretty_traits< Rules, typename Rules::rule_t >::print( os, true ) ), ... );
         if( paren ) {
            os << " )";
         }
      }
   };

   template< typename Name >
   struct pretty_traits< Name, internal::any< internal::peek_char > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << '.';
      }
   };

   template< typename Name, typename... Rules >
   struct pretty_traits< Name, internal::at< Rules... > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << '@';
         pretty_sub_traits< Rules... >::print( os, true );
      }
   };

   template< typename Name >
   struct pretty_traits< Name, internal::bof >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "bof";
      }
   };

   template< typename Name >
   struct pretty_traits< Name, internal::eof >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "eof";
      }
   };

   template< typename Name >
   struct pretty_traits< Name, internal::eol >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "eol";
      }
   };

   template< typename Name >
   struct pretty_traits< Name, internal::eolf >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "eolf";
      }
   };

   template< typename Name >
   struct pretty_traits< Name, internal::failure >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "failure";
      }
   };

   template< typename Name, typename Cond, typename... Rules >
   struct pretty_traits< Name, internal::if_must< false, Cond, Rules... > >
      : pretty_traits< Name, typename seq< Cond, Rules... >::rule_t >
   {};

   template< typename Name, typename Cond, typename... Rules >
   struct pretty_traits< Name, internal::if_must< true, Cond, Rules... > >
      : pretty_traits< Name, typename if_then_else< Cond, Rules..., success >::rule_t >
   {};

   template< typename Name, typename Cond, typename Then, typename Else >
   struct pretty_traits< Name, internal::if_then_else< Cond, Then, Else > >
   {
      static void print( std::ostream& os, const bool paren )
      {
         if( paren ) {
            os << "( ";
         }
         pretty_sub_traits< Cond >::print( os, true );
         os << " ? ";
         pretty_sub_traits< Then >::print( os, true );
         os << " : ";
         pretty_sub_traits< Else >::print( os, true );
         if( paren ) {
            os << " )";
         }
      }
   };

   template< typename Name, typename... Rules >
   struct pretty_traits< Name, internal::must< Rules... > >
      : pretty_traits< Name, typename seq< Rules... >::rule_t >
   {};

   template< typename Name, typename... Rules >
   struct pretty_traits< Name, internal::not_at< Rules... > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << '!';
         pretty_sub_traits< Rules... >::print( os, true );
      }
   };

   template< typename Name, char... Cs >
   struct pretty_traits< Name, internal::one< internal::result_on_found::failure, internal::peek_char, Cs... > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "\"[^";
         ( internal::escape( os, Cs ), ... );
         os << "]\"";
      }
   };

   template< typename Name, char C >
   struct pretty_traits< Name, internal::one< internal::result_on_found::success, internal::peek_char, C > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << '"';
         internal::escape( os, C );
         os << '"';
      }
   };

   template< typename Name, char... Cs >
   struct pretty_traits< Name, internal::one< internal::result_on_found::success, internal::peek_char, Cs... > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "\"[";
         ( internal::escape( os, Cs ), ... );
         os << "]\"";
      }
   };

   template< typename Name, typename... Rules >
   struct pretty_traits< Name, internal::opt< Rules... > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         pretty_sub_traits< Rules... >::print( os, true );
         os << '?';
      }
   };

   template< typename Name, typename... Rules >
   struct pretty_traits< Name, internal::plus< Rules... > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         pretty_sub_traits< Rules... >::print( os, true );
         os << '+';
      }
   };

   template< typename Name, char Lo, char Hi >
   struct pretty_traits< Name, internal::range< internal::result_on_found::failure, internal::peek_char, Lo, Hi > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "\"[^";
         internal::escape( os, Lo );
         os << '-';
         internal::escape( os, Hi );
         os << "\"]";
      }
   };

   template< typename Name >
   struct pretty_traits< Name, internal::range< internal::result_on_found::success, internal::peek_char, '0', '9' > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "digit";
      }
   };

   template< typename Name >
   struct pretty_traits< Name, internal::range< internal::result_on_found::success, internal::peek_char, 'a', 'z' > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "lower";
      }
   };

   template< typename Name >
   struct pretty_traits< Name, internal::range< internal::result_on_found::success, internal::peek_char, 'A', 'Z' > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "upper";
      }
   };

   template< typename Name, char Lo, char Hi >
   struct pretty_traits< Name, internal::range< internal::result_on_found::success, internal::peek_char, Lo, Hi > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "\"[";
         internal::escape( os, Lo );
         os << '-';
         internal::escape( os, Hi );
         os << "]\"";
      }
   };

   template< typename Name, char32_t Lo, char32_t Hi >
   struct pretty_traits< Name, internal::range< internal::result_on_found::success, internal::peek_utf8, Lo, Hi > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "[ u";
         os << int( Lo );
         os << "-u";
         os << int( Hi );
         os << " ]";
      }
   };

   namespace internal
   {
      template< char... Cs >
      struct pretty_ranges_traits;

      template<>
      struct pretty_ranges_traits<>
      {
         static void print( std::ostream& /*unused*/ )
         {
         }
      };

      template< char C >
      struct pretty_ranges_traits< C >
      {
         static void print( std::ostream& os )
         {
            internal::escape( os, C );
         }
      };

      template< char Lo, char Hi, char... Cs >
      struct pretty_ranges_traits< Lo, Hi, Cs... >
      {
         static void print( std::ostream& os )
         {
            internal::escape( os, Lo );
            os << '-';
            internal::escape( os, Hi );
            pretty_ranges_traits< Cs... >::print( os );
         }
      };

   }  // namespace internal

   template< typename Name, char... Cs >
   struct pretty_traits< Name, internal::ranges< internal::peek_char, Cs... > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "\"[";
         internal::pretty_ranges_traits< Cs... >::print( os );
         os << "]\"";
      }
   };

   template< typename Name, unsigned Cnt, typename... Rules >
   struct pretty_traits< Name, internal::rep< Cnt, Rules... > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         pretty_sub_traits< Rules... >::print( os, true );
         os << "{ " << Cnt << " }";
      }
   };

   template< typename Name, typename... Rules >
   struct pretty_traits< Name, internal::seq< Rules... > >
   {
      static void print( std::ostream& os, const bool paren )
      {
         pretty_sub_traits< Rules... >::print( os, paren );
      }
   };

   template< typename Name, typename... Rules >
   struct pretty_traits< Name, internal::sor< Rules... > >
   {
      static void print( std::ostream& os, const bool paren )
      {
         pretty_sub_traits< Rules... >::print( os, paren, " / " );
      }
   };

   template< typename Name, typename... Rules >
   struct pretty_traits< Name, internal::star< Rules... > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         pretty_sub_traits< Rules... >::print( os, true );
         os << '*';
      }
   };

   template< typename Name, char... Cs >
   struct pretty_traits< Name, internal::string< Cs... > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << '"';
         ( internal::escape( os, Cs ), ... );
         os << '"';
      }
   };

   template< typename Name >
   struct pretty_traits< Name, internal::success >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << "success";
      }
   };

   template< typename Name, typename Cond >
   struct pretty_traits< Name, internal::until< Cond > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         os << ".{ ";
         pretty_sub_traits< Cond >::print( os, false );
         os << " }";
      }
   };

   template< typename Name, typename Cond, typename... Rules >
   struct pretty_traits< Name, internal::until< Cond, Rules... > >
   {
      static void print( std::ostream& os, const bool /*unused*/ )
      {
         pretty_sub_traits< Rules... >::print( os, true );
         os << "{ ";
         pretty_sub_traits< Cond >::print( os, false );
         os << " }";
      }
   };

}  // namespace TAO_JSON_PEGTL_NAMESPACE

#endif
