/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <actx.system/native.hpp>
#include <actxlib/asset.hpp>
#include <actxlib/time.hpp>
#include <actxlib/privileged.hpp>
#include <actxlib/singleton.hpp>
#include <actx.system/exchange_state.hpp>

#include <string>
#include <type_traits>
#include <optional>

namespace actxsystem {

   using actx::name;
   using actx::asset;
   using actx::symbol;
   using actx::symbol_code;
   using actx::indexed_by;
   using actx::const_mem_fun;
   using actx::block_timestamp;
   using actx::time_point;
   using actx::microseconds;
   using actx::datastream;

   template<typename E, typename F>
   static inline auto has_field( F flags, E field )
   -> std::enable_if_t< std::is_integral_v<F> && std::is_unsigned_v<F> &&
                        std::is_enum_v<E> && std::is_same_v< F, std::underlying_type_t<E> >, bool>
   {
      return ( (flags & static_cast<F>(field)) != 0 );
   }

   template<typename E, typename F>
   static inline auto set_field( F flags, E field, bool value = true )
   -> std::enable_if_t< std::is_integral_v<F> && std::is_unsigned_v<F> &&
                        std::is_enum_v<E> && std::is_same_v< F, std::underlying_type_t<E> >, F >
   {
      if( value )
         return ( flags | static_cast<F>(field) );
      else
         return ( flags & ~static_cast<F>(field) );
   }

   struct [[actx::table, actx::contract("actx.system")]] name_bid {
     name            newname;
     name            high_bidder;
     int64_t         high_bid = 0; ///< negative high_bid == closed auction waiting to be claimed
     time_point      last_bid_time;

     uint64_t primary_key()const { return newname.value;                    }
     uint64_t by_high_bid()const { return static_cast<uint64_t>(-high_bid); }
   };

   struct [[actx::table, actx::contract("actx.system")]] bid_refund {
      name         bidder;
      asset        amount;

      uint64_t primary_key()const { return bidder.value; }
   };

   typedef actx::multi_index< "namebids"_n, name_bid,
                               indexed_by<"highbid"_n, const_mem_fun<name_bid, uint64_t, &name_bid::by_high_bid>  >
                             > name_bid_table;

   typedef actx::multi_index< "bidrefunds"_n, bid_refund > bid_refund_table;

   struct [[actx::table("global"), actx::contract("actx.system")]] actx_global_state : actx::blockchain_parameters {
      uint64_t free_ram()const { return max_ram_size - total_ram_bytes_reserved; }

      uint64_t             max_ram_size = 64ll*1024 * 1024 * 1024;
      uint64_t             total_ram_bytes_reserved = 0;
      int64_t              total_ram_stake = 0;

      block_timestamp      last_producer_schedule_update;
      time_point           last_pervote_bucket_fill;
      int64_t              pervote_bucket = 0;
      int64_t              perblock_bucket = 0;
      uint32_t             total_unpaid_blocks = 0; /// all blocks which have been produced but not paid
      int64_t              total_activated_stake = 0;
      time_point           thresh_activated_stake_time;
      uint16_t             last_producer_schedule_size = 0;
      double               total_producer_vote_weight = 0; /// the sum of all producer votes
      block_timestamp      last_name_close;

      // explicit serialization macro is not necessary, used here only to improve compilation time
      EOSLIB_SERIALIZE_DERIVED( actx_global_state, actx::blockchain_parameters,
                                (max_ram_size)(total_ram_bytes_reserved)(total_ram_stake)
                                (last_producer_schedule_update)(last_pervote_bucket_fill)
                                (pervote_bucket)(perblock_bucket)(total_unpaid_blocks)(total_activated_stake)(thresh_activated_stake_time)
                                (last_producer_schedule_size)(total_producer_vote_weight)(last_name_close) )
   };

   /**
    * Defines new global state parameters added after version 1.0
    */
   struct [[actx::table("global2"), actx::contract("actx.system")]] actx_global_state2 {
      actx_global_state2(){}

      uint16_t          new_ram_per_block = 0;
      block_timestamp   last_ram_increase;
      block_timestamp   last_block_num; /* deprecated */
      double            total_producer_votepay_share = 0;
      uint8_t           revision = 0; ///< used to track version updates in the future.

      EOSLIB_SERIALIZE( actx_global_state2, (new_ram_per_block)(last_ram_increase)(last_block_num)
                        (total_producer_votepay_share)(revision) )
   };

   struct [[actx::table("global3"), actx::contract("actx.system")]] actx_global_state3 {
      actx_global_state3() { }
      time_point        last_vpay_state_update;
      double            total_vpay_share_change_rate = 0;

      EOSLIB_SERIALIZE( actx_global_state3, (last_vpay_state_update)(total_vpay_share_change_rate) )
   };

   struct [[actx::table, actx::contract("actx.system")]] producer_info {
      name                  owner;
      double                total_votes = 0;
      actx::public_key     producer_key; /// a packed public key object
      bool                  is_active = true;
      std::string           url;
      uint32_t              unpaid_blocks = 0;
      time_point            last_claim_time;
      uint16_t              location = 0;

      uint64_t primary_key()const { return owner.value;                             }
      double   by_votes()const    { return is_active ? -total_votes : total_votes;  }
      bool     active()const      { return is_active;                               }
      void     deactivate()       { producer_key = public_key(); is_active = false; }

      // explicit serialization macro is not necessary, used here only to improve compilation time
      EOSLIB_SERIALIZE( producer_info, (owner)(total_votes)(producer_key)(is_active)(url)
                        (unpaid_blocks)(last_claim_time)(location) )
   };

   struct [[actx::table, actx::contract("actx.system")]] producer_info2 {
      name            owner;
      double          votepay_share = 0;
      time_point      last_votepay_share_update;

      uint64_t primary_key()const { return owner.value; }

      // explicit serialization macro is not necessary, used here only to improve compilation time
      EOSLIB_SERIALIZE( producer_info2, (owner)(votepay_share)(last_votepay_share_update) )
   };

   struct [[actx::table, actx::contract("actx.system")]] voter_info {
      name                owner;     /// the voter
      name                proxy;     /// the proxy set by the voter, if any
      std::vector<name>   producers; /// the producers approved by this voter if no proxy set
      int64_t             staked = 0;

      /**
       *  Every time a vote is cast we must first "undo" the last vote weight, before casting the
       *  new vote weight.  Vote weight is calculated as:
       *
       *  stated.amount * 2 ^ ( weeks_since_launch/weeks_per_year)
       */
      double              last_vote_weight = 0; /// the vote weight cast the last time the vote was updated

      /**
       * Total vote weight delegated to this voter.
       */
      double              proxied_vote_weight= 0; /// the total vote weight delegated to this voter as a proxy
      bool                is_proxy = 0; /// whether the voter is a proxy for others


      uint32_t            flags1 = 0;
      uint32_t            reserved2 = 0;
      actx::asset        reserved3;

      uint64_t primary_key()const { return owner.value; }

      enum class flags1_fields : uint32_t {
         ram_managed = 1,
         net_managed = 2,
         cpu_managed = 4
      };

      // explicit serialization macro is not necessary, used here only to improve compilation time
      EOSLIB_SERIALIZE( voter_info, (owner)(proxy)(producers)(staked)(last_vote_weight)(proxied_vote_weight)(is_proxy)(flags1)(reserved2)(reserved3) )
   };

   typedef actx::multi_index< "voters"_n, voter_info >  voters_table;


   typedef actx::multi_index< "producers"_n, producer_info,
                               indexed_by<"prototalvote"_n, const_mem_fun<producer_info, double, &producer_info::by_votes>  >
                             > producers_table;
   typedef actx::multi_index< "producers2"_n, producer_info2 > producers_table2;

   typedef actx::singleton< "global"_n, actx_global_state >   global_state_singleton;
   typedef actx::singleton< "global2"_n, actx_global_state2 > global_state2_singleton;
   typedef actx::singleton< "global3"_n, actx_global_state3 > global_state3_singleton;

   //   static constexpr uint32_t     max_inflation_rate = 5;  // 5% annual inflation
   static constexpr uint32_t     seconds_per_day = 24 * 3600;

   class [[actx::contract("actx.system")]] system_contract : public native {
      private:
         voters_table            _voters;
         producers_table         _producers;
         producers_table2        _producers2;
         global_state_singleton  _global;
         global_state2_singleton _global2;
         global_state3_singleton _global3;
         actx_global_state      _gstate;
         actx_global_state2     _gstate2;
         actx_global_state3     _gstate3;
         rammarket               _rammarket;

      public:
         static constexpr actx::name active_permission{"active"_n};
         static constexpr actx::name token_account{"actx.token"_n};
         static constexpr actx::name ram_account{"actx.ram"_n};
         static constexpr actx::name ramfee_account{"actx.ramfee"_n};
         static constexpr actx::name stake_account{"actx.stake"_n};
         static constexpr actx::name bpay_account{"actx.bpay"_n};
         static constexpr actx::name vpay_account{"actx.vpay"_n};
         static constexpr actx::name names_account{"actx.names"_n};
         static constexpr actx::name saving_account{"actx.saving"_n};
         static constexpr symbol ramcore_symbol = symbol(symbol_code("RAMCORE"), 4);
         static constexpr symbol ram_symbol     = symbol(symbol_code("RAM"), 0);

         system_contract( name s, name code, datastream<const char*> ds );
         ~system_contract();

         static symbol get_core_symbol( name system_account = "actx"_n ) {
            rammarket rm(system_account, system_account.value);
            const static auto sym = get_core_symbol( rm );
            return sym;
         }

         // Actions:
         [[actx::action]]
         void init( unsigned_int version, symbol core );
         [[actx::action]]
         void onblock( ignore<block_header> header );

         [[actx::action]]
         void setalimits( name account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight );

         [[actx::action]]
         void setacctram( name account, std::optional<int64_t> ram_bytes );

         [[actx::action]]
         void setacctnet( name account, std::optional<int64_t> net_weight );

         [[actx::action]]
         void setacctcpu( name account, std::optional<int64_t> cpu_weight );

         // functions defined in delegate_bandwidth.cpp

         /**
          *  Stakes SYS from the balance of 'from' for the benfit of 'receiver'.
          *  If transfer == true, then 'receiver' can unstake to their account
          *  Else 'from' can unstake at any time.
          */
         [[actx::action]]
         void delegatebw( name from, name receiver,
                          asset stake_net_quantity, asset stake_cpu_quantity, bool transfer );


         /**
          *  Decreases the total tokens delegated by from to receiver and/or
          *  frees the memory associated with the delegation if there is nothing
          *  left to delegate.
          *
          *  This will cause an immediate reduction in net/cpu bandwidth of the
          *  receiver.
          *
          *  A transaction is scheduled to send the tokens back to 'from' after
          *  the staking period has passed. If existing transaction is scheduled, it
          *  will be canceled and a new transaction issued that has the combined
          *  undelegated amount.
          *
          *  The 'from' account loses voting power as a result of this call and
          *  all producer tallies are updated.
          */
         [[actx::action]]
         void undelegatebw( name from, name receiver,
                            asset unstake_net_quantity, asset unstake_cpu_quantity );


         /**
          * Increases receiver's ram quota based upon current price and quantity of
          * tokens provided. An inline transfer from receiver to system contract of
          * tokens will be executed.
          */
         [[actx::action]]
         void buyram( name payer, name receiver, asset quant );
         [[actx::action]]
         void buyrambytes( name payer, name receiver, uint32_t bytes );

         /**
          *  Reduces quota my bytes and then performs an inline transfer of tokens
          *  to receiver based upon the average purchase price of the original quota.
          */
         [[actx::action]]
         void sellram( name account, int64_t bytes );

         /**
          *  This action is called after the delegation-period to claim all pending
          *  unstaked tokens belonging to owner
          */
         [[actx::action]]
         void refund( name owner );

         // functions defined in voting.cpp

         [[actx::action]]
         void regproducer( const name producer, const public_key& producer_key, const std::string& url, uint16_t location );

         [[actx::action]]
         void unregprod( const name producer );

         [[actx::action]]
         void setram( uint64_t max_ram_size );
         [[actx::action]]
         void setramrate( uint16_t bytes_per_block );

         [[actx::action]]
         void voteproducer( const name voter, const name proxy, const std::vector<name>& producers );

         [[actx::action]]
         void regproxy( const name proxy, bool isproxy );

         [[actx::action]]
         void setparams( const actx::blockchain_parameters& params );

         // functions defined in producer_pay.cpp
         [[actx::action]]
         void claimrewards( const name owner );

         [[actx::action]]
         void setpriv( name account, uint8_t is_priv );

         [[actx::action]]
         void rmvproducer( name producer );

         [[actx::action]]
         void updtrevision( uint8_t revision );

         [[actx::action]]
         void bidname( name bidder, name newname, asset bid );

         [[actx::action]]
         void bidrefund( name bidder, name newname );

      private:
         // Implementation details:

         static symbol get_core_symbol( const rammarket& rm ) {
            auto itr = rm.find(ramcore_symbol.raw());
            actx_assert(itr != rm.end(), "system contract must first be initialized");
            return itr->quote.balance.symbol;
         }

         //defined in actx.system.cpp
         static actx_global_state get_default_parameters();
         static time_point current_time_point();
         static block_timestamp current_block_time();

         symbol core_symbol()const;

         void update_ram_supply();

         //defined in delegate_bandwidth.cpp
         void changebw( name from, name receiver,
                        asset stake_net_quantity, asset stake_cpu_quantity, bool transfer );

         //defined in voting.hpp
         void update_elected_producers( block_timestamp timestamp );
         void update_votes( const name voter, const name proxy, const std::vector<name>& producers, bool voting );

         // defined in voting.cpp
         void propagate_weight_change( const voter_info& voter );

         double update_producer_votepay_share( const producers_table2::const_iterator& prod_itr,
                                               time_point ct,
                                               double shares_rate, bool reset_to_zero = false );
         double update_total_votepay_share( time_point ct,
                                            double additional_shares_delta = 0.0, double shares_rate_delta = 0.0 );
   };

} /// actxsystem
