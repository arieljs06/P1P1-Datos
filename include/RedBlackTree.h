#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "Turno.h"
#include <string>

/// @brief Nodo del Árbol Rojo-Negro
struct NodoArbol {
    std::string cedula;
    Turno* turno;
    bool color; // true: rojo, false: negro
    NodoArbol* izquierda;
    NodoArbol* derecha;
    NodoArbol* padre;

    NodoArbol()
        : turno(nullptr), color(true),
          izquierda(nullptr), derecha(nullptr), padre(nullptr) {}
};

/// @brief Clase que representa un Árbol Rojo-Negro para Turnos
class ArbolRojoNegro {
private:
    NodoArbol* raiz;

    NodoArbol* crearNodo(Turno* turno);
    void insertarBalanceado(NodoArbol* nodo);
    void balancearInsertar(NodoArbol* nodo);
    void rotarIzquierda(NodoArbol* nodo);
    void rotarDerecha(NodoArbol* nodo);

    void imprimirArbol(NodoArbol* nodo, std::string espacio, bool esDerecha);
    void imprimirResumenNodo(NodoArbol* nodo, const std::string& prefijo, bool esDerecha);

public:
    ArbolRojoNegro();

    void insertarTurno(Turno* turno);
    NodoArbol* buscarPorCedula(const std::string& cedula);
    void reemplazarTurno(const std::string& cedula, Turno* nuevoTurno);
    void imprimir();

    NodoArbol* getRaiz() const { return raiz; }
};

#endif
