/**
 *  Some config data in chain state db
 *  @copyright defined in actm/LICENSE.txt
 */

#include <eosio/chain/set_config.hpp>
#include <chainbase/chainbase.hpp>
#include <eosio/chain/contract_types.hpp>
#include <eosio/chain/apply_context.hpp>

namespace eosio { namespace chain {

//get config.value by name
int64_t get_config_value( const chainbase::database& db, const name& name)
{
   const auto cfg_itr = db.find<config_data_object, by_name>(name);

   if( cfg_itr == nullptr ) {
      return eosio::chain::setconf::default_value::default_config_value;
   }

   return cfg_itr->value;
}

void set_config( chainbase::database& db, const setconfig &cfg ) {
   auto itr = db.find<config_data_object, by_name>(cfg.name);
   if( itr == nullptr ) {
      ilog("set num config ${n} to ${v}", ( "n", cfg.name )("v", cfg));
      db.create<config_data_object>([&]( auto& c ) {
         c.name = cfg.name;
         c.value = cfg.value;
         c.key = cfg.key;
         c.asset_info = cfg.asset_info;
         c.desc = cfg.desc;
      });
   } else {
      db.modify<config_data_object>(*itr, [&]( auto& c ) {
         c.value = cfg.value;
         c.key = cfg.key;
         c.asset_info = cfg.asset_info;
         c.desc = cfg.desc;
      });
   }
}

bool is_func_open( const controller& ctl, const name &func_typ, const int64_t default_open_block) {
   const auto head_num = static_cast<int64_t>( ctl.head_block_num() );
   const auto open_num = get_config_value( ctl.db(), func_typ );
    
   return (head_num >= 0) && ((open_num >= 0 && head_num >= open_num)
                              || (open_num == setconf::default_value::default_config_value && default_open_block != 0 && head_num >= default_open_block));
}

} }  /// eosio::chain

