// Copyright (c) 2016-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_VALIDATE_EVENT_ORDER_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_VALIDATE_EVENT_ORDER_HPP

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "../byte_view.hpp"
#include "../external/byte.hpp"
#include "../external/string_view.hpp"

namespace tao
{
   namespace json
   {
      namespace events
      {
         // Events consumer that validates the order of events.

         class validate_event_order
         {
         private:
            enum state_t
            {
               EXPECT_TOP_LEVEL_VALUE,
               EXPECT_ARRAY_VALUE_OR_END,
               EXPECT_ARRAY_ELEMENT,
               EXPECT_OBJECT_KEY_OR_END,
               EXPECT_OBJECT_VALUE,
               EXPECT_OBJECT_MEMBER,
               EXPECT_NOTHING
            };

            state_t state = EXPECT_TOP_LEVEL_VALUE;
            std::vector< state_t > stack;
            // TODO: Another stack for array/object sizes.

         public:
            bool is_complete() const noexcept
            {
               return state == EXPECT_NOTHING;
            }

            void null()
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     state = EXPECT_NOTHING;
                     return;
                  case EXPECT_ARRAY_VALUE_OR_END:
                     state = EXPECT_ARRAY_ELEMENT;
                     return;
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but null() was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but null() was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     state = EXPECT_OBJECT_MEMBER;
                     return;
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but null() was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but null() was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void boolean( const bool /*unused*/ )
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     state = EXPECT_NOTHING;
                     return;
                  case EXPECT_ARRAY_VALUE_OR_END:
                     state = EXPECT_ARRAY_ELEMENT;
                     return;
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but boolean() was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but boolean() was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     state = EXPECT_OBJECT_MEMBER;
                     return;
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but boolean() was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but boolean() was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void number( const std::int64_t /*unused*/ )
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     state = EXPECT_NOTHING;
                     return;
                  case EXPECT_ARRAY_VALUE_OR_END:
                     state = EXPECT_ARRAY_ELEMENT;
                     return;
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but number(std::int64_t) was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but number(std::int64_t) was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     state = EXPECT_OBJECT_MEMBER;
                     return;
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but number(std::int64_t) was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but number(std::int64_t) was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void number( const std::uint64_t /*unused*/ )
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     state = EXPECT_NOTHING;
                     return;
                  case EXPECT_ARRAY_VALUE_OR_END:
                     state = EXPECT_ARRAY_ELEMENT;
                     return;
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but number(std::uint64_t) was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but number(std::uint64_t) was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     state = EXPECT_OBJECT_MEMBER;
                     return;
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but number(std::uint64_t) was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but number(std::uint64_t) was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void number( const double /*unused*/ )
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     state = EXPECT_NOTHING;
                     return;
                  case EXPECT_ARRAY_VALUE_OR_END:
                     state = EXPECT_ARRAY_ELEMENT;
                     return;
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but number(double) was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but number(double) was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     state = EXPECT_OBJECT_MEMBER;
                     return;
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but number(double) was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but number(double) was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void string( const tao::string_view /*unused*/ )
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     state = EXPECT_NOTHING;
                     return;
                  case EXPECT_ARRAY_VALUE_OR_END:
                     state = EXPECT_ARRAY_ELEMENT;
                     return;
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but string() was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but string() was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     state = EXPECT_OBJECT_MEMBER;
                     return;
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but string() was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but string() was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void binary( const tao::byte_view /*unused*/ )
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     state = EXPECT_NOTHING;
                     return;
                  case EXPECT_ARRAY_VALUE_OR_END:
                     state = EXPECT_ARRAY_ELEMENT;
                     return;
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but binary() was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but binary() was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     state = EXPECT_OBJECT_MEMBER;
                     return;
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but binary() was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but binary() was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void begin_array( const std::size_t /*unused*/ = 0 )
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     stack.push_back( EXPECT_NOTHING );
                     state = EXPECT_ARRAY_VALUE_OR_END;
                     return;
                  case EXPECT_ARRAY_VALUE_OR_END:
                     stack.push_back( EXPECT_ARRAY_ELEMENT );
                     return;
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but begin_array() was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but begin_array() was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     stack.push_back( EXPECT_OBJECT_MEMBER );
                     state = EXPECT_ARRAY_VALUE_OR_END;
                     return;
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but begin_array() was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but begin_array() was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void element()
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     throw std::logic_error( "expected any value, but element() was called" );  // NOLINT
                  case EXPECT_ARRAY_VALUE_OR_END:
                     throw std::logic_error( "expected any value or end_array(), but element() was called" );  // NOLINT
                  case EXPECT_ARRAY_ELEMENT:
                     state = EXPECT_ARRAY_VALUE_OR_END;
                     return;
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but element() was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     throw std::logic_error( "expected any value, but element() was called" );  // NOLINT
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but element() was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but element() was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void end_array( const std::size_t /*unused*/ = 0 )
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     throw std::logic_error( "expected any value, but end_array() was called" );  // NOLINT
                  case EXPECT_ARRAY_VALUE_OR_END:
                     state = stack.back();
                     stack.pop_back();
                     return;
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but end_array() was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but end_array() was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     throw std::logic_error( "expected any value, but end_array() was called" );  // NOLINT
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but end_array() was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but end_array() was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void begin_object( const std::size_t /*unused*/ = 0 )
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     stack.push_back( EXPECT_NOTHING );
                     state = EXPECT_OBJECT_KEY_OR_END;
                     return;
                  case EXPECT_ARRAY_VALUE_OR_END:
                     stack.push_back( EXPECT_ARRAY_ELEMENT );
                     state = EXPECT_OBJECT_KEY_OR_END;
                     return;
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but begin_object() was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but begin_object() was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     stack.push_back( EXPECT_OBJECT_MEMBER );
                     state = EXPECT_OBJECT_KEY_OR_END;
                     return;
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but begin_object() was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but begin_object() was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void key( const tao::string_view /*unused*/ )
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     throw std::logic_error( "expected any value, but key() was called" );  // NOLINT
                  case EXPECT_ARRAY_VALUE_OR_END:
                     throw std::logic_error( "expected any value or end_array(), but key() was called" );  // NOLINT
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but key() was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     state = EXPECT_OBJECT_VALUE;
                     return;
                  case EXPECT_OBJECT_VALUE:
                     throw std::logic_error( "expected any value, but key() was called" );  // NOLINT
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but key() was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but key() was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void member()
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     throw std::logic_error( "expected any value, but member() was called" );  // NOLINT
                  case EXPECT_ARRAY_VALUE_OR_END:
                     throw std::logic_error( "expected any value or end_array(), but member() was called" );  // NOLINT
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but member() was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     throw std::logic_error( "expected key() or end_object(), but member() was called" );  // NOLINT
                  case EXPECT_OBJECT_VALUE:
                     throw std::logic_error( "expected any value, but member() was called" );  // NOLINT
                  case EXPECT_OBJECT_MEMBER:
                     state = EXPECT_OBJECT_KEY_OR_END;
                     return;
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but member() was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }

            void end_object( const std::size_t /*unused*/ = 0 )
            {
               switch( state ) {
                  case EXPECT_TOP_LEVEL_VALUE:
                     throw std::logic_error( "expected any value, but end_object() was called" );  // NOLINT
                  case EXPECT_ARRAY_VALUE_OR_END:
                     throw std::logic_error( "expected any value or end_array(), but end_object() was called" );  // NOLINT
                  case EXPECT_ARRAY_ELEMENT:
                     throw std::logic_error( "expected element(), but end_object() was called" );  // NOLINT
                  case EXPECT_OBJECT_KEY_OR_END:
                     state = stack.back();
                     stack.pop_back();
                     return;
                  case EXPECT_OBJECT_VALUE:
                     throw std::logic_error( "expected any value, but end_object() was called" );  // NOLINT
                  case EXPECT_OBJECT_MEMBER:
                     throw std::logic_error( "expected member(), but end_object() was called" );  // NOLINT
                  case EXPECT_NOTHING:
                     throw std::logic_error( "expected nothing, but end_object() was called" );  // NOLINT
               }
               throw std::logic_error( "invalid state" );  // NOLINT
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
