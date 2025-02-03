#include "TestUtils.h"

// тестирует инициализацию логгера
void testLoggerInitialization() {
    const std::string logFile = "test_init.log";
    deleteFile(logFile);
    {
        Logger logger(logFile, Priority::MEDIUM);
        assert(std::filesystem::exists(logFile)); // проверяет, что файл создан
        assert(logger.getDefaultPriority() == Priority::MEDIUM); // проверяет, что приоритет установлен правильно
    }

    // тестирует невозможность открытия файла
    try {
        Logger logger("/invalidpath/log.log", Priority::LOW);
        assert(false);
    } catch (const std::runtime_error& e) {
        assert(true);
    }

    std::cout << "Initialization tests passed!" << std::endl;
    deleteFile(logFile);
}

// тестирует простую запись в лог
void testLoggerBasic() {
    std::string logFile = "test_log_basic.txt";
    deleteFile(logFile);

    Priority defaultPriority = Priority::LOW;
    Logger logger(logFile, defaultPriority);



    logger.writeMessage("Test message 1", defaultPriority);

    std::ifstream file(logFile);
    std::string line;
    std::getline(file, line);
    // проверяет, что сообщение записано в файл
    assert(contains(line, "Test message 1"));
    assert(contains(line, priorityToString[defaultPriority])); // проверяет, что приоритет записан в файл
    std::cout << "Test Logger Basic Passed!" << std::endl;
    deleteFile(logFile);
}

// тестирует логгирование при низком приоритете
void testLoggerLowPriority() {
    std::string logFile = "test_log_priority_medium.txt";
    clearLogFile(logFile);
    Priority defaultPriority = Priority::LOW;
    Logger logger(logFile, defaultPriority);

    logger.writeMessage("Test message 1", Priority::LOW);
    logger.writeMessage("Test message 2", Priority::MEDIUM);
    logger.writeMessage("Test message 3", Priority::HIGH);

    std::ifstream file(logFile);
    std::string line;
    bool message1_written= false;
    bool message2_written = false;
    bool message3_written = false;
    while (std::getline(file, line)) {
        if(contains(line, "Test message 1") && contains(line, priorityToString[Priority::LOW])){
            message1_written= true;
        }
        if(contains(line, "Test message 2") && contains(line, priorityToString[Priority::MEDIUM])){
            message2_written = true;
        }
        if(contains(line, "Test message 3") && contains(line, priorityToString[Priority::HIGH])){
            message3_written = true;
        }
    }
    assert(message1_written == true);
    assert(message2_written == true);
    assert(message3_written == true);

    std::cout << "Test Logger Low Priority Passed!" << std::endl;
    deleteFile(logFile);
}

// тестирует логгирование при среднем приоритете
void testLoggerMediumPriority() {
    std::string logFile = "test_log_priority_medium.txt";
    clearLogFile(logFile);
    Logger logger(logFile, Priority::MEDIUM);

    logger.writeMessage("Test message 1", Priority::LOW); // не должно быть записано
    logger.writeMessage("Test message 2", Priority::MEDIUM);
    logger.writeMessage("Test message 3", Priority::HIGH);

    std::ifstream file(logFile);
    std::string line;
    bool message1_written= false;
    bool message2_written = false;
    bool message3_written = false;
    while (std::getline(file, line)) {
        if(contains(line, "Test message 1") && contains(line, priorityToString[Priority::LOW])){
            message1_written= true;
        }
        if(contains(line, "Test message 2") && contains(line, priorityToString[Priority::MEDIUM])){
            message2_written = true;
        }
        if(contains(line, "Test message 3") && contains(line, priorityToString[Priority::HIGH])){
            message3_written = true;
        }
    }
    assert(message1_written == false);
    assert(message2_written == true);
    assert(message3_written == true);
    std::cout << "Test Logger Medium Priority Passed!" << std::endl;
    deleteFile(logFile);
}

// тестирует логгирование при высоком приоритете
void testLoggerHighPriority() {
    std::string logFile = "test_log_priority_medium.txt";
    clearLogFile(logFile);
    Logger logger(logFile, Priority::HIGH);

    logger.writeMessage("Test message 1", Priority::LOW); // не должно быть записано
    logger.writeMessage("Test message 2", Priority::MEDIUM); // не должно быть записано
    logger.writeMessage("Test message 3", Priority::HIGH);

    std::ifstream file(logFile);
    std::string line;
    bool message1_written= false;
    bool message2_written = false;
    bool message3_written = false;
    while (std::getline(file, line)) {
        if(contains(line, "Test message 1") && contains(line, priorityToString[Priority::LOW])){
            message1_written= true;
        }
        if(contains(line, "Test message 2") && contains(line, priorityToString[Priority::MEDIUM])){
            message2_written = true;
        }
        if(contains(line, "Test message 3") && contains(line, priorityToString[Priority::HIGH])){
            message3_written = true;
        }
    }
    assert(message1_written == false);
    assert(message2_written == false);
    assert(message3_written == true);
    std::cout << "Test Logger High Priority Passed!" << std::endl;
    deleteFile(logFile);
}

// тестирует изменение приоритета логгера
void testLoggerPriorityChange() {
    std::string logFile = "test_log_priority_change.txt";
    clearLogFile(logFile);
    Logger logger(logFile, Priority::HIGH);

    logger.writeMessage("Message 1", Priority::LOW); // не должно быть записано
    logger.changeDefaultPriority(Priority::LOW);
    logger.writeMessage("Message 2", Priority::LOW); // должно быть записано

    std::ifstream file(logFile);
    std::string line;
    bool message1_written = false;
    bool message2_written  = false;
    while (std::getline(file, line)) {
        if(contains(line, "Message 1")){
             message1_written = true;
        }
       if(contains(line, "Message 2")){
             message2_written = true;
        }
    }
    assert(message1_written == false);
    assert(message2_written == true);
    std::cout << "Test Logger Priority Change Passed!" << std::endl;
    deleteFile(logFile);
}

// тестирует запись в лог из нескольких потоков
void testThreads() {
    const std::string logfile = "test_threads.log";
    deleteFile(logfile);
    Logger logger(logfile, Priority::MEDIUM);
    const int numThreads = 10;
    const int numMessages = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&logger, i](){
            for (int j = 0; j < numMessages; ++j) {
                 logger.writeMessage("Message from thread " + std::to_string(i) + ", message " + std::to_string(j), Priority::MEDIUM);
             }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::vector<std::string> logContent = readFileLines(logfile);
    assert(logContent.size() == numThreads * numMessages);

    std::cout << "Multiple threads writing tests passed!" << std::endl;
    deleteFile(logfile);
}

// тестирует корректное закрытие файла
void testFileClosing() {
    const std::string logfile = "test_close.log";
    deleteFile(logfile);
    std::ifstream file(logfile);
    {
        Logger logger(logfile, Priority::LOW);
        logger.writeMessage("Test message", Priority::LOW);
    }
    assert(file.is_open() == false);
    std::cout << "File closing tests passed!" << std::endl;
    deleteFile(logfile);
}

int main() {
    testLoggerInitialization();
    testLoggerBasic();
    testLoggerLowPriority();
    testLoggerMediumPriority();
    testLoggerHighPriority();
    testLoggerPriorityChange();
    testThreads();
    testFileClosing();
    return 0;
}