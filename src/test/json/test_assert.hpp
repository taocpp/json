// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SRC_TEST_TEST_ASSERT_HPP
#define TAOCPP_JSON_INCLUDE_SRC_TEST_TEST_ASSERT_HPP

#include <iostream>
#include <stdexcept>

#define TEST_ASSERT( ... )                                         \
   do {                                                            \
      std::cout << "TEST_ASSERT( " #__VA_ARGS__ " )" << std::endl; \
      try {                                                        \
         if( !( __VA_ARGS__ ) ) {                                  \
            std::cerr << "json: unit test assert [ "               \
                      << ( #__VA_ARGS__ )                          \
                      << " ] failed in line [ "                    \
                      << __LINE__                                  \
                      << " ] file [ "                              \
                      << __FILE__ << " ]"                          \
                      << std::endl;                                \
            ++failed;                                              \
         }                                                         \
      }                                                            \
      catch( const std::exception& ex ) {                          \
         std::cerr << "json: unit test [ "                         \
                   << ( #__VA_ARGS__ )                             \
                   << " ] threw an exception [ "                   \
                   << ex.what()                                    \
                   << " ] in line [ "                              \
                   << __LINE__                                     \
                   << " ] file [ "                                 \
                   << __FILE__ << " ]"                             \
                   << std::endl;                                   \
         ++failed;                                                 \
      }                                                            \
      catch( ... ) {                                               \
         std::cerr << "json: unit test [ "                         \
                   << ( #__VA_ARGS__ )                             \
                   << " ] threw an unknown exception"              \
                   << "in line [ "                                 \
                   << __LINE__                                     \
                   << " ] file [ "                                 \
                   << __FILE__ << " ]"                             \
                   << std::endl;                                   \
         ++failed;                                                 \
      }                                                            \
   } while( false )

#define TEST_THROWS( ... )                                         \
   do {                                                            \
      std::cout << "TEST_THROWS( " #__VA_ARGS__ " )" << std::endl; \
      try {                                                        \
         __VA_ARGS__;                                              \
         std::cerr << "json: unit test [ "                         \
                   << ( #__VA_ARGS__ )                             \
                   << " ] did not throw in line [ "                \
                   << __LINE__                                     \
                   << " ] file [ "                                 \
                   << __FILE__ << " ]"                             \
                   << std::endl;                                   \
         ++failed;                                                 \
      }                                                            \
      catch( ... ) {                                               \
      }                                                            \
   } while( false )

#endif
