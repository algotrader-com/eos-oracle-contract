#include <eosio/eosio.hpp>
//#include <eosio/time.hpp>

using namespace eosio;

class [[eosio::contract("priceslist")]] priceslist : public eosio::contract {

public:
  using contract::contract;

  priceslist(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
  uint64_t newsec(std::string symbol, std::string exchange_name, std::string quote_currency, uint64_t security_type) {
    
    require_auth(get_self()); // Set to CREATOR/OWNER

    security_index securities(get_self(), get_first_receiver().value);
    uint64_t newId = securities.available_primary_key();

    // Who pays? Contract or Contract Owner?
    securities.emplace(get_self(), [&]( auto& row ) {
      row.key = newId;
      row.symbol = symbol;
      row.exchange_name = exchange_name;
      row.quote_currency = quote_currency;
      row.security_type = security_type;
      row.price = 0;
      row.time_stamp = 0;
    });

    return newId;
  
  }

  [[eosio::action]]
  void newprice(uint64_t security_id, float price, uint64_t time_stamp) {
    
    require_auth(get_self()); // Set to CREATOR/OWNER

    security_index securities(get_self(), get_first_receiver().value);

    auto iterator = securities.find(security_id);
    if( iterator != securities.end() )
    {
      securities.modify(iterator, get_self(), [&]( auto& row ) {
        row.price = price;
        row.time_stamp = time_stamp;
      });
    }
  
  }

  // iterates the multi index table rows using the secondary index and prints the row's values
  [[eosio::action]] 
  void list( uint64_t type ) {
    // access the secondary index
    security_index securities(get_self(), get_first_receiver().value);
    auto idx = securities.get_index<"securitytype"_n>();

    // iterate through secondary index
    for ( auto itr = idx.begin(); itr != idx.end(); itr++ ) {

      if (itr->security_type == type ) {
        // print each row's values
        print_f("Securities : {%, %, %, %, %, %, %}\n", itr->key, itr->symbol, itr->exchange_name, itr->quote_currency, itr->security_type, itr->price, itr->time_stamp);
      }
    }
  }

  [[eosio::action]]
  void erase(uint64_t security_id) {
    require_auth(get_self()); // Set to CREATOR/OWNER

    security_index securities(get_self(), get_first_receiver().value);

    auto iterator = securities.find(security_id);
    if( iterator != securities.end() )
    {
        iterator = securities.erase(iterator);
    }

  }

  [[eosio::action]]
  void eraseall() {
    require_auth(get_self()); // Set to CREATOR/OWNER
    
    security_index securities(get_self(), get_first_receiver().value);
    auto itr = securities.begin();
    while(itr != securities.end()){
        itr = securities.erase(itr);
    }

  }

private:
  struct [[eosio::table]] security {
    uint64_t key;
    std::string symbol;
    std::string exchange_name;
    std::string quote_currency;
    uint64_t security_type;
    float price;
    uint64_t time_stamp;

    uint64_t primary_key() const { return key; }
    uint64_t by_type( ) const { return security_type; }
  };


   using security_index = multi_index<"securities"_n, security, indexed_by<"securitytype"_n, const_mem_fun<security, uint64_t, &security::by_type>>>;
 
};
