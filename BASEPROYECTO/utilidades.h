// Utilidades.h
#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <iostream>
#include <limits>
#include <string>

using namespace std;

inline int leerEntero(const string& mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (!cin.fail()) break;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida. Intente de nuevo.\n";
    }
    return valor;
}
inline float leerFloat(const string& mensaje) {
    float valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (!cin.fail()) break;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada invalida. Intente de nuevo.\n";
    }
    return valor;
}


#endif

