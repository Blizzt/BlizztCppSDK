#include "pch.h"
#include "BlizztSDK.h"
#include "HttpClient.h"
#include "Utils.h"
#include "Account.h"
#include <rapidjson/document.h>

using namespace rapidjson;

#ifdef _MSC_VER
#undef GetObject
#endif

BlizztSDK::BlizztSDK(const string& _api_key, const string& _api_address)
{
	api_key = _api_key;
	api_address = _api_address;
}

string BlizztSDK::validate_user(const string& _qr)
{
	return NULL;
}

string BlizztSDK::validate_user(const string& _message, const string& _signature)
{
	Account account;
	return account.ecrecover(_signature, _message);
}

blizzt_load_nfts_response BlizztSDK::load_game_nfts()
{
	blizzt_load_nfts_response response;
	response.res = false;

	HttpClient http_client;
	string query_res = http_client.Post(api_address + "/graphql", "{\"query\":\"query { project(id:\\\"" + api_key + "\\\") { nfts { nftId, metadata } } }\",\"variables\":{}}");
	Document doc;
	doc.Parse(query_res.c_str());

	if (doc.HasParseError() == true) return response;
	auto data = doc.GetObject()["data"].GetObject();
	if (data.HasMember("project") == false) return response;
	auto project = data["project"].GetObject();
	if (project.HasMember("nfts") == false) return response;
	auto nfts = project["nfts"].GetArray();
	for (auto& _nft : nfts) {
		nft game_nft;
		game_nft.id = _nft["nftId"].GetInt();
		auto metadata = _nft["metadata"].GetString();
		Document metadata_doc;
		metadata_doc.Parse(metadata);
		if (metadata_doc.HasParseError() == true) return response;
		game_nft.name = metadata_doc["name"].GetString();
		game_nft.description = metadata_doc["description"].GetString();
		string image = metadata_doc["image"].GetString();
		Utils::replace(image, "ipfs://", "https://ipfs.io/ipfs/");
		HttpClient ipfs_client;
		game_nft.image = ipfs_client.Get(image);
		//game_nft.image = image;
		game_nft.external_url = metadata_doc["external_url"].GetString();
		auto attributes_array = metadata_doc["attributes"].GetArray();
		for (auto& _attribute : attributes_array)
		{
			nft_attribute att;
			att.display_type = _attribute["display_type"].GetString();
			att.trait_type = _attribute["trait_type"].GetString();
			if (_attribute["value"].IsInt()) att.value = to_string(_attribute["value"].GetInt());
			else if (_attribute["value"].IsDouble()) att.value = to_string(_attribute["value"].GetDouble());
			else if (_attribute["value"].IsBool()) att.value = to_string(_attribute["value"].GetBool());
			else if (_attribute["value"].IsFloat()) att.value = _attribute["value"].GetFloat();
			else att.value = _attribute["value"].GetString();

			game_nft.attributes.push_back(att);
		}

		response.nfts.push_back(game_nft);
	}

	response.res = true;
	return response;
}

blizzt_load_nfts_response BlizztSDK::load_user_nfts(const string& _user_address)
{
	blizzt_load_nfts_response response;
	response.res = false;

	HttpClient http_client;
	string query_res = http_client.Post(api_address + "/graphql", "{\"query\":\"mutation ObtainUserFromWallet($wallet: String!) { obtainUserFromWallet(wallet: $wallet) { token } }\",\"variables\":{ \"wallet\": \"0xc3B2CFa1684dd33e8Ea8F657122f42b288d32852\" } }");
	Document doc;
	doc.Parse(query_res.c_str());
	if (doc.HasParseError() == true) return response;
	if (doc.GetObject().HasMember("data") == false) return response;
	auto data_wallet = doc.GetObject()["data"].GetObject();
	if (data_wallet.HasMember("obtainUserFromWallet") == false) return response;
	auto wallet_token = data_wallet["obtainUserFromWallet"].GetObject();
	if (wallet_token.HasMember("token") == false) return response;
	string token = wallet_token["token"].GetString();
	string query_res2 = http_client.Post(api_address + "/graphql", "{\"query\":\"query { project(id:\\\"" + api_key + "\\\") { nfts { nftId, metadata, adquired } } }\",\"variables\":{ \"xtoken\": " + token + "\" }}");
	doc.Parse(query_res2.c_str());
	if (doc.HasParseError() == true) return response;
	auto data = doc.GetObject()["data"].GetObject();
	if (data.HasMember("project") == false) return response;
	auto project = data["project"].GetObject();
	if (project.HasMember("nfts") == false) return response;
	auto nfts = project["nfts"].GetArray();
	for (auto& _nft : nfts) {
		nft game_nft;
		game_nft.id = _nft["nftId"].GetInt();
		auto metadata = _nft["metadata"].GetString();
		Document metadata_doc;
		metadata_doc.Parse(metadata);
		if (metadata_doc.HasParseError() == true) return response;
		game_nft.name = metadata_doc["name"].GetString();
		game_nft.description = metadata_doc["description"].GetString();
		game_nft.image = metadata_doc["image"].GetString();
		game_nft.external_url = metadata_doc["external_url"].GetString();
		auto attributes_array = metadata_doc["attributes"].GetArray();
		for (auto& _attribute : attributes_array) {
			nft_attribute att;
			att.display_type = _attribute["display_type"].GetString();
			att.trait_type = _attribute["trait_type"].GetString();
			if (_attribute["value"].IsInt()) att.value = to_string(_attribute["value"].GetInt());
			else if (_attribute["value"].IsDouble()) att.value = to_string(_attribute["value"].GetDouble());
			else if (_attribute["value"].IsBool()) att.value = to_string(_attribute["value"].GetBool());
			else if (_attribute["value"].IsFloat()) att.value = _attribute["value"].GetFloat();
			else att.value = _attribute["value"].GetString();

			game_nft.attributes.push_back(att);
		}

		response.nfts.push_back(game_nft);
	}

	response.res = true;
	return response;
}
