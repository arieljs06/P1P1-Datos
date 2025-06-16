#include "Menu.h"
#include "EntradaDatos.h"
#include "ValidacionFecha.h"
#include "FechaHora.h"
#include "Paciente.h"
#include "listaDoble.h"
#include "ListaPacientes.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include <filesystem>
#include <chrono>
#include <windows.h>
#include <iomanip>
#include <sstream>
#include <ctime>



void Menu::mostrarMenu() {
    const char *opciones[] = {
        "Agregar paciente",
        "Agregar turno",
        "Buscar turno por cedula",
        "Eliminar turno",
        "Reemplazar turno",
        "Mostrar todos los turnos",
        "Mostrar todos los pacientes", 
        "Guardar backup",
        "Cargar backup",
        "Mostrar ayuda",
        "Salir"
    };
    int n = 11;
    int seleccion = 0;
    int tecla;

    do {
        system("cls");
        std::cout << "--- MENU DE TURNOS ---\n\n";
        for (int i = 0; i < n; ++i) {
            if (i == seleccion)
                std::cout << " -> ";
            else
                std::cout << "    ";
            std::cout << opciones[i] << "\n";
        }
        tecla = _getch();
        if (tecla == 0 || tecla == 224) {
            tecla = _getch();
            switch (tecla) {
                case 72: seleccion--; if (seleccion < 0) seleccion = n - 1; break;
                case 80: seleccion++; if (seleccion >= n) seleccion = 0; break;
            }
        } else if (tecla == 13) {
            system("cls");
            std::cout << "Opcion seleccionada: " << opciones[seleccion] << "\n";
            switch (seleccion) {
                case 0: agregarPaciente(); break;
                case 1: agregarTurno(); break;
                case 2: buscarTurno(); break;
                case 3: eliminarTurno(); break;
                case 4: reemplazarTurno(); break;
                case 5: mostrarTurnos(); break;
                case 6: mostrarPacientes(); break; 
                case 7: guardarBackup(); break;
                case 8: cargarBackup(); break;
                case 9: mostrarAyuda(); break;
                case 10:
                    std::cout << "Saliendo...\n";
                    return;
                default:
                    std::cout << "Opcion invalida\n";
            }
            system("pause");
        }
    } while (true);
}

// --- Agregar paciente ---
void Menu::agregarPaciente() {
    std::string cedula = validarCedula("Ingrese cédula: ");
    if (pacientes.buscarPorCedula(cedula)) {
        std::cout << "Ya existe un paciente con esa cédula.\n";
        return;
    }
    std::string telefono = validarTelefono("Ingrese teléfono: ");
    std::string nombre = validarNombre("Ingrese nombre: ");
    capitalizar(nombre);
    if (nombre.empty()) {
        std::cout << "Error: El nombre no puede estar vacío.\n";
        return;
    }
    std::string apellido = validarLetras("Ingrese apellido: ");
    capitalizar(apellido);
    if (apellido.empty()) {
        std::cout << "Error: El apellido no puede estar vacío.\n";
        return;
    }
    std::string sexo;
    do {
        sexo = validarLetras("Ingrese sexo (M/F): ");
        std::transform(sexo.begin(), sexo.end(), sexo.begin(), ::toupper);
        if (sexo != "M" && sexo != "F") {
            std::cout << "Error: El sexo debe ser 'M' o 'F'.\n";
        }
    } while (sexo != "M" && sexo != "F");
    std::string correo = validarCorreo("Ingrese correo: ");
    std::string direccion;
    do {
        std::cout << "Ingrese dirección: ";
        std::getline(std::cin, direccion);
        bool tieneEspecial = std::any_of(direccion.begin(), direccion.end(), [](char c) {
            return !isalnum(c) && c != ' ' && c != '#' && c != '-' && c != '.';
        });
        if (direccion.empty()) {
            std::cout << "Error: La dirección no puede estar vacía.\n";
        } else if (tieneEspecial) {
            std::cout << "Error: La dirección contiene caracteres inválidos.\n";
            direccion.clear();
        }
    } while (direccion.empty());

    Paciente* paciente = new Paciente(nombre, apellido, cedula, direccion, correo, telefono, sexo);
    pacientes.agregar(paciente);
    std::cout << "Paciente agregado correctamente.\n";
}

// --- Agregar turno solo para pacientes ya registrados ---
void Menu::agregarTurno() {
    std::string cedula = validarCedula("Ingrese cédula del paciente: ");
    Paciente* paciente = pacientes.buscarPorCedula(cedula);
    if (!paciente) {
        std::cout << "No existe un paciente con esa cédula. Debe agregarlo primero.\n";
        return;
    }

    std::string provincia = validarProvincia("Ingrese provincia: ");
    std::string ciudad = validarCiudad("Ingrese ciudad: ", provincia.c_str());

    int dia = -1, mes = -1, anio = -1;
    FechaHora fecha;
    do {
        do {
            std::string input = validarNumeros("Ingrese año: ");
            try {
                anio = std::stoi(input);
                if (!validarYear(anio) || anio < 2024) {
                    std::cout << "Error: El año debe ser 2024 o mayor.\n";
                    anio = -1;
                }
            } catch (...) {
                std::cout << "Error: Ingrese dos números.\n";
                anio = -1;
            }
        } while (anio == -1);

        do {
            std::string input = validarNumeros("Ingrese mes (1-12): ");
            try {
                mes = std::stoi(input);
                if (!validarMes(mes)) {
                    std::cout << "Error: El mes debe estar entre 1 y 12.\n";
                    mes = -1;
                }
            } catch (...) {
                std::cout << "Error: Ingrese dos números.\n";
                mes = -1;
            }
        } while (mes == -1);

        do {
            std::string input = validarNumeros("Ingrese el día (1-31): ");
            try {
                dia = std::stoi(input);
                if (!validarDia(dia, mes, anio)) {
                    std::cout << "Día inválido para ese mes y año.\n";
                    dia = -1;
                }
            } catch (...) {
                std::cout << "Error: Ingrese dos números.\n";
                dia = -1;
            }
        } while (dia == -1);

        fecha.setFechaHora(dia, mes, anio, 0, 0);
        if (fecha.esNoValida()) {
            std::cout << "Fecha inválida: ya pasó o es feriado.\n";
        }
    } while (fecha.esNoValida());

    auto disponibles = lista.franjasDisponibles(provincia, ciudad, dia, mes, anio);

    if (disponibles.empty()) {
        std::cout << "No hay franjas disponibles para esa fecha.\n";
        return;
    }

    std::cout << "Franjas disponibles:\n";
    const int maxPorFila = 5;
    for (size_t i = 0; i < disponibles.size(); ++i) {
        auto [h, m] = disponibles[i];
        std::cout << std::setw(2) << std::right << i + 1 << ". " 
                << std::setw(2) << std::setfill('0') << h << ":" 
                << std::setw(2) << std::setfill('0') << m << "\t";
        if ((i + 1) % maxPorFila == 0) {
            std::cout << std::endl;
        }
    }
    if (disponibles.size() % maxPorFila != 0) {
        std::cout << std::endl;
    }

    int opcion = -1;
    do {
        std::string input = validarNumeros("Seleccione una franja disponible (número): ");
        try {
            opcion = std::stoi(input);
            if (opcion < 1 || opcion > disponibles.size()) {
                std::cout << "Opción fuera de rango.\n";
                opcion = -1;
            }
        } catch (...) {
            std::cout << "Opción inválida.\n";
            opcion = -1;
        }
    } while (opcion == -1);

    int hora = disponibles[opcion - 1].first;
    int minuto = disponibles[opcion - 1].second;
    fecha.setFechaHora(dia, mes, anio, hora, minuto);

    Turno* turno = new Turno(*paciente, fecha, provincia, ciudad);
    lista.agregar(turno);
    std::cout << "Turno agregado correctamente a las " << hora << ":" << (minuto < 10 ? "0" : "") << minuto << ".\n";
}


// --- Buscar turno ---
void Menu::buscarTurno() {
    std::string cedula = validarNumeros("Ingrese cedula para buscar: ");
    Turno* turnoPtr = lista.buscarPorCedula(cedula);
    if (turnoPtr) {
        turnoPtr->mostrar();
    } else {
        std::cout << "Turno no encontrado.\n";
    }
}

// --- Eliminar turno ---
void Menu::eliminarTurno() {
    std::string cedula = validarNumeros("Ingrese cedula para eliminar: ");
    if (lista.eliminarPorCedula(cedula)) {
        std::cout << "Turno eliminado.\n";
    } else {
        std::cout << "No se encontro turno con esa cedula.\n";
    }
}


void Menu::reemplazarTurno() {
    std::string cedula = validarCedula("Ingrese cédula del turno a reemplazar: ");
    Turno* turnoPtr = lista.buscarPorCedula(cedula);
    if (!turnoPtr) {
        std::cout << "No se encontró turno con esa cédula.\n";
        return;
    }

    // Eliminar el turno existente
    lista.eliminarPorCedula(cedula);

    std::cout << "Ingrese los nuevos datos:\n";

    std::string provincia = validarProvincia("Ingrese nueva provincia: ");
    std::string ciudad = validarCiudad("Ingrese nueva ciudad: ", provincia.c_str());

    int dia = -1, mes = -1, anio = -1, hora = -1, minuto = -1;
    FechaHora fecha;
    do {
        do {
            std::string input = validarNumeros("Ingrese año (>=2024): ");
            try {
                anio = std::stoi(input);
                if (!validarYear(anio) || anio < 2024) {
                    std::cout << "Error: El año debe ser 2024 o mayor.\n";
                    anio = -1;
                }
            } catch (...) {
                std::cout << "Error: Ingrese solo números.\n";
                anio = -1;
            }
        } while (anio == -1);

        do {
            std::string input = validarNumeros("Ingrese mes (1-12): ");
            try {
                mes = std::stoi(input);
                if (!validarMes(mes)) {
                    std::cout << "Error: El mes debe estar entre 1 y 12.\n";
                    mes = -1;
                }
            } catch (...) {
                std::cout << "Error: Ingrese solo números.\n";
                mes = -1;
            }
        } while (mes == -1);

        do {
            std::string input = validarNumeros("Ingrese el día (1-31): ");
            try {
                dia = std::stoi(input);
                if (!validarDia(dia, mes, anio)) {
                    std::cout << "Día inválido para ese mes y año.\n";
                    dia = -1;
                }
            } catch (...) {
                std::cout << "Error: Ingrese solo números.\n";
                dia = -1;
            }
        } while (dia == -1);

        fecha.setFechaHora(dia, mes, anio, 0, 0);
        if (fecha.esNoValida()) {
            std::cout << "Fecha inválida: ya pasó o es feriado.\n";
        }
    } while (fecha.esNoValida());

    hora = validarHora("Ingrese la hora (0-23): ");
    minuto = validarMinuto("Ingrese el minuto (0-59): ");
    fecha.setFechaHora(dia, mes, anio, hora, minuto);

    if (fecha.esNoValida()) {
        std::cout << "Fecha y hora inválida: ya pasó.\n";
        return;
    }

    if (lista.existeTurno(dia, mes, anio, hora, minuto, provincia, ciudad)) {
        std::cout << "Ya existe un turno en esa fecha, hora y provincia.\n";
        return;
    }

    // Crear un nuevo turno con los datos actualizados
    Paciente* pacienteExistente = pacientes.buscarPorCedula(cedula);
    if (!pacienteExistente) {
        std::cout << "Error: El paciente no existe.\n";
        return;
    }

    Turno* nuevoTurno = new Turno(*pacienteExistente, fecha, provincia, ciudad);
    lista.agregar(nuevoTurno);

    std::cout << "Datos del turno reemplazados correctamente.\n";
}

void Menu::mostrarTurnos() {
    std::vector<Turno*> turnos;
    Nodo* actual = lista.getCabeza();
    while (actual != nullptr) {
        turnos.push_back(actual->dato);
        actual = actual->siguiente;
    }

    // Verifica si hay turnos
    if (turnos.empty()) {
        std::cout << "No hay turnos registrados.\n";
        return;
    }
    const char* opciones[] = {
        "Ordenar por fecha y hora",
        "Ordenar por nombre de paciente",
        "Ordenar por apellido de paciente"
    };
    int n = 3;
    int seleccion = 0;
    int tecla;

    // Submenú con flechas para elegir el criterio de ordenamiento
    do {
        system("cls");
        std::cout << "----- LISTA DE TURNOS REGISTRADOS -----\n";
        std::cout << "¿Cómo desea ordenar la lista de turnos?\n\n";
        for (int i = 0; i < n; ++i) {
            if (i == seleccion)
                std::cout << " -> ";
            else
                std::cout << "    ";
            std::cout << opciones[i] << "\n";
        }
        tecla = _getch();
        if (tecla == 0 || tecla == 224) {
            tecla = _getch();
            switch (tecla) {
                case 72: seleccion--; if (seleccion < 0) seleccion = n - 1; break; // Arriba
                case 80: seleccion++; if (seleccion >= n) seleccion = 0; break;   // Abajo
            }
        } else if (tecla == 13) {
            break; // Enter
        }
    } while (true);

    // Ordena según el criterio seleccionado
    switch (seleccion) {
        case 0: // Fecha y hora
            std::sort(turnos.begin(), turnos.end(), [](Turno* a, Turno* b) {
                return a->getFechaHora() < b->getFechaHora();
            });
            break;
        case 1: // Nombre de paciente
            std::sort(turnos.begin(), turnos.end(), [](Turno* a, Turno* b) {
                return a->getPaciente().getNombre() < b->getPaciente().getNombre();
            });
            break;
        case 2: // Apellido de paciente
            std::sort(turnos.begin(), turnos.end(), [](Turno* a, Turno* b) {
                return a->getPaciente().getApellido() < b->getPaciente().getApellido();
            });
            break;
    }

    // Muestra los turnos ordenados
    std::cout << "\nTurnos ordenados:\n";
    for (Turno* t : turnos) {
        t->mostrar();
        std::cout << "--------------------------\n";
    }
}

void Menu::guardarBackup() {
    // Carpeta fija para los backups
    std::string rutaBase = "C:\\Users\\Rafita Valentina\\Documents\\test\\Proyecto1.1.2P1\\bin\\backups\\";

    // Crear la carpeta si no existe
    if (!std::filesystem::exists(rutaBase)) {
        std::filesystem::create_directories(rutaBase);
    }

    // Obtener fecha y hora actuales
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_info = std::localtime(&now_time);

    // Nombre del archivo con fecha y hora
    std::ostringstream oss;
    oss << "backup_"
        << std::put_time(tm_info, "%Y-%m-%d_%H-%M-%S")
        << ".bin";

    std::string rutaCompleta = rutaBase + oss.str();

    // Abrir archivo binario
    std::ofstream archivo(rutaCompleta, std::ios::binary);
    if (!archivo) {
        std::cout << "No se pudo abrir el archivo para guardar.\n";
        return;
    }

    Nodo* actual = lista.getCabeza();
    while (actual != nullptr) {
        Turno* t = actual->dato;
        Paciente p = t->getPaciente();
        FechaHora f = t->getFechaHora();

        archivo.write(reinterpret_cast<char*>(&f), sizeof(FechaHora));
        archivo.write(p.getNombre().c_str(), p.getNombre().size() + 1);
        archivo.write(p.getApellido().c_str(), p.getApellido().size() + 1);
        archivo.write(p.getCedula().c_str(), p.getCedula().size() + 1);
        archivo.write(p.getDireccion().c_str(), p.getDireccion().size() + 1);
        archivo.write(p.getCorreo().c_str(), p.getCorreo().size() + 1);
        archivo.write(p.getTelefono().c_str(), p.getTelefono().size() + 1);
        archivo.write(p.getSexo().c_str(), p.getSexo().size() + 1);
        archivo.write(t->getProvincia().c_str(), t->getProvincia().size() + 1);
        archivo.write(t->getCiudad().c_str(), t->getCiudad().size() + 1);

        actual = actual->siguiente;
    }

    archivo.close();
    std::cout << "Backup guardado correctamente en:\n" << rutaCompleta << "\n";
}

void Menu::cargarBackup() {
    std::string rutaBase = "C:\\Users\\Rafita Valentina\\Documents\\test\\Proyecto1.1.2P1\\bin\\backups\\";

    if (!std::filesystem::exists(rutaBase) || !std::filesystem::is_directory(rutaBase)) {
        std::cout << "La carpeta de backups no existe.\n";
        return;
    }

    // Recopilar todos los archivos backup_*.bin
    std::vector<std::filesystem::path> backups;

    for (const auto& entry : std::filesystem::directory_iterator(rutaBase)) {
        if (std::filesystem::is_regular_file(entry)) {
            auto nombre = entry.path().filename().string();
            if (nombre.find("backup_") == 0 && entry.path().extension() == ".bin") {
                backups.push_back(entry.path());
            }
        }
    }

    if (backups.empty()) {
        std::cout << "No se encontraron archivos de backup en la carpeta.\n";
        return;
    }

    // Mostrar lista de backups
    std::cout << "\n--- Archivos de Backup Disponibles ---\n";
    for (size_t i = 0; i < backups.size(); ++i) {
        std::cout << i + 1 << ". " << backups[i].filename().string() << "\n";
    }

    // Permitir al usuario elegir un backup
    int opcion = 0;
    std::cout << "Seleccione el número del backup a restaurar: ";
    std::cin >> opcion;
    std::cin.ignore(); // Limpiar el salto de línea

    if (opcion < 1 || static_cast<size_t>(opcion) > backups.size()) {
        std::cout << "Opción no válida.\n";
        return;
    }

    std::filesystem::path archivoElegido = backups[opcion - 1];

    std::ifstream archivo(archivoElegido, std::ios::binary);
    if (!archivo) {
        std::cout << "No se pudo abrir el archivo seleccionado.\n";
        return;
    }

    lista = ListaDoble(); // Limpiar la lista actual

    while (archivo.peek() != EOF) {
        FechaHora fecha;
        archivo.read(reinterpret_cast<char*>(&fecha), sizeof(FechaHora));

        std::string nombre, apellido, cedula, direccion, correo, telefono, sexo, provincia, ciudad;

        std::getline(archivo, nombre, '\0');
        std::getline(archivo, apellido, '\0');
        std::getline(archivo, cedula, '\0');
        std::getline(archivo, direccion, '\0');
        std::getline(archivo, correo, '\0');
        std::getline(archivo, telefono, '\0');
        std::getline(archivo, sexo, '\0');
        std::getline(archivo, provincia, '\0');
        std::getline(archivo, ciudad, '\0');

        Paciente* p = new Paciente(nombre, apellido, cedula, direccion, correo, telefono, sexo);
        Turno* t = new Turno(*p, fecha, provincia, ciudad);
        lista.agregar(t);

        if (!pacientes.buscarPorCedula(cedula)) {
            pacientes.agregar(new Paciente(nombre, apellido, cedula, direccion, correo, telefono, sexo));
        }
    }

    archivo.close();
    std::cout << "Backup restaurado correctamente desde: " << archivoElegido << "\n";
}

// --- Mostrar ayuda ---
void Menu::mostrarAyuda() {
    const char* ayuda =
        "----- AYUDA DEL SISTEMA DE TURNOS MEDICOS -----\n\n"
        "1. Agregar paciente:\n"
        "   Registra los datos de un paciente.\n\n"
        "2. Agregar turno:\n"
        "   Solo para pacientes ya registrados.\n\n"
        "3. Buscar turno:\n"
        "   Busca un turno por cédula.\n\n"
        "4. Eliminar turno:\n"
        "   Elimina un turno por cédula.\n\n"
        "5. Reemplazar turno:\n"
        "   Cambia la fecha, hora, provincia o ciudad de un turno.\n\n"
        "6. Mostrar todos los turnos:\n"
        "   Lista todos los turnos agendados.\n\n"
        "7. Guardar backup:\n"
        "   Guarda todos los turnos y pacientes en un archivo .bin.\n\n"
        "8. Cargar backup:\n"
        "   Recupera los turnos y pacientes desde un archivo .bin.\n\n"
        "9. Salir:\n"
        "   Cierra el programa.";

    MessageBoxA(
        NULL,
        ayuda,
        "Ayuda del Sistema de Turnos Médicos",
        MB_OK | MB_ICONINFORMATION
    );
}

// --- Capitalizar nombres ---
void Menu::capitalizar(std::string& texto) {
    std::transform(texto.begin(), texto.end(), texto.begin(), ::tolower);
    if (!texto.empty())
        texto[0] = ::toupper(texto[0]);
    for (size_t i = 1; i < texto.length(); ++i) {
        if (texto[i - 1] == ' ')
            texto[i] = ::toupper(texto[i]);
    }
}

// --- Mostrar todos los pacientes ---
void Menu::mostrarPacientes() {
    std::cout << "----- LISTA DE PACIENTES REGISTRADOS -----\n";
    pacientes.mostrar(); 
}
