#include <iostream>
#include <iomanip>
#include <fstream>
#include "Flight.h"

using namespace std;

/// @brief Namespace for Client interface
namespace ClientInterface
{

    // Vectors of all necessary objects to store   
    vector<Airplane> planes;
    vector<Client> clients;
    vector<Flight> flights;
    vector<Record> records;

    /// @brief Clients are loaded from file only when this is false
    bool loaded_clients = false;
    /// @brief Planes and flights are loaded from file only when this is false
    bool loaded_planes_flights = false;

    /// @brief Pointer to the current user
    Client* current_user = nullptr;

    /// @brief Books an inventory item and creates a record of the transaction
    /// @param inventory Pointer of Inventory item to be booked
    /// @param client Pointer to client object that performed the transaction
    /// @param reservation_time The time related to the transaction
    void BookInventory(Inventory *inventory, Client *client, tm reservation_time)
    {
        records.push_back(inventory->Purchase(client, reservation_time));
    }

    // Forward declaration
    /// @brief Namespace for Home interface screen interactions
    namespace Home {

        int BookingMap(int menu_num);

        int Menu(int menu_num);

    }

    // Forward declaration
    /// @brief Namespace for SignUp/Login interface screen interactions
    namespace signup_login {

        void StartUp();

        bool login(string username, string password);
        
        void SignUp(string name, string username, string password, Passport passport, string email, long phone);

        int Menu(int menu_num);
    }

    /// @brief Namespace for Flights interface screen interactions
    namespace Flights {

        /// @brief Perform necessary start up processes before entering Flights interface
        void StartUp() {
            if (!loaded_planes_flights) {
                planes = Airplane::loadAll();
                flights = Flight::loadAll(planes);
                loaded_planes_flights = true;
            }
            
        }

        /// @brief Searches through currently loaded flights to find those that match the parameters
        /// @param from Flight's origin airport
        /// @param to Flight's destination airport
        /// @param departure Flight's departure date
        /// @return Number of found flights
        vector<Flight*> DisplayFlights(Airport from, Airport to, tm departure) {
            system("clear");
            cout << "---------------------------------------------------------" << endl;
            cout << "Available Flights: " << endl;
            vector<Flight*> available_flights;
            for (int i = 0; i < flights.size(); i++) {
                // Checking if the current flight matches the parameters
                if ((flights[i].getOrigin() == from) && (flights[i].getDestination() == to) && (flights[i].getT_Depart().tm_mday == departure.tm_mday) && (flights[i].getT_Depart().tm_mon == departure.tm_mon) && (flights[i].getT_Depart().tm_year == departure.tm_year)){
                    available_flights.push_back(&flights[i]);
                    cout << (available_flights.size() - 1) << " - ";
                    flights[i].print_info();
                }
            } 
            if (available_flights.size() == 0) {
                cout << "No such flights found..." << endl;
            }
            return available_flights;
        }

        /// @brief Books a flight seat for given client
        /// @param flight_ID The flight to book
        /// @param client_ID The client to book for
        /// @param category Which category of seat to book
        /// @param row Row of the booked seat
        /// @param col Column of booked seat
        void BookFlightSeat(string flight_ID, string client_ID, int category, int row, int col)
        {
            if (!flights[stoi(flight_ID)].getSeat(category, row, col)->getReserved()) {
                BookInventory(flights[stoi(flight_ID)].getSeat(category, row, col), &clients[stoi(client_ID)], flights[stoi(flight_ID)].getT_Depart());
                flights[stoi(flight_ID)].OverwriteSave();
            }
            else
                cerr << "Seat already reserved..." << endl;
        }

        /// @brief Recursive menu display and user input reader for Flight Booking interface
        /// @param menu_num  Menu to display
        /// @return Next menu in the recursive call or -1 if exit
        int Menu(int menu_num) {
            system("clear");
            cout << "---------------------------------------------------------" << endl;
            int selection;
            if (menu_num == 0) {
                string origin, destination, departure_day;
                cout << "One Way Flight" << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                cout << "From (Airport Code): ";
                cin >> origin;
                cout << "To (Airport Code): ";
                cin >> destination;
                cout << "Departure Date (DD/MM/YYYY): ";
                cin >> departure_day;
                vector<Flight*> available = DisplayFlights(string_to_Airport(origin), string_to_Airport(destination), date_to_tm(departure_day));
                if (available.size() == 0) {
                    cout << "Enter any number to return..." << endl;
                    cin >> selection;
                    return Home::Menu(0);
                }
                cin >> selection;
                system("clear");
                cout << "---------------------------------------------------------" << endl;
                available[selection]->printSeats();
                cout << "Pick seat (e.g. 0 A 3): ";
                int cat, row;
                string col;
                cin >> cat >> col >> row;
                BookFlightSeat(available[selection]->getID(), current_user->getID(), cat, row, (int) string_to_Column(col));
                return Home::Menu(0);
            }
            return Home::Menu(0);
        }

    }


    /// @brief Namespace for handling Cars rental screen interface for future incorporation of the module
    namespace Cars {

        /// @brief Perform necessary start up processes before entering Cars interface
        void StartUp() {

        }

        /// @brief Recursive menu display and user input reader for Car Rental interface
        /// @param menu_num  Menu to display
        /// @return Next menu in the recursive call or -1 if exit
        int Menu(int menu_num) {
            system("clear");
            cout << "---------------------------------------------------------" << endl;
            int selection;
            cout << "Car Rental Interface" << endl;
            cout << "Under Construction :(" << endl;
            cout << "Enter any number to return..." << endl;
            cin >> selection;
            return Home::Menu(0);
        }

    }
    
    /// @brief Namespace for handling Hotel Booking screen interface for future incorporation of the module
    namespace Hotels {

        /// @brief Perform necessary start up processes before entering Hotels interface
        void StartUp() {

        }

        /// @brief Recursive menu display and user input reader for Hotel Booking interface
        /// @param menu_num  Menu to display
        /// @return Next menu in the recursive call or -1 if exit
        int Menu(int menu_num) {
            system("clear");
            cout << "---------------------------------------------------------" << endl;
            int selection;
            cout << "Hotel Reservation Interface" << endl;
            cout << "Under Construction :(" << endl;
            cout << "Enter any number to return..." << endl;
            cin >> selection;
            return Home::Menu(0);
        }

    }


    /// @brief Portal function between different interface modules
    /// @param menu_num Which interface module to link to
    /// @return Output of Menu function in the chosen module
    int Home::BookingMap(int menu_num) {
        if (menu_num == 0) { 
            Flights::StartUp();
            return Flights::Menu(0);
        }
        else if (menu_num == 1) {
            Cars::StartUp();
            return Cars::Menu(0);
        }
        else if (menu_num == 2) {
            Hotels::StartUp();
            return Hotels::Menu(0);
        }
        // Space for future menus
        return -1;
    }

    /// @brief Recursive menu display and user input reader for Home interface
    /// @param menu_num  Menu to display
    /// @return Next menu in the recursive call or -1 if exit
    int Home::Menu(int menu_num) {
        system("clear");
        cout << "---------------------------------------------------------" << endl;
        int selection;
        if (menu_num == 0) {
            cout << "Home Screen" << endl;
            cout << "Enter the associated number for your choice:" << endl;
            cout << "0 - Book" << endl;
            cout << "1 - Manage" << endl;
            cout << "2 - Check user info" << endl;
            cout << "3 - Log Out" << endl;
            cin >> selection;
            if (selection == 3) {
                current_user = nullptr;
                return signup_login::Menu(0);
            }
            return Menu(selection + 1);
        }
        else if (menu_num == 1) {
            cout << "Booking Screen" << endl;
            cout << "Enter the associated number for your choice:" << endl;
            cout << "0 - Flights" << endl;
            cout << "1 - Car Rental" << endl;
            cout << "2 - Hotel Booking" << endl;
            cout << "3 - Return" << endl;
            cin >> selection;
            if (selection == 3) {
                return Menu(selection - 1);
            }
            return BookingMap(selection);
        }
        else if (menu_num == 2) {
            cout << "Manage Booking Screen" << endl;
            cout << "Under Construction :(" << endl;
            cout << "Enter any number to return..." << endl;
            cin >> selection;
            return Menu(0);
        }
        else if (menu_num == 3) {
            cout << "User info: " << endl;
            current_user->print_details();
            cout << "Enter any number to return..." << endl;
            cin >> selection;
            return Menu(0);
        }
        return -1;
    }

    /// @brief Performs necessary start up processes for SignUp/Login screen
    void signup_login::StartUp() {
        if (!loaded_clients) {
            current_user = nullptr;
            clients = Client::loadAll();
            loaded_clients = true;
        }
    }

    /// @brief Logs in using the entered information
    /// @param username Input username
    /// @param password Input password
    /// @return Whether username and password match an existing client
    bool signup_login::login(string username, string password) {
        for (int i = 0; i < clients.size(); i++) {
            if (clients[i].getUsername() == username) {
                current_user = clients[i].validate(password);
            }
        }
        if (current_user != nullptr)
            return true;
        return false;
    }

    /// @brief Creates a new account with the given information
    /// @param name New client's name
    /// @param username New client's username
    /// @param password New client's password
    /// @param passport New client's passport
    /// @param email New client's email
    /// @param phone New client's phone
    void signup_login::SignUp(string name, string username, string password, Passport passport, string email, long phone)
    {
        clients.push_back(Client(name, passport, email, phone, username, password));
    }

    /// @brief Recursive menu display and user input reader for SignUp/Login interface
    /// @param menu_num  Menu to display
    /// @return Next menu in the recursive call or -1 if exit
    int signup_login::Menu(int menu_num) {
        system("clear");
        cout << "---------------------------------------------------------" << endl;
        int selection;
        if (menu_num == 0) {
            cout << "Welcome to Mistika Airways Computer Reservation System..." << endl;
            cout << "---------------------------------------------------------" << endl;
            cout << "Log in or Register" << endl;
            cout << "Enter the associated number for your choice:" << endl;
            cout << "0 - Log in" << endl;
            cout << "1 - Register" << endl;
            cout << "2 - Exit" << endl;
            cin >> selection;
            return Menu(selection + 1);
        }
        else if (menu_num == 1) {
            string username, password;
            cout << "Log in" << endl;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            while (!login(username, password)) {
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                cout << "Incorrect user details..." << endl;
                cout << "Enter 0 to retry, any other number else to return: ";
                cin >> selection;
                if (selection == 0) {
                    cout << "Username: ";
                    cin >> username;
                    cout << "Password: ";
                    cin >> password;
                }
                else {
                    return Menu(menu_num - 1);
                }
            }
            return Home::Menu(0);
        }

        else if (menu_num == 2) {
            string name, username, password, passport_ID, country, DoB, DoI, DoE, email;
            long phone;
            char passport_type, sex;
            cout << "Register" << endl;
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
            cout << "Passport details: " << endl;
            cout << "Country: ";
            cin >> country;
            cout << "Full name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Passport ID: ";
            cin >> passport_ID;
            cout << "Passport Type: ";
            cin >> passport_type;
            cout << "Date of Birth (DD/MM/YYYY): ";
            cin >> DoB;
            cout << "Date of Issue (DD/MM/YYYY): ";
            cin >> DoI;
            cout << "Date of Expiry (DD/MM/YYYY): ";
            cin >> DoE;
            cout << "Sex: ";
            cin >> sex;
            cout << "Account Details: " << endl;
            system("clear");
            cout << "Register" << endl;
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            cout << "Email: ";
            cin >> email;
            cout << "Phone number: ";
            cin >> phone;
            SignUp(name, username, password, Passport(passport_ID, passport_type, name, string_to_CountryEnum(country), date_to_tm(DoB), date_to_tm(DoI), date_to_tm(DoE), sex), email, phone);
            return Menu(1);
        }
        return -1;
    }

}

using namespace ClientInterface;

int main()
{
    // Starting up the SignUp/Login interface
    signup_login::StartUp();
    // Opening the SignUp/Login interface
    signup_login::Menu(0);

    // Only reaches this point if the user exits
    cout << "Thanks for using our services..." << endl;
    return 0;
}