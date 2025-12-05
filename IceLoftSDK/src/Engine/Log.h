#pragma once

#include <iostream>

#define LOG(x) std::cout << x << "\n";
#define WARN(x) std::cout << "\x1b[33m" << x << "\033[0m\n";
#define ERROR(x) std::cerr << "\x1b[31m" << x << "\033[0m\n";
