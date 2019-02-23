#define BOOST_TEST_MODULE Main
#define NON_VALIDATING_TEST
#include <boost/test/unit_test.hpp>

#include <eosio/chain/abi_serializer.hpp>
#include <eosio/testing/tester.hpp>

#include <Runtime/Runtime.h>
// #include <fc/variant_object.hpp>

// #include "eosio.token2/eosio.token2.wast.hpp"
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

using mvo = fc::mutable_variant_object;

class eosio_token_tester : public tester {
public:

   eosio_token_tester() {
        produce_blocks( 2 );

        create_accounts({N(eosio.token), N(alice)});
        produce_blocks(10);
        
        std::ifstream stream("../contracts/eosio.token2/eosio.token2.wasm", std::ios::in | std::ios::binary);
        std::vector<uint8_t> wasm{std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
        

        BOOST_REQUIRE_EQUAL(false, wasm.empty());
        // chain.set_code(N(eosio.token), eosio_token2_wast);
        set_code(N(eosio.token), wasm);
        set_abi(N(eosio.token), eosio_token2_abi);
        produce_blocks();

        const auto& accnt = control->db().get<account_object,by_name>( N(eosio.token) );
        abi_def abi;
        BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
        abi_ser.set_abi(abi, abi_serializer_max_time);
   }

   action_result push_action( const account_name& signer, const action_name &name, const variant_object &data ) {
        string action_type_name = abi_ser.get_action_type(name);

        action act;
        act.account = N(eosio.token);
        act.name    = name;
        act.data    = abi_ser.variant_to_binary( action_type_name, data, abi_serializer_max_time );

        return base_tester::push_action( std::move(act), uint64_t(signer));
   }

    transaction_trace_ptr push_action(const account_name& code,
                                    const action_name& acttype,
                                    const account_name& actor,
                                    const variant_object& data,
                                    uint32_t expiration = DEFAULT_EXPIRATION_DELTA,
                                    uint32_t delay_sec = 0)
    {
        base_tester::push_action(code, acttype, actor, data, expiration, delay_sec);
    }

    fc::variant get_stats( const string& symbolname )
   {
        auto symb = eosio::chain::symbol::from_string(symbolname);
        auto symbol_code = symb.to_symbol_code().value;
        vector<char> data = get_row_by_account( N(eosio.token), symbol_code, N(stat), symbol_code );
        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "currency_stats", data, abi_serializer_max_time );
   }

   fc::variant get_account( account_name acc, const string& symbolname)
   {
        auto symb = eosio::chain::symbol::from_string(symbolname);
        auto symbol_code = symb.to_symbol_code().value;
        vector<char> data = get_row_by_account( N(eosio.token), acc, N(accounts), symbol_code );
        return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "account", data, abi_serializer_max_time );
   }

   action_result create( account_name issuer,
                        asset maximum_supply ) {

      return push_action( N(eosio.token), N(create), mvo()
            ( "issuer", issuer)
            ( "maximum_supply", maximum_supply)
      );
   }

   abi_serializer abi_ser;
};

BOOST_AUTO_TEST_SUITE(eosio_token_tests)

BOOST_FIXTURE_TEST_CASE( create_tests, eosio_token_tester ) try {

//    auto token = create( N(alice), asset::from_string("1000.000 TKN"));

   push_action( N(eosio.token), N(create), N(eosio.token),
        mvo()("issuer", "alice"), asset::from_string(("maximum_supply", "1000.000 TKN")));

   auto stats = get_stats("3,TKN");
   REQUIRE_MATCHING_OBJECT( stats, mvo()
        ("supply", "0.000 TKN")
        ("max_supply", "1000.000 TKN")
        ("issuer", "alice")
   );
   produce_blocks(1);
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()