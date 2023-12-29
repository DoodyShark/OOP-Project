#include <iostream>
#include <vector>
#include "Passport.h"
#include "SaveItem.h"
#ifndef CLIENT_H
#define CLIENT_H
using namespace std;

/// @brief Class for handling clients. Inherits SaveItem class to be saved on files
class Client : public SaveItem {
    private:
    /// @brief Save path of storage file
    static const string save_path;
    /// @brief Number of created clients
    static int num_clients;
    /// @brief ID of each client
    const string ID;
    /// @brief Client name same as passport name
    const string name;
    /// @brief Client username used for login
    string username;
    /// @brief Client password used for login
    string password;
    /// @brief Passport object linked to the client
    const Passport passport;
    /// @brief Client's email
    string email;
    /// @brief Client's phone number stored as a long
    long phone;
    /// @brief number of miles owned by the client
    int miles = 0;

    string generateID() {
        return to_string(num_clients++);
    }

    public:
    // Constructors
    /// @brief Non-default constructor that creates Client from file record (ID is given)
    /// @param ID Client ID from files
    /// @param name 
    /// @param passport 
    /// @param email 
    /// @param phone 
    /// @param username 
    /// @param password 
    Client(string ID, string name, Passport passport, string email, long phone, string username, string password) :  ID(ID), name(name), passport(passport) {
        this->email = email;
        this->phone = phone;
        this->username = username;
        this->password = password;
        miles = 0;
    }

    /// @brief Non-default for creating new clients from parameters
    /// @param name 
    /// @param passport 
    /// @param email 
    /// @param phone 
    /// @param username 
    /// @param password 
    Client(string name, Passport passport, string email, long phone, string username, string password) :  ID(generateID()), name(name), passport(passport) {
        this->email = email;
        this->phone = phone;
        this->username = username;
        this->password = password;
        miles = 0;
        save();
    }

    // Getter functions

    string getID() const { return ID; }
    string getName() const { return name; }
    Passport getPassport() const { return passport; }
    string getEmail() const { return email; }
    long getPhone() const { return phone; }
    int getMiles() const { return miles; }
    string getUsername() const { return username; }
    
    // Setter functions

    void setEmail(string email) { this->email = email; }
    void setPhone(int phone) { this->phone = phone; }
    void setMiles(int miles) { this->miles = miles; }

    /// @brief Compares given password to saved password
    /// @param pass Entered password
    /// @return Returns pointer to calling object if the password is correct
    Client* validate(string pass) {
        if (pass == password)
            return this;
        return nullptr;
    }

    /// @brief Prints client details
    void print_details() {
        cout << "Client " << ID << endl;
        passport.print_details();
    }

    /// @brief Implemtation of abstract function save from SaveItem class. Saves the client into corresponding storage file
    /// @return True if writing was successful, false otherwise.
    bool save() {
        ofstream writer;
        writer.open(save_path, std::ios_base::app);
        if (writer.fail()) {
            cerr << "Error saving client..." << endl;
            writer.close();
            return false;
        }
        writer << encrypt(ID) << ',';
        writer << encrypt(name) << ',';
        writer << encrypt(passport.getID()) << ',';
        writer << encrypt(to_string(passport.getType())) << ',';
        writer << encrypt(passport.getName()) << ',';
        writer << encrypt(CountryEnum_to_string(passport.getCountry())) << ',';
        writer << encrypt(tm_to_date(passport.getDoB())) << ',';
        writer << encrypt(tm_to_date(passport.getDoI())) << ',';
        writer << encrypt(tm_to_date(passport.getDoE())) << ',';
        writer << encrypt(to_string(passport.getSex())) << ',';
        writer << encrypt(email) << ',';
        writer << encrypt(to_string(phone)) << ',';
        writer << encrypt(username) << ',';
        writer << encrypt(password) << ',';
        writer << endl;
        writer.close();
        return true;
    }

    /// @brief Loads all the Clients stored in the corresponding file into a vector
    /// @return Vector of all loaded clients
    static vector<Client> loadAll() {
        vector<Client> clients;
        ifstream reader;
        reader.open(save_path);
        if (reader.fail()) {
            cerr << "Error loading clients..." << endl;
            reader.close();
            return vector<Client>();
        }
        string temp;
        num_clients = 0;
        while (getline(reader, temp)) {
            num_clients++;
            string ID, name, email, username, password, passport_ID;
            tm DoB, DoI, DoE;
            CountryEnum country;
            char type, sex;
            long phone;
            stringstream s_stream(temp);
            getline(s_stream, temp, ',');
            ID = decrypt(temp);
            cout << ID << endl;
            getline(s_stream, temp, ',');
            name = decrypt(temp);
            cout << name << endl;
            getline(s_stream, temp, ',');
            passport_ID = decrypt(temp);
            cout << passport_ID << endl;
            getline(s_stream, temp, ',');
            type = decrypt(temp)[0];
            cout << type << endl;
            getline(s_stream, temp, ',');
            name = decrypt(temp);
            getline(s_stream, temp, ',');
            temp = decrypt(temp);
            cout << temp << endl;
            country = string_to_CountryEnum(temp);
            getline(s_stream, temp, ',');
            temp = decrypt(temp);
            cout << temp << endl;
            DoB = date_to_tm(temp);
            getline(s_stream, temp, ',');
            temp = decrypt(temp);
            cout << temp << endl;
            DoI = date_to_tm(temp);
            getline(s_stream, temp, ',');
            temp = decrypt(temp);
            cout << temp << endl;
            DoE = date_to_tm(temp);
            getline(s_stream, temp, ',');
            sex = decrypt(temp)[0];
            cout << "Test" << endl;
            getline(s_stream, temp, ',');
            email = decrypt(temp);
            cout << "Test" << endl;
            getline(s_stream, temp, ',');
            phone = stol(decrypt(temp));
            cout << "Test" << endl;
            getline(s_stream, temp, ',');
            username = decrypt(temp);
            cout << "Test" << endl;
            getline(s_stream, temp, ',');
            password = decrypt(temp);
            cout << "Test" << endl;
            clients.push_back(Client(ID, name, Passport(passport_ID, type, name, country, DoB, DoI, DoE, sex), email, phone, username, password));
        }
        reader.close();
        return clients;
    }

};

// Static variables
int Client::num_clients = 0;
const string Client::save_path = "SaveData/Clients.csv";

#endif