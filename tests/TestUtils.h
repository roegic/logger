#ifndef TESTUTILS_H
#define TESTUTILS_H

#include "../LoggingService/LoggingService.h"
#include "../Parser/Parser.h"
#include "../Parser/ProgramConfig.h"
#include "../lib/Logger.h"
#include "../LoggingService/CommandHandler.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <thread>
#include <filesystem>
#include <map>

inline std::map<Priority, std::string> priorityToString = {
    {Priority::LOW, "LOW"},
    {Priority::MEDIUM, "MEDIUM"},
    {Priority::HIGH, "HIGH"}
};

// читает строки из файла
std::vector<std::string> readFileLines(const std::string &filename);

// удаляет файл
void deleteFile(const std::string &filename);

// проверяет, содержит ли строка подстроку
bool contains(const std::string &line, const std::string &substr);

// очищает содержимое лог файла
void clearLogFile(const std::string &logFile);
#endif //TESTUTILS_H
