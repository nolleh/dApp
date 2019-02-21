#define BOOST_TEST_MODULE Main
#define NON_VALIDATING_TEST
#include <boost/test/unit_test.hpp>
#include <eosio/testing/tester.hpp>
#include <eosio/chain/wast_to_wasm.hpp>

#include "eosio.token2/eosio.token2.wast.hpp"
#include "eosio.token2/eosio.token2.abi.hpp"

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

    chain.create_account(N(eosio.token));
    chain.produce_blocks(10);
    
    // chain.set_code(N(eosio.token), eosio_token2_wast);
    // chain.set_code(N(eosio.token), wast_to_wasm(eosio_token_wast));
    // chain.set_abi(N(eosio.token), eosio_token2_abi);

    // fc::temp_directory dir; 
    BOOST_REQUIRE_EQUAL("",  fc::canonical(fc::temp_directory_path()).string());
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()