#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "ConfigFile.h"
#pragma once

class GenerateConfigFile : public ConfigFile
{
public:
	GenerateConfigFile(std::string file_name);
};

