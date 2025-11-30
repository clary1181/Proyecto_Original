#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Venta.h"

using std::string;
using std::vector;

class Reporte
{
protected:
    string titulo;
    vector<Venta> ventas;
    mutable std::ofstream archivoSalida; 

public:
    Reporte(const string& titulo);
    virtual ~Reporte() = default;
    virtual void generar() const = 0;
    virtual void mostrarEncabezado() const;
    virtual void exportarATxt(const string& nombreArchivo) const;

    void setVentas(const vector<Venta>& ventas);
    void setTitulo(const string& titulo);
    string getTitulo() const;

    vector<Venta> filtrarVentasPorPeriodo(int mes, int anio) const;
    double calcularTotalVentas(const vector<Venta>& ventasFiltradas) const;

protected:
    virtual void escribirEncabezadoArchivo() const;
    virtual void escribirContenidoArchivo() const = 0;
    virtual void escribirPieArchivo() const;
};