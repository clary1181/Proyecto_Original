#pragma once
#include "Reporte.h"
#include <map>

using std::map;

class ReporteMensual : public Reporte
{
private:
    int mes;
    int anio;

    map<int, int> calcularVentasPorProducto(const vector<Venta>& ventasFiltradas) const;
    void mostrarTopProductos(const map<int, int>& ventasPorProducto) const;

public:
    ReporteMensual(int mes, int anio);
    void generar() const override;
    void mostrarEncabezado() const override;

    void escribirContenidoArchivo() const override;

    int getMes() const;
    int getAnio() const;
    void setMes(int mes);
    void setAnio(int anio);
};