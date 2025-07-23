#ifndef FECHAHORA_H
#define FECHAHORA_H

#include <string>

/// @file FechaHora.h
/// @brief Clase para representar y validar fechas y horas.

/**
 * @class FechaHora
 * @brief Representa una fecha y hora, e incluye funciones de validación y comparación.
 */
class FechaHora {
private:
    int dia, mes, anio, hora, minuto; ///< Componentes de fecha y hora

public:
    /// @brief Constructor por defecto. Inicializa con valores inválidos.
    FechaHora();

    /// @brief Constructor que recibe fecha y hora como cadenas.
    /// @param fecha Cadena en formato "dd/mm/yyyy"
    /// @param hora Cadena en formato "hh:mm"
    FechaHora(const std::string& fecha, const std::string& hora);

    /// @brief Constructor con parámetros enteros.
    FechaHora(int d, int m, int a, int h, int min);

    /// @brief Establece una nueva fecha y hora.
    void setFechaHora(int d, int m, int a, int h, int min);

    int getDia() const;     ///< Retorna el día.
    int getMes() const;     ///< Retorna el mes.
    int getAnio() const;    ///< Retorna el año.
    int getHora() const;    ///< Retorna la hora.
    int getMinuto() const;  ///< Retorna el minuto.

    /// @brief Verifica si la fecha/hora ya pasó.
    bool esPasada() const;

    /// @brief Verifica si la fecha/hora es inválida (pasada o feriado).
    bool esNoValida() const;

    /// @brief Muestra la fecha y hora por consola.
    void mostrar() const;

    /// @brief Devuelve la fecha y hora como string en formato "dd/mm/yyyy -> hh:mm".
    std::string toString() const;

    /// @brief Operador menor para comparar fechas.
    bool operator<(const FechaHora& other) const;
};

#endif
