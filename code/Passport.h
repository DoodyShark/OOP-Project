#include "Conversions.h"
#include "Country.h"
#ifndef PASSPORT_H
#define PASSPORT_H

using namespace Country;
using namespace std;

/// @brief Class to handle passports
class Passport {
    private:
    const string ID;
    char type;
    const string name;
    const CountryEnum country;
    const tm DoB;
    const tm DoI;
    const tm DoE;
    const char sex;
    public:
    Passport(string ID, char type, string name, CountryEnum country, tm DoB, tm DoI, tm DoE, char sex) : ID(ID), type(type), name(name), country(country), DoB(DoB), DoI(DoI), DoE(DoE), sex(sex){}

    // Getter functions
    string getID() const { return ID; }
    char getType() const { return type; }
    string getName() const { return name; }
    CountryEnum getCountry() const { return country; }
    tm getDoB() const { return DoB; }
    tm getDoI() const { return DoI; }
    tm getDoE() const { return DoE; }
    char getSex() const { return sex; } 

    /// @brief Prints passport details
    void print_details() const {
        cout << "Passport Holder: " << name << " | Sex: " << sex << endl;
        cout << "Country: " << CountryEnum_to_string(country) << endl;
        cout << "Passport Number: " << ID << " | Type: " << type << endl;
        cout << "DoB: " << tm_to_date(DoB) << endl;
        cout << "DoI: " << tm_to_date(DoI) << " | DoE: " << tm_to_date(DoE) << endl;
    }

};

#endif