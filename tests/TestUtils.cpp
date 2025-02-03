#include "TestUtils.h"

std::vector<std::string> readFileLines(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return {};
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) { // читаем файл построчно
        lines.push_back(line);
    }

    return lines;
}

void deleteFile(const std::string &filename) {
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
}

bool contains(const std::string &line, const std::string &substr) {
    return line.find(substr) != std::string::npos;
}

void clearLogFile(const std::string &logFile) {
    std::ofstream file(logFile, std::ios::trunc);
    file.close();
}
