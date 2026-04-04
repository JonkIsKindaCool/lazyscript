#pragma once

#include <iostream>

#ifdef DEBUG_ENABLED
    #define TRACE(msg) std::cout << "[TRACE] " << msg << std::endl
    #define ERROR(msg) std::cout << "[ERROR] " << msg << std::endl
#else
    #define TRACE(msg)
    #define ERROR(msg)
#endif