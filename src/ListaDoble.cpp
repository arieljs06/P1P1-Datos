#include <iostream>
#include <fstream>
#include <algorithm>
#include "ListaDoble.h"

ListaDoble::ListaDoble() : cabeza(nullptr), cola(nullptr), size(0) {}

ListaDoble::~ListaDoble() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguiente = actual->siguiente;
        delete actual->dato;
        delete actual;
        actual = siguiente;
    }
    size = 0;
}

void ListaDoble::agregar(Turno* dato) {
    if (buscarPorCedula(dato->getPaciente().getCedula()) != nullptr) {
        std::cerr << "Error: Ya existe un turno con la cédula " << dato->getPaciente().getCedula() << std::endl;
        delete dato;
        return;
    }
    Nodo* nuevo = new Nodo(dato);
    if (!cabeza) {
        cabeza = cola = nuevo;
    } else {
        cola->siguiente = nuevo;
        nuevo->anterior = cola;
        cola = nuevo;
    }
    size++;
}

bool ListaDoble::eliminarPorCedula(const std::string& cedula) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->dato->getPaciente().getCedula() == cedula) {
            if (actual->anterior) actual->anterior->siguiente = actual->siguiente;
            else cabeza = actual->siguiente;

            if (actual->siguiente) actual->siguiente->anterior = actual->anterior;
            else cola = actual->anterior;

            delete actual->dato;
            delete actual;
            size--;
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

Turno* ListaDoble::buscarPorCedula(const std::string& cedula) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->dato->getPaciente().getCedula() == cedula) {
            return actual->dato;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

bool ListaDoble::reemplazarDatosTurnoPorCedula(const std::string& cedula, const FechaHora& nuevaFecha,
                                               const std::string& nuevaProvincia, const std::string& nuevaCiudad) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->dato->getPaciente().getCedula() == cedula) {
            actual->dato->setFechaHora(nuevaFecha);
            actual->dato->setProvincia(nuevaProvincia);
            actual->dato->setCiudad(nuevaCiudad);
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

void ListaDoble::mostrar() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        actual->dato->mostrar();
        actual = actual->siguiente;
    }
}

bool ListaDoble::existeTurno(int dia, int mes, int anio, int hora, int minuto, const std::string& provincia, const std::string& ciudad) {
    Nodo* actual = cabeza;

    while (actual != nullptr) {
        const FechaHora& fh = actual->dato->getFechaHora();

        // Validar coincidencia de provincia, ciudad y fecha
        if (actual->dato->getProvincia() == provincia &&
            actual->dato->getCiudad() == ciudad &&
            fh.getDia() == dia &&
            fh.getMes() == mes &&
            fh.getAnio() == anio) {

            // Calcular minutos de ambos turnos
            int minutosActual = fh.getHora() * 60 + fh.getMinuto();
            int minutosNuevo  = hora * 60 + minuto;
            int diferencia = std::abs(minutosActual - minutosNuevo);

           
        }

        actual = actual->siguiente;
    }

    return false; // No hay conflicto
}



void ListaDoble::guardarBackup(const std::string& archivo) {
    std::ofstream out(archivo, std::ios::binary);
    if (!out) {
        std::cerr << "No se pudo crear el archivo de backup." << std::endl;
        return;
    }

    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Turno* t = actual->dato;
        Paciente p = t->getPaciente();
        FechaHora f = t->getFechaHora();
        std::string provincia = t->getProvincia();
        std::string ciudad = t->getCiudad();

        // Guardar campos primitivos
        int len;

        // Cedula
        len = p.getCedula().size(); out.write((char*)&len, sizeof(int));
        out.write(p.getCedula().c_str(), len);

        // Nombre
        len = p.getNombre().size(); out.write((char*)&len, sizeof(int));
        out.write(p.getNombre().c_str(), len);

        // Apellido
        len = p.getApellido().size(); out.write((char*)&len, sizeof(int));
        out.write(p.getApellido().c_str(), len);

        // Correo
        len = p.getCorreo().size(); out.write((char*)&len, sizeof(int));
        out.write(p.getCorreo().c_str(), len);

        // Telefono
        len = p.getTelefono().size(); out.write((char*)&len, sizeof(int));
        out.write(p.getTelefono().c_str(), len);

        // Direccion
        len = p.getDireccion().size(); out.write((char*)&len, sizeof(int));
        out.write(p.getDireccion().c_str(), len);

        // Sexo
        len = p.getSexo().size(); out.write((char*)&len, sizeof(int));
        out.write(p.getSexo().c_str(), len);

        // FechaHora
        int d = f.getDia(), m = f.getMes(), a = f.getAnio(), h = f.getHora(), min = f.getMinuto();
        out.write((char*)&d, sizeof(int));
        out.write((char*)&m, sizeof(int));
        out.write((char*)&a, sizeof(int));
        out.write((char*)&h, sizeof(int));
        out.write((char*)&min, sizeof(int));

        // Provincia y ciudad
        len = provincia.size(); out.write((char*)&len, sizeof(int));
        out.write(provincia.c_str(), len);

        len = ciudad.size(); out.write((char*)&len, sizeof(int));
        out.write(ciudad.c_str(), len);

        actual = actual->siguiente;
    }

    out.close();
    std::cout << "Backup guardado correctamente." << std::endl;
}

void ListaDoble::cargarBackup(const std::string& archivo) {
    std::ifstream in(archivo, std::ios::binary);
    if (!in) {
        std::cerr << "No se pudo abrir el archivo de backup." << std::endl;
        return;
    }

    while (in.peek() != EOF) {
        std::string cedula, nombre, apellido, correo, telefono, direccion, sexo, provincia, ciudad;
        int len;

        // Cedula
        in.read((char*)&len, sizeof(int));
        cedula.resize(len); in.read(&cedula[0], len);

        // Nombre
        in.read((char*)&len, sizeof(int));
        nombre.resize(len); in.read(&nombre[0], len);

        // Apellido
        in.read((char*)&len, sizeof(int));
        apellido.resize(len); in.read(&apellido[0], len);

        // Correo
        in.read((char*)&len, sizeof(int));
        correo.resize(len); in.read(&correo[0], len);

        // Telefono
        in.read((char*)&len, sizeof(int));
        telefono.resize(len); in.read(&telefono[0], len);

        // Direccion
        in.read((char*)&len, sizeof(int));
        direccion.resize(len); in.read(&direccion[0], len);

        // Sexo
        in.read((char*)&len, sizeof(int));
        sexo.resize(len); in.read(&sexo[0], len);

        // FechaHora
        int d, m, a, h, min;
        in.read((char*)&d, sizeof(int));
        in.read((char*)&m, sizeof(int));
        in.read((char*)&a, sizeof(int));
        in.read((char*)&h, sizeof(int));
        in.read((char*)&min, sizeof(int));

        // Provincia
        in.read((char*)&len, sizeof(int));
        provincia.resize(len); in.read(&provincia[0], len);

        // Ciudad
        in.read((char*)&len, sizeof(int));
        ciudad.resize(len); in.read(&ciudad[0], len);

        Paciente p(cedula, nombre, apellido, correo, telefono, direccion, sexo);
        FechaHora fh(d, m, a, h, min);
        Turno* t = new Turno(p, fh, provincia, ciudad);
        agregar(t);
    }

    in.close();
    std::cout << "Backup cargado correctamente." << std::endl;
}

Nodo* ListaDoble::getCabeza() {
    return cabeza;
}

std::vector<std::pair<int, int>> ListaDoble::franjasDisponibles(const std::string& provincia, const std::string& ciudad, int dia, int mes, int anio) {
    bool ocupado[24 * 60] = { false }; 

    Nodo* actual = cabeza;
    while (actual != nullptr) {
        auto turno = actual->dato;
        auto fh = turno->getFechaHora();

        if (fh.getDia() == dia && fh.getMes() == mes && fh.getAnio() == anio &&
            turno->getProvincia() == provincia && turno->getCiudad() == ciudad) {
            int inicio = fh.getHora() * 60 + fh.getMinuto();
            for (int i = 0; i < 20; ++i) {
                if (inicio + i < 24 * 60) {
                    ocupado[inicio + i] = true;
                }
            }
        }

        actual = actual->siguiente;
    }

    std::vector<std::pair<int, int>> disponibles;

    for (int i = 8 * 60; i <= 19 * 60 - 20; i += 20) {
        bool libre = true;
        for (int j = 0; j < 20; ++j) {
            if (ocupado[i + j]) {
                libre = false;
                break;
            }
        }

        if (libre) {
            disponibles.emplace_back(i / 60, i % 60);
        }
    }

    return disponibles;
}
