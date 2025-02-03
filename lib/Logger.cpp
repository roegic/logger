#include "Logger.h"

Logger::Logger(const std::string &filename, Priority priority) : defaultPriority(priority) {

    logFileStream.open(filename, std::ios::out | std::ios::app); // открываем лог файл для записи

    if (!logFileStream.is_open()) {
        throw std::runtime_error("Ошибка при открытии файла: " + filename);
    }
}

void Logger::writeMessage(const std::string &logMessage, const Priority &logPriority) {
    std::lock_guard<std::mutex> lock(logMutex);

    if (!logFileStream.is_open()) {
        throw std::runtime_error("Невозможно открыть файл");
    }

    if (static_cast<int>(logPriority) >= static_cast<int>(defaultPriority)) {
        // если приоритет сообщения больше или равен приоритету по умолчанию, то записываем в лог
        logFileStream << getCurrentTime() << " | " << priorityToString(logPriority) << " | " << logMessage << std::endl;
    }
}

void Logger::changeDefaultPriority(const Priority priority) {
    std::lock_guard<std::mutex> lock(logMutex);
    defaultPriority = priority;
}

Priority Logger::getDefaultPriority() {
    std::lock_guard<std::mutex> lock(logMutex);
    return defaultPriority;
}

Logger::~Logger() {
    if (logFileStream.is_open()) {
        logFileStream.close();
    }
}

std::string Logger::priorityToString(const Priority priority) {
    switch (priority) {
        case Priority::LOW:
            return "LOW";
        case Priority::MEDIUM:
            return "MEDIUM";
        case Priority::HIGH:
            return "HIGH";
        default:
            return "UNKNOWN";
    }
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_time);

    std::stringstream ss;
    ss << std::put_time(&local_tm, "%d-%m-%Y %H:%M:%S");
    return ss.str();
}
