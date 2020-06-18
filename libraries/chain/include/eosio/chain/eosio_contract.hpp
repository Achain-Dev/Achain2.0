/**
 *  @file
 *  @copyright defined in Achainplus/LICENSE
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
   void apply_act_newaccount(apply_context&);
   void apply_act_updateauth(apply_context&);
   void apply_act_deleteauth(apply_context&);
   void apply_act_linkauth(apply_context&);
   void apply_act_unlinkauth(apply_context&);

   void apply_act_setcode(apply_context&);
   void apply_act_setabi(apply_context&);

   void apply_act_canceldelay(apply_context&);
   //add for achainplus
   void apply_act_setconfig(apply_context&);
   ///@}  end action handlers

} } /// namespace eosio::chain
