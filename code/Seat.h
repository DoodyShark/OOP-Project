#include "Column.h"
#include "Record.h"
#define stringify( name ) #name
#ifndef SEAT_H
#define SEAT_H

using namespace std;
using namespace ColumnInfo;

/// @brief Class to handle seats
class Seat : public Inventory {
    
    private:
    /// @brief Linked flight ID
    const string flight_ID;
    /// @brief Column enum
    Column col;
    int row;
    bool reserved = false;

    /// @brief Generates a seat ID based on the flight ID and seat position
    /// @param flight_ID 
    /// @param row 
    /// @param col 
    /// @return unique string identifier
    string generateID(string flight_ID, int row, Column col) {
        return flight_ID + Col_to_String(col) + to_string(row);
    }

    public:
    Seat(string flight_ID, int row, Column col, double price) : flight_ID(flight_ID), Inventory(generateID(flight_ID, row, col), price) {
        this->row = row;
        this->col = col;
    }

    // Getter functions
    string getFlightID() const { return flight_ID; }
    int getRow() const { return row; }
    Column getCol() const { return col; }
    int getColAsInt() const { return (int) col; }
    bool getReserved() const { return reserved; }

    /// @brief Reserves the seat
    /// @return true if successful, false otherwise
    bool Reserve() {
        if (reserved)
            return false;
        reserved = true;
        return true;
    }

    /// @brief For canceling reservations
    void Cancel() {
        reserved = false;
    }

    /// @brief Implementation of abstract function in Inventory class
    /// @param client Client purchasing the seat
    /// @param reservation_time Date of the flight
    /// @return Record (PNR) of the transaction
    Record Purchase(Client* client, tm reservation_time) {
        if (Reserve())
            return Record(this, client, reservation_time);
        return Record();
    }

    /// @brief Prints Seat details
    void print_details() {
        cout << "Seat " << Col_to_String(col) << row << " | Flight: " << flight_ID << endl; 
    }

};

#endif