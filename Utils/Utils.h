#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <algorithm>
#include "../lib/Logger.h"

namespace Utils {
    bool startsWith(const std::string& str, const std::string& prefix);

    // преобразует строку в enum Priority
    Priority parsePriority(std::string priority);

    // форматирует строку приоритета в верхний регистр
    std::string formatPriority(const std::string& priority);

    // убирает лишние пробелы в начале и конце строки
    std::string trimSpaces(const std::string& line);
}

#endif