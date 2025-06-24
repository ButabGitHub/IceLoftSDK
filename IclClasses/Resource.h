#pragma once
#include <iostream>

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <stdexcept>

class Resource {
public:
	virtual ~Resource() = default;
	std::string read_file(const char* path);
	//virtual std::unique_ptr<Resource> Load(const char* path) = 0;
};

