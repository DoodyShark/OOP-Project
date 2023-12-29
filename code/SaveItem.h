#include <string>
#include <fstream>
#include <filesystem>
#ifndef SAVEITEM_H
#define SAVEITEM_H

using namespace std;

/// @brief Abstract class for defining behavior of saveable item with RSA encryption decryption implemented
class SaveItem {
    private:
    static int n;
    static int p;
    static int q;
    static int e;
    static int d;

    public:
    virtual bool save() = 0;

    protected:
    /// @brief Implements RSA encryption
    /// @param message message to encrypt
    /// @return encrypted message
    static string encrypt(string message) {
        string encrypted = "";
        for (int i = 0; i < message.length(); i++) {
            encrypted += to_string(fastExponentiation((int) message[i], e, n)) + " ";
        }
        return encrypted;
    }

    /// @brief Implements RSA decryption
    /// @param message message to decrypt
    /// @return decrypted message
    static string decrypt(string message) {
        stringstream stream(message);
        string current;
        string decrypted = "";
        while (stream >> current) {
            decrypted += (char)(fastExponentiation(stoi(current), d, n));
        }
        return decrypted;
    }

    static int fastExponentiation(int b, int e, int p) {
        int res = 1;
        for (int i = 0; i < e; i++) {
            res *= b;
            res %= p;
        }
        return res;
    }

};

// Static variables (RSA parameters)
int SaveItem::n = 23*29;
int SaveItem::p = 23;
int SaveItem::q = 29;
int SaveItem::e = 3;
int SaveItem::d = 411;

#endif