#pragma once

#include "ini.h"

typedef std::vector<std::string>::const_iterator str_iter;

/// <summary>
/// Parses pre-structured ini file (containing headers from 1...n)
/// </summary>
/// <param name="url">File to parse, relative to binary</param>
/// <param name="callback">Function to call with the inner structure</param>
/// <param name="max">Stops looking for more data at this value</param>
void parseIni(const std::string& url, void (*callback)(const int, const std::string, mINI::INIStructure), const int max = 64)
{
	mINI::INIFile file(url);
	mINI::INIStructure ini;
	file.read(ini);

	int ID = 1;
	while (ID < max) {
		std::string IDString = std::to_string(ID);
		if (ini.has(IDString)) {
			callback(ID, IDString, ini);
		}
		ID++;
	}
}

std::vector<std::string> parseArguments(const std::string& input, const char delimiter)
{
	std::stringstream test(input);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, delimiter))
	{
		seglist.push_back(segment);
	}

	return seglist;
}

std::vector<std::string> parseArguments(const std::string& input)
{
	return parseArguments(input, ' ');
}