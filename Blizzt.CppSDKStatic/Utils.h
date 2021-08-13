#pragma once
#include <string>
#include <vector>

using namespace std;

class Utils
{
public:
	static bool replace(string& str, const string& from, const string& to);
	static vector<char> HexToBytes(const string& hex);
	static vector<char> keccak_256(const unsigned char* message, int len);
};

