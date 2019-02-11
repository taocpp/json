// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_PAIR_TRAITS_HPP
#define TAO_JSON_CONTRIB_PAIR_TRAITS_HPP

#include <utility>

#include "../binding.hpp"

namespace tao
{
   namespace json
   {
      template< typename U, typename V >
      struct pair_traits
         : public binding::array< TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::first ),
                                  TAO_JSON_BIND_ELEMENT( &std::pair< U, V >::second ) >
      {
      };

   }  // namespace json

}  // namespace tao

#endif
