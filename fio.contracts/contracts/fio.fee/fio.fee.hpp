#pragma once

#include <eosiolib/eosio.hpp>
//#include <string>
//#include <eosiolib/singleton.hpp>


using std::string;

namespace fioio {

    using namespace eosio;


    enum class feetype {
        mandatory = 0,  // these fees are applied each time an operation takes place.
        bundleeligible = 1  // these fees are free until a predetermined number of operations takes place,
        // then a fee is applied
    };

    // Structure for "FIO fee" .
    // @abi table fiofee i64
    struct [[eosio::action]] fiofee {        // FIO fee
        uint64_t fee_id;     // one up index starting at 0
        string end_point;  // the blockchain endpoint to which the fee relates.
        uint64_t end_point_hash;    // this is a hash of the end point, lending search-ability
        uint64_t type;      // this is the fee type from the feetype enumeration.
        uint64_t suf_amount;      // this is the amount of the fee in small units of FIO.
        // 1 billion per fio as of 04/23/2019

        uint64_t primary_key() const { return fee_id; }

        uint64_t by_endpoint() const { return end_point_hash; }

        uint64_t by_type() const { return type; }

        EOSLIB_SERIALIZE(fiofee, (fee_id)(end_point)(end_point_hash)(type)(suf_amount)
        )
    };

    // FIO fee table
    typedef multi_index<"fiofees"_n, fiofee,
            indexed_by<"byendpoint"_n, const_mem_fun < fiofee, uint64_t, &fiofee::by_endpoint>>,
    indexed_by<"bytype"_n, const_mem_fun<fiofee, uint64_t, &fiofee::by_type>
    >>
    fiofee_table;


    // Structure for "feevoter" .
    // @abi table feevoter i64
    struct [[eosio::action]] feevoter {
        name block_producer_name;     // name of the bp
        uint64_t fee_multiplier;    // this is the fee multiplier, will be converted to double with 6 decimal
                                    // places so a value of 1000000 becomes 1.0
        uint64_t lastvotetimestamp;      // this is the timestamp of the last successful vote for this BP.


        uint64_t primary_key() const { return block_producer_name.value; }

        EOSLIB_SERIALIZE(feevoter, (block_producer_name)(fee_multiplier)(lastvotetimestamp)
        )
    };

    //  fee voters table
    typedef multi_index<"feevoters"_n, feevoter> feevoters_table;

    // Structure for "feevote" .
    // @abi table feevote i64
    struct [[eosio::action]] feevote {
        name block_producer_name;  // bp name for searching.
        string end_point;    // this is the name of the fee end point
        uint64_t end_point_hash;  // hash of the end point for searching.
        uint64_t suf_amount;      // this is the amount of the fee in small units of FIO.
        // 1 billion per fio as of 04/23/2019

        uint64_t primary_key() const { return block_producer_name.value; }

        uint64_t by_endpoint() const { return end_point_hash; }

        EOSLIB_SERIALIZE(feevote, (block_producer_name)(end_point)(end_point_hash)(suf_amount)
        )
    };

    //fee votes table
    typedef multi_index<"feevotes"_n, feevote,
            indexed_by<"byendpoint"_n, const_mem_fun < fiofee, uint64_t, &fiofee::by_endpoint>>
    >
    feevotes_table;


} // namespace fioio