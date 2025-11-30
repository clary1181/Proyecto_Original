#include "Producto.h"
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

Producto::Producto(int id, const string& nombre, const string& talla, const string& color, double precio, int stock, const string& ubicacion)
	: id(id), nombre(nombre), talla(talla), color(color), precio(precio), stock(stock), ubicacion(ubicacion) {}

const string& Producto::getUbicacion() const
{
	return ubicacion;
}


int Producto::getId() const
{
	return id;
}

string Producto::getNombre() const
{
	return nombre;
}

string Producto::getTalla() const
{
	return talla;
}

string Producto::getColor() const
{
	return color;
}

double Producto::getPrecio() const
{
	return precio;
}

int Producto::getStock() const
{
	return stock;
}

void Producto::setStock(int nuevoStock)
{
	stock = nuevoStock;
}

void Producto::mostrarInfo() const
{
	cout << left << setw(5) << id
		<< setw(15) << nombre
		<< setw(10) << talla
		<< setw(10) << color
		<< "$" << setw(8) << fixed << setprecision(2) << precio
		<< setw(6) << stock
		<< setw(12) << ubicacion
		<< endl;
}
