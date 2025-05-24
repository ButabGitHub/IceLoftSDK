#pragma once
#include <iostream>

#include <iostream>
#include <fstream>
#include <sstream>

class Resource {
public:
	virtual ~Resource() = default;
	virtual Resource Load(const char* path);
};

