#pragma once
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

class Cliente 
{
private:
    int id;
    string nombre;
    string telefono;
    string email;

public:
    Cliente(int id, const string& nombre, const string& telefono, const string& email);

    int getId() const;
    string getNombre() const;
    string getTelefono() const;
    string getEmail() const;

    void mostrarInfo() const;
};
