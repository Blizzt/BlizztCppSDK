#include "pch.h"
#include "Utils.h"

#include <string>
#include "keccak.h"

using namespace std;

bool Utils::replace(string& str, const string& from, const string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == string::npos)
		return false;
	str.replace(start_pos, from.length(), to);

	return true;
}

vector<char> Utils::HexToBytes(const string& hex) {
    vector<char> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        char byte = (char)strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    return bytes;
}


vector<char> Utils::keccak_256(const unsigned char* message, int len)
{
    Keccak keccak;
    keccak.add(message, len);
    string hex_hash = keccak.getHash();
    return HexToBytes(hex_hash);
}
