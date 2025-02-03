#include "LoggingService.h"

void LoggingService::processInput(Logger &logger, std::istream &input) {
    std::string line;
    std::vector<std::future<void> > futures;
    try {
        while (std::getline(input, line)) {
            // читаем ввод построчно
            if (line.empty()) continue; // пропускаем пустые строки
            // разбираем ввод пользователя и получаем структуру ParsedTask
            auto result = Parser::ParseUserInput(Utils::trimSpaces(line), logger.getDefaultPriority());
            // передаем команду в обработчик
            bool isExitCmd = CommandHandler::handle(result, logger, futures);
            if (isExitCmd) {
                break;
            }
        }
    } catch (const std::runtime_error &e) {
        std::cerr << "Ошибка " << e.what() << std::endl;
    }
    // ждем завершения всех тредов
    for (auto &future: futures) {
        future.get();
    }
}


void LoggingService::run(const ProgramConfig &config) {
    Logger logger(config.logFile, config.defaultPriority);
    processInput(logger, std::cin);
}
