#include <iostream>
#include "Client.h"
#ifndef INVENTORY_H
#define INVENTORY_H

using namespace std;

class Record;

class Client;

/// @brief Abstract class to define functions and parameters of Inventory items
class Inventory {
    protected:
    /// @brief Unique string identifier
    const string ID;
    /// @brief Price of inventory item
    double price = 0.0;

    public:
    /// @brief Non-default constructor for creating inventory items based on ID only
    /// @param ID 
    Inventory(string ID) : ID(ID) {
    }

    /// @brief Non-default constructor for creating inventory items based on ID and price
    /// @param ID 
    /// @param price 
    Inventory(string ID, double price) : ID(ID) {
        this->price = price;
    }
    
    string getID() const { return ID; }
    
    /// @brief Abstract function to print details
    virtual void print_details() = 0;
    /// @brief Abstract function to reserve the inventory item
    /// @return 
    virtual bool Reserve() = 0;
    /// @brief Abstract function to purchase the inventory item for the client with the corresponding reservation date
    /// @param client
    /// @param reservation_date 
    /// @return Record (PNR) of the transaction
    virtual Record Purchase(Client* client, tm reservation_date) = 0;

};
#endif