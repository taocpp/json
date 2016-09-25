// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_INTERNAL_DIGEST_HH
#define TAOCPP_JSON_INCLUDE_INTERNAL_DIGEST_HH

#include <memory>
#include <stdexcept>
#include <string>

#include <openssl/evp.h>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         class digest
         {
         private:
            const std::unique_ptr< EVP_MD_CTX, void (*)( EVP_MD_CTX * ) > mdctx_;
            const ::EVP_MD * md_;

         public:
            explicit digest( const ::EVP_MD * md )
                 : mdctx_( ::EVP_MD_CTX_create(), &::EVP_MD_CTX_destroy ),
                   md_( md )
            {
               reset();
            }

            void reset()
            {
               if ( ::EVP_DigestInit_ex( mdctx_.get(), md_, nullptr ) == 0 ) {
                  throw std::runtime_error( "::EVP_DigestInit_ex() failed" ); // LCOV_EXCL_LINE
               }
            }

            void feed( const void * data, const std::size_t size )
            {
               if ( ::EVP_DigestUpdate( mdctx_.get(), data, size ) == 0 ) {
                  throw std::runtime_error( "::EVP_DigestUpdate() failed" ); // LCOV_EXCL_LINE
               }
            }

            void feed( const std::string & data )
            {
               feed( data.data(), data.size() );
            }

            std::size_t store_unsafe( unsigned char * buffer ) const
            {
               unsigned length;
               if ( ::EVP_DigestFinal_ex( mdctx_.get(), buffer, & length ) == 0 ) {
                 throw std::runtime_error( "::EVP_DigestFinal_ex() failed" ); // LCOV_EXCL_LINE
               }
               return length;
            }

            std::string get() const
            {
               unsigned char buffer[ EVP_MAX_MD_SIZE ];
               const auto length = store_unsafe( buffer );
               return std::string( (const char *)buffer, length );
            }
         };

      } // internal

   } // json

} // tao

#endif
