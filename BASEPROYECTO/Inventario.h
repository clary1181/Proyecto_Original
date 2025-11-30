#pragma once
#include <vector>
#include <iostream>
#include "Producto.h"

using std::string;
using std::cout;
using std::endl;

class Inventario
{
private:
    vector<Producto> productos;

public:
    void guardarInventarioEnArchivo(const string& nombreArchivo) const;
    void cargarInventarioDesdeArchivo(const string& nombreArchivo);
    bool productosVacios() const;
    void agregarProducto(const Producto& p);
    Producto* buscarProducto(int id);
    const Producto* buscarProducto(int id) const;
    void mostrarProductos() const;
    void buscarPorDescripcion(const string& texto) const;
    void mostrarProductoPorBusqueda() const;
};


