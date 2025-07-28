#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace BackupUtils {
    std::string ejecutarComandoPython(const std::string& cmd);
    std::string codificarBase64(const std::string& input);
    std::string decodificarBase64(const std::string& input);
    std::string extraerCampoJSON(const std::string& json, const std::string& campo);
}

#endif // UTILS_H