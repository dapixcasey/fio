/** fio_address_validator definitions file
 *  Description: Takes a fio.name and validates it as spec.
 *  @author Casey Gardiner
 *  @file fio_name_validator.hpp
 *  @copyright Dapix
 *
 *  Changes:
 */
#pragma once

#include <string>
#include <fio.common/fio_common_validator.hpp>
#include <fc/crypto/ripemd160.hpp>

#pragma once

using namespace eosio;
namespace fioio {

        inline bool isPubKeyValid(const string &pubkey){

          if (pubkey.length() != 53) return false;

          string pubkey_prefix("FIO");
          auto result = mismatch(pubkey_prefix.begin(), pubkey_prefix.end(),
                                 pubkey.begin());
          if (result.first != pubkey_prefix.end()) return false;
          auto base58substr = pubkey.substr(pubkey_prefix.length());

          vector<unsigned char> vch;
          if (!decode_base58(base58substr, vch) || (vch.size() != 37) ) return false;

          array<unsigned char, 33> pubkey_data;
          copy_n(vch.begin(), 33, pubkey_data.begin());
          char key[33];
          for (int i = 0; i < 33; i++) {
            key[i] = pubkey_data[i];
          }
          fc::ripemd160 hash_val = fc::ripemd160::hash(key, 33);
          if (memcmp(&hash_val, &vch.end()[-4], 4) != 0) return false;
          //end of the public key validity check.

          return true;
        }


}