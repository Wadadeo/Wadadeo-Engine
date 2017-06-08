#pragma once

#include <list>
#include <string>

using namespace std;

string	getTextFromFile(const std::string& filename);
string	getFilename(const std::string& filePath);
string	getDirectoryOfPath(const std::string& path);