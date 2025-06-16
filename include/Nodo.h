// Nodo.h
#ifndef NODO_H
#define NODO_H

/// @file Nodo.h
/// @brief Nodo de una lista doblemente enlazada que contiene un Turno.

#include "Turno.h"

class Nodo {
public:
    Turno* dato;         ///< Apunta al turno contenido.
    Nodo* siguiente;     ///< Apunta al siguiente nodo.
    Nodo* anterior;      ///< Apunta al nodo anterior.

    Nodo(Turno* d) : dato(d), siguiente(nullptr), anterior(nullptr) {}
};

#endif