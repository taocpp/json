// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_DEFAULT_KEY_HH
#define TAOCPP_JSON_INCLUDE_DEFAULT_KEY_HH

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T >
         const char* no_default_key_defined() noexcept
         {
            static_assert( sizeof( T ) == 0, "no default key defined for T" );
            return nullptr;
         }

      } // internal

      template< typename >
      struct default_key
      {
         static const char* value;
      };

      template< typename T >
      const char* default_key< T >::value = internal::no_default_key_defined< T >();

   } // json

} // tao

#endif
