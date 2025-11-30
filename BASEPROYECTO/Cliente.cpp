#include "Cliente.h"

Cliente::Cliente::Cliente(int id, const string& nombre, const string& telefono, const string& email)
	: id(id), nombre(nombre), telefono(telefono), email(email) {}

int Cliente::getId() const
{
	return id;
}

string Cliente::getNombre() const
{
	return nombre;
}

string Cliente::getTelefono() const
{
	return telefono;
}

string Cliente::getEmail() const
{
	return email;
}

void Cliente::mostrarInfo() const
{
	cout << "Cliente ID: " << id
		<< ", Nombre: " << nombre
		<< ", Teléfono: " << telefono
		<< ", Email: " << email << endl;
}
