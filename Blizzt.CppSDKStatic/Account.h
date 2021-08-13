#pragma once

using namespace std;
#include <string>

class Account
{
public:
	string ecrecover(string sig, string msg);

private:
	string bytes_to_hex_string(const uint8_t* str, const uint64_t s);
	string hex_to_string(const string& input);
};

