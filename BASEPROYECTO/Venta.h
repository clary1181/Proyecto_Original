#pragma once
#include <vector>
#include <string>
#include "Cliente.h"
#include <ctime>
#include <map>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::time_t;

struct ProductoVendido
{
    int idProducto;
    int cantidad;
    double precioUnitario;
};

class Venta
{
private:
    int id;
    Cliente cliente;
    vector<ProductoVendido> productos;
    time_t tiempo;

public:
    Venta(int id, const Cliente& cliente);

    void agregarProducto(int idProducto, int cantidad, double precioUnitario);
    double getTotal() const;
    string getTiempo() const;
    void mostrarInfo() const;

    int getId() const { return id; }
    const Cliente& getCliente() const { return cliente; }
    const std::vector<ProductoVendido>& getProductos() const { return productos; }
    int getMes() const;
    int getAnio() const;

};