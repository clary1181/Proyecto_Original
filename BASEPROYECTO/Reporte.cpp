#include "Reporte.h"
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

Reporte::Reporte(const string& titulo) : titulo(titulo) {}

void Reporte::mostrarEncabezado() const {
    cout << "\n" << string(60, '=') << endl;
    cout << "    " << titulo << endl;
    cout << string(60, '=') << endl;
}

void Reporte::exportarATxt(const string& nombreArchivo) const {
    archivoSalida.open(nombreArchivo);
    if (!archivoSalida) {
        cout << "Error al abrir el archivo para escritura: " << nombreArchivo << endl;
        return;
    }

    escribirEncabezadoArchivo();
    escribirContenidoArchivo();
    escribirPieArchivo();

    archivoSalida.close();
    cout << "Reporte exportado correctamente a: " << nombreArchivo << endl;
}

void Reporte::escribirEncabezadoArchivo() const {
    archivoSalida << string(60, '=') << "\n";
    archivoSalida << "    " << titulo << "\n";
    archivoSalida << string(60, '=') << "\n\n";
}

void Reporte::escribirPieArchivo() const {
    archivoSalida << "\n" << string(60, '=') << "\n";
    archivoSalida << "Fin del reporte\n";
    archivoSalida << string(60, '=') << "\n";
}

void Reporte::setVentas(const vector<Venta>& ventas) {
    this->ventas = ventas;
}

void Reporte::setTitulo(const string& titulo) {
    this->titulo = titulo;
}

string Reporte::getTitulo() const {
    return titulo;
}

vector<Venta> Reporte::filtrarVentasPorPeriodo(int mes, int anio) const {
    vector<Venta> ventasFiltradas;
    for (const auto& venta : ventas) {
        if (venta.getMes() == mes && venta.getAnio() == anio) {
            ventasFiltradas.push_back(venta);
        }
    }
    return ventasFiltradas;
}

double Reporte::calcularTotalVentas(const vector<Venta>& ventasFiltradas) const {
    double total = 0.0;
    for (const auto& venta : ventasFiltradas) {
        total += venta.getTotal();
    }
    return total;
}