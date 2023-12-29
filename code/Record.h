#include <iostream>
#include "Client.h"
#include "Inventory.h"
#ifndef RECORD_H
#define RECORD_H

using namespace std;

class Inventory;

/// @brief Record class to create PNRs of transactions
class Record : public SaveItem {
    private:
    /// @brief Number of created records
    static int num_records;
    /// @brief Path of storage file
    static const string save_path;
    /// @brief Unique string identifier
    const string ID;
    /// @brief Pointer to the linked inventory item
    Inventory* linked_inventory;
    /// @brief Pointer to the linked client
    Client* linked_client;
    /// @brief Date the reservation corresponds to
    tm reservation_date;

    /// @brief Creating a new ID from the number of created records
    /// @return Unique string identifier
    string generateID() {
        return to_string(num_records++);
    }

    public:
    /// @brief Default construct only used to create a useless record instance to indicate an error happened
    Record() : ID(""){
        linked_client = nullptr;
        linked_inventory = nullptr;
        reservation_date = tm();
    }

    /// @brief Non-default constructor for creating records from the storage file (ID is given)
    /// @param ID 
    /// @param linked_inventory 
    /// @param linked_client 
    /// @param reservation_date 
    Record(string ID, Inventory* linked_inventory, Client* linked_client, tm reservation_date) : ID(ID) {
        this->linked_inventory = linked_inventory;
        this->linked_client = linked_client;
        this->reservation_date = reservation_date;
    }

    /// @brief Non-default constructor for creating new records
    /// @param linked_inventory 
    /// @param linked_client 
    /// @param reservation_date 
    Record(Inventory* linked_inventory, Client* linked_client, tm reservation_date) : ID(generateID()) {
        this->linked_inventory = linked_inventory;
        this->linked_client = linked_client;
        this->reservation_date = reservation_date;
        save();
    }

    // Getter functions
    string getID() const { return ID; }
    Client* getClient() const { return linked_client; }
    Inventory* getInventory() const { return linked_inventory; }
    tm getReserevationDate() const { return reservation_date; }

    /// @brief Finds the client with the corresponding ID from given vector
    /// @param ID ID to search for
    /// @param clients Vector of clients to search through
    /// @return Pointer to the corresponding client
    static Client* findClientfromID(string ID, vector<Client> &clients) {
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].getID() == ID)
                return &(clients[i]);
        }
        return nullptr;
    }

    /// @brief Loads all the records from storage file given all loaded clients and inventory items
    /// @param clients Vector of all loaded clients
    /// @param inventoryItems Vector of all loaded inventory items
    /// @return Vector of all records from the storage file
    vector<Record> loadAll(vector<Client> &clients, vector<Inventory> &inventoryItems) {
        vector<Record> records;
        ifstream reader;
        reader.open(save_path);
        if (reader.fail()) {
            cerr << "Error loading planes..." << endl;
            reader.close();
            return vector<Record>();
        }
        string temp;
        num_records = 0;
        while (getline(reader, temp)) {
            num_records++;
            string recordID, inventoryID, clientID, reservation_date;
            stringstream s_stream(temp);
            getline(s_stream, temp, ',');
            recordID = decrypt(temp);
            getline(s_stream, temp, ',');
            inventoryID = decrypt(temp);
            Inventory* linked_inventory = findInventoryfromID(inventoryID, inventoryItems);
            getline(s_stream, temp, ',');
            clientID = decrypt(temp);
            Client* linked_client = findClientfromID(clientID, clients);
            getline(s_stream, temp, ',');
            reservation_date = decrypt(temp);
            records.push_back(Record(recordID, linked_inventory, linked_client, date_to_tm(reservation_date)));
        }
        reader.close();
        return records;
    }

    /// @brief Prints details of a record
    void print_details() {
            cout << "Record " << ID << endl;
            linked_client->print_details();
            linked_inventory->print_details();
    }

    /// @brief Saves the record into the storage file
    /// @return True if writing was a success, false otherwise
    bool save() {
        ofstream writer;
        writer.open(save_path, std::ios_base::app);
        if (writer.fail()) {
            cerr << "Error saving plane..." << endl;
            writer.close();
            return false;
        }
        writer << encrypt(ID) << ',';
        writer << encrypt(linked_inventory->getID()) << ',';
        writer << encrypt(linked_client->getID()) << ',';
        writer << encrypt(tm_to_date(reservation_date)) << ',';
        writer << endl;
        writer.close();
        return true;
    }

    /// @brief Searches for the inventory item with the corresponding ID from the given inventory vector
    /// @param ID ID to search for
    /// @param inventoryItems Vector of inventory items to search through
    /// @return Pointer to inventory items with given ID
    Inventory* findInventoryfromID(string ID, vector<Inventory> &inventoryItems) {
        for (int i = 0; i < inventoryItems.size(); i++) {
            if (inventoryItems[i].getID() == ID)
                return &(inventoryItems[i]);
        }
        return nullptr;
    }
};

// Static variables
const string Record::save_path = "SaveData/Records.csv";
int Record::num_records = 0;

#endif