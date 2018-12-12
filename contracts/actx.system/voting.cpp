/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include "actx.system.hpp"

#include <eosiolib/eosio.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/print.hpp>
#include <eosiolib/datastream.hpp>
#include <eosiolib/serialize.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/privileged.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/transaction.hpp>
#include <actx.token/actx.token.hpp>

#include <algorithm>
#include <cmath>

namespace eosiosystem {
   using eosio::indexed_by;
   using eosio::const_mem_fun;
   using eosio::bytes;
   using eosio::print;
   using eosio::singleton;
   using eosio::transaction;

   /**
    *  This method will create a producer_config and producer_info object for 'producer'
    *
    *  @pre producer is not already registered
    *  @pre producer to register is an account
    *  @pre authority of producer to register
    *
    */
   void system_contract::regproducer( const account_name producer, const eosio::public_key& producer_key, const std::string& url, uint16_t location ) {
      eosio_assert( url.size() < 512, "url too long" );
      eosio_assert( producer_key != eosio::public_key(), "public key should not be the default value" );
      require_auth( producer );

      auto prod = _producers.find( producer );

      if ( prod != _producers.end() ) {
         _producers.modify( prod, producer, [&]( producer_info& info ){
               info.producer_key = producer_key;
               info.is_active    = true;
               info.url          = url;
               info.location     = location;
            });
      } else {
         _producers.emplace( producer, [&]( producer_info& info ){
               info.owner         = producer;
               info.total_votes   = 0;
               info.producer_key  = producer_key;
               info.is_active     = true;
               info.url           = url;
               info.location      = location;
         });
      }
   }

   void system_contract::unregprod( const account_name producer ) {
      require_auth( producer );

      const auto& prod = _producers.get( producer, "producer not found" );

      _producers.modify( prod, 0, [&]( producer_info& info ){
            info.deactivate();
      });
   }

   void system_contract::update_elected_producers( block_timestamp block_time ) {
      _gstate.last_producer_schedule_update = block_time;

      auto idx = _producers.get_index<N(prototalvote)>();

      std::vector< std::pair<eosio::producer_key,uint16_t> > top_producers;
      top_producers.reserve(21);

      for ( auto it = idx.cbegin(); it != idx.cend() && top_producers.size() < 21 && 0 < it->total_votes && it->active(); ++it ) {
         top_producers.emplace_back( std::pair<eosio::producer_key,uint16_t>({{it->owner, it->producer_key}, it->location}) );
      }

      if ( top_producers.size() < _gstate.last_producer_schedule_size ) {
         return;
      }

      /// sort by producer name
      std::sort( top_producers.begin(), top_producers.end() );

      std::vector<eosio::producer_key> producers;

      producers.reserve(top_producers.size());
      for( const auto& item : top_producers )
         producers.push_back(item.first);

      bytes packed_schedule = pack(producers);

      if( set_proposed_producers( packed_schedule.data(),  packed_schedule.size() ) >= 0 ) {
         _gstate.last_producer_schedule_size = static_cast<decltype(_gstate.last_producer_schedule_size)>( top_producers.size() );
      }
   }

   double stake2vote( int64_t staked ) {
      /// TODO subtract 2080 brings the large numbers closer to this decade
      double weight = int64_t( (now() - (block_timestamp::block_timestamp_epoch / 1000)) / (seconds_per_day * 7) )  / double( 52 );
      return double(staked) * std::pow( 2, weight );
   }
   /**
    *  @pre every listed producer must have been previously registered
    *  @pre voter must authorize this action
    *  @pre voter must have previously staked some ACTX for voting
    *  @pre voter->staked must be up to date
    *
    *  @post every producer previously voted for will have vote reduced by previous vote weight
    *  @post every producer newly voted for will have vote increased by new vote amount
    *
    */
   void system_contract::voteproducer( const account_name voter_name, const account_name producer, asset stake ) {
      require_auth( voter_name );
      update_votes( voter_name, producer, stake, true );
   }

   void system_contract::update_votes( const account_name voter_name, const account_name producer, asset stake, bool voting ) {

      auto voter = _voters.find(voter_name);
      eosio_assert( voter != _voters.end(), "user must stake before they can vote" ); /// staking creates voter object

	  eosio_assert( voter->current_stake + stake.amount < voter->staked, "attempt to vote more votes" );
	   

      _gstate.total_activated_stake += stake.amount;
      if ( _gstate.total_activated_stake >= min_activated_stake && _gstate.thresh_activated_stake_time == 0 ) {
         _gstate.thresh_activated_stake_time = current_time();
      }

      if (_gstate.total_activated_stake < min_activated_stake && _gstate.thresh_activated_stake_time > 0 )
      {
          _gstate.thresh_activated_stake_time = 0;
      }
      auto producers = voter->producers;

      //check count of producers
      auto p_iter = producers.find(producer);
      if (p_iter == producers.end())
      {
         if (stake.amount < 0)
	 {
            eosio_assert( false, "the producer is not in the vote list" );
	 }
         eosio_assert( producers.size() <= 30, "attempt to vote for too many producers" );
         producers.emplace(std::map<account_name, int64_t>::value_type(producer, stake.amount));
      }
      else
      {
         if (stake.amount < 0)
	 {
            eosio_assert(p_iter->second + stake.amount >= 0, "attempt to unvote more votes" );
	 }
         
         p_iter->second += stake.amount;
      }
      
      auto pitr = _producers.find(producer);
      if ( pitr != _producers.end() ) {
         _producers.modify( pitr, 0, [&]( auto& p ) {
            p.total_votes += stake.amount;
            if ( p.total_votes < 0 ) { // floating point arithmetics can give small negative numbers
               p.total_votes = 0;
            }
         });
      } else {
         eosio_assert( false, "producer is not registered" ); 
      }
      _voters.modify( voter, 0, [&]( auto& av ) {
         av.producers = producers;
         av.current_stake += stake.amount;
      });
   }

} /// namespace eosiosystem
