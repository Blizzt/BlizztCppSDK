#pragma once

#include <string>

using namespace std;

class HttpClient
{
public:
	HttpClient();
	virtual ~HttpClient();

	string Get(string address);
	string Post(string address, string body);

private:
	void* easy_handle;
};

