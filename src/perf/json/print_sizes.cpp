// Copyright (c) 2015-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include <iostream>
#include <list>
#include <unordered_map>

#include <tao/json/pointer.hpp>
#include <tao/json/value.hpp>

int main( int /*unused*/, char** /*unused*/ )
{
   std::cout << "json::value: " << sizeof( tao::json::value ) << std::endl;
   std::cout << "json::pointer: " << sizeof( tao::json::pointer ) << std::endl;

   std::cout << "json::internal::value_union< json::value >: " << sizeof( tao::json::internal::value_union< tao::json::value > ) << std::endl;

   std::cout << "std::string: " << sizeof( std::string ) << std::endl;
   std::cout << "std::string_view: " << sizeof( std::string_view ) << std::endl;

   std::cout << "std::vector< json::value >: " << sizeof( std::vector< tao::json::value > ) << std::endl;
   std::cout << "std::list< json::value >: " << sizeof( std::list< tao::json::value > ) << std::endl;

   std::cout << "std::map< std::string, json::value, std::less<> >: " << sizeof( std::map< std::string, tao::json::value, std::less<> > ) << std::endl;
   std::cout << "std::multimap< std::string, json::value >: " << sizeof( std::multimap< std::string, tao::json::value > ) << std::endl;
   std::cout << "std::unordered_map< std::string, json::value >: " << sizeof( std::unordered_map< std::string, tao::json::value > ) << std::endl;
   std::cout << "std::unordered_multimap< std::string, json::value >: " << sizeof( std::unordered_multimap< std::string, tao::json::value > ) << std::endl;

   return 0;
}
