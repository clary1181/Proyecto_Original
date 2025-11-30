#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include "Inventario.h"
#include "Cliente.h"
#include "Venta.h"
#include "Reporte.h"

using std::string;
using std::cout;
using std::endl;
using std::unique_ptr;

class SistemaTienda
{
private:
    vector<Cliente> clientes;
    vector<Venta> ventas;
    int siguienteIdVenta = 1;
    int siguienteIdCliente = 1;

public:
    Inventario inventario;

    // Métodos existentes
    void guardarInventario() const;
    void cargarInventario();
    void agregarProductoInventario(int id, const string& nombre, const string& talla, const string& color, double precio, int stock, string ubicacion);
    bool inventarioVacio() const;
    void mostrarInventario() const;
    void menuCatalogoProductos() const;
    Cliente* agregarCliente(const string& nombre, const string& telefono, const string& email);
    Cliente* buscarCliente(int id);
    void realizarVenta(int idCliente, const map<int, int>& productosComprados);
    int obtenerSiguienteIdVenta();
    void guardarVentaEnArchivo(const Venta& venta);
    void cargarVentasDesdeArchivo();
    void mostrarVentas() const;
    bool actualizarStockProducto(int id, int cantidadAdicional);

    void buscarProductoInteractivo() const;
    void buscarProductoPorDescripcion(const std::string& texto) const;
    void mostrarProductoPorId(int id);


    void cargarVentasEnMemoria();
    void generarReporte(unique_ptr<Reporte> reporte) const;
    void menuReportes() const;
    const vector<Venta>& getVentas() const;
};