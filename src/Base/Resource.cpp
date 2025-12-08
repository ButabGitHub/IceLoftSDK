#include "Resource.h"

std::string Resource::read_file(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::string error_msg = "Failed to open file: ";
        error_msg += path;
        throw std::runtime_error(error_msg);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
