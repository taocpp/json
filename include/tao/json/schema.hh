// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SCHEMA_HH
#define TAOCPP_JSON_INCLUDE_SCHEMA_HH

#include <tao/json/value.hh>
#include <tao/json/pointer.hh>
#include <tao/json/reference.hh>
#include <tao/json/sax/compare.hh>
#include <tao/json/sax/hash.hh>
#include <tao/json/sax/traverse_value.hh>

#include <cstdint>
#include <set>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <regex>
#include <type_traits>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         enum schema_flags
         {
            NONE = 0,

            HAS_TYPE = 1 << 0,
            NULL_ = 1 << 1,
            BOOLEAN = 1 << 2,
            INTEGER = 1 << 3,
            NUMBER = 1 << 4,
            STRING = 1 << 5,
            ARRAY = 1 << 6,
            OBJECT = 1 << 7,

            HAS_ENUM = 1 << 8,

            HAS_MULTIPLE_OF_UNSIGNED = 1 << 9,
            HAS_MULTIPLE_OF_DOUBLE = 1 << 10,
            HAS_MULTIPLE_OF = 3 << 9,

            HAS_MAXIMUM_SIGNED = 1 << 11,
            HAS_MAXIMUM_UNSIGNED = 1 << 12,
            HAS_MAXIMUM_DOUBLE = 3 << 11,
            HAS_MAXIMUM = 3 << 11,
            EXCLUSIVE_MAXIMUM = 1 << 13,

            HAS_MINIMUM_SIGNED = 1 << 14,
            HAS_MINIMUM_UNSIGNED = 1 << 15,
            HAS_MINIMUM_DOUBLE = 3 << 14,
            HAS_MINIMUM = 3 << 14,
            EXCLUSIVE_MINIMUM = 1 << 16,

            HAS_MAX_LENGTH = 1 << 17,
            HAS_MIN_LENGTH = 1 << 18,

            HAS_MAX_ITEMS = 1 << 19,
            HAS_MIN_ITEMS = 1 << 20,
            HAS_UNIQUE_ITEMS = 1 << 21,

            HAS_MAX_PROPERTIES = 1 << 22,
            HAS_MIN_PROPERTIES = 1 << 23,
            NO_ADDITIONAL_PROPERTIES = 1 << 24,
         };

         inline constexpr schema_flags operator|( const schema_flags lhs, const schema_flags rhs ) noexcept
         {
            return static_cast< schema_flags >( static_cast< std::underlying_type< schema_flags >::type >( lhs ) | static_cast< std::underlying_type< schema_flags >::type >( rhs ) );
         }

         union schema_limit
         {
            std::int64_t i;
            std::uint64_t u;
            double d;
         };

         template< template< typename ... > class Traits >
         class schema_container;

         template< template< typename ... > class Traits >
         struct schema_node
         {
            const schema_container< Traits > * m_container;
            const basic_value< Traits > * m_value;

            std::set< const basic_value< Traits > * > m_referenced_pointers;

            // number
            schema_limit m_multiple_of;
            schema_limit m_maximum;
            schema_limit m_minimum;

            // string
            std::uint64_t m_max_length;
            std::uint64_t m_min_length;
            std::unique_ptr< std::regex > m_pattern;

            // array
            std::uint64_t m_max_items;
            std::uint64_t m_min_items;

            // object
            std::uint64_t m_max_properties;
            std::uint64_t m_min_properties;
            std::set< std::string > m_required;

            schema_flags m_flags = NONE;

            void add_type( const schema_flags v )
            {
               if ( ( m_flags & v ) != 0 ) {
                  throw std::runtime_error( "invalid JSON Schema: duplicate primitive type" );
               }
               m_flags = m_flags | v;
            }

            void add_type( const std::string & v )
            {
               if ( ! v.empty() ) {
                  switch ( v[ 0 ] ) {
                     case 'n':
                        if ( v == "number" ) {
                           return add_type( NUMBER );
                        }
                        else if ( v == "null" ) {
                           return add_type( NULL_ );
                        }
                        break;
                     case 'b':
                        if ( v == "boolean" ) {
                           return add_type( BOOLEAN );
                        }
                        break;
                     case 'i':
                        if ( v == "integer" ) {
                           return add_type( INTEGER );
                        }
                        break;
                     case 's':
                        if ( v == "string" ) {
                           return add_type( STRING );
                        }
                        break;
                     case 'a':
                        if ( v == "array" ) {
                           return add_type( ARRAY );
                        }
                        break;
                     case 'o':
                        if ( v == "object" ) {
                           return add_type( OBJECT );
                        }
                        break;
                  }
               }
               throw std::runtime_error( "invalid JSON Schema: invalid primitive type '" + v + '\'' );
            }

            const basic_value< Traits > * find( const char * s ) const
            {
               const auto * p = m_value->find( s );
               if ( p != nullptr ) {
                  p = p->skip_raw_ptr();
               }
               return p;
            }

            schema_node( const schema_container< Traits > * c, const basic_value< Traits > & v2 )
                 : m_container( c ),
                   m_value( & v2 )
            {
               if ( ! m_value->is_object() ) {
                  throw std::runtime_error( "invalid JSON Schema: a schema must be of type 'object'" );
               }

               // title
               if ( const auto * p = find( "title" ) ) {
                  if ( ! p->is_string() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"title\" must be of type 'string'" );
                  }
               }

               // description
               if ( const auto * p = find( "description" ) ) {
                  if ( ! p->is_string() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"description\" must be of type 'string'" );
                  }
               }

               // type
               if ( const auto * p = find( "type" ) ) {
                  switch ( p->type() ) {
                     case json::type::STRING:
                        add_type( p->unsafe_get_string() );
                        break;
                     case json::type::ARRAY:
                        for ( const auto & e : p->unsafe_get_array() ) {
                           if ( ! e.is_string() ) {
                              throw std::runtime_error( "invalid JSON Schema: elements in array \"type\" must be of type 'string'" );
                           }
                           add_type( e.unsafe_get_string() );
                        }
                        break;
                     default:
                        throw std::runtime_error( "invalid JSON Schema: \"type\" must be of type 'string' or 'array'" );
                  }
                  m_flags = m_flags | HAS_TYPE;
               }

               // enum
               if ( const auto * p = find( "enum" ) ) {
                  if ( ! p->is_array() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"enum\" must be of type 'array'" );
                  }
                  m_flags = m_flags | HAS_ENUM;
               }

               // allOf
               if ( const auto * p = find( "allOf" ) ) {
                  if ( ! p->is_array() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"allOf\" must be of type 'array'" );
                  }
                  if ( p->empty() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"allOf\" must have at least one element" );
                  }
                  for ( const auto & e : p->unsafe_get_array() ) {
                     m_referenced_pointers.insert( e.skip_raw_ptr() );
                  }
               }

               // anyOf
               if ( const auto * p = find( "anyOf" ) ) {
                  if ( ! p->is_array() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"anyOf\" must be of type 'array'" );
                  }
                  if ( p->empty() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"anyOf\" must have at least one element" );
                  }
                  for ( const auto & e : p->unsafe_get_array() ) {
                     m_referenced_pointers.insert( e.skip_raw_ptr() );
                  }
               }

               // oneOf
               if ( const auto * p = find( "oneOf" ) ) {
                  if ( ! p->is_array() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"oneOf\" must be of type 'array'" );
                  }
                  if ( p->empty() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"oneOf\" must have at least one element" );
                  }
                  for ( const auto & e : p->unsafe_get_array() ) {
                     m_referenced_pointers.insert( e.skip_raw_ptr() );
                  }
               }

               // not
               if ( const auto * p = find( "not" ) ) {
                  m_referenced_pointers.insert( p );
               }

               // definitions
               if ( const auto * p = find( "definitions" ) ) {
                  if ( ! p->is_object() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"definitions\" must be of type 'object'" );
                  }
                  for ( const auto & e : p->unsafe_get_object() ) {
                     m_referenced_pointers.insert( e.second.skip_raw_ptr() );
                  }
               }

               // multipleOf
               if ( const auto * p = find( "multipleOf" ) ) {
                  switch ( p->type() ) {
                     case json::type::SIGNED:
                        {
                           const auto i = p->unsafe_get_signed();
                           if ( i <= 0 ) {
                              throw std::runtime_error( "invalid JSON Schema: \"multipleOf\" must be strictly greater than zero" );
                           }
                           m_multiple_of.u = i;
                           m_flags = m_flags | HAS_MULTIPLE_OF_UNSIGNED;
                        }
                        break;
                     case json::type::UNSIGNED:
                        {
                           const auto u = p->unsafe_get_unsigned();
                           if ( u == 0 ) {
                              throw std::runtime_error( "invalid JSON Schema: \"multipleOf\" must be strictly greater than zero" );
                           }
                           m_multiple_of.u = u;
                           m_flags = m_flags | HAS_MULTIPLE_OF_UNSIGNED;
                        }
                        break;
                     case json::type::DOUBLE:
                        {
                           const auto d = p->unsafe_get_double();
                           if ( d <= 0 ) {
                              throw std::runtime_error( "invalid JSON Schema: \"multipleOf\" must be strictly greater than zero" );
                           }
                           m_multiple_of.d = d;
                           m_flags = m_flags | HAS_MULTIPLE_OF_DOUBLE;
                        }
                        break;
                     default:
                        throw std::runtime_error( "invalid JSON Schema: \"multipleOf\" must be of type 'number'" );
                  }
               }

               // maximum
               if ( const auto * p = find( "maximum" ) ) {
                  switch ( p->type() ) {
                     case json::type::SIGNED:
                        m_maximum.i = p->unsafe_get_signed();
                        m_flags = m_flags | HAS_MAXIMUM_SIGNED;
                        break;
                     case json::type::UNSIGNED:
                        m_maximum.u = p->unsafe_get_unsigned();
                        m_flags = m_flags | HAS_MAXIMUM_UNSIGNED;
                        break;
                     case json::type::DOUBLE:
                        m_maximum.d = p->unsafe_get_double();
                        m_flags = m_flags | HAS_MAXIMUM_DOUBLE;
                        break;
                     default:
                        throw std::runtime_error( "invalid JSON Schema: \"maximum\" must be of type 'number'" );
                  }
               }

               // exclusiveMaximum
               if ( const auto * p = find( "exclusiveMaximum" ) ) {
                  if ( ! p->is_boolean() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"exclusiveMaximum\" must be of type 'boolean'" );
                  }
                  if ( ( m_flags & HAS_MAXIMUM ) == 0 ) {
                     throw std::runtime_error( "invalid JSON Schema: \"exclusiveMaximum\" requires presence of \"maximum\"" );
                  }
                  if ( p->get_boolean() ) {
                     m_flags = m_flags | EXCLUSIVE_MAXIMUM;
                  }
               }

               // minimum
               if ( const auto * p = find( "minimum" ) ) {
                  switch ( p->type() ) {
                     case json::type::SIGNED:
                        m_minimum.i = p->unsafe_get_signed();
                        m_flags = m_flags | HAS_MINIMUM_SIGNED;
                        break;
                     case json::type::UNSIGNED:
                        m_minimum.u = p->unsafe_get_unsigned();
                        m_flags = m_flags | HAS_MINIMUM_UNSIGNED;
                        break;
                     case json::type::DOUBLE:
                        m_minimum.d = p->unsafe_get_double();
                        m_flags = m_flags | HAS_MINIMUM_DOUBLE;
                        break;
                     default:
                        throw std::runtime_error( "invalid JSON Schema: \"minimum\" must be of type 'number'" );
                  }
               }

               // exclusiveMinimum
               if ( const auto * p = find( "exclusiveMinimum" ) ) {
                  if ( ! p->is_boolean() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"exclusiveMinimum\" must be of type 'boolean'" );
                  }
                  if ( ( m_flags & HAS_MINIMUM ) == 0 ) {
                     throw std::runtime_error( "invalid JSON Schema: \"exclusiveMinimum\" requires presence of \"minimum\"" );
                  }
                  if ( p->get_boolean() ) {
                     m_flags = m_flags | EXCLUSIVE_MINIMUM;
                  }
               }

               // maxLength
               if ( const auto * p = find( "maxLength" ) ) {
                  switch ( p->type() ) {
                     case json::type::SIGNED:
                        {
                           const auto i = p->unsafe_get_signed();
                           if ( i < 0 ) {
                              throw std::runtime_error( "invalid JSON Schema: \"maxLength\" must be greater than or equal to zero" );
                           }
                           m_max_length = i;
                           m_flags = m_flags | HAS_MAX_LENGTH;
                        }
                        break;
                     case json::type::UNSIGNED:
                        m_max_length = p->unsafe_get_unsigned();
                        m_flags = m_flags | HAS_MAX_LENGTH;
                        break;
                     default:
                        throw std::runtime_error( "invalid JSON Schema: \"maxLength\" must be of type 'integer'" );
                  }
               }

               // minLength
               if ( const auto * p = find( "minLength" ) ) {
                  switch ( p->type() ) {
                     case json::type::SIGNED:
                        {
                           const auto i = p->unsafe_get_signed();
                           if ( i < 0 ) {
                              throw std::runtime_error( "invalid JSON Schema: \"minLength\" must be greater than or equal to zero" );
                           }
                           if ( i > 0 ) {
                              m_min_length = i;
                              m_flags = m_flags | HAS_MIN_LENGTH;
                           }
                        }
                        break;
                     case json::type::UNSIGNED:
                        {
                           const auto u = p->unsafe_get_unsigned();
                           if ( u > 0 ) {
                              m_min_length = u;
                              m_flags = m_flags | HAS_MIN_LENGTH;
                           }
                        }
                        break;
                     default:
                        throw std::runtime_error( "invalid JSON Schema: \"minLength\" must be of type 'integer'" );
                  }
               }

               // pattern
               if ( const auto * p = find( "pattern" ) ) {
                  if ( ! p->is_string() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"maxItems\" must be of type 'integer'" );
                  }
                  try {
                     m_pattern.reset( new std::regex( p->unsafe_get_string() ) );
                  }
                  catch( const std::exception & e ) {
                     throw std::runtime_error( "invalid JSON Schema: \"pattern\" must be regular expressions: " + std::string( e.what() ) );
                  }
               }

               // TODO: format

               // items
               if ( const auto * p = find( "items" ) ) {
                  if ( p->is_array() ) {
                     for ( const auto & e : p->unsafe_get_array() ) {
                        m_referenced_pointers.insert( e.skip_raw_ptr() );
                     }
                  }
                  else if ( p->is_object() ) {
                     m_referenced_pointers.insert( p );
                  }
                  else {
                     throw std::runtime_error( "invalid JSON Schema: \"items\" must be of type 'object' or 'array'" );
                  }
               }

               // additionalItems
               if ( const auto * p = find( "additionalItems" ) ) {
                  if ( p->is_object() ) {
                     m_referenced_pointers.insert( p );
                  }
                  else if ( ! p->is_boolean() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"additionalItems\" must be of type 'boolean' or 'object'" );
                  }
               }

               // maxItems
               if ( const auto * p = find( "maxItems" ) ) {
                  switch ( p->type() ) {
                     case json::type::SIGNED:
                        {
                           const auto i = p->unsafe_get_signed();
                           if ( i < 0 ) {
                              throw std::runtime_error( "invalid JSON Schema: \"maxItems\" must be greater than or equal to zero" );
                           }
                           m_max_items = i;
                           m_flags = m_flags | HAS_MAX_ITEMS;
                        }
                        break;
                     case json::type::UNSIGNED:
                        m_max_items = p->unsafe_get_unsigned();
                        m_flags = m_flags | HAS_MAX_ITEMS;
                        break;
                     default:
                        throw std::runtime_error( "invalid JSON Schema: \"maxItems\" must be of type 'integer'" );
                  }
               }

               // minItems
               if ( const auto * p = find( "minItems" ) ) {
                  switch ( p->type() ) {
                     case json::type::SIGNED:
                        {
                           const auto i = p->unsafe_get_signed();
                           if ( i < 0 ) {
                              throw std::runtime_error( "invalid JSON Schema: \"minItems\" must be greater than or equal to zero" );
                           }
                           m_min_items = i;
                           m_flags = m_flags | HAS_MIN_ITEMS;
                        }
                        break;
                     case json::type::UNSIGNED:
                        m_min_items = p->unsafe_get_unsigned();
                        m_flags = m_flags | HAS_MIN_ITEMS;
                        break;
                     default:
                        throw std::runtime_error( "invalid JSON Schema: \"minItems\" must be of type 'integer'" );
                  }
               }

               // uniqueItems
               if ( const auto * p = find( "uniqueItems" ) ) {
                  if ( p->get_boolean() ) {
                     m_flags = m_flags | HAS_UNIQUE_ITEMS;
                  }
               }

               // maxProperties
               if ( const auto * p = find( "maxProperties" ) ) {
                  switch ( p->type() ) {
                     case json::type::SIGNED:
                        {
                           const auto i = p->unsafe_get_signed();
                           if ( i < 0 ) {
                              throw std::runtime_error( "invalid JSON Schema: \"maxProperties\" must be greater than or equal to zero" );
                           }
                           m_max_properties = i;
                           m_flags = m_flags | HAS_MAX_PROPERTIES;
                        }
                        break;
                     case json::type::UNSIGNED:
                        m_max_properties = p->unsafe_get_unsigned();
                        m_flags = m_flags | HAS_MAX_PROPERTIES;
                        break;
                     default:
                        throw std::runtime_error( "invalid JSON Schema: \"maxProperties\" must be of type 'integer'" );
                  }
               }

               // minProperties
               if ( const auto * p = find( "minProperties" ) ) {
                  switch ( p->type() ) {
                     case json::type::SIGNED:
                        {
                           const auto i = p->unsafe_get_signed();
                           if ( i < 0 ) {
                              throw std::runtime_error( "invalid JSON Schema: \"minProperties\" must be greater than or equal to zero" );
                           }
                           m_min_properties = i;
                           m_flags = m_flags | HAS_MIN_PROPERTIES;
                        }
                        break;
                     case json::type::UNSIGNED:
                        m_min_properties = p->unsafe_get_unsigned();
                        m_flags = m_flags | HAS_MIN_PROPERTIES;
                        break;
                     default:
                        throw std::runtime_error( "invalid JSON Schema: \"minProperties\" must be of type 'integer'" );
                  }
               }

               // required
               if ( const auto * p = find( "required" ) ) {
                  if ( ! p->is_array() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"required\" must be of type 'array'" );
                  }
                  if ( p->empty() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"required\" must have at least one element" );
                  }
                  for ( const auto & e : p->unsafe_get_array() ) {
                     if ( ! m_required.insert( e.get_string() ).second ) {
                        throw std::runtime_error( "invalid JSON Schema: duplicate required key" );
                     }
                  }
               }

               // properties
               if ( const auto * p = find( "properties" ) ) {
                  if ( ! p->is_object() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"properties\" must be of type 'object'" );
                  }
                  for ( const auto & e : p->unsafe_get_object() ) {
                     m_referenced_pointers.insert( e.second.skip_raw_ptr() );
                  }
               }

               // additionalProperties
               if ( const auto * p = find( "additionalProperties" ) ) {
                  const type t = p->type();
                  if ( t == type::OBJECT ) {
                     m_referenced_pointers.insert( p );
                  }
                  else if ( t != type::BOOLEAN ) {
                     throw std::runtime_error( "invalid JSON Schema: \"additionalProperties\" must be of type 'boolean' or 'object'" );
                  }
               }

               // patternProperties
               if ( const auto * p = find( "patternProperties" ) ) {
                  if ( ! p->is_object() ) {
                     throw std::runtime_error( "invalid JSON Schema: \"patternProperties\" must be of type 'object'" );
                  }
                  for ( const auto & e : p->unsafe_get_object() ) {
                     try {
                        std::regex( e.first );
                     }
                     catch( const std::exception & e ) {
                        throw std::runtime_error( "invalid JSON Schema: keys of \"patternProperties\" must be regular expressions: " + std::string( e.what() ) );
                     }
                     m_referenced_pointers.insert( e.second.skip_raw_ptr() );
                  }
               }

               // TODO: dependencies

               // TODO: default
               if ( const auto * p = find( "default" ) ) {
                  // TODO: the value must validate against the JSON Schema itself
               }
            }

            schema_node( const schema_node & ) = delete;
            schema_node( schema_node && ) = delete;

            void operator= ( const schema_node & ) = delete;
            void operator= ( schema_node && ) = delete;

            const std::set< const basic_value< Traits > * > & referenced_pointers() const noexcept
            {
               return m_referenced_pointers;
            }
         };

         template< template< typename ... > class Traits >
         class schema_consumer
         {
         private:
            const std::shared_ptr< const schema_container< Traits > > m_container;
            const schema_node< Traits > * m_node;

            std::vector< std::unique_ptr< sax_compare< Traits > > > m_enum;
            std::unique_ptr< sax::hash > m_hash;
            std::set< std::string > m_unique;
            std::set< std::string > m_required;
            std::vector< std::size_t > m_count;
            std::vector< std::unique_ptr< schema_consumer > > m_properties;
            std::vector< std::unique_ptr< schema_consumer > > m_all_of;
            std::vector< std::unique_ptr< schema_consumer > > m_any_of;
            std::vector< std::unique_ptr< schema_consumer > > m_one_of;
            std::unique_ptr< schema_consumer > m_not;
            bool m_match = true;

            void validate_type( const schema_flags t )
            {
               if ( ! m_count.empty() ) {
                  return;
               }
               if ( ( m_node->m_flags & HAS_TYPE ) == 0 ) {
                  return;
               }
               if ( ( m_node->m_flags & t ) == 0 ) {
                  m_match = false;
               }
            }

            // note: lambda returns true if validation failure detected
            template< typename F >
            void validate_enum( F && f )
            {
               assert( m_match );
               if ( m_node->m_flags & HAS_ENUM ) {
                  m_enum.erase( std::remove_if( m_enum.begin(), m_enum.end(), [&]( const std::unique_ptr< sax_compare< Traits > > & p ){ return f( * p ); } ), m_enum.end() );
                  if ( m_enum.empty() ) {
                     m_match = false;
                  }
               }
            }

            template< typename F >
            void validate_properties( F && f )
            {
               for ( auto & p : m_properties ) {
                  if ( f( p ) ) {
                     m_match = false;
                     break;
                  }
               }
            }

            template< typename F >
            void validate_all_of( F && f )
            {
               for ( auto & p : m_all_of ) {
                  if ( f( p ) ) {
                     m_match = false;
                     break;
                  }
               }
            }

            template< typename F >
            void validate_any_of( F && f )
            {
               if ( ! m_any_of.empty() ) {
                  m_any_of.erase( std::remove_if( m_any_of.begin(), m_any_of.end(), f ), m_any_of.end() );
                  if ( m_any_of.empty() ) {
                     m_match = false;
                  }
               }
            }

            template< typename F >
            void validate_one_of( F && f )
            {
               if ( ! m_one_of.empty() ) {
                  m_one_of.erase( std::remove_if( m_one_of.begin(), m_one_of.end(), f ), m_one_of.end() );
                  if ( m_one_of.empty() ) {
                     m_match = false;
                  }
               }
            }

            template< typename F >
            void validate_not( F && f )
            {
               if ( m_not ) {
                  if ( f( m_not ) ) {
                     m_not.reset();
                  }
               }
            }

            // note: lambda returns true if validation failure detected
            template< typename F >
            void validate_collections( F && f )
            {
               assert( m_match );
               const auto f2 = [&]( const std::unique_ptr< schema_consumer > & p ){ return f( * p ); };
               if ( m_match ) validate_properties( f2 );
               if ( m_match ) validate_all_of( f2 );
               if ( m_match ) validate_any_of( f2 );
               if ( m_match ) validate_one_of( f2 );
               if ( m_match ) validate_not( f2 );
            }

            static bool is_multiple_of( const double v, const double d )
            {
               const auto r = std::fmod( v, d );
               if ( std::fabs( r ) < std::numeric_limits< double >::epsilon() ) return true;
               if ( std::fabs( r - d ) < std::numeric_limits< double >::epsilon() ) return true;
               return false;
            }

            template< typename T >
            void validate_multiple_of( const T v )
            {
               switch ( m_node->m_flags & HAS_MULTIPLE_OF ) {
               case HAS_MULTIPLE_OF_UNSIGNED:
                  if ( ( v % m_node->m_multiple_of.u ) != 0 ) m_match = false;
                  break;
               case HAS_MULTIPLE_OF_DOUBLE:
                  if ( ! is_multiple_of( v, m_node->m_multiple_of.d ) ) m_match = false;
                  break;
               }
            }

            void validate_multiple_of( const double v )
            {
               switch ( m_node->m_flags & HAS_MULTIPLE_OF ) {
               case HAS_MULTIPLE_OF_UNSIGNED:
                  if ( ! is_multiple_of( v, m_node->m_multiple_of.u ) ) m_match = false;
                  break;
               case HAS_MULTIPLE_OF_DOUBLE:
                  if ( ! is_multiple_of( v, m_node->m_multiple_of.d ) ) m_match = false;
                  break;
               }
            }

            void validate_number( const std::int64_t v )
            {
               validate_multiple_of( v );
               switch ( m_node->m_flags & ( HAS_MAXIMUM | EXCLUSIVE_MAXIMUM ) ) {
               case HAS_MAXIMUM_SIGNED:
                  if ( v >= m_node->m_maximum.i ) m_match = false;
                  break;
               case HAS_MAXIMUM_SIGNED | EXCLUSIVE_MAXIMUM:
                  if ( v > m_node->m_maximum.i ) m_match = false;
                  break;
               case HAS_MAXIMUM_UNSIGNED:
                  if ( v >= 0 && static_cast< std::uint64_t >( v ) >= m_node->m_maximum.u ) m_match = false;
                  break;
               case HAS_MAXIMUM_UNSIGNED | EXCLUSIVE_MAXIMUM:
                  if ( v >= 0 && static_cast< std::uint64_t >( v ) > m_node->m_maximum.u ) m_match = false;
                  break;
               case HAS_MAXIMUM_DOUBLE:
                  if ( v >= m_node->m_maximum.d ) m_match = false;
                  break;
               case HAS_MAXIMUM_DOUBLE | EXCLUSIVE_MAXIMUM:
                  if ( v > m_node->m_maximum.d ) m_match = false;
                  break;
               }
               switch ( m_node->m_flags & ( HAS_MINIMUM | EXCLUSIVE_MINIMUM ) ) {
               case HAS_MINIMUM_SIGNED:
                  if ( v <= m_node->m_minimum.i ) m_match = false;
                  break;
               case HAS_MINIMUM_SIGNED | EXCLUSIVE_MINIMUM:
                  if ( v < m_node->m_minimum.i ) m_match = false;
                  break;
               case HAS_MINIMUM_UNSIGNED:
                  if ( v < 0 || static_cast< std::uint64_t >( v ) <= m_node->m_minimum.u ) m_match = false;
                  break;
               case HAS_MINIMUM_UNSIGNED | EXCLUSIVE_MINIMUM:
                  if ( v < 0 || static_cast< std::uint64_t >( v ) < m_node->m_minimum.u ) m_match = false;
                  break;
               case HAS_MINIMUM_DOUBLE:
                  if ( v <= m_node->m_minimum.d ) m_match = false;
                  break;
               case HAS_MINIMUM_DOUBLE | EXCLUSIVE_MINIMUM:
                  if ( v < m_node->m_minimum.d ) m_match = false;
                  break;
               }
            }

            void validate_number( const std::uint64_t v )
            {
               validate_multiple_of( v );
               switch ( m_node->m_flags & ( HAS_MAXIMUM | EXCLUSIVE_MAXIMUM ) ) {
               case HAS_MAXIMUM_SIGNED:
                  if ( m_node->m_maximum.i < 0 || v >= static_cast< std::uint64_t >( m_node->m_maximum.i ) ) m_match = false;
                  break;
               case HAS_MAXIMUM_SIGNED | EXCLUSIVE_MAXIMUM:
                  if ( m_node->m_maximum.i < 0 || v > static_cast< std::uint64_t >( m_node->m_maximum.i ) ) m_match = false;
                  break;
               case HAS_MAXIMUM_UNSIGNED:
                  if ( v >= m_node->m_maximum.u ) m_match = false;
                  break;
               case HAS_MAXIMUM_UNSIGNED | EXCLUSIVE_MAXIMUM:
                  if ( v > m_node->m_maximum.u ) m_match = false;
                  break;
               case HAS_MAXIMUM_DOUBLE:
                  if ( v >= m_node->m_maximum.d ) m_match = false;
                  break;
               case HAS_MAXIMUM_DOUBLE | EXCLUSIVE_MAXIMUM:
                  if ( v > m_node->m_maximum.d ) m_match = false;
                  break;
               }
               switch ( m_node->m_flags & ( HAS_MINIMUM | EXCLUSIVE_MINIMUM ) ) {
               case HAS_MINIMUM_SIGNED:
                  if ( m_node->m_minimum.i >= 0 && v <= static_cast< std::uint64_t >( m_node->m_minimum.i ) ) m_match = false;
                  break;
               case HAS_MINIMUM_SIGNED | EXCLUSIVE_MINIMUM:
                  if ( m_node->m_minimum.i >= 0 && v < static_cast< std::uint64_t >( m_node->m_minimum.i ) ) m_match = false;
                  break;
               case HAS_MINIMUM_UNSIGNED:
                  if ( v <= m_node->m_minimum.u ) m_match = false;
                  break;
               case HAS_MINIMUM_UNSIGNED | EXCLUSIVE_MINIMUM:
                  if ( v < m_node->m_minimum.u ) m_match = false;
                  break;
               case HAS_MINIMUM_DOUBLE:
                  if ( v <= m_node->m_minimum.d ) m_match = false;
                  break;
               case HAS_MINIMUM_DOUBLE | EXCLUSIVE_MINIMUM:
                  if ( v < m_node->m_minimum.d ) m_match = false;
                  break;
               }
            }

            void validate_number( const double v )
            {
               validate_multiple_of( v );
               switch ( m_node->m_flags & ( HAS_MAXIMUM | EXCLUSIVE_MAXIMUM ) ) {
               case HAS_MAXIMUM_SIGNED:
                  if ( v >= m_node->m_maximum.i ) m_match = false;
                  break;
               case HAS_MAXIMUM_SIGNED | EXCLUSIVE_MAXIMUM:
                  if ( v > m_node->m_maximum.i ) m_match = false;
                  break;
               case HAS_MAXIMUM_UNSIGNED:
                  if ( v >= m_node->m_maximum.u ) m_match = false;
                  break;
               case HAS_MAXIMUM_UNSIGNED | EXCLUSIVE_MAXIMUM:
                  if ( v > m_node->m_maximum.u ) m_match = false;
                  break;
               case HAS_MAXIMUM_DOUBLE:
                  if ( v >= m_node->m_maximum.d ) m_match = false;
                  break;
               case HAS_MAXIMUM_DOUBLE | EXCLUSIVE_MAXIMUM:
                  if ( v > m_node->m_maximum.d ) m_match = false;
                  break;
               }
               switch ( m_node->m_flags & ( HAS_MINIMUM | EXCLUSIVE_MINIMUM ) ) {
               case HAS_MINIMUM_SIGNED:
                  if ( v <= m_node->m_minimum.i ) m_match = false;
                  break;
               case HAS_MINIMUM_SIGNED | EXCLUSIVE_MINIMUM:
                  if ( v < m_node->m_minimum.i ) m_match = false;
                  break;
               case HAS_MINIMUM_UNSIGNED:
                  if ( v <= m_node->m_minimum.u ) m_match = false;
                  break;
               case HAS_MINIMUM_UNSIGNED | EXCLUSIVE_MINIMUM:
                  if ( v < m_node->m_minimum.u ) m_match = false;
                  break;
               case HAS_MINIMUM_DOUBLE:
                  if ( v <= m_node->m_minimum.d ) m_match = false;
                  break;
               case HAS_MINIMUM_DOUBLE | EXCLUSIVE_MINIMUM:
                  if ( v < m_node->m_minimum.d ) m_match = false;
                  break;
               }
            }

            void validate_string( const std::string & v )
            {
               if ( m_node->m_flags & HAS_MAX_LENGTH && v.size() > m_node->m_max_length ) {
                  m_match = false;
               }
               if ( m_node->m_flags & HAS_MIN_LENGTH && v.size() < m_node->m_min_length ) {
                  m_match = false;
               }
               if ( m_match && m_node->m_pattern ) {
                  if ( ! std::regex_search( v, * m_node->m_pattern ) ) {
                     m_match = false;
                  }
               }
               // TODO: format
            }

            void validate_elements( const std::size_t v )
            {
               if ( m_node->m_flags & HAS_MAX_ITEMS && v > m_node->m_max_items ) {
                  m_match = false;
               }
               if ( m_node->m_flags & HAS_MIN_ITEMS && v < m_node->m_min_items ) {
                  m_match = false;
               }
            }

            void validate_members( const std::size_t v )
            {
               if ( m_node->m_flags & HAS_MAX_PROPERTIES && v > m_node->m_max_properties ) {
                  m_match = false;
               }
               if ( m_node->m_flags & HAS_MIN_PROPERTIES && v < m_node->m_min_properties ) {
                  m_match = false;
               }
            }

         public:
            schema_consumer( const std::shared_ptr< const schema_container< Traits > > & c, const schema_node< Traits > & n )
                 : m_container( c ),
                   m_node( & n ),
                   m_required( n.m_required )
            {
               if ( m_node->m_flags & HAS_ENUM ) {
                  const auto & a = m_node->m_value->unsafe_at( "enum" ).unsafe_get_array();
                  m_enum.reserve( a.size() );
                  for ( const auto & e : a ) {
                     m_enum.emplace_back( new sax_compare< Traits >() );
                     m_enum.back()->push( & e );
                  }
               }
               if ( const auto * p = m_node->find( "allOf" ) ) {
                  for ( const auto & e : p->unsafe_get_array() ) {
                     const auto it = m_container->m_nodes.find( e.skip_raw_ptr() );
                     assert( it != m_container->m_nodes.end() );
                     m_all_of.emplace_back( new schema_consumer( m_container, * it->second ) );
                  }
               }
               if ( const auto * p = m_node->find( "anyOf" ) ) {
                  for ( const auto & e : p->unsafe_get_array() ) {
                     const auto it = m_container->m_nodes.find( e.skip_raw_ptr() );
                     assert( it != m_container->m_nodes.end() );
                     m_any_of.emplace_back( new schema_consumer( m_container, * it->second ) );
                  }
               }
               if ( const auto * p = m_node->find( "oneOf" ) ) {
                  for ( const auto & e : p->unsafe_get_array() ) {
                     const auto it = m_container->m_nodes.find( e.skip_raw_ptr() );
                     assert( it != m_container->m_nodes.end() );
                     m_one_of.emplace_back( new schema_consumer( m_container, * it->second ) );
                  }
               }
               if ( const auto * p = m_node->find( "not" ) ) {
                  const auto it = m_container->m_nodes.find( p );
                  assert( it != m_container->m_nodes.end() );
                  m_not.reset( new schema_consumer( m_container, * it->second ) );
               }
            }

            schema_consumer( const schema_consumer & ) = delete;
            schema_consumer( schema_consumer && ) = delete;

            void operator= ( const schema_consumer & ) = delete;
            void operator= ( schema_consumer && ) = delete;

            void finalize()
            {
               if ( m_match && ( m_one_of.size() > 1 ) ) m_match = false;
               if ( m_match && m_not ) m_match = false;
            }

            bool match() const noexcept
            {
               return m_match;
            }

            void null()
            {
               if ( m_match ) validate_type( NULL_ );
               if ( m_match ) validate_enum( []( sax_compare< Traits > & c ){ c.null(); return ! c.match(); } );
               if ( m_match ) validate_collections( []( schema_consumer & c ){ c.null(); return ! c.match(); } );
               if ( m_match && m_hash ) m_hash->null();
            }

            void boolean( const bool v )
            {
               if ( m_match ) validate_type( BOOLEAN );
               if ( m_match ) validate_enum( [=]( sax_compare< Traits > & c ){ c.boolean( v ); return ! c.match(); } );
               if ( m_match ) validate_collections( [=]( schema_consumer & c ){ c.boolean( v ); return ! c.match(); } );
               if ( m_match && m_hash ) m_hash->boolean( v );
            }

            void number( const std::int64_t v )
            {
               if ( m_match ) validate_type( INTEGER | NUMBER );
               if ( m_match ) validate_enum( [=]( sax_compare< Traits > & c ){ c.number( v ); return ! c.match(); } );
               if ( m_match ) validate_collections( [=]( schema_consumer & c ){ c.number( v ); return ! c.match(); } );
               if ( m_match && m_count.empty() ) validate_number( v );
               if ( m_match && m_hash ) m_hash->number( v );
            }

            void number( const std::uint64_t v )
            {
               if ( m_match ) validate_type( INTEGER | NUMBER );
               if ( m_match ) validate_enum( [=]( sax_compare< Traits > & c ){ c.number( v ); return ! c.match(); } );
               if ( m_match ) validate_collections( [=]( schema_consumer & c ){ c.number( v ); return ! c.match(); } );
               if ( m_match && m_count.empty() ) validate_number( v );
               if ( m_match && m_hash ) m_hash->number( v );
            }

            void number( const double v )
            {
               if ( m_match ) validate_type( NUMBER );
               if ( m_match ) validate_enum( [=]( sax_compare< Traits > & c ){ c.number( v ); return ! c.match(); } );
               if ( m_match ) validate_collections( [=]( schema_consumer & c ){ c.number( v ); return ! c.match(); } );
               if ( m_match && m_count.empty() ) validate_number( v );
               if ( m_match && m_hash ) m_hash->number( v );
            }

            void string( const std::string & v )
            {
               if ( m_match ) validate_type( STRING );
               if ( m_match ) validate_enum( [&]( sax_compare< Traits > & c ){ c.string( v ); return ! c.match(); } );
               if ( m_match ) validate_collections( [&]( schema_consumer & c ){ c.string( v ); return ! c.match(); } );
               if ( m_match && m_count.empty() ) validate_string( v );
               if ( m_match && m_hash ) m_hash->string( v );
            }

            // array
            void begin_array()
            {
               if ( m_match ) validate_type( ARRAY );
               if ( m_match ) validate_enum( []( sax_compare< Traits > & c ){ c.begin_array(); return ! c.match(); } );
               if ( m_match ) validate_collections( []( schema_consumer & c ){ c.begin_array(); return ! c.match(); } );
               if ( m_match ) {
                  if ( m_hash ) {
                     m_hash->begin_array();
                  }
                  else if ( m_count.empty() && ( ( m_node->m_flags & HAS_UNIQUE_ITEMS ) != 0 ) ) {
                     m_hash.reset( new sax::hash );
                  }
               }
               m_count.push_back( 0 );
            }

            void element()
            {
               if ( m_match ) validate_enum( []( sax_compare< Traits > & c ){ c.element(); return ! c.match(); } );
               if ( m_match ) validate_collections( []( schema_consumer & c ){ c.element(); return ! c.match(); } );
               if ( m_match && m_hash ) {
                  if ( m_count.empty() ) {
                     if ( ! m_unique.emplace( m_hash->value() ).second ) {
                        m_match = false;
                     }
                     m_hash->reset();
                  }
                  else {
                     m_hash->element();
                  }
               }
               ++m_count.back();
            }

            void end_array()
            {
               if ( m_match ) validate_enum( []( sax_compare< Traits > & c ){ c.end_array(); return ! c.match(); } );
               if ( m_match ) validate_collections( []( schema_consumer & c ){ c.end_array(); return ! c.match(); } );
               if ( m_match && m_hash ) m_hash->end_array();
               if ( m_match && ( m_count.size() == 1 ) ) validate_elements( m_count.back() );
               m_count.pop_back();
            }

            // object
            void begin_object()
            {
               if ( m_match ) validate_type( OBJECT );
               if ( m_match ) validate_enum( []( sax_compare< Traits > & c ){ c.begin_object(); return ! c.match(); } );
               if ( m_match ) validate_collections( []( schema_consumer & c ){ c.begin_object(); return ! c.match(); } );
               if ( m_match && m_hash ) m_hash->begin_object();
               m_count.push_back( 0 );
            }

            void key( const std::string & v )
            {
               if ( m_match ) validate_enum( [&]( sax_compare< Traits > & c ){ c.key( v ); return ! c.match(); } );
               if ( m_match ) validate_collections( [&]( schema_consumer & c ){ c.key( v ); return ! c.match(); } );
               if ( m_match && m_hash ) m_hash->key( v );
               if ( m_match ) m_required.erase( v );
               if ( m_match && m_properties.empty() && ( m_count.size() == 1 ) ) {
                  if ( const auto * p = m_node->find( "properties" ) ) {
                     const auto & o = p->unsafe_get_object();
                     const auto it = o.find( v );
                     if ( it != o.end() ) {
                        const auto jt = m_container->m_nodes.find( it->second.skip_raw_ptr() );
                        assert( jt != m_container->m_nodes.end() );
                        m_properties.emplace_back( new schema_consumer( m_container, * jt->second ) );
                     }
                  }
                  if ( const auto * p = m_node->find( "patternProperties" ) ) {
                     for ( const auto & e : p->unsafe_get_object() ) {
                        std::regex re( e.first );
                        if ( std::regex_search( v, re ) ) {
                           const auto it = m_container->m_nodes.find( & e.second );
                           assert( it != m_container->m_nodes.end() );
                           m_properties.emplace_back( new schema_consumer( m_container, * it->second ) );
                        }
                     }
                  }
                  if ( m_properties.empty() ) {
                     if ( const auto * p = m_node->find( "additionalProperties" ) ) {
                        if ( p->is_boolean() ) {
                           if ( ! p->unsafe_get_boolean() ) {
                              m_match = false;
                           }
                        }
                        else {
                           const auto it = m_container->m_nodes.find( p );
                           assert( it != m_container->m_nodes.end() );
                           m_properties.emplace_back( new schema_consumer( m_container, * it->second ) );
                        }
                     }
                  }
               }
            }

            void member()
            {
               if ( m_match ) validate_enum( []( sax_compare< Traits > & c ){ c.member(); return ! c.match(); } );
               if ( m_match && ! m_properties.empty() && ( m_count.size() == 1 ) ) {
                  for ( auto & e : m_properties ) {
                     e->finalize();
                     if ( ! e->match() ) {
                        m_match = false;
                        break;
                     }
                  }
                  m_properties.clear();
               }
               if ( m_match ) validate_collections( []( schema_consumer & c ){ c.member(); return ! c.match(); } );
               if ( m_match && m_hash ) m_hash->member();
               ++m_count.back();
            }

            void end_object()
            {
               if ( m_match ) validate_enum( []( sax_compare< Traits > & c ){ c.end_object(); return ! c.match(); } );
               if ( m_match ) validate_collections( []( schema_consumer & c ){ c.end_object(); return ! c.match(); } );
               if ( m_match && m_hash ) m_hash->end_object();
               if ( m_match && ( m_count.size() == 1 ) ) validate_members( m_count.back() );
               if ( m_match && ! m_required.empty() ) m_match = false; // missing required members
               m_count.pop_back();
            }
         };

         template< template< typename ... > class Traits >
         class schema_container
              : public std::enable_shared_from_this< schema_container< Traits > >
         {
         private:
            basic_value< Traits > m_value;
            friend struct schema_node< Traits >;
            friend class schema_consumer< Traits >;

            using nodes_t = std::map< const basic_value< Traits > *, std::unique_ptr< schema_node< Traits > > >;
            const nodes_t m_nodes;

            const schema_node< Traits > * m_root;

            nodes_t parse()
            {
               nodes_t result;
               resolve_references( m_value );
               m_root = result.emplace( & m_value, std::unique_ptr< schema_node< Traits > >( new schema_node< Traits >( this, m_value ) ) ).first->second.get();
               while ( true ) {
                  std::set< const basic_value< Traits > * > required;
                  for ( const auto & e : result ) {
                     auto s = e.second->referenced_pointers();
                     required.insert( s.begin(), s.end() );
                  }
                  for ( const auto & e : result ) {
                     required.erase( e.first );
                  }
                  if ( required.empty() ) {
                     break;
                  }
                  for ( const auto & e : required ) {
                     result.emplace( e, std::unique_ptr< schema_node< Traits > >( new schema_node< Traits >( this, * e ) ) );
                  }
               }
               return result;
            }

         public:
            explicit schema_container( const basic_value< Traits > & v )
                 : m_value( * v.skip_raw_ptr() ),
                   m_nodes( parse() )
            { }

            std::unique_ptr< schema_consumer< Traits > > consumer() const
            {
               return std::unique_ptr< schema_consumer< Traits > >( new schema_consumer< Traits >( this->shared_from_this(), * m_root ) );
            }
         };

      } // internal

      template< template< typename ... > class Traits >
      class basic_schema
      {
      private:
         const std::shared_ptr< const internal::schema_container< Traits > > m_container;

      public:
         explicit basic_schema( const basic_value< Traits > & v )
              : m_container( std::make_shared< internal::schema_container< Traits > >( v ) )
         { }

         std::unique_ptr< internal::schema_consumer< Traits > > consumer() const
         {
            return m_container->consumer();
         }

         bool validate( const basic_value< Traits > & v ) const
         {
            // TODO: DOM validation should be implemented independently,
            // as it could be more efficient than SAX validation!
            const auto c = consumer();
            sax::traverse_value( v, * c );
            c->finalize();
            return c->match();
         }
      };

      using schema = basic_schema< traits >;

      template< template< typename ... > class Traits >
      basic_schema< Traits > make_schema( const basic_value< Traits > & v )
      {
         return basic_schema< Traits >( v );
      }

   } // json

} // tao

#endif
