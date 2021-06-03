/**
 * Simple C++ concurrent test with threads
 * Author: Erno Pakarinen, codesmith.fi@gmail.com
 */
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include "Worker.h"
#include "DebugLogger.h"

static const uint16_t KNumberOfWorkers = 10;
static const std::string KCommandEnd = "quit";

int main()
{
    std::cout << "ThreadWorkerDemo 0.1.0 by Erno Pakarinen\n";
    std::cout << "codesmith.fi@gmail.com\n\n";
    std::cout << "Enter the number of workers: " << std::flush;
    int16_t workerCount = 0;
    std::cin >> workerCount;
    workerCount = workerCount != 0 ? workerCount : KNumberOfWorkers;

    std::cout << "\nCreating [" << workerCount << "] workers\n";
    std::vector<std::shared_ptr<Worker>> my_workers;
    for (uint16_t i = 0; i < workerCount; ++i) {
        my_workers.emplace_back(std::make_shared<Worker>(i));
    }

    for (auto w = my_workers.begin(); w != my_workers.end(); ++w) {
        w->get()->start();
    }

    std::string command;
    while (command.compare(KCommandEnd)!=0) {
        std::cout << "CMD: " << std::flush;
        std::cin >> command;
        std::cout << "Given command: " << command << "\n";
    }
    std::cout << "Terminating\n";

    for (auto w = my_workers.begin(); w != my_workers.end(); ++w) {
        w->get()->stop();
    }

    LOG_INFO() << "Ending";
}
