CC = gcc
CFLAGS = -Wall -std=c++17 -fPIC

# Цели сборки
APP_TARGET = app
LIB_TARGET = liblogger.so
TEST_LOGGER_TARGET = test_logger
TEST_APP_TARGET = test_app
TEST_UTILS_TARGET = test_utils

# Исходные файлы
APP_SRCS = main.cpp \
    LoggingService/CommandHandler.cpp \
    LoggingService/LoggingService.cpp \
    Parser/Parser.cpp \
    Utils/Utils.cpp

LIB_SRCS = lib/Logger.cpp

TEST_LOGGER_SRC = tests/testLogger.cpp
TEST_APP_SRC = tests/testApp.cpp
TEST_UTILS_SRC = tests/TestUtils.cpp

# Заголовочные файлы
TEST_INCLUDES = tests/TestUtils.h

INCLUDES = LoggingService/CommandHandler.h \
    LoggingService/LoggingService.h \
    Parser/Parser.h \
    Parser/ProgramConfig.h \
    Utils/Utils.h \
    lib/Logger.h

# Объектные файлы
APP_OBJS = $(APP_SRCS:.cpp=.o)
LIB_OBJS = $(LIB_SRCS:.cpp=.o)
TEST_LOGGER_OBJS = $(TEST_LOGGER_SRC:.cpp=.o)
TEST_APP_OBJS = $(TEST_APP_SRC:.cpp=.o)
TEST_UTILS_OBJS = $(TEST_UTILS_SRC:.cpp=.o)
APP_TEST_SRCS = LoggingService/CommandHandler.cpp \
    LoggingService/LoggingService.cpp \
    Parser/Parser.cpp \
    Utils/Utils.cpp

TEST_APP_OBJS_WITHOUT_MAIN = $(APP_TEST_SRCS:.cpp=.o)

# Сборка приложения
$(APP_TARGET): $(APP_OBJS) $(LIB_TARGET)
	$(CC) $(CFLAGS) -L. -llogger $^ -o $@ -lstdc++

%.o: %.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -x c++ -c $< -o $@

# Сборка динамической библиотеки
$(LIB_TARGET): $(LIB_OBJS)
	$(CC) -shared $^ -o $@ -lstdc++

# Сборка файлов для тестов
$(TEST_LOGGER_TARGET): $(LIB_OBJS) $(TEST_LOGGER_OBJS) $(TEST_UTILS_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lstdc++

$(TEST_APP_TARGET): $(LIB_TARGET) $(TEST_APP_OBJS) $(TEST_APP_OBJS_WITHOUT_MAIN) $(TEST_UTILS_OBJS)
	$(CC) $(CFLAGS) -L. -llogger $^ -o $@ -lstdc++

$(TEST_UTILS_TARGET): $(TEST_UTILS_OBJS) $(TEST_INCLUDES)
	$(CC) $(CFLAGS) -x c++ -c $^ -o $@ -lstdc++

clean:
	rm -f $(APP_OBJS) $(LIB_OBJS) $(APP_TARGET) $(LIB_TARGET) $(TEST_LOGGER_OBJS) $(TEST_APP_OBJS) $(TEST_LOGGER_TARGET) $(TEST_APP_TARGET) $(TEST_APP_OBJS_WITHOUT_MAIN)  $(TEST_UTILS_OBJS) $(TEST_UTILS_TARGET)

# запуск тестов
test: $(TEST_LOGGER_TARGET) $(TEST_APP_TARGET)
	./$(TEST_LOGGER_TARGET)
	./$(TEST_APP_TARGET)

.PHONY: clean test
