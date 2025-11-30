#include "ReporteMensual.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using std::cout;
using std::endl;
using std::left;
using std::setw;

ReporteMensual::ReporteMensual(int mes, int anio)
    : Reporte("REPORTE MENSUAL DE VENTAS"), mes(mes), anio(anio) {
}

void ReporteMensual::mostrarEncabezado() const {
    cout << "\n" << string(70, '=') << endl;
    cout << "    " << titulo << " - ";

    // Mostrar nombre del mes
    string nombresMeses[] = { "", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                            "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre" };

    if (mes >= 1 && mes <= 12) {
        cout << nombresMeses[mes] << " " << anio;
    }
    else {
        cout << "Mes " << mes << " " << anio;
    }

    cout << endl;
    cout << string(70, '=') << endl;
}

void ReporteMensual::generar() const {
    mostrarEncabezado();

    // Filtrar ventas del mes y año especificados
    vector<Venta> ventasMes = filtrarVentasPorPeriodo(mes, anio);

    if (ventasMes.empty()) {
        cout << "\nNo se encontraron ventas para el período especificado.\n" << endl;
        return;
    }

    // Calcular totales
    double totalVentas = calcularTotalVentas(ventasMes);
    int cantidadVentas = ventasMes.size();
    double promedioVenta = totalVentas / cantidadVentas;

    // Mostrar resumen general
    cout << "\n--- RESUMEN GENERAL ---" << endl;
    cout << "Numero total de ventas: " << cantidadVentas << endl;
    cout << "Total de ingresos: $" << std::fixed << std::setprecision(2) << totalVentas << endl;
    cout << "Promedio por venta: $" << std::fixed << std::setprecision(2) << promedioVenta << endl;

    // Calcular y mostrar estadísticas por producto
    map<int, int> ventasPorProducto = calcularVentasPorProducto(ventasMes);
    mostrarTopProductos(ventasPorProducto);

    // Mostrar detalle de ventas
    cout << "\n--- DETALLE DE VENTAS ---" << endl;
    cout << left << setw(8) << "ID Venta" << setw(20) << "Cliente"
        << setw(20) << "Fecha" << setw(12) << "Total" << endl;
    cout << string(60, '-') << endl;

    for (const auto& venta : ventasMes) {
        cout << left << setw(8) << venta.getId()
            << setw(20) << venta.getCliente().getNombre()
            << setw(20) << venta.getTiempo()
            << "$" << std::fixed << std::setprecision(2) << venta.getTotal() << endl;
    }

    cout << string(70, '=') << endl;
}

map<int, int> ReporteMensual::calcularVentasPorProducto(const vector<Venta>& ventasFiltradas) const {
    map<int, int> ventasPorProducto;

    for (const auto& venta : ventasFiltradas) {
        for (const auto& producto : venta.getProductos()) {
            ventasPorProducto[producto.idProducto] += producto.cantidad;
        }
    }

    return ventasPorProducto;
}

void ReporteMensual::mostrarTopProductos(const map<int, int>& ventasPorProducto) const {
    if (ventasPorProducto.empty()) {
        return;
    }

    // Convertir el map a vector de pares para poder ordenarlo
    vector<std::pair<int, int>> productosOrdenados(ventasPorProducto.begin(), ventasPorProducto.end());

    // Ordenar por cantidad vendida (descendente)
    std::sort(productosOrdenados.begin(), productosOrdenados.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second > b.second;
        });

    cout << "\n--- TOP PRODUCTOS MAS VENDIDOS ---" << endl;
    cout << left << setw(12) << "ID Producto" << setw(15) << "Cantidad" << endl;
    cout << string(27, '-') << endl;

    int contador = 1;
    for (const auto& producto : productosOrdenados) {
        cout << left << setw(12) << producto.first
            << setw(15) << producto.second << " unidades" << endl;

        if (contador >= 5) break; // Mostrar solo top 5
        contador++;
    }
}

int ReporteMensual::getMes() const {
    return mes;
}

int ReporteMensual::getAnio() const {
    return anio;
}

void ReporteMensual::setMes(int mes) {
    this->mes = mes;
}

void ReporteMensual::setAnio(int anio) {
    this->anio = anio;
}

void ReporteMensual::escribirContenidoArchivo() const {
    vector<Venta> ventasMes = filtrarVentasPorPeriodo(mes, anio);

    if (ventasMes.empty()) {
        archivoSalida << "\nNo se encontraron ventas para el período especificado.\n";
        return;
    }

    // Calcular totales
    double totalVentas = calcularTotalVentas(ventasMes);
    int cantidadVentas = ventasMes.size();
    double promedioVenta = totalVentas / cantidadVentas;

    // Escribir resumen general
    archivoSalida << "\n--- RESUMEN GENERAL ---\n";
    archivoSalida << "Numero total de ventas: " << cantidadVentas << "\n";
    archivoSalida << "Total de ingresos: $" << std::fixed << std::setprecision(2) << totalVentas << "\n";
    archivoSalida << "Promedio por venta: $" << std::fixed << std::setprecision(2) << promedioVenta << "\n";

    // Calcular y escribir estadísticas por producto
    map<int, int> ventasPorProducto = calcularVentasPorProducto(ventasMes);

    if (!ventasPorProducto.empty()) {
        archivoSalida << "\n--- TOP PRODUCTOS MAS VENDIDOS ---\n";
        archivoSalida << std::left << std::setw(12) << "ID Producto" << std::setw(15) << "Cantidad" << "\n";
        archivoSalida << std::string(27, '-') << "\n";

        int contador = 1;
        for (const auto& producto : ventasPorProducto) {
            archivoSalida << std::left << std::setw(12) << producto.first
                << std::setw(15) << producto.second << " unidades\n";
            if (contador >= 5) break;
            contador++;
        }
    }

    // Escribir detalle de ventas
    archivoSalida << "\n--- DETALLE DE VENTAS ---\n";
    archivoSalida << std::left << std::setw(8) << "ID Venta" << std::setw(20) << "Cliente"
        << std::setw(20) << "Fecha" << std::setw(12) << "Total" << "\n";
    archivoSalida << std::string(60, '-') << "\n";

    for (const auto& venta : ventasMes) {
        archivoSalida << std::left << std::setw(8) << venta.getId()
            << std::setw(20) << venta.getCliente().getNombre()
            << std::setw(20) << venta.getTiempo()
            << "$" << std::fixed << std::setprecision(2) << venta.getTotal() << "\n";
    }
}