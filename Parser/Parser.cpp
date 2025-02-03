#include "Parser.h"

Parser::ParsedTask Parser::ParseUserInput(const std::string& inputLine, const Priority& defaultPriority) {
    // Проверяем, является ли введенная строка командой выхода
    if (inputLine == EXIT_CMD) {
        return {CmdType::EXIT, "exit", defaultPriority};
    }
    // Проверяем начинается ли строка с команды установки приоритета
    if (Utils::startsWith(inputLine, SET_PRIORITY_CMD)) {
        // Извлекаем строку приоритета и разбираем её
        const std::string priorityStr = Utils::formatPriority(inputLine.substr(SET_PRIORITY_CMD.length()));
        const Priority newPriority = Utils::parsePriority(priorityStr);

        CmdType newCmd = CmdType::SET_PRIORITY;
        // Если приоритет неверный устанавливаем тип команды в INVALID
        if (newPriority == Priority::UNKNOWN) {
            newCmd = CmdType::INVALID;
        }
        return {newCmd, "", newPriority};
    }

    // Ищем последний пробел, чтобы отделить сообщение и приоритет
    size_t spacePos = inputLine.find_last_of(" ");
    std::string message = inputLine;
    Priority priority = defaultPriority; // Используем приоритет по умолчанию, если он не указан во вводе

    // Если пробел найден, пытаемся распарсить приоритет
    if (spacePos != std::string::npos) {
        Priority parsedPriority = Utils::parsePriority(Utils::formatPriority(inputLine.substr(spacePos + 1)));
        // Если приоритет верен, обновляем значения
        if (parsedPriority != Priority::UNKNOWN) {
            priority = parsedPriority;
            message = inputLine.substr(0, spacePos);
        }
    }
    return { Parser::CmdType::LOG_MESSAGE, message, priority};
}

ProgramConfig Parser::ParseCommandLineArguments(int argc, char* argv[]) {
    if (argc < 3) {
        throw std::runtime_error("Ошибка: Неверный формат ввода. Использование: ./app <имя_файла_журнала> <уровень_приоритетности>" );
    }
    ProgramConfig config;
    config.logFile = argv[1]; // получаем имя файла журнала
    config.defaultPriority = Utils::parsePriority(argv[2]); // получаем и парсим приоритет по умолчанию

    // Проверяем верерн ли приоритет по умолчанию
    if (config.defaultPriority == Priority::UNKNOWN) {
        throw std::runtime_error("Ошибка: Неверный уровень приоритетности. Уровни приоритетности: LOW, MEDIUM, HIGH");
    }
    return config;
}