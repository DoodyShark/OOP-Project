#include <iostream>
#include <fstream>
#include "Flight.h"

/// @brief Interface for handling all Administrator interactions
namespace AdminInterface {

    // Vectors storing all loaded objects
    vector<Airplane> planes;
    vector<Client> clients;
    vector<Flight> flights;
    vector<Record> records;

    /// @brief Loading of data from files is only done when loaded is false
    bool loaded = false;

    /// @brief All save paths to the files.
    vector<string> paths = {"SaveData/Airplanes.csv", "SaveData/Clients.csv", "SaveData/Flights.csv", "SaveData/Records.csv"};


    /// @brief Clear all data in the program and in the files
    void clearAll() {
        for (int i = 0; i < paths.size(); i++) {
            ofstream clearer;
            clearer.open(paths[i], std::ofstream::out | std::ofstream::trunc);
            clearer.close();
        }
        planes.clear();
        clients.clear();
        flights.clear();
        records.clear();
        loaded = false;
    }
    
    // Forward declaration \
    /// @brief Namespace for handling Home screen of the interface
    namespace Home {

        void StartUp();

        int InventoryMapping(int menu_num);
        
        int Menu(int menu_num);
        
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
            cout << "Car Rental Creation" << endl;
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
            cout << "Hotel Rental Creation" << endl;
            cout << "Under Construction :(" << endl;
            cout << "Enter any number to return..." << endl;
            cin >> selection;
            return Home::Menu(0);
        }

    }


    /// @brief Namespace for handling Flight Booking screen interface 
    namespace Flights {

        /// @brief Perform necessary start up processes before entering FLights interface
        void StartUp() {
            if (!loaded) {
                planes = Airplane::loadAll();
                flights = Flight::loadAll(planes);
                loaded = true;
            }
        }

        /// @brief Create a plane object using given parameters
        /// @param model 
        /// @param num_categories 
        /// @param dimensions 
        void CreatePlane(string model, int num_categories, vector<vector<int>> dimensions) {
            planes.push_back(Airplane(model, num_categories, dimensions));
        }


        /// @brief Create flight object using given information
        /// @param plane 
        /// @param t_depart 
        /// @param t_arrive 
        /// @param origin 
        /// @param destination 
        /// @param category_price 
        void CreateFlight(Airplane* plane, tm t_depart, tm t_arrive, Airport origin, Airport destination, vector<double> category_price) {
            flights.push_back(Flight(plane, t_depart, t_arrive, origin, destination, category_price));
        }

        /// @brief Recursive menu display and user input reader for Flight Booking interface
        /// @param menu_num  Menu to display
        /// @return Next menu in the recursive call or -1 if exit
        int Menu(int menu_num) {
            system("clear");
            cout << "------------------------------------------------------------------------" << endl;
            int selection;
            if (menu_num == 0) {
                cout << "Flight Interactions" << endl;
                cout << "Enter the associated number for your choice:" << endl;
                cout << "0 - Add Airplanes" << endl;
                cout << "1 - Create Flights" << endl;
                cout << "2 - View Flights" << endl;
                cout << "3 - Return" << endl;
                cin >> selection;
                return Menu(selection + 1);
            }
            else if (menu_num == 1) {
                string model;
                int num_categories;
                vector<vector<int>> dimensions;
                cout << "Add Plane" << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                cout << "Model: ";
                cin.ignore();
                getline(cin, model);
                cout << "Number of categories (First class/Business Class/Economy Class etc.): ";
                cin >> num_categories;
                for (int i = 0; i < num_categories; i++) {
                    int row, col;
                    cout << "Enter row and column count for category " << i << ": ";
                    cin >> row >> col;
                    vector<int> temp = {row, col};
                    dimensions.push_back(temp);
                }
                CreatePlane(model, num_categories, dimensions);
                return Menu(0);
            }
            else if (menu_num == 2) {
                cout << "Create Flight" << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                cout << "Plane selection: " << endl;
                cout << "Enter the associated number for your choice and -1 to create plane..." << endl;
                for (int i = 0; i < planes.size(); i++) {
                    cout << i << " - ";
                    planes[i].print_details();
                }
                int plane_index;
                cin >> plane_index;
                if (plane_index == -1) {
                    return Menu(1);
                }
                system("clear");
                cout << "------------------------------------------------------------------------" << endl;
                cout << "Create Flight" << endl;
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                vector<double> category_price;
                string t_depart, date_depart, t_arrive, date_arrive, date, origin, destination;
                cout << "Departure date and time GMT (hh:mm DD/MM/YYYY): ";
                cin >> t_depart >> date_depart;
                cout << "Arrival date and time GMT (hh:mm DD/MM/YYYY): ";
                cin >> t_arrive >> date_arrive;
                cout << "From (Airport Code): ";
                cin >> origin;
                cout << "To (Airport Code): ";
                cin >> destination;
                for (int i = 0; i < planes[plane_index].getNumCategories(); i++) {
                    double price;
                    cout << "Price (in $) for category " << i << ": ";
                    cin >> price;
                    category_price.push_back(price);
                }
                CreateFlight(&planes[plane_index], date_time_to_tm(date_depart, t_depart), date_time_to_tm(date_arrive, t_arrive), string_to_Airport(origin), string_to_Airport(destination), category_price);
                return Menu(0);
            }
            else if (menu_num == 3) {
                cout << "Created Flights" << endl;
                for (int i = 0; i < flights.size(); i++) {
                    flights[i].print_info();
                }
                cout << "Enter any number to return..." << endl;
                cin >> selection;
                return Menu(0);
            }
            else if (menu_num == 4) {
                Home::Menu(0);
            }
            return -1;
        }

    }

    /// @brief Perform necessary start up processes before entering Home interface
    void Home::StartUp() {
        
    }

    /// @brief Portal function between different interface modules
    /// @param menu_num Which interface module to link to
    /// @return Output of Menu function in the chosen module
    int Home::InventoryMapping(int menu_num) {
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
        // Space for future modules
        return -1;
    }
    

    /// @brief Recursive menu display and user input reader for Home interface
    /// @param menu_num  Menu to display
    /// @return Next menu in the recursive call or -1 if exit
    int Home::Menu(int menu_num) {
        system("clear");
        cout << "------------------------------------------------------------------------" << endl;
        int selection;
        if (menu_num == 0) {
            cout << "Welcome to Mistika Airways Computer Reservation System - Admin Interface" << endl;
            cout << "------------------------------------------------------------------------" << endl;
            cout << "Home screen" << endl;
            cout << "Enter the associated number for your choice:" << endl;
            cout << "0 - Create Inventory" << endl;
            cout << "1 - Reset All Files" << endl;
            cout << "2 - Exit" << endl;
            cin >> selection;
            return Menu(selection + 1);
        }
        else if (menu_num == 1) {
            cout << "Select inventory type:" << endl;
            cout << "Enter the associated number for your choice:" << endl;
            cout << "0 - Flight" << endl;
            cout << "1 - Car" << endl;
            cout << "2 - Hotel" << endl;
            cin >> selection;
            return InventoryMapping(selection);
        }
        else if (menu_num == 2) {
            clearAll();
            return Menu(0);
        }
        return -1;
    }
}

using namespace AdminInterface;
using namespace std;

int main() {

    // Starting the interface
    Home::Menu(0);

    // Only reached when the user chooses to exit
    cout << "Thanks for using our services..." << endl;
    return 0;
}