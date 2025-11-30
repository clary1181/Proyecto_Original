#include "SistemaTienda.h"
#include "ReporteMensual.h"
#include "ReporteAnual.h"
#include "Utilidades.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>

using std::make_unique;

const string ARCHIVO_INVENTARIO = "inventario.txt";

void SistemaTienda::guardarInventario() const {
    inventario.guardarInventarioEnArchivo(ARCHIVO_INVENTARIO);
}

void SistemaTienda::cargarInventario() {
    inventario.cargarInventarioDesdeArchivo(ARCHIVO_INVENTARIO);
}

void SistemaTienda::agregarProductoInventario(int id, const string& nombre, const string& talla, const string& color, double precio, int stock, string ubicacion)
{
    Producto p(id, nombre, talla, color, precio, stock, ubicacion);
    inventario.agregarProducto(p);

    // Guardar el producto individualmente en el archivo
    std::ofstream archivo(ARCHIVO_INVENTARIO, std::ios::app);
    if (!archivo) {
        std::cerr << "Error al abrir archivo para agregar producto al inventario.\n";
        return;
    }

    archivo << p.getId() << " " << p.getNombre() << " " << p.getTalla() << " "
        << p.getColor() << " " << p.getPrecio() << " " << p.getStock() << " " << p.getUbicacion() << "\n";
}

bool SistemaTienda::inventarioVacio() const {
    return inventario.productosVacios();
}

void SistemaTienda::mostrarInventario() const
{
    inventario.mostrarProductos();
}

void SistemaTienda::buscarProductoInteractivo() const {
    inventario.mostrarProductoPorBusqueda();
}

void SistemaTienda::buscarProductoPorDescripcion(const std::string& texto) const {
    inventario.buscarPorDescripcion(texto);
}

void SistemaTienda::mostrarProductoPorId(int id) {
    Producto* producto = inventario.buscarProducto(id);
    if (producto) {
        cout << "\n=== PRODUCTO ENCONTRADO ===\n";
        cout << left
            << setw(5) << "ID"
            << setw(15) << "Nombre"
            << setw(10) << "Talla"
            << setw(10) << "Color"
            << setw(9) << "Precio"
            << setw(6) << "Stock"
            << "Ubicacion\n";
        cout << string(70, '-') << endl;
        producto->mostrarInfo();
    }
    else {
        cout << "Producto no encontrado.\n";
    }
}

void SistemaTienda::menuCatalogoProductos() const {
    int subop;
    do {
        system("cls");
        cout << "\n======= CATALOGO DE PRODUCTOS =======\n";
        cout << "1. Buscar producto por ID\n";
        cout << "2. Buscar producto por descripcion\n";
        cout << "3. Ver todo el catalogo\n";
        cout << "0. Volver al menu anterior\n";
        cout << "Seleccione una opcion: ";
        cin >> subop;

        switch (subop) {
        case 1:
            system("cls");
            buscarProductoInteractivo();
            cout << "\n\n";
            system("pause");
            break;
        case 2: {
            system("cls");
            string palabra;
            cout << "\n=== BUSCAR PRODUCTO POR DESCRIPCION ===\n";
            cout << "Ingrese la descripcion del producto: ";
            cin.ignore();
            getline(cin, palabra);
            buscarProductoPorDescripcion(palabra);
            cout << "\n\n";
            system("pause");
            break;
        }
        case 3:
            system("cls");
            mostrarInventario();
            cout << "\n\n";
            system("pause");
            break;
        case 0:
            break;
        default:
            cout << "Opcion invalida.\n";
            system("pause");
        }
    } while (subop != 0);
}

Cliente* SistemaTienda::agregarCliente(const string& nombre, const string& telefono, const string& email)
{
    Cliente c(siguienteIdCliente++, nombre, telefono, email);
    clientes.push_back(c);
    return &clientes.back();
}

Cliente* SistemaTienda::buscarCliente(int id)
{
    for (auto& c : clientes)
    {
        if (c.getId() == id)
            return &c;
    }
    return nullptr;
}

void SistemaTienda::realizarVenta(int idCliente, const map<int, int>& productosComprados)
{
    Cliente* cliente = buscarCliente(idCliente);
    if (!cliente)
    {
        cout << "Cliente no encontrado.\n";
        return;
    }

    int nuevoId = obtenerSiguienteIdVenta();
    Venta venta(nuevoId, *cliente);

    for (const auto& item : productosComprados)
    {
        int idProd = item.first;
        int cantidad = item.second;
        Producto* producto = inventario.buscarProducto(idProd);

        if (!producto)
        {
            cout << "Producto ID " << idProd << " no encontrado.\n";
            continue;
        }
        if (producto->getStock() < cantidad)
        {
            cout << "Stock insuficiente para producto ID " << idProd << endl;
            continue;
        }

        double precio = producto->getPrecio();
        if (producto->getStock() <= 3)
        {
            cout << "Descuento aplicado al producto ID " << idProd << endl;
            precio *= 0.7;
        }

        venta.agregarProducto(idProd, cantidad, precio);
        int nuevoStock = producto->getStock() - cantidad;
        producto->setStock(nuevoStock);
    }

    ventas.push_back(venta);
    guardarVentaEnArchivo(venta);
    guardarInventario();

    double total = venta.getTotal();
    double dineroRecibido;

    // Solicitar pago
    cout << fixed << setprecision(2);
    cout << "\nTotal a pagar: $" << total << endl;

    do {
        cout << "Dinero recibido: $";
        cin >> dineroRecibido;

        if (dineroRecibido < total) {
            cout << "El dinero recibido es insuficiente. Ingrese una cantidad mayor o igual al total.\n";
        }

    } while (dineroRecibido < total);

    double cambio = dineroRecibido - total;

    system("cls");

    // Obtener fecha y hora actual
    time_t tiempoActual = time(0);
    tm localTime;
    localtime_s(&localTime, &tiempoActual);

    char fechaHora[30];
    strftime(fechaHora, sizeof(fechaHora), "%Y-%m-%d %H:%M:%S", &localTime);

    // === Mostrar recibo completo en consola ===
    cout << "\n====================== RECIBO =====================\n";
    cout << "Cliente:        " << venta.getCliente().getNombre() << endl;
    cout << "Fecha:          " << fechaHora << endl;
    cout << "---------------------------------------------------\n";
    cout << "Productos:\n";

    for (const auto& p : venta.getProductos()) {
        cout << "  - ID: " << p.idProducto
             << ", Cantidad: " << p.cantidad
             << ", Precio Unitario: $" << p.precioUnitario << endl;
    }

    cout << "---------------------------------------------------\n";
    cout << "Total:          $" << total << endl;
    cout << "Recibido:       $" << dineroRecibido << endl;
    cout << "Cambio:         $" << cambio << endl;
    cout << "====================== RECIBO =====================\n";
    cout << "             ¡Gracias por su compra!\n";
    cout << "====================== RECIBO =====================\n";
}

int SistemaTienda::obtenerSiguienteIdVenta() {
    std::ifstream archivo("ventas.txt");
    int ultimoId = 0;
    std::string linea;

    while (std::getline(archivo, linea)) {
        if (linea.rfind("Venta ID:", 0) == 0) {
            try {
                int id = std::stoi(linea.substr(9));
                if (id > ultimoId)
                    ultimoId = id;
            }
            catch (...) {
                // Si falla la conversión, lo ignora
            }
        }
    }

    return ultimoId + 1;
}

void SistemaTienda::guardarVentaEnArchivo(const Venta& venta) {
    // Guardar en formato estructurado (CSV)
    std::ofstream archivoCSV("ventas.csv", std::ios::app);
    if (archivoCSV) {
        archivoCSV << venta.getId() << ","
            << venta.getCliente().getId() << ","
            << venta.getTiempo() << ","
            << venta.getTotal();
        for (const auto& p : venta.getProductos()) {
            archivoCSV << "," << p.idProducto << ":" << p.cantidad << ":" << p.precioUnitario;
        }
        archivoCSV << "\n";
    }

    std::ofstream archivoTXT("ventas.txt", std::ios::app);
    if (archivoTXT) {
        archivoTXT << "Venta ID: " << venta.getId() << "\n";
        archivoTXT << "Cliente: " << venta.getCliente().getNombre() << "\n";
        archivoTXT << "Fecha: " << venta.getTiempo() << "\n";
        archivoTXT << "Productos:\n";
        for (const auto& p : venta.getProductos()) {
            archivoTXT << "  - ID: " << p.idProducto << ", Cantidad: " << p.cantidad
                << ", Precio: $" << p.precioUnitario << "\n";
        }
        archivoTXT << "Total: $" << venta.getTotal() << "\n";
        archivoTXT << "-----------------------------------------------------\n\n";
    }
}

void SistemaTienda::cargarVentasDesdeArchivo() {
    std::ifstream archivo("ventas.csv");
    if (!archivo) return;

    ventas.clear();
    string linea;
    while (getline(archivo, linea)) {
        std::stringstream ss(linea);
        string token;
        vector<string> tokens;

        // Parsear l nea CSV
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 4) continue;

        // Crear venta b sica
        int id = stoi(tokens[0]);
        int idCliente = stoi(tokens[1]);
        Cliente clienteTemp(idCliente, "Cliente Temporal", "", "");
        Venta venta(id, clienteTemp);

        // Parsear productos
        for (int i = 3; i < tokens.size(); i++) {
            size_t pos1 = tokens[i].find(':');
            size_t pos2 = tokens[i].rfind(':');
            if (pos1 == string::npos || pos2 == string::npos) continue;

            int idProd = stoi(tokens[i].substr(0, pos1));
            int cantidad = stoi(tokens[i].substr(pos1 + 1, pos2 - pos1 - 1));
            double precio = stod(tokens[i].substr(pos2 + 1));

            venta.agregarProducto(idProd, cantidad, precio);
        }
        ventas.push_back(venta);
        if (id >= siguienteIdVenta) siguienteIdVenta = id + 1;
    }
}

void SistemaTienda::mostrarVentas() const
{
    std::ifstream archivo("ventas.txt");
    if (!archivo) {
        std::cerr << "No se pudo abrir el archivo de ventas.\n";
        return;
    }

    cout << "\n================ HISTORIAL DE VENTAS ================\n\n";
    string linea;
    while (getline(archivo, linea)) {
        cout << linea << endl;
    }
}

bool SistemaTienda::actualizarStockProducto(int id, int cantidadAdicional) {
    Producto* producto = inventario.buscarProducto(id);
    if (producto) {
        producto->setStock(producto->getStock() + cantidadAdicional);

        // Actualizar el archivo completo
        guardarInventario();

        return true;
    }
    return false;
}


// Nuevos m todos para el sistema de reportes

void SistemaTienda::cargarVentasEnMemoria() {
    // Las ventas ya est n cargadas en el vector 'ventas' durante la ejecuci n
    // Si necesitas cargar desde archivo, aqu  implementar as la l gica
    cout << "Ventas cargadas en memoria: " << ventas.size() << " registros." << endl;
}

void SistemaTienda::generarReporte(unique_ptr<Reporte> reporte) const {
    if (reporte) {
        reporte->setVentas(ventas);
        reporte->generar();
    }
}

void SistemaTienda::menuReportes() const {
    int opcion;
    do {
        system("cls");
        cout << "\n--- MENU DE REPORTES ---\n";
        cout << "1. Reporte Mensual\n";
        cout << "2. Reporte Anual\n";
        cout << "0. Volver al menu anterior\n";
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
        case 1: {
            system("cls");
            int mes = leerEntero("Ingrese el mes (1-12): ");
            int anio = leerEntero("Ingrese el anio: ");

            if (mes < 1 || mes > 12) {
                cout << "Mes invalido. Debe estar entre 1 y 12.\n";
                system("pause");
                break;
            }

            auto reporteMensual = make_unique<ReporteMensual>(mes, anio);
            reporteMensual->setVentas(ventas);

            // Mostrar en pantalla
            reporteMensual->generar();

            // Preguntar si desea exportar
            char exportar;
            cout << "\n¿Desea exportar este reporte a un archivo? (s/n): ";
            cin >> exportar;

            if (exportar == 's' || exportar == 'S') {
                string nombreArchivo;
                cout << "Ingrese el nombre del archivo (sin extension): ";
                cin >> nombreArchivo;
                nombreArchivo += ".txt";
                reporteMensual->exportarATxt(nombreArchivo);
            }

            cout << "\n";
            system("pause");
            break;
        }
        case 2: {
            system("cls");
            int anio = leerEntero("Ingrese el anio: ");

            auto reporteAnual = make_unique<ReporteAnual>(anio);
            reporteAnual->setVentas(ventas);

            // Mostrar en pantalla
            reporteAnual->generar();

            // Preguntar si desea exportar
            char exportar;
            cout << "\n¿Desea exportar este reporte a un archivo? (s/n): ";
            cin >> exportar;

            if (exportar == 's' || exportar == 'S') {
                string nombreArchivo;
                cout << "Ingrese el nombre del archivo (sin extensión): ";
                cin >> nombreArchivo;
                nombreArchivo += ".txt";
                reporteAnual->exportarATxt(nombreArchivo);
            }

            cout << "\n";
            system("pause");
            break;
        }
        case 0:
            break;
        default:
            cout << "Opcion invalida.\n";
            system("pause");
        }
    } while (opcion != 0);
}
const vector<Venta>& SistemaTienda::getVentas() const {
    return ventas;
}