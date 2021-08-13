#include "pch.h"
#include "Account.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <array>

#include "secp256k1.h"
#include "secp256k1_recovery.h"
#include "Utils.h"

string Account::bytes_to_hex_string(const uint8_t* str, const uint64_t s)
{
    ostringstream ret;

    for (size_t i = 0; i < s; ++i)
        ret << hex << setfill('0') << setw(2) << nouppercase << (int)str[i];

    return ret.str();
}

string string_to_hex(const string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

string Account::hex_to_string(const string& input)
{
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();
    if (len & 1) throw invalid_argument("odd length");

    string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = input[i];
        const char* p = lower_bound(lut, lut + 16, a);
        if (*p != a) throw invalid_argument("not a hex digit");

        char b = input[i + 1];
        const char* q = lower_bound(lut, lut + 16, b);
        if (*q != b) throw invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}

string Account::ecrecover(string sig, string msg)
{
    string hashed = string("\x19") + "Ethereum Signed Message:\n" + to_string(msg.length()) + msg;
    vector<char> message = Utils::keccak_256((unsigned char*)hashed.c_str(), hashed.length());

    string _sig = hex_to_string(sig.substr(2)); // strip 0x

    if (_sig.size() != 65)
        return ("0x00000000000000000000000000000000");

    int v = _sig[64];
    _sig = _sig.substr(0, 64);

    if (v > 3)
        v -= 27;

    auto* ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

    secp256k1_ecdsa_recoverable_signature rawSig;
    if (!secp256k1_ecdsa_recoverable_signature_parse_compact(ctx, &rawSig, (unsigned char*)_sig.data(), v))
        return ("0x00000000000000000000000000000000");

    secp256k1_pubkey rawPubkey;
    if (!secp256k1_ecdsa_recover(ctx, &rawPubkey, &rawSig, (unsigned char*)message.data()))                                                   // 32 bit hash
        return ("0x00000000000000000000000000000000");

    array<uint8_t, 65> pubkey;
    size_t biglen = 65;

    secp256k1_ec_pubkey_serialize(ctx, pubkey.data(), &biglen, &rawPubkey, SECP256K1_EC_UNCOMPRESSED);

    string out = string(pubkey.begin(), pubkey.end()).substr(1);

    vector<char> hash = Utils::keccak_256((const unsigned char*)out.data(), out.length());

    return("0x" + bytes_to_hex_string((unsigned char*)hash.data(), hash.size()).substr(24));
}