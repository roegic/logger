#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "../Parser/Parser.h"
#include <iostream>
#include <thread>
#include <future>

class CommandHandler {
public:
    // обрабатывает все команды, возвращает true если пользователь ввел команду exit, false иначе
    static bool handle(const Parser::ParsedTask &task, Logger &logger, std::vector<std::future<void>>& futures);

private:
    // обрабатывает команду логирования сообщения
    static void handleLogMessage(const Parser::ParsedTask &task, Logger &logger, std::vector<std::future<void>>& futures);
    // обрабатывает команду изменения приоритета
    static void handlePriorityChange(const Parser::ParsedTask& task, Logger& logger);
    // обрабатывает невалидную команду
    static void handleInvalidCommand();
};

#endif //COMMANDHANDLER_H