//
// Created by Henrique Silva on 13/03/2023.
//

#include <fstream>

#include "../include/utils.h"

std::vector<std::string> utils::file::readFile(const std::string &filePath) {
    std::ifstream file(filePath);

    if (!file.is_open())
        return {};

    file.ignore(INT32_MAX, '\n');

    std::vector<std::string> lines;

    std::string line;

    while (std::getline(file, line))
        lines.push_back(line);
}