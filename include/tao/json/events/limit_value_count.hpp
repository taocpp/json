// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_EVENTS_LIMIT_VALUE_COUNT_HPP
#define TAO_JSON_EVENTS_LIMIT_VALUE_COUNT_HPP

#include <cstddef>
#include <cstdint>

#include <stdexcept>

namespace tao
{
   namespace json
   {
      namespace events
      {
         template< typename Consumer, std::size_t Limit >
         class limit_value_count
            : public Consumer
         {
         private:
            std::size_t m_count = 1;  // Top-level value is implied.

            void count_and_limit()
            {
               if( ++m_count > Limit ) {
                  throw std::runtime_error( "value count limit exceeded" );  // NOLINT
               }
            }

         public:
            using Consumer::Consumer;

            void element()
            {
               count_and_limit();
               Consumer::element();
            }

            void member()
            {
               count_and_limit();
               Consumer::member();
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
