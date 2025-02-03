#include "LoggingService/LoggingService.h"
#include "Parser/Parser.h"
#include "Parser/ProgramConfig.h"

int main(int argc, char* argv[]) {
    ProgramConfig config = Parser::ParseCommandLineArguments(argc, argv);
    LoggingService::run(config);
    return 0;
}