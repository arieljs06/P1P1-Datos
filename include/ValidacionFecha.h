// ValidacionFecha.h
#ifndef VALIDACION_FECHA_H
#define VALIDACION_FECHA_H

#include "ConfigDLL.h"

// FUNCIONES EXPORTADAS
DLL_EXPORT_API bool esBisiesto(int year);
DLL_EXPORT_API bool esFeriado(int day, int month, int year);
DLL_EXPORT_API bool validarFecha(int dia, int mes, int anio);
DLL_EXPORT_API bool esNumero(const char* texto);
DLL_EXPORT_API bool validarYear(int year);
DLL_EXPORT_API bool validarMes(int mes);
DLL_EXPORT_API bool validarDia(int dia, int mes, int anio);

#endif // VALIDACION_FECHA_H