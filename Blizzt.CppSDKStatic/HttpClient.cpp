#include "pch.h"
#include "HttpClient.h"
#include "curl.h"

using namespace std;

HttpClient::HttpClient()
{
	easy_handle = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);
}

HttpClient::~HttpClient()
{
	curl_global_cleanup();
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

string HttpClient::Get(string address)
{
	string html;

	curl_easy_setopt(easy_handle, CURLOPT_URL, address.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &html);
	CURLcode returnValue = curl_easy_perform(easy_handle);

	if (returnValue != CURLE_OK)
	{
		html = "";
	}

	curl_easy_cleanup(easy_handle);
	return html;
}

string HttpClient::Post(string address, string body)
{
	string html;

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(easy_handle, CURLOPT_URL, address.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, body.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &html);
	CURLcode returnValue = curl_easy_perform(easy_handle);

	if (returnValue != CURLE_OK)
	{
		html = "";
	}

	curl_easy_cleanup(easy_handle);
	return html;
}