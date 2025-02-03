#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <optional>

enum class Priority {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2,
    UNKNOWN = -1
};

class Logger {
public:
    Logger(const std::string &filename, Priority priority);

     // записывает сообщение в лог файл
    void writeMessage(const std::string &logMessage, const Priority &logPriority);

     // изменяет приоритет по умолчанию
    void changeDefaultPriority(Priority priority);

     // возвращает текущий приоритет по умолчанию
    Priority getDefaultPriority();

    ~Logger();

private:
    Priority defaultPriority;  // приоритет по умолчанию
    std::ofstream logFileStream; // поток вывода в файл
    std::mutex logMutex;       // мьютекс для потоков
    // преобразует приоритет в строку
    std::string priorityToString(Priority priority);
    // возвращает текущее время
    std::string getCurrentTime();
};

#endif //LOGGER_H