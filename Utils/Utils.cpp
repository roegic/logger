#include "Utils.h"

bool Utils::startsWith(const std::string &str, const std::string &prefix) {
    return str.size() >= prefix.size() &&
           str.compare(0, prefix.size(), prefix) == 0;
}


Priority Utils::parsePriority(std::string priority) {
    priority = Utils::formatPriority(priority);
    if (priority == "LOW") {
        return Priority::LOW;
    }
    if (priority == "MEDIUM") {
        return Priority::MEDIUM;
    }
    if (priority == "HIGH") {
        return Priority::HIGH;
    }
    return Priority::UNKNOWN;
}


std::string Utils::formatPriority(const std::string &priority) {
    std::string formattedPriority = priority;
    std::transform(formattedPriority.begin(), formattedPriority.end(), formattedPriority.begin(),
                   [](const unsigned char c) {
                       return std::toupper(c);
                   });
    return formattedPriority;
}


std::string Utils::trimSpaces(const std::string &line) {
    auto start = line.begin();
    while (start != line.end() && std::isspace(*start)) {
        ++start;
    }
    auto end = line.end();
    if (start == line.end()) {
        return "";
    }

    while (end != start && std::isspace(*(end - 1))) {
        --end;
    }
    return std::string(start, end);
}