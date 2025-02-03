#ifndef PARSERESULT_H
#define PARSERESULT_H

#include <string>
#include "ProgramConfig.h"
#include "../Utils/Utils.h"


namespace Parser {
    enum class CmdType {
        LOG_MESSAGE,
        SET_PRIORITY,
        INVALID,
        EXIT
    };

    struct ParsedTask {
        CmdType commandName;
        std::string message;
        Priority priority;
    };

    inline static const std::string SET_PRIORITY_CMD = "set_default_priority ";
    inline static const std::string EXIT_CMD = "exit";

    // парсит пользовательский ввод для определения типа команды и её параметров
    ParsedTask ParseUserInput(const std::string& inputLine, const Priority& defaultPriority);
    // парсит аргументы командной строки для получения файла и приоритета по умолчанию
    ProgramConfig ParseCommandLineArguments(int argc, char* argv[]);

}
#endif