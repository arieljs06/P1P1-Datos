#ifndef TURNO_H
#define TURNO_H

#include <string>

/// @file Turno.h
/// @brief Clase que representa un turno médico.

#include "Paciente.h"
#include "FechaHora.h"
#include <string>

class Turno {
private:
    Paciente paciente;         ///< Información del paciente.
    FechaHora fechaHora;       ///< Fecha y hora del turno.
    std::string provincia;     ///< Provincia del turno.
    std::string ciudad;        ///< Ciudad del turno.

public:
    Turno();
    Turno(Paciente p, FechaHora f, const std::string& prov, const std::string& ciu);

    void setPaciente(const Paciente& p);
    void setFechaHora(const FechaHora& f);
    void setProvincia(const std::string& prov);
    void setCiudad(const std::string& ciu);

    const Paciente& getPaciente() const;
    FechaHora getFechaHora() const;
    std::string getProvincia() const;
    std::string getCiudad() const;

    void mostrar() const;
};

#endif