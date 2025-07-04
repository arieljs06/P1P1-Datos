#include "ListaPacientes.h"
#include "BucketSortUtils.h"
#include "Turno.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

ListaPacientes::ListaPacientes() : cabeza(nullptr) {}

ListaPacientes::~ListaPacientes() {
    NodoPaciente* actual = cabeza;
    while (actual != nullptr) {
        NodoPaciente* siguiente = actual->siguiente;
        delete actual->dato;
        delete actual;
        actual = siguiente;
    }
}

void ListaPacientes::agregar(Paciente* p) {
    if (Paciente* existente = buscarPorCedula(p->getCedula())) {
        std::cerr << "Error: Ya existe un paciente con la cédula " << p->getCedula() << std::endl;
        delete p;
        return;
    }
    NodoPaciente* nuevo = new NodoPaciente(p);
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
}

Paciente* ListaPacientes::buscarPorCedula(const std::string& cedula) {
    NodoPaciente* actual = cabeza;
    while (actual != nullptr) {
        if (actual->dato->getCedula() == cedula) {
            return actual->dato;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

void ListaPacientes::mostrar() {
    NodoPaciente* actual = cabeza;
    while (actual != nullptr) {
        actual->dato->mostrar();
        actual = actual->siguiente;
    }
}

void ListaPacientes::bucketSortPorCriterio(CriterioOrdenPaciente criterio) {
    // Contar pacientes
    int count = 0;
    NodoPaciente* actual = cabeza;
    while (actual != nullptr) {
        count++;
        actual = actual->siguiente;
    }

    // Crear arreglo dinámico de punteros
    Paciente** pacientesArr = new Paciente*[count];
    actual = cabeza;
    for (int i = 0; i < count; ++i) {
        pacientesArr[i] = actual->dato;
        actual = actual->siguiente;
    }

    // Ordenar usando el criterio
    switch (criterio) {
        case CriterioOrdenPaciente::NOMBRE:
            bucketSortAlpha(pacientesArr, count, [](Paciente* p) { return p->getNombre(); });
            break;
        case CriterioOrdenPaciente::APELLIDO:
            bucketSortAlpha(pacientesArr, count, [](Paciente* p) { return p->getApellido(); });
            break;
        case CriterioOrdenPaciente::FECHA:
            bucketSortAlpha(pacientesArr, count, [](Paciente* p) {
                if (p->getTurno())
                    return p->getTurno()->getFechaHora().toString();
                return std::string("");
            });
            break;
    }

    // Eliminar la lista enlazada actual (sin borrar los Paciente*)
    actual = cabeza;
    while (actual != nullptr) {
        NodoPaciente* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    cabeza = nullptr;

    // Reconstruir la lista enlazada ordenada
    NodoPaciente* ultimo = nullptr;
    for (int i = 0; i < count; ++i) {
        NodoPaciente* nuevo = new NodoPaciente(pacientesArr[i]);
        if (!cabeza) {
            cabeza = nuevo;
            ultimo = nuevo;
        } else {
            ultimo->siguiente = nuevo;
            ultimo = nuevo;
        }
    }

    delete[] pacientesArr;
}