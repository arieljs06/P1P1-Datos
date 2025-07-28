#include "Utils.h"
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <vector>

using namespace BackupUtils;

std::string BackupUtils::ejecutarComandoPython(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Error al ejecutar comando Python");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string BackupUtils::codificarBase64(const std::string& input) {
    std::string cmd = "python -c \"import base64; print(base64.b64encode(open(0,'rb').read()).decode('utf-8'))\"";
    
    FILE* pipe = popen(cmd.c_str(), "w");
    if (!pipe) {
        throw std::runtime_error("Error al ejecutar comando Python para base64");
    }
    fwrite(input.data(), 1, input.size(), pipe);
    int status = pclose(pipe);
    
    if (status != 0) {
        throw std::runtime_error("Error en la codificación base64");
    }
    
    std::string output = ejecutarComandoPython("echo \"" + input + "\" | " + cmd);
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    return output;
}

std::string BackupUtils::decodificarBase64(const std::string& input) {
    std::string cmd = "python -c \"import base64; import sys; print(base64.b64decode(sys.argv[1]).decode('latin-1'))\" \"" + input + "\"";
    std::string output = ejecutarComandoPython(cmd);
    
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    return output;
}

std::string BackupUtils::extraerCampoJSON(const std::string& json, const std::string& campo) {
    size_t start_pos = json.find("\"" + campo + "\":") + campo.length() + 3;
    if (start_pos == std::string::npos) return "";
    
    start_pos = json.find_first_of("\"", start_pos) + 1;
    size_t end_pos = json.find("\"", start_pos);
    
    if (end_pos == std::string::npos) return "";
    
    return json.substr(start_pos, end_pos - start_pos);
}