#include <iostream>
#include <iomanip>
#include <ctime>
#ifndef CONVERSIONS_H
#define CONVERSIONS_H

using namespace std;

/// @brief Namespace for converting date time conversions between strings and tm
namespace Conversions {

    namespace tm_conversions {

        string tm_to_date(tm time) {
            stringstream stream;
            stream << setfill('0') << setw(2) << time.tm_mday << '/' << setfill('0') << setw(2) << time.tm_mon << '/' << setfill('0') << setw(2) << time.tm_year;
            string date;
            stream >> date; 
            return date;
        }

        tm date_to_tm(string date) {
            tm time;
            time.tm_year = stoi(date.substr(6,4));
            time.tm_mon = stoi(date.substr(3,2));
            time.tm_mday = stoi(date.substr(0,2));
            return time;
        }

        string tm_to_date_time(tm time) {
            stringstream stream;
            stream << setfill('0') << setw(2) << time.tm_hour << ':' << setfill('0') << setw(2) << time.tm_min << " "; 
            stream << setfill('0') << setw(2) << time.tm_mday << '/' << setfill('0') << setw(2) << time.tm_mon << '/' << setfill('0') << setw(2) << time.tm_year;
            string time_str;
            string date;
            stream >> time_str;
            stream >> date; 
            return time_str + ' ' + date;
        }

        tm date_time_to_tm(string date, string time_str) {
        tm time;
        time.tm_year = stoi(date.substr(6,4));
        time.tm_mon = stoi(date.substr(3,2));
        time.tm_mday = stoi(date.substr(0,2));
        time.tm_hour = stoi(time_str.substr(0,2));
        time.tm_min = stoi(time_str.substr(3,2));
        return time;
    }

    }

}

using namespace Conversions::tm_conversions;

#endif