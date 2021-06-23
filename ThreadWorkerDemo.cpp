/**
 A simple example of a class that encapsulates a thread and a worker which
 works within that thread.

 See the blog article at https://codesmith.hashnode.dev/ for more info
 on this tutorial.

 By far this is NOT perfect. As all persons we all are learning new things
 I want to share everything I have learned in the past to anyone
 willing to come to the industry or learn these things just for hobby!

 For an example, please see the provided example in
 ThreadWorkerDemo.cpp

 You can pass any task with a lambda to a free worker as showed.

 I love programming, I work in the field and it is my hobby!

 I hope you will contact me.

 If you find this thing useful, I would not mind of you to get in to
 contact with a fellow and old time programmer <3

LICENSE - MIT
Copyright (c) 2012-2021 Erno Pakarinen (codesmith-fi@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
