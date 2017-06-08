#include "CatalogueUtils.h"

#include <iostream>
#include <fstream>

string getTextFromFile(const std::string& filename)
{
	std::string code;
	std::ifstream stream(filename, std::ios::in);

	if (!stream.is_open()) {
		cerr << "Impossible to open : " << filename << ". Are you in the right directory ?" << endl;
		return code;
	}
	std::string line = "";
	while (getline(stream, line)) {
		code += "\n" + line;
	}
	stream.close();
	return code;
}

string getFilename(const std::string& filePath)
{
	size_t lastSlash = filePath.find_last_of('/');
	size_t lastPoint = filePath.find_last_of('.');

	if (lastPoint == string::npos) filePath.size();

	if (lastSlash == string::npos) lastSlash = 0;
	else lastSlash += 1; // +1 for keeping the last '/'

	return filePath.substr(lastSlash, lastPoint - lastSlash);
}

string getDirectoryOfPath(const std::string& path) {
	size_t lastSlash = path.find_last_of('/');
	if (lastSlash == string::npos)
		return path;
	else
		return  path.substr(0, lastSlash + 1); // +1 for keeping the last '/'
}