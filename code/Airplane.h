#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Seat.h"
#ifndef AIRPLANE_H
#define AIRPLANE_H

using namespace std;

/// @brief Airplane class for creating Airplane objects.
class Airplane : public SaveItem {
    private:
    /// @brief Total number of planes created
    static int num_planes;
    /// @brief Path to storage file
    static string save_path;
    /// @brief  The ID should never be altered after a plane object has been created
    const string ID; 
    /// @brief Model of airplane
    string model;
    /// @brief Number of seating categories (first class, business class, etc)
    int num_categories; 
    /// @brief Dimensions of each category in a plane [[r_1, c_1], [r_2, c_2], ...]
    vector<vector<int>> dimensions;  

    /// @brief Creates a unique ID based on the current number of created planes
    /// @return Unique string ID
    string generateID() {
        return to_string(num_planes++);
    }

    public:
    /// @brief Non-default Airplane constructor to load Airplane from file (ID is given)
    /// @param ID 
    /// @param model 
    /// @param num_categories 
    /// @param dimensions 
    Airplane(string ID, string model, int num_categories, vector<vector<int>> dimensions) : ID(ID) {
        this->model = model;
        this->num_categories = num_categories;
        this->dimensions = dimensions;
    }

    /// @brief Non-default Airplane constructor
    /// @param model 
    /// @param num_categories 
    /// @param dimensions 
    Airplane(string model, int num_categories, vector<vector<int>> dimensions) : ID(generateID()) {
        this->model = model;
        this->num_categories = num_categories;
        this->dimensions = dimensions;
        save();
    }

    /// @brief Updates dimensions to vec
    /// @param vec New dimensions vector
    void setDimensions(vector<vector<int>> vec) {
        num_categories = vec.size();
        dimensions = vec;
    }

    // Getter Functions
    string getID() const { return ID; }
    int getNumCategories() const { return num_categories; }
    vector<vector<int>> getDimensions() const { return dimensions; }

    /// @brief Implementation of abstract function in SaveItem class. Used to save the plane object to a file
    /// @return True if the writing process was a success, false otherwise
    bool save() {
        ofstream writer;
        writer.open(save_path, std::ios_base::app);
        if (writer.fail()) {
            cerr << "Error saving plane..." << endl;
            writer.close();
            return false;
        }
        writer << encrypt(ID) << ',';
        writer << encrypt(model) << ',';
        writer << encrypt(to_string(num_categories)) << ',';
        for (int i = 0; i < dimensions.size(); i++) {
            writer << encrypt(to_string(dimensions[i][0]) + ' ' + to_string(dimensions[i][1])) << ',';
        }
        writer << endl;
        writer.close();
        return true;
    }

    /// @brief Loads all the Airplanes into a vector from the file save_path
    /// @return A vector of planes extracted from the files
    static vector<Airplane> loadAll() {
        vector<Airplane> planes;
        ifstream reader;
        reader.open(save_path);
        if (reader.fail()) {
            cerr << "Error loading planes..." << endl;
            reader.close();
            return vector<Airplane>();
        }
        string temp;
        num_planes = 0;
        // csv file read line by line then with commas as delimiters
        while (getline(reader, temp)) {
            num_planes++;
            string ID, model;
            int num_categories;
            vector<vector<int>> dimensions;
            stringstream s_stream(temp);
            getline(s_stream, temp, ',');
            ID = decrypt(temp);
            getline(s_stream, temp, ',');
            model = decrypt(temp);
            getline(s_stream, temp, ',');
            temp = decrypt(temp);
            num_categories = stoi(temp);
            for (int i = 0; i < num_categories; i++) {
                getline(s_stream, temp, ',');
                temp = decrypt(temp);

                stringstream s_stream2(temp);
                vector<int> temp_vec;
                s_stream2 >> temp;
                temp_vec.push_back(stoi(temp));
                s_stream2 >> temp;
                temp_vec.push_back(stoi(temp));

                dimensions.push_back(temp_vec);
            }
            planes.push_back(Airplane(ID, model, num_categories, dimensions));
        }
        reader.close();
        return planes;
    }

    /// @brief Prints Airplane details
    void print_details() {
        cout << "ID: " << ID << " | Model: " << model << " | Category count: " << num_categories << endl;
    }

};

// Static variables
int Airplane::num_planes = 0;
string Airplane::save_path = "SaveData/Airplanes.csv";

#endif