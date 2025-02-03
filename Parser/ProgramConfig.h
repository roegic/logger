#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <string>
#include "../lib/Logger.h"
struct ProgramConfig {
    std::string logFile;
    Priority defaultPriority;
};

#endif