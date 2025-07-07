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
#include "Marquee.h"

void guardarTurnoEnArchivo(Turno *); 
void guardarPacienteEnArchivo(Paciente *);
void eliminarTurnoPorCedula(const std::string&);
std::string base64_encode(const std::string &);
void generarTablaHash();
void imprimirTablaHash();
void buscarHashEnArchivo();
void eliminarHashPorValor(const std::string& );

void Menu::mostrarMenu() {
    cargarPacientesDesdeArchivo();
    cargarTurnosDesdeArchivoTXT();
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
        "Mostrar Tabla Hash",
        "Buscar por Hash",
        "Mostrar ayuda",
        "Salir"
    };
    int n = 13;
    int seleccion = 0;
    int tecla;

    do {
        system("cls");
        std::cout << "\n\n--- MENU DE TURNOS ---\n\n";
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
                case 0: std::cout<<"\n";agregarPaciente(); break;
                case 1: std::cout<<"\n";agregarTurno(); break;
                case 2: std::cout<<"\n";buscarTurno(); break;
                case 3: std::cout<<"\n";eliminarTurno(); break;
                case 4: std::cout<<"\n";reemplazarTurno(); break;
                case 5: std::cout<<"\n";mostrarTurnos(); break;
                case 6: std::cout<<"\n";mostrarPacientes(); break; 
                case 7: std::cout<<"\n";guardarBackup(); break;
                case 8: std::cout<<"\n";cargarBackup(); break;
                case 9: std::cout<<"\n";imprimirTablaHash(); break;
                case 10: std::cout<<"\n";buscarHashEnArchivo(); break;
                case 11: std::cout<<"\n";mostrarAyuda(); break;
                case 12:
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
    guardarPacienteEnArchivo(paciente);
    std::cout << "Paciente agregado correctamente.\n";
}

// --- Agregar turno solo para pacientes ya registrados ---
void Menu::agregarTurno() {
    std::string cedula = validarCedula("Ingrese cédula del paciente: ");
    Paciente* paciente = pacientes.buscarPorCedula(cedula);
    Turno* turnopre = lista.buscarPorCedula(cedula);
    if (!paciente) {
        std::cout << "No existe un paciente con esa cédula. Debe agregarlo primero.\n";
        return;
    }else if(turnopre){
        std::cout << "El paciente ya tiene turno asignado\n";
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
    guardarTurnoEnArchivo(turno);
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
// Sobrecarga del metodo para el hash
void Menu::buscarTurno(std::string cedula) {
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
        eliminarTurnoPorCedula(cedula);
        eliminarHashPorValor(base64_encode(cedula));
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
    std::string rutaBase = "bin\\backups\\";

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

void limpiarArchivo(const std::string& nombreArchivo) {
    try {
        // Usar filesystem para manejo correcto de rutas
        std::filesystem::path rutaArchivo(nombreArchivo);
        
        // Verificar si la ruta es válida
        if (rutaArchivo.empty()) {
            std::cerr << "Error: Ruta de archivo vacía" << std::endl;
            return;
        }

        // Abrir el archivo en modo truncate (borra contenido)
        std::ofstream archivo(rutaArchivo, std::ios::binary | std::ios::trunc);
        
        // Verificar si se abrió correctamente
        if (!archivo.is_open()) {
            std::cerr << "Error: No se pudo abrir " << rutaArchivo << std::endl;
            return;
        }

        // Forzar escritura y cierre inmediato
        archivo.close();
        
    }
    catch (const std::exception& e) {
        std::cerr << "Excepción al limpiar archivo: " << e.what() << std::endl;
    }
}

void Menu::cargarBackup() {
    std::string rutaBase = "bin\\backups\\";
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
    limpiarArchivo("bin\\data\\Turnos.txt");
    limpiarArchivo("bin\\data\\Tabla-Hash.txt");
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
        guardarTurnoEnArchivo(t);

        if (!pacientes.buscarPorCedula(cedula)) {
            pacientes.agregar(new Paciente(nombre, apellido, cedula, direccion, correo, telefono, sexo));
        }
    }

    archivo.close();
    generarTablaHash();
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

std::string base64_encode(const std::string &in) {
    const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
    
    std::string out;
    int val = 0, valb = -6;
    size_t len = in.length();
    unsigned int cnt = 0;
    
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
            cnt++;
        }
    }
    
    if (valb > -6) {
        out.push_back(base64_chars[(val << 8) >> (valb + 8) & 0x3F]);
        cnt++;
    }
    
    while (cnt % 4) {
        out.push_back('=');
        cnt++;
    }
    
    return out;
}

// --- Mostrar todos los pacientes ---
void Menu::mostrarPacientes() {
    std::cout << "----- LISTA DE PACIENTES REGISTRADOS -----\n";
    pacientes.mostrar(); 
}

void guardarPacienteEnArchivo(Paciente *pacienteP) {
    Paciente paciente = *pacienteP;
    const std::string RUTA_ARCHIVO = "bin\\data\\Pacientes.txt"; // Ruta fija interna
    
    std::ofstream archivo(RUTA_ARCHIVO, std::ios::app); // Modo append (añadir)

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo." << std::endl;
        return;
    }

    // Escribe los datos en el archivo (formato legible)
    archivo << "Nombre: " << paciente.getNombre() << "\n"
            << "Apellido: " << paciente.getApellido() << "\n"
            << "Cédula:  " << paciente.getCedula() << "\n"
            << "Dirección: " << paciente.getDireccion() << "\n"
            << "Correo: " << paciente.getCorreo() << "\n"
            << "Telefono: " << paciente.getTelefono() << "\n"
            << "Sexo: " << paciente.getSexo() << "\n"
            << "------------------------------\n";

    archivo.close();
    std::cout << "Paciente guardado en: " << RUTA_ARCHIVO << std::endl;
}

void guardarTurnoEnArchivo(Turno *turnoP) {
    Turno turno = *turnoP;
    const std::string RUTA_ARCHIVO = "bin\\data\\Turnos.txt"; // Ruta fija interna
    
    std::ofstream archivo(RUTA_ARCHIVO, std::ios::app); // Modo append (añadir)

    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo." << std::endl;
        return;
    }

    // Escribe los datos en el archivo (formato legible)
    archivo << "Nombre: " << turno.getPaciente().getNombre() << "\n"
            << "Apellido: " << turno.getPaciente().getApellido() << "\n"
            << "Cédula:  " << turno.getPaciente().getCedula() << "\n"
            << "Dirección: " << turno.getPaciente().getDireccion() << "\n"
            << "Correo: " << turno.getPaciente().getCorreo() << "\n"
            << "Telefono: " << turno.getPaciente().getTelefono() << "\n"
            << "Sexo: " << turno.getPaciente().getSexo() << "\n"
            << "Provincia: " << turno.getProvincia() << "\n"
            << "Ciudad: " << turno.getCiudad() << "\n"
            << "Fecha y Hora " << turno.getFechaHora().getDia() << "/" << turno.getFechaHora().getMes() << "/" << turno.getFechaHora().getAnio() << " -> " <<  turno.getFechaHora().getHora() << ":" << turno.getFechaHora().getMinuto() << "\n"
            << "------------------------------\n";

    archivo.close();
}

void Menu::cargarPacientesDesdeArchivo() {
    const std::string pacienteArchivo = "bin\\data\\Pacientes.txt";
    std::ifstream archivo(pacienteArchivo);
    
    // Variables temporales para almacenar los datos
    std::string nombre, apellido, cedula, direccion, correo, telefono, sexo;
    std::string linea;

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << pacienteArchivo << std::endl;
    }

    while (std::getline(archivo, linea)) {
        if (linea.find("Nombre: ") != std::string::npos) {
            nombre = linea.substr(8);
            nombre.erase(std::remove(nombre.begin(), nombre.end(), ' '), nombre.end());
        }
        else if (linea.find("Apellido: ") != std::string::npos) {
            apellido = linea.substr(10);
            apellido.erase(std::remove(apellido.begin(), apellido.end(), ' '), apellido.end());
        }
        else if (linea.find("Cédula: ") != std::string::npos) {
            cedula = linea.substr(8);
            cedula.erase(std::remove(cedula.begin(), cedula.end(), ' '), cedula.end());
        }
        else if (linea.find("Dirección: ") != std::string::npos) {
            direccion = linea.substr(11);
            direccion.erase(std::remove(direccion.begin(), direccion.end(), ' '), direccion.end());
        }
        else if (linea.find("Correo: ") != std::string::npos) {
            correo = linea.substr(8);
            correo.erase(std::remove(correo.begin(), correo.end(), ' '), correo.end());
        }
        else if (linea.find("Telefono: ") != std::string::npos) {
            telefono = linea.substr(10);
            telefono.erase(std::remove(telefono.begin(), telefono.end(), ' '), telefono.end());
        }
        else if (linea.find("Sexo: ") != std::string::npos) {
            sexo = linea.substr(6);
            sexo.erase(std::remove(sexo.begin(), sexo.end(), ' '), sexo.end());
        }
        else if (linea.find("------------------------------") != std::string::npos) {
            // Crear el objeto Paciente usando el constructor
            Paciente* paciente = new Paciente(nombre, apellido, cedula, direccion, correo, telefono, sexo);
            pacientes.agregar(paciente);
            // Reiniciar variables para el próximo paciente
            nombre = apellido = cedula = direccion = correo = telefono = sexo = "";
        }
    }

    archivo.close();
}

void eliminarHashPorValor(const std::string& hashBuscado) {
    const std::string rutaArchivo = "bin\\data\\Tabla-Hash.txt"; // Ajusta la ruta según tu necesidad
    std::ifstream archivoLectura(rutaArchivo);
    std::vector<std::string> lineas;
    std::string linea;
    bool encontrado = false;

    if (!archivoLectura.is_open()) {
        std::cerr << "Error al abrir el archivo para lectura." << std::endl;
        return;
    }

    // Leer todas las líneas y buscar el hash
    while (std::getline(archivoLectura, linea)) {
        if (linea.find(hashBuscado) != std::string::npos) {
            encontrado = true;
            continue; // Saltamos esta línea (la eliminamos)
        }
        lineas.push_back(linea);
    }
    archivoLectura.close();

    if (!encontrado) {
        std::cout << "No se encontró el hash " << hashBuscado << std::endl;
        return;
    }

    // Reescribir el archivo con las líneas restantes (renumerando)
    std::ofstream archivoEscritura(rutaArchivo, std::ios::out | std::ios::trunc);
    if (!archivoEscritura.is_open()) {
        std::cerr << "Error al abrir el archivo para escritura." << std::endl;
        return;
    }

    int nuevoIndice = 0;
    for (const auto& linea : lineas) {
        // Extraemos el hash (asumiendo formato "índice - hash")
        size_t guionPos = linea.find(" - ");
        if (guionPos != std::string::npos) {
            std::string hash = linea.substr(guionPos + 3);
            archivoEscritura << nuevoIndice << " - " << hash << "\n";
            nuevoIndice++;
        }
    }
    archivoEscritura.close();
}

void Menu::cargarTurnosDesdeArchivoTXT() {
    const std::string rutaArchivo = "bin\\data\\Turnos.txt"; // Ruta fija interna
    std::ifstream archivo(rutaArchivo);
    
    // Variables temporales para almacenar los datos
    std::string nombre, apellido, cedula, direccion, correo, telefono, sexo;
    std::string provincia, ciudad, fechaStr, horaStr;
    int dia, mes, anio, hora, minuto;
    std::string linea;

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << rutaArchivo << std::endl;
    }

    while (std::getline(archivo, linea)) {
        if (linea.find("Nombre: ") != std::string::npos) {
            nombre = linea.substr(8);
            nombre.erase(std::remove(nombre.begin(), nombre.end(), ' '), nombre.end());
        }
        else if (linea.find("Apellido: ") != std::string::npos) {
            apellido = linea.substr(10);
            apellido.erase(std::remove(apellido.begin(), apellido.end(), ' '), apellido.end());
        }
        else if (linea.find("Cédula:  ") != std::string::npos) {
            cedula = linea.substr(8);
            cedula.erase(std::remove(cedula.begin(), cedula.end(), ' '), cedula.end());
        }
        else if (linea.find("Dirección: ") != std::string::npos) {
            direccion = linea.substr(11);
            direccion.erase(std::remove(direccion.begin(), direccion.end(), ' '), direccion.end());
        }
        else if (linea.find("Correo: ") != std::string::npos) {
            correo = linea.substr(8);
            correo.erase(std::remove(correo.begin(), correo.end(), ' '), correo.end());
        }
        else if (linea.find("Telefono: ") != std::string::npos) {
            telefono = linea.substr(10);
            telefono.erase(std::remove(telefono.begin(), telefono.end(), ' '), telefono.end());
        }
        else if (linea.find("Sexo: ") != std::string::npos) {
            sexo = linea.substr(6);
            sexo.erase(std::remove(sexo.begin(), sexo.end(), ' '), sexo.end());
        }
        else if (linea.find("Provincia: ") != std::string::npos) {
            provincia = linea.substr(11);
            provincia.erase(std::remove(provincia.begin(), provincia.end(), ' '), provincia.end());
        }
        else if (linea.find("Ciudad: ") != std::string::npos) {
            ciudad = linea.substr(8);
            ciudad.erase(std::remove(ciudad.begin(), ciudad.end(), ' '), ciudad.end());
        }
        else if (linea.find("Fecha y Hora ") != std::string::npos) {
            std::string fechaHoraStr = linea.substr(13);
            std::replace(fechaHoraStr.begin(), fechaHoraStr.end(), '/', ' ');
            std::istringstream ss(fechaHoraStr);
            ss >> dia >> mes >> anio;
            ss.ignore(4); // Ignora " -> "
            ss >> hora >> minuto;
        }
        else if (linea.find("------------------------------") != std::string::npos) {
            // Crear objetos necesarios
            Paciente* paciente = new Paciente(nombre, apellido, cedula, direccion, correo, telefono, sexo);
            FechaHora fecha;
            fecha.setFechaHora(dia, mes, anio, hora, minuto);
            Turno* turno = new Turno(*paciente, fecha, provincia, ciudad);
            lista.agregar(turno);
            
            // Reiniciar variables para el próximo turno
            nombre = apellido = cedula = direccion = correo = telefono = sexo = "";
            provincia = ciudad = "";
            dia = mes = anio = hora = minuto = 0;
        }
    }

    archivo.close();
}

void eliminarTurnoPorCedula(const std::string& cedula) {
    const std::string rutaArchivo = "bin\\data\\Turnos.txt";
    std::ifstream archivoLectura(rutaArchivo);
    std::vector<std::string> lineas;
    std::string linea;
    bool encontrado = false;
    int inicioTurno = -1;

    if (!archivoLectura.is_open()) {
        std::cerr << "Error al abrir el archivo para lectura." << std::endl;
        return;
    }

    // Leer todas las líneas
    int index = 0;
    while (std::getline(archivoLectura, linea)) {
        lineas.push_back(linea);
        
        // Buscar la cédula con DOS espacios después de "Cédula:"
        if (linea.find("Cédula:  " + cedula) != std::string::npos) {
            encontrado = true;
            inicioTurno = index - 2; // Retroceder 2 líneas hasta el "Nombre:"
        }
        index++;
    }
    archivoLectura.close();

    if (!encontrado) {
        std::cout << "No se encontró un turno con la cédula " << cedula << std::endl;
        return;
    }

    // Eliminar las 11 líneas que componen el turno
    if (inicioTurno >= 0 && inicioTurno + 11 <= lineas.size()) {
        lineas.erase(lineas.begin() + inicioTurno, lineas.begin() + inicioTurno + 11);
    } else {
        std::cerr << "Error: Índices de turno inválidos." << std::endl;
        return;
    }

    // Reescribir el archivo
    std::ofstream archivoEscritura(rutaArchivo, std::ios::out | std::ios::trunc);
    if (!archivoEscritura.is_open()) {
        std::cerr << "Error al abrir el archivo para escritura." << std::endl;
        return;
    }

    for (const auto& l : lineas) {
        archivoEscritura << l << "\n";
    }
    archivoEscritura.close();

    std::cout << "Turno con cédula " << cedula << " eliminado correctamente." << std::endl;
}

void generarTablaHash() {
    const std::string archivoEntrada = "bin\\data\\Turnos.txt";
    const std::string archivoSalida = "bin\\data\\Tabla-Hash.txt";
    std::ifstream entrada(archivoEntrada);
    std::ofstream salida(archivoSalida);
    
    if (!entrada.is_open()) {
        std::cerr << "Error al abrir " << archivoEntrada << std::endl;
        return;
    }
    
    if (!salida.is_open()) {
        std::cerr << "Error al crear " << archivoSalida << std::endl;
        entrada.close();
        return;
    }

    std::string linea;
    std::string cedula;  // Variable temporal para almacenar cada nombre
    int contador = 0;

    while (std::getline(entrada, linea)) {
        // Cuando encontramos la línea que comienza con "Nombre: "
        if (linea.find("Cédula: ") == 0) {  // Verifica que empiece exactamente con "Nombre: "
            cedula = linea.substr(8);
            cedula.erase(std::remove(cedula.begin(), cedula.end(), ' '), cedula.end());
            cedula = base64_encode(cedula);
            
            // Escribimos en el archivo de salida
            salida << contador << " - " << cedula << std::endl;
            contador++;
            
            while (std::getline(entrada, linea) && linea != "------------------------------") {}
        }
    }

    entrada.close();
    salida.close();    
}

void imprimirTablaHash() {
    generarTablaHash();
    const std::string nombreArchivo = "bin\\data\\Tabla-Hash.txt";
    std::ifstream archivo(nombreArchivo);
    
    if (!archivo) {
        std::cerr << "Error al abrir el archivo" << std::endl;
        return;
    }

    std::cout<<"\n---------Tabla Hash Cedulas --------\n"<<std::endl;
    std::string linea;
    while (std::getline(archivo, linea)) {
        std::cout << linea << std::endl;
    }

    std::cout<<"\n\n";

    archivo.close();
}  

std::string base64_decode(const std::string &encoded_string) {
    // Tabla de caracteres Base64
    const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
    
    std::vector<unsigned char> bytes;
    bytes.reserve(encoded_string.size() * 3 / 4);
    
    int i = 0;
    int in_len = encoded_string.size();
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    
    while (in_len-- && (encoded_string[in_] != '=')) {
        // Convertir caracteres Base64 a valores 0-63
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            // Convertir 4 caracteres Base64 a 3 bytes
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                bytes.push_back(char_array_3[i]);
            i = 0;
        }
    }
    
    // Manejar padding '='
    if (i) {
        for (int j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (int j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (int j = 0; (j < i - 1); j++)
            bytes.push_back(char_array_3[j]);
    }
    
    return std::string(bytes.begin(), bytes.end());
}

void Menu::buscarHashEnArchivo() {
    generarTablaHash();
    const std::string rutaArchivo = "bin\\data\\Tabla-Hash.txt";
    // 1. Pedir el hash al usuario
    std::string hashBuscado;
    std::cout << "Ingrese el hash a buscar: ";
    std::getline(std::cin, hashBuscado);
    
    // Eliminar posibles espacios en blanco
    hashBuscado.erase(std::remove(hashBuscado.begin(), hashBuscado.end(), ' '), hashBuscado.end());

    // 2. Leer el archivo y cargar hashes en lista
    std::ifstream archivo(rutaArchivo);
    std::vector<std::string> listaHashes;
    
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << rutaArchivo << std::endl;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        // Extraer solo la parte del hash (asumiendo formato "número - hash")
        size_t separador = linea.find(" - ");
        if (separador != std::string::npos) {
            std::string hash = linea.substr(separador + 3);
            listaHashes.push_back(hash);
        }
    }
    archivo.close();

    // 3. Buscar el hash en la lista
    for (const auto& hash : listaHashes) {
        if (hash == hashBuscado) {
            std::string cedula = base64_decode(hash);
            cedula.erase(std::remove(cedula.begin(), cedula.end(), ' '), cedula.end());
            buscarTurno(cedula);
        }
    }
}