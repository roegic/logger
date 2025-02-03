#ifndef LOGGINGSERVICE_H
#define LOGGINGSERVICE_H

#include "../Parser/ProgramConfig.h"
#include "CommandHandler.h"


namespace LoggingService {
    // Обрабатывает ввод пользователя
    void processInput(Logger& logger, std::istream& input);

    // Запускает процесс логирования
    void run(const ProgramConfig& config);
}

#endif //LOGGINGSERVICE_H