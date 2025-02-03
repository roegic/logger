#include "TestUtils.h"

// тестирует парсинг аргументов командной строки
void testCommandLineParsing() {
    // тестирует корректные аргументы
    {
        char *argv[] = {(char *) "./program", (char *) "test_cmd_line.log", (char *) "MEDIUM"};
        int argc = 3;
        ProgramConfig config = Parser::ParseCommandLineArguments(argc, argv);
        assert(config.logFile == "test_cmd_line.log");
        assert(config.defaultPriority == Priority::MEDIUM);
    }
    // тестирует корректные аргументы в нижнем регистре
    {
        char *argv[] = {(char *) "./program", (char *) "test_cmd_line.log", (char *) "medium"};
        int argc = 3;
        ProgramConfig config = Parser::ParseCommandLineArguments(argc, argv);
        assert(config.logFile == "test_cmd_line.log");
        assert(config.defaultPriority == Priority::MEDIUM);
    }

    // тестирует невалидный приоритет
    {
        char *argv[] = {(char *) "./program", (char *) "test_cmd_line.log", (char *) "INVALID"};
        int argc = 3;
        try {
            ProgramConfig config = Parser::ParseCommandLineArguments(argc, argv);
            assert(false);
        } catch (const std::runtime_error &e) {
            assert(true);
        }
    }

    //тестирует отсутствующие аргументы
    {
        char *argv[] = {(char *) "./program"};
        int argc = 1;
        try {
            ProgramConfig config = Parser::ParseCommandLineArguments(argc, argv);
            assert(false);
        } catch (const std::runtime_error &e) {
            assert(true);
        }
    }

    std::cout << "Command line parsing tests passed!" << std::endl;
}

// тестирует разбор пользовательского ввода
void testUserInputParsing() {
    // тестирует сообщение с приоритетом по умолчанию
    {
        Parser::ParsedTask task = Parser::ParseUserInput("Test message", Priority::MEDIUM);
        assert(task.commandName == Parser::CmdType::LOG_MESSAGE); // проверяет тип команды
        assert(task.message == "Test message"); // проверяет сообщение
        assert(task.priority == Priority::MEDIUM); // проверяет приоритет
    }
    // тестирует сообщение с указанием приоритета
    {
        Parser::ParsedTask task = Parser::ParseUserInput("Test message HIGH", Priority::LOW);
        assert(task.commandName == Parser::CmdType::LOG_MESSAGE);
        assert(task.message == "Test message");
        assert(task.priority == Priority::HIGH);
    }

    // тестирует сообщение с приоритетом в нижнем регистре
    {
        Parser::ParsedTask task = Parser::ParseUserInput("Test message low", Priority::LOW);
        assert(task.commandName == Parser::CmdType::LOG_MESSAGE);
        assert(task.message == "Test message");
        assert(task.priority == Priority::LOW);
    }

    // тестирует установку приоритета по умолчанию
    {
        Parser::ParsedTask task = Parser::ParseUserInput("set_default_priority HIGH", Priority::LOW);
        assert(task.commandName == Parser::CmdType::SET_PRIORITY);
        assert(task.priority == Priority::HIGH);
    }

    // тестирует установку приоритета по умолчанию с некорректным значением
    {
        Parser::ParsedTask task = Parser::ParseUserInput("set_default_priority error", Priority::LOW);
        assert(task.commandName == Parser::CmdType::INVALID);
        assert(task.priority == Priority::UNKNOWN);
    }

    // тестирует установку приоритета по умолчанию с невалидным приоритетом
    {
        Parser::ParsedTask task = Parser::ParseUserInput("set_default_priority INVALID", Priority::LOW);
        assert(task.commandName == Parser::CmdType::INVALID);
        assert(task.priority == Priority::UNKNOWN);
    }
    {
        Parser::ParsedTask task = Parser::ParseUserInput("exit", Priority::LOW);
        assert(task.commandName == Parser::CmdType::EXIT);
    }

    std::cout << "User input parsing tests passed!" << std::endl;
}

// тестирует обработчик команд
void testCommandHandler() {
    const std::string filename = "test_command_handler.log";
    deleteFile(filename);

    Logger logger(filename, Priority::MEDIUM);
    std::vector<std::future<void> > futures;

    // тестирует обработку лог сообщения
    {
        Parser::ParsedTask task = {Parser::CmdType::LOG_MESSAGE, "Test log message", Priority::HIGH};
        CommandHandler::handle(task, logger, futures);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::vector<std::string> logContent = readFileLines(filename);
        if (logContent.size() != 1) {
            assert(false);
        }
        assert(logContent[0].find("Test log message") != std::string::npos);
        assert(logContent[0].find("HIGH") != std::string::npos);
    }

    // тестирует обработку лог сообщения с низким приоритетом
    {
        Parser::ParsedTask task = {Parser::CmdType::LOG_MESSAGE, "Test log message LOW", Priority::LOW};
        CommandHandler::handle(task, logger, futures);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::vector<std::string> logContent = readFileLines(filename);
        if (logContent.size() != 1) {
            assert(false);
        }
        assert(logContent[0].find("Test log message LOW") == std::string::npos);
    }
    // тестирует обработку изменения приоритета
    {
        Parser::ParsedTask task = {Parser::CmdType::SET_PRIORITY, "", Priority::LOW};
        CommandHandler::handle(task, logger, futures);
        assert(logger.getDefaultPriority() == Priority::LOW);
    }

    // тестирует обработку команды exit
    {
        Parser::ParsedTask task = {Parser::CmdType::EXIT, "", Priority::LOW};
        bool isExit = CommandHandler::handle(task, logger, futures);
        assert(isExit == true);
    }

    deleteFile(filename);
    std::cout << "Command handler tests passed!" << std::endl;
}

// тестирует сервис логирования
void testLoggingService() {
    const std::string filename = "test_logging_service.log";
    deleteFile(filename);
    // тестирует простое логгирование сообщения
    {
        Logger logger(filename, Priority::MEDIUM);
        std::stringstream ss;
        ss << "Test message 1\n" << "Test message 2 HIGH\n" << "set_default_priority LOW\nTest message 3\n";
        LoggingService::processInput(logger, ss);

        std::vector<std::string> lines = readFileLines(filename);
        if (lines.size() != 3) {
            assert(false);
        }
        const std::string &logContent1 = lines[0];
        const std::string &logContent2 = lines[1];
        const std::string &logContent3 = lines[2];

        assert(logContent1.find("Test message 1") != std::string::npos);
        assert(logContent2.find("Test message 2") != std::string::npos);
        assert(logContent3.find("Test message 3") != std::string::npos);
        clearLogFile(filename);
    }
    // тестирует изменение приоритета
    {
        Logger logger(filename, Priority::MEDIUM);
        std::stringstream ss;
        ss << "Test message 1\n" << "Test message 2 HIGH\n" << "set_default_priority LOW\nTest message 3\nexit\n";
        LoggingService::processInput(logger, ss);

        assert(logger.getDefaultPriority() == Priority::LOW);
        clearLogFile(filename);
    }
    // тестирует пустой ввод
    {
        Logger logger(filename, Priority::MEDIUM);
        std::stringstream ss;
        LoggingService::processInput(logger, ss);
        std::vector<std::string> lines = readFileLines(filename);
        assert(lines.empty());
        clearLogFile(filename);
    }
    // тестирует сообщения с лишними пробелами в начале и конце
    {
        Logger logger(filename, Priority::MEDIUM);
        std::stringstream ss;
        std::string string_with_extra_spaces = "   Test message   \n";
        ss << string_with_extra_spaces;
        LoggingService::processInput(logger, ss);
        std::vector<std::string> lines = readFileLines(filename);

        if (lines.size() != 1) {
            assert(false);
        }
        assert(lines[0].find(string_with_extra_spaces) == std::string::npos);
        clearLogFile(filename);
    }
    deleteFile(filename);
    std::cout << "Logging service tests passed!" << std::endl;
}

int main() {
    testCommandLineParsing();
    testUserInputParsing();
    testCommandHandler();
    testLoggingService();

    std::cout << "All app tests passed!" << std::endl;
    return 0;
}
