#pragma once

#include <fstream>
#include <string>
#include <iostream>

std::string read_file(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        throw std::runtime_error("Error abriendo archivo");
    }
    
    return std::string((std::istreambuf_iterator<char>(ifs)),
                       std::istreambuf_iterator<char>());
}