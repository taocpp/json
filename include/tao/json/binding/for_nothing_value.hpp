// Copyright (c) 2018-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_FOR_NOTHING_VALUE_HPP
#define TAO_JSON_BINDING_FOR_NOTHING_VALUE_HPP

namespace tao
{
   namespace json
   {
      namespace binding
      {
         enum class for_nothing_value : bool
         {
            encode,
            suppress
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
