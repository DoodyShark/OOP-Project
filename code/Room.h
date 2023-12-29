#include "Inventory.h"
#ifndef ROOM_H
#define ROOM_H

using namespace std;

/// @brief Room class for dealing with Hotel Room Booking. Class is added for showing modularity and isn't fully functional
class Room : public Inventory {
    private:
    static int num_rooms;
    string type;

    string generateID() {
        return to_string(num_rooms++);
    }

    public:
    Room(double price, string type) : Inventory(generateID(), price) {
        this->type = type;
    }

    Record Purchase(Client* client, tm reservation_date) {
        return Record(this, client, reservation_date);
    }  
};

int Room::num_rooms = 0;

#endif