#pragma once

using namespace std;

#include <list>
#include <string>

struct nft_attribute
{
	string display_type;
	string trait_type;
	string value;
};

struct nft
{
	int id;
	int quantity;
	int user_quantity;
	string name;
	string description;
	string image;
	string external_url;
	list<nft_attribute> attributes;
};

struct blizzt_load_nfts_response
{
	bool res;
	list<nft> nfts;
};

class BlizztSDK
{
public:
	BlizztSDK(const string& _nft_contract_address, const string& _api_address);

	string validate_user(const string& _qr);
	string validate_user(const string& _message, const string& _signature);
	blizzt_load_nfts_response load_game_nfts();
	blizzt_load_nfts_response load_user_nfts(const string& _user_address);

private:
	string api_key;
	string api_address;
};

