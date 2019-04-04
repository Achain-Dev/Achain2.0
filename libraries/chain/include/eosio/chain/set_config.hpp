/**
 *  @file
 *  @copyright defined in actm/LICENSE.txt
 */
#pragma once

#include <eosio/chain/types.hpp>
#include <eosio/chain/asset.hpp>
#include <eosio/chain/multi_index_includes.hpp>
#include <eosio/chain/contract_types.hpp>
#include <eosio/chain/apply_context.hpp>


namespace eosio { namespace chain {

class config_data_object;

namespace setconf{
   namespace res_type{
      static const auto cpu_per_fee = N(r.cpufee);
      static const auto net_per_fee = N(r.netfee);
      static const auto free_ram_per_account = N(r.freeram);
   };


   namespace func_type{
      static const auto vote_for_ram = N(f.ram4vote);
      static const auto onfee_action = N(f.onfeeact);
   }


   namespace default_value{
      static const auto default_config_value = -1;
      static const auto default_config_key = N(k.null);
   }
};

// some spec type for fast
class config_data_object : public chainbase::object<config_data_object_type, config_data_object> {
   OBJECT_CTOR(config_data_object);
   id_type id;
   account_name name;
   int64_t value = setconf::default_value::default_config_value;
   account_name key = setconf::default_value::default_config_key;
   asset asset_info;
   string desc = "";
};


struct by_name;
using config_data_object_index = chainbase::shared_multi_index_container<
      config_data_object,
      indexed_by<
            ordered_unique< tag<by_id>,member<config_data_object, config_data_object::id_type, &config_data_object::id>>,
            ordered_unique< tag<by_name>,member<config_data_object, account_name, &config_data_object::name>>
      >
>;

int64_t get_config_value( const chainbase::database& db, const name& name);

account_name get_config_key( const chainbase::database& db, const name& name);

asset get_config_asset(const chainbase::database& db, const name& name);


//set chain cfg
void set_config( chainbase::database& db, const setconfig &cfg );

// is_func_has_open is a func is open
bool is_func_open( const controller& ctl, const name &func_typ, const int64_t default_open_block = 0);

} } /// namespace eosio::chain

FC_REFLECT(eosio::chain::config_data_object, (name)(value)(key)(asset_info)(desc))
CHAINBASE_SET_INDEX_TYPE(eosio::chain::config_data_object, eosio::chain::config_data_object_index)


