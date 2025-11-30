#pragma once
#include "Reporte.h"
#include <map>

using std::map;

class ReporteAnual : public Reporte
{
private:
    int anio;

    map<int, double> calcularVentasPorMes() const;
    void mostrarGraficoVentasMensuales(const map<int, double>& ventasPorMes) const;

public:
    ReporteAnual(int anio);
    void generar() const override;
    void mostrarEncabezado() const override;

    void escribirContenidoArchivo() const override;

    int getAnio() const;
    void setAnio(int anio);
};
