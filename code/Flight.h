#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include "Airplane.h"
#include "Airport.h"
#ifndef FLIGHT_H
#define FLIGHT_H

using namespace std;
using namespace AirportInfo;

/// @brief Class for handling Flights
class Flight : public SaveItem {
    private:
    /// @brief Path for storage file
    static const string save_path;
    /// @brief Number of created flights
    static int num_flights;
    /// @brief Corresponding flight ID
    const string ID;
    /// @brief 3D array of seats (2D array for each category)
    vector<vector<vector<Seat>>> seats;
    /// @brief Prive per category
    vector<double> category_price;
    /// @brief Associated plane
    Airplane* plane;
    /// @brief Departure time and date
    tm t_depart;
    /// @brief Arrival time and date
    tm t_arrive;
    /// @brief Origin airport
    Airport origin;
    /// @brief Destination airport
    Airport destination;

    /// @brief Creates a unique ID based on the number of created flights
    /// @return unique string ID for the flight
    string generateID() {
        return to_string(num_flights++);
    }

    /// @brief Generates 3D array of seats based on the dimensions of the given plane
    void generateSeats() {
        int numCategories = plane->getNumCategories();
        for (int i = 0 ; i < plane->getNumCategories(); i++) {
            int numRows = plane->getDimensions()[i][0];
            int numColumns = plane->getDimensions()[i][1];
            vector<vector<Seat>> temp_2D;
            for (int r = 0; r < numRows; r++) {
                vector<Seat> temp;
                for (int c = 0; c < numColumns; c++) {
                    temp.push_back(Seat(ID, r, (Column)c, category_price[i]));
                }
                temp_2D.push_back(temp);
            }
            seats.push_back(temp_2D);
        }    
    }

    public:
    /// @brief Non default construct to create Flight from file (ID is given not generated)
    /// @param ID 
    /// @param plane 
    /// @param t_depart 
    /// @param t_arrive 
    /// @param origin 
    /// @param destination 
    /// @param category_price 
    Flight(string ID, Airplane* plane, tm t_depart, tm t_arrive, Airport origin, Airport destination, vector<double> category_price) : ID(ID) {
        this->plane = plane;
        this->t_depart = t_depart;
        this->t_arrive = t_arrive;
        this->origin = origin;
        this->destination = destination;
        this->category_price = category_price;
        generateSeats();
    }

    /// @brief Non default constructor for creating new flights based on input
    /// @param plane 
    /// @param t_depart 
    /// @param t_arrive 
    /// @param origin 
    /// @param destination 
    /// @param category_price 
    Flight(Airplane* plane, tm t_depart, tm t_arrive, Airport origin, Airport destination, vector<double> category_price) : ID(generateID()) {
        this->plane = plane;
        this->t_depart = t_depart;
        this->t_arrive = t_arrive;
        this->origin = origin;
        this->destination = destination;
        this->category_price = category_price;
        generateSeats();
        save();
    }

    // Getter functions

    string getID() const { return ID; }
    Seat* getSeat(int category, int row, int col) { return &(seats[category][row][col]); }
    vector<double> getCategoryPrice() const { return category_price; }
    Airplane* getPlane() const { return plane; }
    tm getT_Depart() const { return t_depart; }
    tm getT_Arrive() const { return t_arrive; }
    Airport getOrigin() const { return origin; }
    Airport getDestination() const { return destination; }

    /// @brief Generate strings of the reservation state of the seats in each category
    /// @return vector of strings detailing the resercation state of each category's seat
    vector<string> getAllSeatStates() {
        vector <string> res;
        for (int i = 0; i < seats.size(); i++) {
            string str = "";
            for (int r = 0; r < seats[i].size(); r++) {
                for (int c = 0; c < seats[i][r].size(); c++) {
                    str += to_string(seats[i][r][c].getReserved());
                }
            }
            res.push_back(str);
        }
        return res;
    }

    /// @brief Takes in the reservation state of all the seats in each category and reflects them on the actual seat objects
    /// @param string_vec vector of strings storing the reservation state of all seats
    void AssignSeatStatesfromStrings(vector<string> string_vec) {
        for (int i = 0; i < seats.size(); i++) {
            for (int r = 0; r < seats[i].size(); r++) {
                for (int c = 0; c < seats[i][r].size(); c++) {
                    if (string_vec[i][r*seats[i][r].size() + c] == '1') {
                        seats[i][r][c].Reserve();
                    }
                }
            }
        }
    }


    /// @brief Prints all the seats including prices, categories, columns, rows, and reservation state
    void printSeats() const {
        int numCategories = plane->getNumCategories();
        for (int i = 0 ; i < plane->getNumCategories(); i++) {
            int numRows = plane->getDimensions()[i][0];
            int numColumns = plane->getDimensions()[i][1];
            cout << "Category " << i << " ($" << category_price[i] << ")" << endl;
            for (int c = 0; c < numColumns; c++)
                cout << ' ' << ColumnInfo::Col_to_String((Column) c) << ' ';
            cout << endl;
            for (int r = 0; r < numRows; r++) {
                for (int c = 0; c < numColumns; c++)
                    cout << '[' << seats[i][r][c].getReserved() << ']';
                cout << r << endl;
            }
        }   
    }

    /// @brief Prints flight information
    void print_info() const {
        cout << "Flight: " << ID << " | " << Airport_to_String(origin) << " (" << tm_to_date_time(t_depart) << ") --> " << Airport_to_String(destination) << "(" << tm_to_date_time(t_arrive) << ")" << endl;
    }

    /// @brief Updates the flight record in the storage file to match any changes
    /// @return True if the update works, false otherwise
    bool OverwriteSave() {
        bool saved = false;
        vector<string> flights;
        ifstream reader;
        reader.open(save_path);
        if (reader.fail()) {
            cerr << "Error loading planes..." << endl;
            reader.close();
            return false;
        }
        string temp;
        // Save old file
        while(getline(reader, temp)) {
            flights.push_back(temp);
        }
        reader.close();
        ofstream writer;
        writer.open(save_path, ios::out | std::ofstream::trunc); // Clear the file
        if (writer.fail()) {
            cerr << "Error updating flights..." << endl;
            return false;
        }
        // Rewrite all the flight records except for the one with representing the calling Flight object
        // For the calling flight object, the save() function is called to reflect the new changes
        for (int i = 0; i < flights.size(); i++) {
            if (!saved) {
                string temp2;
                stringstream s_stream(flights[i]);
                getline(s_stream, temp2, ',');
                temp2 = decrypt(temp2);
                if (temp2 == ID) {
                    writer.close();
                    save();
                    saved = true;
                    writer.open(save_path, ios::app);
                    if (writer.fail()) {
                        cerr << "Error updating flights..." << endl;
                        return false;
                    }
                    continue;
                }
            }
            writer << flights[i] << endl;
        }
        writer.close();
        return true;
    }

    /// @brief Implementation of the abstract function in the SaveItem class to save Flight to the storage file
    /// @return True if the writing is successful, false otherwise
    bool save() {
        ofstream writer;
        writer.open(save_path, std::ios_base::app);
        if (writer.fail()) {
            cerr << "Error saving plane..." << endl;
            writer.close();
            return false;
        }
        writer << encrypt(ID) << ',';
        writer << encrypt(plane->getID()) << ',';
        writer << encrypt(tm_to_date_time(t_depart)) << ',';
        writer << encrypt(tm_to_date_time(t_arrive)) << ',';
        writer << encrypt(Airport_to_String(origin)) << ',';
        writer << encrypt(Airport_to_String(destination)) << ',';
        for (int i = 0; i < category_price.size(); i++) {
            writer << encrypt(to_string(category_price[i])) << ',';
        }
        vector<string> seat_strings = getAllSeatStates();
        for (int i = 0; i < seat_strings.size(); i++) {
            writer << encrypt(seat_strings[i]) << ',';
        }
        writer << endl;
        writer.close();
        return true;
    }


    /// @brief Returns the plane with the corresponding ID
    /// @param ID ID to search for
    /// @param planes Vector of all planes passed by reference
    /// @return Airplane pointer for the matching plane
    static Airplane* findPlanefromID(string ID, vector<Airplane> &planes) {
        for (int i = 0; i < planes.size(); i++) {
            if (planes[i].getID() == ID)
                return &(planes[i]);
        }
        return nullptr;
    }

    /// @brief Loads all the flights 
    /// @param planes Currently loaded plane to link them to the flights
    /// @return vector of loaded flights
    static vector<Flight> loadAll(vector<Airplane> &planes) {
        vector<Flight> flights;
        ifstream reader;
        reader.open(save_path);
        if (reader.fail()) {
            cerr << "Error loading planes..." << endl;
            reader.close();
            return vector<Flight>();
        }
        string temp;
        num_flights = 0;
        while (getline(reader, temp)) {
            num_flights++;
            string flightID, planeID, t_depart, t_arrive, origin, destination;
            double price;
            vector<double> category_price;
            stringstream s_stream(temp);
            getline(s_stream, temp, ',');
            flightID = decrypt(temp);
            getline(s_stream, temp, ',');
            planeID = decrypt(temp);
            Airplane* plane = findPlanefromID(planeID, planes);
            getline(s_stream, temp, ',');
            t_depart = decrypt(temp);
            getline(s_stream, temp, ',');
            t_arrive = decrypt(temp);
            getline(s_stream, temp, ',');
            origin = decrypt(temp);
            getline(s_stream, temp, ',');
            destination = decrypt(temp);
            for (int i = 0; i < plane->getNumCategories(); i++) {
                getline(s_stream, temp, ',');
                temp = decrypt(temp);
                category_price.push_back(stod(temp));
            }
            flights.push_back(Flight(flightID, plane, Conversions::tm_conversions::date_time_to_tm(t_depart.substr(6, 10), t_depart.substr(0, 5)), Conversions::tm_conversions::date_time_to_tm(t_arrive.substr(6, 10), t_arrive.substr(0, 5)), string_to_Airport(origin), string_to_Airport(destination), category_price));
            vector<string> seats_vec;
            for (int i = 0; i < plane->getNumCategories(); i++) {
                getline(s_stream, temp, ',');
                temp = decrypt(temp);
                seats_vec.push_back(temp);
            }

            flights[flights.size() - 1].AssignSeatStatesfromStrings(seats_vec);

        }
        reader.close();
        return flights;
    }

};

// Static variables
const string Flight::save_path = "SaveData/Flights.csv";
int Flight::num_flights = 0;

#endif