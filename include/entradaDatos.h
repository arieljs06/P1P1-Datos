// EntradaDatos.h
#ifndef ENTRADADATOS_H
#define ENTRADADATOS_H

/// @file EntradaDatos.h
/// @brief Declaración de funciones para validación de datos de entrada usando DLL.

#include "ConfigDLL.h"

#ifdef __cplusplus
extern "C" {
#endif

    DLL_EXPORT const char* validarLetras(const char* msj);          ///< Valida que la entrada contenga solo letras.
    DLL_EXPORT const char* validarNumeros(const char* msj);         ///< Valida que la entrada contenga solo números.
    DLL_EXPORT const char* validarNombre(const char* msj);          ///< Valida el nombre.
    DLL_EXPORT const char* validarTelefono(const char* msj);        ///< Valida un número de teléfono.
    DLL_EXPORT const char* validarCedula(const char* msj);          ///< Valida una cédula.
    DLL_EXPORT const char* validarProvincia(const char* msj);       ///< Valida el nombre de la provincia.
    DLL_EXPORT const char* validarCiudad(const char* mensaje, const char* provincia);          ///< Valida el nombre de la ciudad.
    DLL_EXPORT int validarHora(const char* msj);                    ///< Valida la hora.
    DLL_EXPORT int validarMinuto(const char* msj);                  ///< Valida el minuto.
    DLL_EXPORT const char* validarCorreo(const char* msj);          ///< Valida un correo electrónico.

#ifdef __cplusplus
}
#endif

#endif
