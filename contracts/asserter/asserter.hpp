/**
 *  @file
 *  @copyright defined in Achainplus/LICENSE
 */

#include <eosiolib/eosio.hpp>

namespace asserter {
   struct assertdef {
      int8_t      condition;
      std::string message;

      EOSLIB_SERIALIZE( assertdef, (condition)(message) )
   };
}
