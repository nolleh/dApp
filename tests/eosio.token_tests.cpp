//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main

#include <boost/test/unit_test.hpp>
#include <eosio/testing/tester.hpp>

#include "eosio.token/eosio.token.wast.hpp"
#include "eosio.token/eosio.token.abi.hpp"

#ifdef NON_VALIDATING_TEST
#define TESTER tester
#else
#define TESTER validating_tester
#endif

using namespace eosio;
using namespace eosio::chain;
using namespace eosio::testing;
using namespace fc;

BOOST_AUTO_TEST_SUITE(foo)

BOOST_AUTO_TEST_CASE(eosio_token_test) try {
    TESTER chain;

    chain.create_account(N("acc_token"));
    chain.produce_blocks(10);
    
    chain.set_code(N("eosio.token"), eosio_token_wast);
    chain.set_abi(N("eosio.token"), eosio_token_abi);


} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()