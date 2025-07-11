#ifndef FECHAHORA_H
#define FECHAHORA_H

#include <string>
/// @file FechaHora.h
/// @brief Clase para representar y validar fechas y horas.

class FechaHora {
private:
    int dia, mes, anio, hora, minuto;
public:
    FechaHora();
    FechaHora(const std::string& fecha, const std::string& hora);
    FechaHora(int d, int m, int a, int h, int min);

    void setFechaHora(int d, int m, int a, int h, int min);

    int getDia() const;
    int getMes() const;
    int getAnio() const;
    int getHora() const;
    int getMinuto() const;

    bool esPasada() const;
    bool esNoValida() const;

    void mostrar() const;

    std::string toString() const;

    bool operator<(const FechaHora& other) const;
};

#endif