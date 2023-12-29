#include "Inventory.h"
#include <iostream>
#ifndef CAR_H
#define CAR_H

using namespace std;

/// @brief Car class for dealing with Car rental. Class is added for showing modularity and isn't fully functional
class Car : public Inventory {
    private:
    /// @brief Number of cars created
    static int num_cars;
    /// @brief Model of car for rent
    string model;
    /// @brief Color of car for rent
    string color;
    // Other attributes will be necessary for full implementation

    string generateID() {
        return to_string(num_cars++);
    }

    public:
    Car(double price, string model, string color ) : Inventory(generateID(), price) {
        this->model = model;
        this->color = color;
    }

    /// @brief Implementation of the abstract function in Inventory to create Record from Purchasing this record
    /// @param client Pointer to client performing the purchase
    /// @param reservation_date Reservation date
    /// @return Record of transaction (PNR)
    Record Purchase(Client* client, tm reservation_date) {
        return Record(this, client, reservation_date);
    }  
};

// Static variables
int Car::num_cars = 0;

#endif