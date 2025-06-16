#include "Turno.h"
#include "entradaDatos.h"
#include "ValidacionFecha.h"
#include "Paciente.h"
#include <iostream>

Turno::Turno(Paciente p, FechaHora f, const std::string& prov, const std::string& ciu)
    : paciente(p), fechaHora(f), provincia(prov), ciudad(ciu) {}

void Turno::setPaciente(const Paciente& p) {
    paciente = p;
}

void Turno::setFechaHora(const FechaHora& f) {
    fechaHora = f;
}

void Turno::setProvincia(const std::string& prov) {
    provincia = prov;
}

void Turno::setCiudad(const std::string& ciu) {
    ciudad = ciu;
}

Paciente Turno::getPaciente() const {
    return paciente;
}

FechaHora Turno::getFechaHora() const {
    return fechaHora;
}

std::string Turno::getProvincia() const {
    return provincia;
}

std::string Turno::getCiudad() const {
    return ciudad;
}

void Turno::mostrar() const {
    std::cout << "-------------------------------------\n";
    std::cout << "Detalles del turno:\n";
    std::cout << "Provincia: " << provincia << "\n"
              << "Ciudad: " << ciudad << "\n";
    std::cout << "Fecha y hora: ";
    fechaHora.mostrar(); 
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\nDatos del paciente:\n" << std::endl;
    paciente.mostrar();
}
