#ifndef PACIENTE_H
#define PACIENTE_H


#include <string>
class Turno;


/// @file Paciente.h
/// @brief Clase que representa a un paciente.



class Paciente {
private:
    std::string nombre;
    std::string apellido;
    std::string cedula;
    std::string direccion;
    std::string correo;
    std::string telefono;
    std::string sexo;
    Turno* turno;
public:
    Paciente(const std::string& nombre, const std::string& apellido, const std::string& cedula,
             const std::string& direccion, const std::string& correo,
             const std::string& telefono, const std::string& sexo);

    ~Paciente();

    std::string getNombre() const;
    std::string getApellido() const;
    std::string getCedula() const;
    std::string getDireccion() const;
    std::string getCorreo() const;
    std::string getTelefono() const;
    std::string getSexo() const;

    void mostrar() const;

    Turno* getTurno() const { return turno; }
    void setTurno(Turno* t) { turno = t; }
};

#endif