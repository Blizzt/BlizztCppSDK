# Blizzt Cpp SDK
## Installation setup

- Clone the repo
- Compile the Blizzt.CppSDKStatic library in Release mode using VS 2019
- Create a new project in VS 2019
- In project settings/C++/General/Additional Include Directories add the BlizztCppSDKStatic folder
- In project settings/Linker/Input/AdditionalDependencies add the library Blizzt.CppSDKStatic.lib

## Example
```sh
    BlizztSDK sdk("50966076-310f-4f59-983f-b592f3b77fd3", "http://localhost:8000");
    auto account = sdk.validate_user("I confirm my user account in Blizzt is 0xea4ca1270cc91ac072c3b1d273da95ce8d024fb5", "0xb8134632e9b30b36315574f9dbd696aa0f53b1a4ccad10dc3a78810e49dffab41b79487423af8370c31aac0ad8da4c466416919a69ae55a439bc8e08aaa949991b");
    auto nfts = sdk.load_game_nfts();
    auto player_nfts = sdk.load_user_nfts("0xc3B2CFa1684dd33e8Ea8F657122f42b288d32852");
```
