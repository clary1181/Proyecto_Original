// Proyecto_Poo_y_Requerimientos.cpp : Sistema de tienda de ropa 
// Creadoras: Gonzalez Alondra y Carpio Clarita 

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include "SistemaTienda.h"
#include "Usuario.h"
#include "Cliente.h"
#include "Inventario.h"

#include <cstdlib>
#include <fstream> // para ifstream y ofstream
#include <sstream> // para stringstream
#include "Utilidades.h"  //Para entrada de datos 

using std::string;
using std::cout;
using std::endl;

// === Usuarios del sistema ===
vector<Usuario> usuarios;

const string ARCHIVO_USUARIOS = "usuarios.txt";

void cargarUsuariosDesdeArchivo()
{
    usuarios.clear();
    std::ifstream archivo(ARCHIVO_USUARIOS);
    if (!archivo) {
        std::cerr << "No se encontro archivo de usuarios, se creara uno nuevo al registrar.\n";
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        std::stringstream ss(linea);
        string nombre, clave, rolTexto;
        ss >> nombre >> clave >> rolTexto;
        RolUsuario rol = (rolTexto == "GERENTE") ? GERENTE : EMPLEADO;
        usuarios.emplace_back(nombre, clave, clave, rol);
    }
}

void guardarUsuarioEnArchivo(const Usuario& u) {
    std::ofstream archivo(ARCHIVO_USUARIOS, std::ios::app);
    if (!archivo) {
        std::cerr << "Error al abrir archivo de usuarios para escritura.\n";
        return;
    }

    archivo << u.getNombre() << " " << u.getContrasena() << " " << u.getRolComoTexto() << "\n";
}

// === Lógica de autenticación ===
static Usuario* iniciarSesion()
{
    string nombre, clave;
    cout << "Usuario: ";
    cin >> nombre;
    cout << "Contrasenia: ";
    cin >> clave;

    for (auto& u : usuarios)
    {
        if (u.getNombre() == nombre && u.autenticar(clave))
        {
            cout << "\nBienvenido, " << u.getNombre() << " (" << u.getRolComoTexto() << ")\n";
            system("pause");
            return &u;
        }
    }

    cout << "\nNombre o contrasenia incorrectos.\n";
    system("pause");
    return nullptr;
}

// === Menú Gerente ===
static void menuGerente(SistemaTienda& tienda)
{
    int opcion;
    do {
        system("cls");
        cout << "\n--- Menu Gerente ---\n";
        cout << "1. Agregar producto\n";
        cout << "2. Ver inventario\n";
        cout << "3. Ver historial de ventas\n";
        cout << "4. Generar reportes\n";  // Nueva opción
        cout << "0. Salir\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {
        case 1:
        {
            system("cls");
            int id = leerEntero("ID: ");

            // Verificar si el producto ya existe
            Producto* productoExistente = tienda.inventario.buscarProducto(id);

            if (productoExistente != nullptr) {
                // El producto ya existe, mostrar información y preguntar por stock adicional
                tienda.mostrarProductoPorId(id);
                cout << "Este producto ya existe en el inventario.\n";

                int stockAdicional = leerEntero("¿Cuantas unidades desea agregar al stock existente? ");

                if (stockAdicional > 0) {
                    if (tienda.actualizarStockProducto(id, stockAdicional)) {
                        cout << "Stock actualizado correctamente.\n";
                        cout << "Nuevo stock: " << (productoExistente->getStock()) << " unidades\n";
                    }
                    else {
                        cout << "Error al actualizar el stock.\n";
                    }
                }
                else {
                    cout << "No se agregaron unidades al stock.\n";
                }
            }
            else {
                // El producto no existe, continuar con el flujo normal
                int stock;
                string nombre, talla, color, ubicacion;
                double precio;

                cout << "Nombre: "; cin >> nombre;
                cout << "Talla: "; cin >> talla;
                cout << "Color: "; cin >> color;
                cout << "Seccion: "; cin >> ubicacion;
                precio = leerFloat("Ingrese el precio: ");
                stock = leerEntero("Cantidad: ");

                tienda.agregarProductoInventario(id, nombre, talla, color, precio, stock, ubicacion);
                cout << "Producto agregado correctamente.\n";
            }

            cout << "\n\n";
            system("pause");
            break;
        }

        case 2:
            system("cls");
            tienda.menuCatalogoProductos();
            break;
        case 3:
            system("cls");
            tienda.mostrarVentas();
            cout << "\n\n";
            system("pause");
            break;
        case 4:
            tienda.menuReportes();
            break;
        case 0:
            break;
        default:
            cout << "Opcion no valida.\n";
            system("pause");
        }
    } while (opcion != 0);
}

// === Menú Empleado ===
static void menuEmpleado(SistemaTienda& tienda)
{
    int opcion;
    do {
        system("cls");
        cout << "\n--- Menu Empleado ---\n";
        cout << "1. Ver inventario\n";
        cout << "2. Realizar venta\n";
        cout << "0. Salir\n";
        opcion = leerEntero("Opcion: ");
        switch (opcion) {
        case 1:
            system("cls");
            tienda.menuCatalogoProductos();
            break;
        case 2: {
            system("cls");
            string nombre, tel, email;
            cout << "Nombre del cliente: ";
            cin >> nombre;
            cout << "Telefono: ";
            cin >> tel;
            cout << "Email: ";
            cin >> email;
            Cliente* cliente = tienda.agregarCliente(nombre, tel, email);
            map<int, int> productos;
            int idProd, cantidad;
            char mas;
            do {
                idProd = leerEntero("ID del producto: ");
                cantidad = leerEntero("Cantidad: ");
                productos[idProd] = cantidad;
                cout << "¿Agregar otro producto? (s/n): ";
                cin >> mas;
            } while (mas == 's' || mas == 'S');
            tienda.realizarVenta(cliente->getId(), productos);
            cout << "\n\n";
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

static void crearCuenta()
{
    string nombre, clave, confirmacion;
    system("cls");
    cout << "\n------ Crear nueva cuenta ------\n";
    cout << "Nombre de usuario: ";
    cin >> nombre;

    for (const auto& u : usuarios) {
        if (u.getNombre() == nombre) {
            cout << "Ese nombre de usuario ya existe.\n";
            return;
        }
    }

    cout << "Contrasenia: ";
    cin >> clave;
    cout << "Confirmar contrasenia: ";
    cin >> confirmacion;

    if (clave != confirmacion) {
        cout << "Las contrasenias no coinciden.\n";
        return;
    }

    RolUsuario nuevoRol = EMPLEADO;
    Usuario nuevo(nombre, clave, clave, nuevoRol);
    usuarios.push_back(nuevo);
    guardarUsuarioEnArchivo(nuevo);

    cout << "Cuenta creada exitosamente. Ahora puede iniciar sesion.\n\n";
    system("pause");
}

// === Función principal ===
int main()
{
    SistemaTienda tienda;

    // Cargar usuarios desde archivo
    cargarUsuariosDesdeArchivo();

    tienda.cargarInventario();  // << cargar desde archivo
    tienda.cargarVentasDesdeArchivo();

    // Si quieres crear productos solo si el archivo no existe
    if (tienda.inventarioVacio()) {
        tienda.agregarProductoInventario(1, "Camisa", "M", "Azul", 19.99, 4, "Camisas");
        tienda.agregarProductoInventario(2, "Pantalón", "L", "Negro", 39.99, 20, "Pantalones");
        tienda.agregarProductoInventario(3, "Chaqueta", "S", "Rojo", 59.99, 2, "Chamarras");
    }

    int opcion;
    do {
        Usuario* usuarioActual = nullptr;

        do {
            system("cls");
            cout << "\n====== Bienvenido a la Tienda de Ropa ======\n";
            cout << "1. Iniciar sesion\n";
            cout << "2. Crear cuenta\n";
            cout << "0. Salir\n";
            opcion = leerEntero("Seleccione una opcion : ");

            switch (opcion) {
            case 1:
                usuarioActual = iniciarSesion();
                break;
            case 2:
                crearCuenta();
                break;
            case 0:
                cout << "Saliendo del sistema...\n";
                return 0;
            default:
                cout << "Opcion invalida.\n";
            }
        } while (!usuarioActual && opcion != 0);

        // Si se inició sesión correctamente, se accede al menú según el rol
        if (usuarioActual) {
            if (usuarioActual->getRol() == GERENTE) {
                menuGerente(tienda);
            }
            else {
                menuEmpleado(tienda);
            }
        }

        // Al salir de un menú, se vuelve a mostrar el menú principal

    } while (true);  // Vuelve al inicio del programa tras salir de un menú
    tienda.guardarInventario();
    return 0;
}