#include "Usuario.h"

Usuario::Usuario(string nombre, string usuario, string contrasena, RolUsuario rol) : nombre(nombre), usuario(usuario), contrasena(contrasena), rol(rol) {}

string Usuario::getUsuario() const 
{
    return usuario;
}

string Usuario::getContrasena() const 
{
    return contrasena;
}

RolUsuario Usuario::getRol() const
{
    return rol;
}

string Usuario::getRolComoTexto() const 
{
    if (rol == GERENTE) 
    {
        return "Gerente";
    }
    else if (rol == EMPLEADO) 
    {
        return "Empleado";
    }
    else 
    {
        return "Unknown";
    }
}

bool Usuario::autenticar(const string& claveIngresada) const
{
    return contrasena == claveIngresada;
}

string Usuario::getNombre() const
{
    return nombre;
}