#include "Inventario.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <algorithm>

void Inventario::guardarInventarioEnArchivo(const string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    if (!archivo) {
        std::cerr << "Error al abrir archivo para guardar inventario.\n";
        return;
    }

    for (const auto& p : productos) {
        archivo << p.getId() << " " << p.getNombre() << " " << p.getTalla() << " "
            << p.getColor() << " " << p.getPrecio() << " " << p.getStock() << " " << p.getUbicacion() << "\n";

    }
}

void Inventario::cargarInventarioDesdeArchivo(const string& nombreArchivo) {
    productos.clear();
    std::ifstream archivo(nombreArchivo);
    if (!archivo) {
        std::cerr << "No se encontró el archivo de inventario.\n";
        return;
    }

    int id, stock;
    double precio;
    string nombre, talla, color, ubicacion;

    while (archivo >> id >> nombre >> talla >> color >> precio >> stock >> ubicacion) {
        Producto p(id, nombre, talla, color, precio, stock, ubicacion);
        productos.push_back(p);
    }
}

bool Inventario::productosVacios() const {
    return productos.empty();
}


void Inventario::agregarProducto(const Producto& p) 
{
	productos.push_back(p);
}

Producto* Inventario::buscarProducto(int id)
{
    for (auto& p : productos) 
    {
        if (p.getId() == id)
        {
            return &p;
        }
    }
    return nullptr;
}

const Producto* Inventario::buscarProducto(int id) const {
    for (const auto& p : productos) {
        if (p.getId() == id) {
            return &p;
        }
    }
    return nullptr;
}


void Inventario::mostrarProductos() const
{
    cout << "\n                       INVENTARIO\n";
    std::set<string> categorias;
    for (const auto& p : productos) categorias.insert(p.getUbicacion());

    for (const auto& cat : categorias) {
        cout << "\n-- " << cat << " --\n";
        cout << left << setw(5) << "ID" << setw(15) << "Nombre" << setw(10)
            << "Talla" << setw(10) << "Color" << setw(9) << "Precio" << setw(6) << "Stock" << endl;
        cout << string(55, '-') << endl;
        for (const auto& p : productos) {
            if (p.getUbicacion() == cat) p.mostrarInfo();
        }
    }
}

void Inventario::buscarPorDescripcion(const string& texto) const {
    cout << "\nResultados para: \"" << texto << "\"\n";
    bool encontrado = false;
    for (const auto& p : productos) {
        string data = p.getNombre() + " " + p.getColor();
        std::transform(data.begin(), data.end(), data.begin(), ::tolower);
        string textoLower = texto;
        std::transform(textoLower.begin(), textoLower.end(), textoLower.begin(), ::tolower);
        if (data.find(textoLower) != string::npos) {
            if (!encontrado) {
                cout << left << setw(5) << "ID" << setw(15) << "Nombre" << setw(10)
                    << "Talla" << setw(10) << "Color" << setw(9) << "Precio" << setw(6) << "Stock" << endl;
                cout << string(55, '-') << endl;
            }
            p.mostrarInfo();
            encontrado = true;
        }
    }

    if (!encontrado) cout << "No se encontraron productos.\n";
}

void Inventario::mostrarProductoPorBusqueda() const {
    int id;
    cout << "\n=== BUSCAR PRODUCTO POR ID ===\n";
    cout << "Ingrese el ID del producto: ";
    cin >> id;

    const Producto* p = buscarProducto(id);

    if (p) {
        cout << "\n\nProducto encontrado:\n";
        cout << left
            << setw(5) << "ID"
            << setw(15) << "Nombre"
            << setw(10) << "Talla"
            << setw(10) << "Color"
            << setw(9) << "Precio"
            << setw(6) << "Stock"
            << "Ubicacion\n";
        cout << string(70, '-') << endl;
        p->mostrarInfo();

        char opcion;
        cout << "\n\nDesea volver al catalogo general? (s/n): ";
        cin >> opcion;
        if (opcion == 's' || opcion == 'S') {
            mostrarProductos();
        }

    }
    else {
        cout << "\nNo se encontro ningun producto con ese ID.\n";
        system("\npause");
    }
}
