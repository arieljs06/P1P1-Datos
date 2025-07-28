#ifndef MENU_H
#define MENU_H

/// @file Menu.h
/// @brief Clase que gestiona la interfaz de usuario para agendar turnos.

#include "ListaDoble.h"
#include "ListaPacientes.h"
#include "Turno.h"
#include "Paciente.h"



class Menu {
private:
    ListaDoble lista;         ///< Lista de turnos médicos.
    ListaPacientes pacientes; ///< Lista de pacientes registrados.
public:
    void cargarPacientesDesdeArchivo();
    void cargarTurnosDesdeArchivoTXT();
    void buscarTurno(std::string cedula);
    void buscarHashEnArchivo();
    void mostrarMenu();
    void agregarPaciente();
    void agregarTurno();
    void buscarTurno();
    void mostrarTurnos();
    void mostrarPacientes();
    void eliminarTurno();
    void reemplazarTurno();
    void guardarBackup();
    void cargarBackup();
    void mostrarAyuda();
    void capitalizar(std::string& nom);
    void moduloArbolTurnos();
    void cargarTurnosEnArbol();
    void buscarTurnoQr();

    std::vector<std::pair<std::string, std::string>> parsearBackupsJSON(const std::string& json);
    std::string extraerCampoJSON(const std::string& json, const std::string& campo);

};

#endif // MENU_H