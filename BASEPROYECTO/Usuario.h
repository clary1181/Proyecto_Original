#include <string>
using namespace std;
using std::string;

enum RolUsuario { GERENTE, EMPLEADO};

class Usuario {
private:
    string nombre;
    string usuario;
    string contrasena;
    RolUsuario rol;

public:
    Usuario(string nombre, string usuario, string contrasena, RolUsuario rol);

    bool autenticar(const std::string& claveIngresada) const;
    string getNombre() const;
    string getUsuario() const;
    string getContrasena() const;
    RolUsuario getRol() const;  // Changed here
    string getRolComoTexto() const; // Add this function
};