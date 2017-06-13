// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/json5/parse_file.hpp>

namespace tao
{
   namespace json
   {
      bool test_file( const char* name )
      {
         json5::parse_file( name );
         return true;
      }

      void unit_test()
      {
         TEST_ASSERT( test_file( "tests/json5/arrays/empty-array.json" ) );
         TEST_THROWS( test_file( "tests/json5/arrays/leading-comma-array.js" ) );
         TEST_THROWS( test_file( "tests/json5/arrays/lone-trailing-comma-array.js" ) );
         TEST_THROWS( test_file( "tests/json5/arrays/no-comma-array.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/arrays/regular-array.json" ) );
         TEST_ASSERT( test_file( "tests/json5/arrays/trailing-comma-array.json5" ) );

         TEST_ASSERT( test_file( "tests/json5/comments/block-comment-following-array-element.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/comments/block-comment-following-top-level-value.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/comments/block-comment-in-string.json" ) );
         TEST_ASSERT( test_file( "tests/json5/comments/block-comment-preceding-top-level-value.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/comments/block-comment-with-asterisks.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/comments/inline-comment-following-array-element.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/comments/inline-comment-following-top-level-value.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/comments/inline-comment-in-string.json" ) );
         TEST_ASSERT( test_file( "tests/json5/comments/inline-comment-preceding-top-level-value.json5" ) );
         TEST_THROWS( test_file( "tests/json5/comments/top-level-block-comment.txt" ) );
         TEST_THROWS( test_file( "tests/json5/comments/top-level-inline-comment.txt" ) );
         TEST_THROWS( test_file( "tests/json5/comments/unterminated-block-comment.txt" ) );

         TEST_THROWS( test_file( "tests/json5/misc/empty.txt" ) );
         TEST_THROWS( test_file( "tests/json5/misc/invalid-whitespace.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/misc/npm-package.json" ) );
         TEST_ASSERT( test_file( "tests/json5/misc/npm-package.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/misc/readme-example.json5" ) );

         // TEST_ASSERT( test_file( "tests/json5/new-lines/comment-cr.json5" ) ); // depends on eol-mode
         TEST_ASSERT( test_file( "tests/json5/new-lines/comment-crlf.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/new-lines/comment-lf.json5" ) );
         // TEST_ASSERT( test_file( "tests/json5/new-lines/escaped-cr.json5" ) ); // depends on eol-mode
         TEST_ASSERT( test_file( "tests/json5/new-lines/escaped-crlf.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/new-lines/escaped-lf.json5" ) );

         TEST_ASSERT( test_file( "tests/json5/numbers/float.json" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/float-leading-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/float-leading-zero.json" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/float-trailing-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/float-trailing-decimal-point-with-integer-exponent.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/float-with-integer-exponent.json" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/hexadecimal-empty.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/hexadecimal.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/hexadecimal-lowercase-letter.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/hexadecimal-uppercase-x.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/hexadecimal-with-integer-exponent.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/infinity.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/integer.json" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/integer-with-float-exponent.txt" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/integer-with-hexadecimal-exponent.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/integer-with-integer-exponent.json" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/integer-with-negative-float-exponent.txt" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/integer-with-negative-hexadecimal-exponent.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/integer-with-negative-integer-exponent.json" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/integer-with-negative-zero-integer-exponent.json" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/integer-with-positive-float-exponent.txt" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/integer-with-positive-hexadecimal-exponent.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/integer-with-positive-integer-exponent.json" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/integer-with-positive-zero-integer-exponent.json" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/integer-with-zero-integer-exponent.json" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/lone-decimal-point.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/nan.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-float.json" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-float-leading-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-float-leading-zero.json" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-float-trailing-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-hexadecimal.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-infinity.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-integer.json" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/negative-noctal.js" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/negative-octal.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-zero-float.json" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-zero-float-leading-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-zero-float-trailing-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-zero-hexadecimal.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/negative-zero-integer.json" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/negative-zero-octal.txt" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/noctal.js" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/noctal-with-leading-octal-digit.js" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/octal.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-float.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-float-leading-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-float-leading-zero.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-float-trailing-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-hexadecimal.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-infinity.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-integer.json5" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/positive-noctal.js" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/positive-octal.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-zero-float.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-zero-float-leading-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-zero-float-trailing-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-zero-hexadecimal.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/positive-zero-integer.json5" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/positive-zero-octal.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/zero-float.json" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/zero-float-leading-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/zero-float-trailing-decimal-point.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/zero-hexadecimal.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/zero-integer.json" ) );
         TEST_ASSERT( test_file( "tests/json5/numbers/zero-integer-with-integer-exponent.json" ) );
         TEST_THROWS( test_file( "tests/json5/numbers/zero-octal.txt" ) );

         TEST_ASSERT( test_file( "tests/json5/objects/duplicate-keys.json" ) );
         TEST_ASSERT( test_file( "tests/json5/objects/empty-object.json" ) );
         TEST_THROWS( test_file( "tests/json5/objects/illegal-unquoted-key-number.txt" ) );
         TEST_THROWS( test_file( "tests/json5/objects/illegal-unquoted-key-symbol.txt" ) );
         TEST_THROWS( test_file( "tests/json5/objects/leading-comma-object.txt" ) );
         TEST_THROWS( test_file( "tests/json5/objects/lone-trailing-comma-object.txt" ) );
         TEST_THROWS( test_file( "tests/json5/objects/no-comma-object.txt" ) );
         TEST_ASSERT( test_file( "tests/json5/objects/reserved-unquoted-key.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/objects/single-quoted-key.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/objects/trailing-comma-object.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/objects/unquoted-keys.json5" ) );

         TEST_ASSERT( test_file( "tests/json5/strings/escaped-single-quoted-string.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/strings/multi-line-string.json5" ) );
         TEST_ASSERT( test_file( "tests/json5/strings/single-quoted-string.json5" ) );
         TEST_THROWS( test_file( "tests/json5/strings/unescaped-multi-line-string.txt" ) );
      }

   }  // namespace json

}  // namespace tao

#include "main.hpp"
