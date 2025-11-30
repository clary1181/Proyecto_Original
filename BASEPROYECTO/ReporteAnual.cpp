#include "ReporteAnual.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using std::cout;
using std::endl;
using std::left;
using std::setw;

ReporteAnual::ReporteAnual(int anio)
    : Reporte("REPORTE ANUAL DE VENTAS"), anio(anio) {
}

void ReporteAnual::mostrarEncabezado() const {
    cout << "\n" << string(60, '=') << endl;
    cout << "    " << titulo << " - ANIO " << anio << endl;
    cout << string(60, '=') << endl;
}

void ReporteAnual::generar() const {
    mostrarEncabezado();

    // Filtrar ventas del año
    vector<Venta> ventasAnio;
    for (const auto& venta : ventas) {
        if (venta.getAnio() == anio) {
            ventasAnio.push_back(venta);
        }
    }

    if (ventasAnio.empty()) {
        cout << "\nNo se encontraron ventas para el anio " << anio << ".\n" << endl;
        return;
    }

    // Calcular totales anuales
    double totalAnual = calcularTotalVentas(ventasAnio);
    int cantidadVentas = ventasAnio.size();

    cout << "\n--- RESUMEN ANUAL ---" << endl;
    cout << "Numero total de ventas: " << cantidadVentas << endl;
    cout << "Total de ingresos anuales: $" << std::fixed << std::setprecision(2) << totalAnual << endl;
    cout << "Promedio mensual: $" << std::fixed << std::setprecision(2) << (totalAnual / 12) << endl;

    // Calcular y mostrar ventas por mes
    map<int, double> ventasPorMes = calcularVentasPorMes();

    cout << "\n--- VENTAS POR MES ---" << endl;
    string nombresMeses[] = { "", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                            "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre" };

    cout << left << setw(12) << "Mes" << setw(15) << "Total Ventas" << setw(10) << "Cantidad" << endl;
    cout << string(37, '-') << endl;

    for (int mes = 1; mes <= 12; mes++) {
        double totalMes = ventasPorMes[mes];
        int cantidadMes = 0;

        // Contar ventas del mes
        for (const auto& venta : ventasAnio) {
            if (venta.getMes() == mes) {
                cantidadMes++;
            }
        }

        cout << left << setw(12) << nombresMeses[mes]
            << "$" << std::fixed << std::setprecision(2) << setw(14) << totalMes
                << setw(10) << cantidadMes << endl;
    }

    // Mostrar gráfico simple
    mostrarGraficoVentasMensuales(ventasPorMes);

    // Encontrar mejor y peor mes
    auto mejorMes = std::max_element(ventasPorMes.begin(), ventasPorMes.end(),
        [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
            return a.second < b.second;
        });

    auto peorMes = std::min_element(ventasPorMes.begin(), ventasPorMes.end(),
        [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
            return a.second < b.second && a.second > 0;
        });

    cout << "\n--- ANALISIS ---" << endl;
    if (mejorMes != ventasPorMes.end() && mejorMes->second > 0) {
        cout << "Mejor mes: " << nombresMeses[mejorMes->first]
            << " ($" << std::fixed << std::setprecision(2) << mejorMes->second << ")" << endl;
    }

    if (peorMes != ventasPorMes.end() && peorMes->second > 0) {
        cout << "Mes con menores ventas: " << nombresMeses[peorMes->first]
            << " ($" << std::fixed << std::setprecision(2) << peorMes->second << ")" << endl;
    }

    cout << string(60, '=') << endl;
}

map<int, double> ReporteAnual::calcularVentasPorMes() const {
    map<int, double> ventasPorMes;

    // Inicializar todos los meses en 0
    for (int mes = 1; mes <= 12; mes++) {
        ventasPorMes[mes] = 0.0;
    }

    // Sumar ventas por mes
    for (const auto& venta : ventas) {
        if (venta.getAnio() == anio) {
            ventasPorMes[venta.getMes()] += venta.getTotal();
        }
    }

    return ventasPorMes;
}

void ReporteAnual::mostrarGraficoVentasMensuales(const map<int, double>& ventasPorMes) const {
    cout << "\n--- GRAFICO DE VENTAS MENSUALES ---" << endl;

    // Encontrar el valor máximo para escalar el gráfico
    double maxVenta = 0;
    for (const auto& par : ventasPorMes) {
        if (par.second > maxVenta) {
            maxVenta = par.second;
        }
    }

    if (maxVenta == 0) {
        cout << "No hay datos para mostrar el grafico." << endl;
        return;
    }

    string nombresMeses[] = { "", "Ene", "Feb", "Mar", "Abr", "May", "Jun",
                            "Jul", "Ago", "Sep", "Oct", "Nov", "Dic" };

    for (int mes = 1; mes <= 12; mes++) {
        double ventaMes = ventasPorMes.at(mes);
        int barras = static_cast<int>((ventaMes / maxVenta) * 30); // Escalar a 30 caracteres máximo

        cout << nombresMeses[mes] << " |";
        for (int i = 0; i < barras; i++) {
            cout << " ";
        }
        cout << " $" << std::fixed << std::setprecision(0) << ventaMes << endl;
    }
}

int ReporteAnual::getAnio() const {
    return anio;
}

void ReporteAnual::setAnio(int anio) {
    this->anio = anio;
}

void ReporteAnual::escribirContenidoArchivo() const {
    // Filtrar ventas del año
    vector<Venta> ventasAnio;
    for (const auto& venta : ventas) {
        if (venta.getAnio() == anio) {
            ventasAnio.push_back(venta);
        }
    }

    if (ventasAnio.empty()) {
        archivoSalida << "\nNo se encontraron ventas para el año " << anio << ".\n";
        return;
    }

    // Calcular totales anuales
    double totalAnual = calcularTotalVentas(ventasAnio);
    int cantidadVentas = ventasAnio.size();

    archivoSalida << "\n--- RESUMEN ANUAL ---\n";
    archivoSalida << "Numero total de ventas: " << cantidadVentas << "\n";
    archivoSalida << "Total de ingresos anuales: $" << std::fixed << std::setprecision(2) << totalAnual << "\n";
    archivoSalida << "Promedio mensual: $" << std::fixed << std::setprecision(2) << (totalAnual / 12) << "\n";

    // Calcular y escribir ventas por mes
    map<int, double> ventasPorMes = calcularVentasPorMes();
    string nombresMeses[] = { "", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                            "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre" };

    archivoSalida << "\n--- VENTAS POR MES ---\n";
    archivoSalida << std::left << std::setw(12) << "Mes" << std::setw(15) << "Total Ventas" << std::setw(10) << "Cantidad" << "\n";
    archivoSalida << std::string(37, '-') << "\n";

    for (int mes = 1; mes <= 12; mes++) {
        double totalMes = ventasPorMes[mes];
        int cantidadMes = 0;

        for (const auto& venta : ventasAnio) {
            if (venta.getMes() == mes) {
                cantidadMes++;
            }
        }

        archivoSalida << std::left << std::setw(12) << nombresMeses[mes]
            << "$" << std::fixed << std::setprecision(2) << std::setw(14) << totalMes
                << std::setw(10) << cantidadMes << "\n";
    }

    // Encontrar mejor y peor mes
    auto mejorMes = std::max_element(ventasPorMes.begin(), ventasPorMes.end(),
        [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
            return a.second < b.second;
        });

    auto peorMes = std::min_element(ventasPorMes.begin(), ventasPorMes.end(),
        [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
            return a.second < b.second && a.second > 0;
        });

    archivoSalida << "\n--- ANALISIS ---\n";
    if (mejorMes != ventasPorMes.end() && mejorMes->second > 0) {
        archivoSalida << "Mejor mes: " << nombresMeses[mejorMes->first]
            << " ($" << std::fixed << std::setprecision(2) << mejorMes->second << ")\n";
    }

    if (peorMes != ventasPorMes.end() && peorMes->second > 0) {
        archivoSalida << "Mes con menores ventas: " << nombresMeses[peorMes->first]
            << " ($" << std::fixed << std::setprecision(2) << peorMes->second << ")\n";
    }
}