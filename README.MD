# EOS Oracle Smart Contract

This Smart Contract is used for storing live market data for selected instruments on EOS blockchain.

## Running the project
(brew part is MacOS-only, consult EOS docs on instructions for other operating systems)
1. Install EOS / cleos (MacOS only)
```
brew tap eosio/eosio
brew install eosio
```
2. install EOS C++ (to compile contract)
```
brew tap eosio/eosio.cdt
brew install eosio.cdt
```
3. Create private/public key pair, store it in file:
```
cleos create key -f key-priceslist
```
4. Create dummy wallet named local, output password to file:
```
cleos wallet create --name priceslist -f wallet-priceslist
```
5. Import eosio private key to newly created priceslist wallet (it’s needed to create a new account with ‘eosio’ parent:
```
cleos wallet import -n priceslist --private-key XXXXX
```
6. import your private key from key-priceslist:
```
cleos wallet import -n priceslist --private-key XXXXX
```
7. start local EOS node
```
nodeos -e -p eosio --plugin eosio::producer_plugin --plugin eosio::producer_api_plugin --plugin eosio::chain_api_plugin --plugin eosio::http_plugin --plugin eosio::history_plugin --plugin eosio::history_api_plugin --filter-on="*" --access-control-alloworigin='*' --contracts-console --http-validate-host=false --verbose-http-errors
```
8. Create account (the last part is public key from key-priceslist file)
```
cleos create account eosio priceslist XXXXX
```
9. Deploy the contract (point to your contract directory):
```
cleos set contract priceslist /Users/you/Projects/algotrader/eos-oracle/eosoracle/contracts/priceslist
```

## Calling Contract methods
### add instrument using cleos
```
cleos push action priceslist newinstr '["MSFT","NASDAQ","USD",0]' -p eosio@active
```

### Use node.js wrapper
For convenience you can use [EOS Oracle API](https://github.com/algotrader-com/eos-oracle-api) which is a thin Node.js wrapper around the Contract and interact with Contract methods using REST API.