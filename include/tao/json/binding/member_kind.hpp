// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_BINDING_MEMBER_KIND_HPP
#define TAO_JSON_BINDING_MEMBER_KIND_HPP

namespace tao
{
   namespace json
   {
      namespace binding
      {
         enum class member_kind : bool
         {
            OPTIONAL_MEMBER,
            REQUIRED_MEMBER
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#endif
