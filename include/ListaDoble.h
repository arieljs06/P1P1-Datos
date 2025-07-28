#ifndef LISTADOBLE_H
#define LISTADOBLE_H

#include <string>
#include <iostream>
#include "Nodo.h"
#include "FechaHora.h"
#include "Turno.h"
#include <vector>

class ListaDoble {
private:
    Nodo* cabeza;
    Nodo* cola;
    int size;

public:
    ListaDoble();
    ~ListaDoble();

    void agregar(Turno* dato);
    bool eliminarPorCedula(const std::string& cedula);
    Turno* buscarPorCedula(const std::string& cedula);

    // Cambia solo los datos del turno (fecha, provincia, ciudad)
    bool reemplazarDatosTurnoPorCedula(const std::string& cedula, const FechaHora& nuevaFecha,
                                       const std::string& nuevaProvincia, const std::string& nuevaCiudad);

    void mostrar();
    void limpiar();
    Nodo* getCabeza();
    bool existeTurno(int dia, int mes, int anio, int hora, int minuto, const std::string& provincia, const std::string& ciudad);
    int getSize() const;
    void guardarBackup(const std::string& archivo);
    void cargarBackup(const std::string& archivo);
    std::vector<std::pair<int, int>> franjasDisponibles(const std::string& provincia, const std::string& ciudad, int dia,
                                                        int mes, int anio);
};

#endif