// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_FORWARD_HPP
#define TAOCPP_JSON_FORWARD_HPP

namespace tao
{
   namespace json
   {
      template< typename T, typename = void >
      struct traits;

      template< template< typename... > class Traits, typename Base >
      class basic_value;

      template< template< typename... > class Traits, typename Base >
      struct single;

      template< template< typename... > class Traits, typename Base >
      struct pair;

   } // namespace json

} // namespace tao

#endif
