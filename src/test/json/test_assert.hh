// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SRC_TEST_TEST_ASSERT_HH
#define TAOCPP_JSON_INCLUDE_SRC_TEST_TEST_ASSERT_HH

#include <iostream>

#define TEST_ASSERT( eXPReSSioN )                       \
   do {                                                 \
   if ( ! ( eXPReSSioN ) ) {                            \
      std::cerr << "json: unit test assert [ "          \
                << ( # eXPReSSioN )                     \
                << " ] failed in line [ "               \
                << __LINE__                             \
                << " ] file [ "                         \
                << __FILE__ << " ]"                     \
                << std::endl;                           \
      ++failed;                                         \
   }  } while ( 0 )

#define TEST_THROWS( STaTeMeNT )                        \
   do {                                                 \
      try {                                             \
         STaTeMeNT;                                     \
         std::cerr << "json: unit test assert [ "       \
                   << ( # STaTeMeNT )                   \
                   << " ] did not throw in line [ "     \
                   << __LINE__                          \
                   << " ] file [ "                      \
                   << __FILE__ << " ]"                  \
                   << std::endl;                        \
         ++failed;                                      \
      }                                                 \
      catch ( ... ) {                                   \
      }                                                 \
   } while ( 0 )

#endif
