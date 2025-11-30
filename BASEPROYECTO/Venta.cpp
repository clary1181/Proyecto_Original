#include "Venta.h"
#include <iostream>
#include <ctime>
#include <iomanip>  
#include <sstream> 

using std::cout;
using std::endl;
using std::string;

Venta::Venta(int id, const Cliente& cliente) : id(id), cliente(cliente) {
    tiempo = time(nullptr); 
}

void Venta::agregarProducto(int idProducto, int cantidad, double precioUnitario) {
    productos.push_back({ idProducto, cantidad, precioUnitario });
}

double Venta::getTotal() const
{
    double total = 0;
    for (const auto& p : productos)
    {
        total += p.cantidad * p.precioUnitario;
    }
    return total;
}

string Venta::getTiempo() const
{
    std::tm time_info;
    localtime_s(&time_info, &tiempo);

    std::ostringstream ss;
    ss << std::put_time(&time_info, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void Venta::mostrarInfo() const {
    cout << "Venta ID: " << id << endl;
    cout << "Cliente: " << cliente.getNombre() << endl;
    cout << "Fecha: " << getTiempo() << endl;
    cout << "Productos:\n";
    for (const auto& p : productos) {
        cout << "  - ID: " << p.idProducto << ", Cantidad: " << p.cantidad
            << ", Precio Unitario: $" << p.precioUnitario << endl;
    }
    cout << "Total: $" << getTotal() << endl;
}


int Venta::getMes() const {
    std::tm time_info;
    localtime_s(&time_info, &tiempo);
    return time_info.tm_mon + 1;  // tm_mon es 0-based
}

int Venta::getAnio() const {
    std::tm time_info;
    localtime_s(&time_info, &tiempo);
    return time_info.tm_year + 1900;  // tm_year es años desde 1900
}