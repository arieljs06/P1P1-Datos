// Nueva lista para pacientes
#ifndef LISTAPACIENTES_H
#define LISTAPACIENTES_H

#include "Paciente.h"
#include <string>

class NodoPaciente {
public:
    Paciente* dato;
    NodoPaciente* siguiente;
    NodoPaciente(Paciente* p) : dato(p), siguiente(nullptr) {}
};

class ListaPacientes {
private:
    NodoPaciente* cabeza;
public:
    ListaPacientes();
    ~ListaPacientes();
    void agregar(Paciente* p);
    Paciente* buscarPorCedula(const std::string& cedula);
    void mostrar();
    enum class CriterioOrdenPaciente { NOMBRE, APELLIDO, FECHA };
    void bucketSortPorCriterio(CriterioOrdenPaciente criterio);
    void limpiar();

};

#endif