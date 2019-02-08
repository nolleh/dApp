/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#include "eosio.token.hpp"

namespace eosio {

void token::create( name   issuer,
                    asset  maximum_supply )
{
   //요청자가 컨트랙트 주소인지 체크
   require_auth( _self );
    
   //토큰 정보에 대한 체크
   auto sym = maximum_supply.symbol;
   eosio_assert( sym.is_valid(), "invalid symbol name" );
   eosio_assert( maximum_supply.is_valid(), "invalid supply");
   eosio_assert( maximum_supply.amount > 0, "max-supply must be positive");
   //stats 테이블에 해당 토큰 정보가 존재하는지 체크
   stats statstable( _self, sym.code().raw() );
   auto existing = statstable.find( sym.code().raw() );
   eosio_assert( existing == statstable.end(), "token with symbol already exists" );
   //스테이트 테이블에 토큰정보를 저장합니다.
   statstable.emplace( _self, [&]( auto& s ) {
      s.supply.symbol = maximum_supply.symbol;
      s.max_supply    = maximum_supply;
      s.issuer        = issuer;
   });
}

void token::issue( name to, asset quantity, string memo )
{
   auto sym = quantity.symbol;
   eosio_assert( sym.is_valid(), "invalid symbol name" );
   eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

   //토큰 심볼정보가 stats테이블에 존재하는지 체크합니다.
   stats statstable( _self, sym.code().raw() );
   auto existing = statstable.find( sym.code().raw() );
   eosio_assert( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
   /*existing 변수는 statstable.find()를 통해 iterator 형태의 값을 받고, st는 existing이 가리키는 포인터 즉 실제 변수입니다. 이를 통해 C++ 포인터의 -> 대신에 st.functionName 형태로 멤버 변수에 접근이 가능합니다.*/
   const auto& st = *existing;

   require_auth( st.issuer );
   eosio_assert( quantity.is_valid(), "invalid quantity" );
   eosio_assert( quantity.amount > 0, "must issue positive quantity" );

   eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
   eosio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

   //states테이블에 토큰정보를 업데이트 합니다.
   /*
   테이블의 update기능은
   github의 eos/contracts/eosiolib/multi_index.hpp
   파일에 define되어있습니다.
   */    
   statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply += quantity;
   });
   //add_balance메서드를 실행합니다.
   //ram_payer가 issuer이 됩니다.
   add_balance( st.issuer, quantity, st.issuer );
   //to, issuer가 다를때는 전송 메서드를 실행합니다.
   if( to != st.issuer ) {
      SEND_INLINE_ACTION( *this, transfer, { {st.issuer, "active"_n} },
                          { st.issuer, to, quantity, memo }
      );
   }
}

void token::retire( asset quantity, string memo )
{
    auto sym = quantity.symbol;
    eosio_assert( sym.is_valid(), "invalid symbol name" );
    eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    eosio_assert( existing != statstable.end(), "token with symbol does not exist" );
    const auto& st = *existing;

    require_auth( st.issuer );
    eosio_assert( quantity.is_valid(), "invalid quantity" );
    eosio_assert( quantity.amount > 0, "must retire positive quantity" );

    eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply -= quantity;
    });

    sub_balance( st.issuer, quantity );
}

void token::transfer( name from,
                      name to,
                      asset        quantity,
                      string       memo )
{
   eosio_assert( from != to, "cannot transfer to self" );
   require_auth( from );
   eosio_assert( is_account( to ), "to account does not exist");
   auto sym = quantity.symbol.code();
   stats statstable( _self, sym.raw() );
   const auto& st = statstable.get( sym.raw() );

   require_recipient( from );
   require_recipient( to );

   eosio_assert( quantity.is_valid(), "invalid quantity" );
   eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
   eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
   eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

   auto payer = has_auth( to ) ? to : from;

   sub_balance( from, quantity );
   add_balance( to, quantity, payer );
}

void token::sub_balance( name owner, asset value ) {
   accounts from_acnts( _self, owner.value );

   const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found" );
   eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
   });
}

void token::add_balance( name owner, asset value, name ram_payer )
{
   accounts to_acnts( _self, owner.value );
   auto to = to_acnts.find( value.symbol.code().raw() );
   if( to == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, same_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
}

} /// namespace eosio

EOSIO_DISPATCH( eosio::token, (create)(issue)(retire)(transfer) )
