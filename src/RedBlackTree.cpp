#include "RedBlackTree.h"
#include <iostream>

ArbolRojoNegro::ArbolRojoNegro() {
    raiz = nullptr;
}

NodoArbol* ArbolRojoNegro::crearNodo(Turno* turno) {
    NodoArbol* nuevo = new NodoArbol;
    nuevo->turno = turno;
    nuevo->cedula = turno->getPaciente().getCedula();
    nuevo->color = true; // rojo
    nuevo->izquierda = nullptr;
    nuevo->derecha = nullptr;
    nuevo->padre = nullptr;
    return nuevo;
}

void ArbolRojoNegro::insertarTurno(Turno* turno) {
    if (!turno) return;
    const std::string& ced = turno->getPaciente().getCedula();
    if (ced.empty()) return;
    // Si ya existe nodos con esa cédula, reemplaza el turno
    NodoArbol* existe = buscarPorCedula(ced);
    if (existe) {
        existe->turno = turno;
        return;
    }
    NodoArbol* nuevo = crearNodo(turno);
    insertarBalanceado(nuevo);
}

void ArbolRojoNegro::insertarBalanceado(NodoArbol* nodo) {
    NodoArbol* y = nullptr;
    NodoArbol* x = raiz;
    while (x) {
        y = x;
        if (nodo->cedula < x->cedula)
            x = x->izquierda;
        else
            x = x->derecha;
    }
    nodo->padre = y;
    if (!y)
        raiz = nodo;
    else if (nodo->cedula < y->cedula)
        y->izquierda = nodo;
    else
        y->derecha = nodo;
    nodo->color = true; // rojo
    balancearInsertar(nodo);
}

void ArbolRojoNegro::balancearInsertar(NodoArbol* nodo) {
    while (nodo != raiz && nodo->padre->color) {
        NodoArbol* abuelo = nodo->padre->padre;
        if (nodo->padre == abuelo->izquierda) {
            NodoArbol* tio = abuelo->derecha;
            if (tio && tio->color) {
                nodo->padre->color = false;
                tio->color = false;
                abuelo->color = true;
                nodo = abuelo;
            } else {
                if (nodo == nodo->padre->derecha) {
                    nodo = nodo->padre;
                    rotarIzquierda(nodo);
                }
                nodo->padre->color = false;
                abuelo->color = true;
                rotarDerecha(abuelo);
            }
        } else {
            NodoArbol* tio = abuelo->izquierda;
            if (tio && tio->color) {
                nodo->padre->color = false;
                tio->color = false;
                abuelo->color = true;
                nodo = abuelo;
            } else {
                if (nodo == nodo->padre->izquierda) {
                    nodo = nodo->padre;
                    rotarDerecha(nodo);
                }
                nodo->padre->color = false;
                abuelo->color = true;
                rotarIzquierda(abuelo);
            }
        }
    }
    raiz->color = false; // raíz siempre negro
}

void ArbolRojoNegro::rotarIzquierda(NodoArbol* x) {
    NodoArbol* y = x->derecha;
    x->derecha = y->izquierda;
    if (y->izquierda)
        y->izquierda->padre = x;
    y->padre = x->padre;
    if (!x->padre)
        raiz = y;
    else if (x == x->padre->izquierda)
        x->padre->izquierda = y;
    else
        x->padre->derecha = y;
    y->izquierda = x;
    x->padre = y;
}

void ArbolRojoNegro::rotarDerecha(NodoArbol* x) {
    NodoArbol* y = x->izquierda;
    x->izquierda = y->derecha;
    if (y->derecha)
        y->derecha->padre = x;
    y->padre = x->padre;
    if (!x->padre)
        raiz = y;
    else if (x == x->padre->derecha)
        x->padre->derecha = y;
    else
        x->padre->izquierda = y;
    y->derecha = x;
    x->padre = y;
}

NodoArbol* ArbolRojoNegro::buscarPorCedula(const std::string& cedula) {
    NodoArbol* actual = raiz;
    while (actual) {
        if (cedula == actual->cedula) return actual;
        actual = (cedula < actual->cedula)
                 ? actual->izquierda
                 : actual->derecha;
    }
    return nullptr;
}

void ArbolRojoNegro::reemplazarTurno(const std::string& cedula, Turno* nuevoTurno) {
    // Para reemplazar un turno, reutilizamos insertarTurno, que ya maneja sustitución si existe la cédula
    insertarTurno(nuevoTurno);
}

// Impresión limpia del árbol con solo datos clave
void ArbolRojoNegro::imprimirResumenNodo(NodoArbol* nodo, const std::string& prefijo, bool esDerecha) {
    if (!nodo || !nodo->turno) return;
    std::cout << prefijo << (esDerecha ? "└──" : "├──");
    std::string color = nodo->color ? "🔴" : "⚫";
    const Paciente& p = nodo->turno->getPaciente();
    std::cout << color << " " << nodo->cedula
              << " | " << p.getNombre() << " " << p.getApellido()
              << std::endl;
}

void ArbolRojoNegro::imprimirArbol(NodoArbol* nodo, std::string espacio, bool esDerecha) {
    if (!nodo || !nodo->turno || nodo->cedula.empty()) return;
    imprimirResumenNodo(nodo, espacio, esDerecha);
    imprimirArbol(nodo->izquierda, espacio + (esDerecha ? "    " : "│   "), false);
    imprimirArbol(nodo->derecha, espacio + (esDerecha ? "    " : "│   "), true);
}

void ArbolRojoNegro::imprimir() {
    std::cout << "Árbol Rojo-Negro:" << std::endl;
    imprimirArbol(raiz, "", true);
}
