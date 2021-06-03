/**
 * Simple C++ concurrent test with threads
 * Author: Erno Pakarinen, codesmith.fi@gmail.com
 */
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <string>
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

    // Create requested number of workers
    std::vector<std::shared_ptr<Worker>> my_workers;
    for (uint16_t i = 0; i < workerCount; ++i) {
        my_workers.emplace_back(std::make_shared<Worker>(i));
    }

    // Start worker threads
    for (auto w = my_workers.begin(); w != my_workers.end(); ++w) {
        w->get()->start();
    }

    std::string command;
    while (command.compare(KCommandEnd)!=0) {
        std::cout << "CMD: " << std::flush;
        std::cin >> command;
        std::cout << "Given command: " << command << "\n";
        int16_t steps = -1;
        try {
            steps = std::stoi(command);
        }
        catch(std::exception&) { }

        if(steps>0) {
            for (auto w = my_workers.begin(); w != my_workers.end(); ++w) {
                // Find first idle worker
                if (w->get()->state() == Worker::WorkerState::EIdle) {
                    Worker* worker = w->get();
                    auto lfunc = [worker, steps]() {
                        for (auto i = 0; i < steps && worker->state()==Worker::WorkerState::EWorking; ++i) {
                            LOG_INFO() << "Lambda worker: " << worker->id() << " at work, step: " << i;
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }
                    };
                    w->get()->work(lfunc);
                    break;
                }
            }
        }
    }
    std::cout << "Terminating\n";

    // Stop the workers, this will do thread join()
    for (auto w = my_workers.begin(); w != my_workers.end(); ++w) {
        w->get()->stop();
    }

    LOG_INFO() << "Ending";
}
