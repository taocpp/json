// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/data.hpp>
#include <tao/json/pointer.hpp>
#include <tao/json/value.hpp>

#include <list>
#include <unordered_map>

namespace tao
{
   namespace json
   {
      void unit_test()
      {
         std::cout << "json::data: " << sizeof( data ) << std::endl;
         std::cout << "json::custom_value: " << sizeof( custom_value ) << std::endl;
         std::cout << "json::pointer: " << sizeof( pointer ) << std::endl;

         std::cout << "json::internal::data_union: " << sizeof( internal::data_union ) << std::endl;

         std::cout << "std::string: " << sizeof( std::string ) << std::endl;

         std::cout << "std::vector< json::data >: " << sizeof( std::vector< data > ) << std::endl;
         std::cout << "std::list< json::data >: " << sizeof( std::list< data > ) << std::endl;

         std::cout << "std::map< std::string, json::data >: " << sizeof( std::map< std::string, data > ) << std::endl;
         std::cout << "std::multimap< std::string, json::data >: " << sizeof( std::multimap< std::string, data > ) << std::endl;
         std::cout << "std::unordered_map< std::string, json::data >: " << sizeof( std::unordered_map< std::string, data > ) << std::endl;
         std::cout << "std::unordered_multimap< std::string, json::data >: " << sizeof( std::unordered_multimap< std::string, data > ) << std::endl;
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
