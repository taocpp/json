#include <stddef.h>
#include <stdexcept>

#include <tao/json.hpp>

// extern "C" int FUZZ_INIT_WITH_ARGS(int *argc, char ***argv) {
extern "C" int FUZZ_INIT()
{
   // Add global setup code here - called once before fuzzing starts

   return 0;  // Non-zero return values are reserved for future use.
}

extern "C" int FUZZ( const char* data, size_t size )
{
   // process fuzzer input (*data) and setup local objects necessary to call the function under test

   try {
      tao::json::events::limit_nesting_depth< tao::json::events::validate_event_order, 15 > consumer;
      tao::json::events::from_string( consumer, data, size );

      if( !consumer.is_complete() ) {
         throw std::logic_error( "incomplete consumer state after input" );
      }
   }
   catch( const tao::pegtl::parse_error& ) {
   }
   catch( const std::runtime_error& ) {
   }

   // reset state and free all locally allocated resources

   return 0;  // Non-zero return values are reserved for future use.
}
