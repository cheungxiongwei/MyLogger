#include "logger/logger.h"

#include <thread>

int main() {

    Logger log;
    log.init(Logger::Level::Verbose, "ABC.log", 1024 * 1024 * 5);

    std::vector<std::jthread> task;
    for (int i = 0; i < 10; i++) {
        task.push_back(std::jthread([&log = log,number = i]() {
            for (auto index = 0; index < 100; index++) {
                log.println("number {} index {}",number,index);
            }
        }));
    }

    return 0;
}
