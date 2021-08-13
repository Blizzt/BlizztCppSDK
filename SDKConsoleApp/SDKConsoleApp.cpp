// SDKConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BlizztSDK.h"

int main()
{
    std::cout << "Initializing SDK\n";
    BlizztSDK sdk("50966076-310f-4f59-983f-b592f3b77fd3", "http://localhost:8000");
    std::cout << "Validating Account\n";
    auto account = sdk.validate_user("I confirm my user account in Blizzt is 0xea4ca1270cc91ac072c3b1d273da95ce8d024fb5", "0xb8134632e9b30b36315574f9dbd696aa0f53b1a4ccad10dc3a78810e49dffab41b79487423af8370c31aac0ad8da4c466416919a69ae55a439bc8e08aaa949991b");
    std::cout << "Loading game NFTs\n";
    auto nfts = sdk.load_game_nfts();
    auto player_nfts = sdk.load_user_nfts("0xc3B2CFa1684dd33e8Ea8F657122f42b288d32852");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
