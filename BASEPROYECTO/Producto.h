#pragma once
#include <string>
using namespace std;
using std::string;

class Producto
{
private:
    int id;
    string nombre;
    string talla;
    string color;
    double precio;
    int stock;
    string ubicacion;

public:
    Producto(int id, const string& nombre, const string& talla, const string& color, double precio, int stock, const string& ubicacion);
    const string& getUbicacion() const;
    int getId() const;
    string getNombre() const;
    string getTalla() const;
    string getColor() const;
    double getPrecio() const;
    int getStock() const;

    void setStock(int nuevoStock);
    void mostrarInfo() const;
};