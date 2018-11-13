/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosio/chain/types.hpp>
#include <eosio/chain/contract_types.hpp>

namespace eosio { namespace chain {

   class apply_context;

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_actx_newaccount(apply_context&);
   void apply_actx_updateauth(apply_context&);
   void apply_actx_deleteauth(apply_context&);
   void apply_actx_linkauth(apply_context&);
   void apply_actx_unlinkauth(apply_context&);

   void apply_actx_setcode(apply_context&);
   void apply_actx_setabi(apply_context&);

   void apply_actx_canceldelay(apply_context&);
   ///@}  end action handlers

} } /// namespace eosio::chain
