#include "Inventory.h"
#ifndef COLUMN_H
#define COLUMN_H
using namespace std;

/// @brief Namespace for handling The interactions of the Column Enum
namespace ColumnInfo {

    /// @brief Number of possibilities
    int size = 26;

    /// @brief Enum for Possible Columns in Airplane
    enum Column { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z };

    /// @brief String array of all corresponding string values
    const char* col_to_string[] = {
        stringify(A),
        stringify(B),
        stringify(C),
        stringify(D),
        stringify(E),
        stringify(F),
        stringify(G),
        stringify(H),
        stringify(I),
        stringify(J),
        stringify(K),
        stringify(L),
        stringify(M),
        stringify(N),
        stringify(O),
        stringify(P),
        stringify(Q),
        stringify(R),
        stringify(S),
        stringify(T),
        stringify(U),
        stringify(V),
        stringify(W),
        stringify(X),
        stringify(Y),
        stringify(Z),
    };

    /// @brief Inline function to convert Column enum to string
    /// @param col Column Enum to convert
    /// @return The corresponding string
    inline string Col_to_String(Column col) {
        return col_to_string[(int)col];
    }

    /// @brief Binary search to find the string that matches the enum name to convert string to Enum.
    /// @param str String to search for
    /// @param low Left hand bound of the search
    /// @param high Right hand bound of the search
    /// @return The Column Enum corresponding with the string value
    Column binarySearch(string str, int low, int high) {
        if (low > high)
            return (Column) 0; 
        else {
            int mid = (low + high) / 2;
            if (str == Col_to_String((Column) mid))
                return (Column)mid;
            else if (str > Col_to_String((Column) mid))    // str is on the right side
                return binarySearch(str, mid + 1, high);
            else                        // str is on the left side
                return binarySearch(str, low, mid - 1);
        }
    }

    /// @brief Convert string to Column enum
    /// @param str String to convert
    /// @return Corresponding Column enum
    Column string_to_Column(string str) {
        return binarySearch(str, 0, size - 1);
    } 
}

#endif