#include "CommandHandler.h"


void CommandHandler::handleLogMessage(const Parser::ParsedTask &task, Logger &logger,
                                      std::vector<std::future<void> > &futures) {
    auto future = std::async(std::launch::async, [&logger, task]() {
        try {
            logger.writeMessage(task.message, task.priority);
        } catch (const std::runtime_error &e) {
            std::cerr << "Ошибка при записи в журнал: " << e.what() << std::endl;
        }
    });
    futures.push_back(std::move(future));
}

void CommandHandler::handlePriorityChange(const Parser::ParsedTask &task, Logger &logger) {
    auto future = std::async(std::launch::async, [&logger, task]() {
        logger.changeDefaultPriority(task.priority);
    });

    future.get();
    std::cout << "Приоритет успешно обновлен!\n";
}

void CommandHandler::handleInvalidCommand() {
    std::cerr << "Ошибка: Неверный формат ввода\n";
}

bool CommandHandler::handle(const Parser::ParsedTask &task, Logger &logger,
                                         std::vector<std::future<void> > &futures) {
    switch (task.commandName) {
        case Parser::CmdType::LOG_MESSAGE: {
            handleLogMessage(task, logger, futures);
            return false;
        }
        case Parser::CmdType::SET_PRIORITY: {
            handlePriorityChange(task, logger);
            return false;
        }
        case Parser::CmdType::INVALID: {
            handleInvalidCommand();
            return false;
        }
        case Parser::CmdType::EXIT: {
            return true;
        }
        default: {
            return false;
        }
    }
}