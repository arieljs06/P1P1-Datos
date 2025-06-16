// ConfigDLL.h
#ifndef CONFIG_DLL_H
#define CONFIG_DLL_H

/// @file ConfigDLL.h
/// @brief Define macros for exporting functions from DLLs on different platforms.

#ifdef __cplusplus
#define DLL_EXPORT extern "C"
#else
#define DLL_EXPORT
#endif

#ifdef _WIN32
#ifdef BUILDING_DLL
#define DLL_EXPORT_SPEC __declspec(dllexport)
#else
#define DLL_EXPORT_SPEC __declspec(dllimport)
#endif
#else
#define DLL_EXPORT_SPEC
#endif

// Combine the two macros for use in function declarations
#define DLL_EXPORT_API DLL_EXPORT DLL_EXPORT_SPEC

#endif // CONFIG_DLL_H