#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <tao/json.hpp>

// extern "C" int FUZZ_INIT_WITH_ARGS(int *argc, char ***argv) {
extern "C" int FUZZ_INIT()
{
   // Add global setup code here - called once before fuzzing starts

   return 0;  // Non-zero return values are reserved for future use.
}

extern "C" int FUZZ( const char* Data, size_t Size )
{
   // process fuzzer input (*Data) and setup local objects necessary to call the function under test

   std::string input( Data, Size );

   try {
      tao::json::events::limit_nesting_depth< tao::json::events::validate_event_order, 15 > lnd;
      tao::json::cbor::events::from_string( lnd, input );
   }
   catch( const tao::pegtl::parse_error& ) {
   }
   catch( const std::logic_error& ) {
   }
   catch( const std::runtime_error& rtex ) {
   }

   // reset state and free all locally allocated resources

   return 0;  // Non-zero return values are reserved for future use.
}
