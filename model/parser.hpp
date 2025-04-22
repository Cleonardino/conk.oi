#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "map.hpp"

std::vector<std::string> split(const std::string& s, char delimiter);
Map parse_csv(std::string filepath);

#endif