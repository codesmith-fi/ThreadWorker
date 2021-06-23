/**
 ThreadWorker (Worker.h)

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
#ifndef __WORKER_H_DEFINED__
#define __WORKER_H_DEFINED__

#include <cstdint>
#include <chrono>
#include <thread>
#include <string>
#include <mutex>
#include <functional>
#include "DebugLogger.h"

class Worker
{
public: // enums
	enum class WorkerState : uint16_t {
		ECreated,
		EIdle,
		EWorking,
		EExiting
	};

	static std::string WorkerStateToString(const WorkerState& _state) {
		std::string statestr;
		switch(_state) {
			case WorkerState::EIdle: 
			{
				statestr = "Idle";
				break;
			}
			case WorkerState::EWorking:
			{
				statestr = "Working";
				break;
			}
			case WorkerState::EExiting:
			{
				statestr = "Exiting";
				break;
			}
			default:
			{
				statestr = "Created";
				break;
			}
		}
		return statestr;
	}

	static const uint16_t KThreadWaitSeconds = 1;

public:
	Worker() : Worker(0) { };
	Worker(uint16_t _id) : m_thread{}, m_state(WorkerState::ECreated), 
	m_id(_id), m_running(false), m_usefunction(false) {
		LOG_INFO() << "Created Worker number: " << m_id;
	};
	virtual ~Worker() { 
		LOG_INFO() << "Destroyed Worker number: " << m_id;
	};

public:
	std::uint16_t id() const { return m_id; };
	const WorkerState& state() const { return m_state; }
	void start() {
		LOG_INFO() << "Starting worker: " << m_id;
		m_running = true;
		m_state = WorkerState::EIdle;
		m_thread = std::thread([this]() { doRun(); });
	};
	void stop() {
		LOG_INFO() << "Stopping worker: " << m_id;
		if (m_running) {
			m_running = false;
			m_state = WorkerState::EExiting;
			m_thread.join();
		}
	};
	void work() {
		const std::lock_guard<std::mutex> lock(m_mutex);
		if (m_state == WorkerState::EIdle) {
			m_usefunction = false;
			m_state = WorkerState::EWorking;
		}
	};
	void work(std::function<void(void)> task) {
		const std::lock_guard<std::mutex> lock(m_mutex);
		if (m_state == WorkerState::EIdle) {
			m_task = task;
			m_usefunction = true;
			m_state = WorkerState::EWorking;
		}
	}

protected:
	virtual void doWork() {
		LOG_INFO() << "Default worker: " << m_id << " work ended\n";	
	}

private:
	void doRun() {
		while(m_running) {	
			auto timep1 = std::chrono::system_clock::now();
			if (m_state == WorkerState::EWorking) {
				LOG_INFO() << "Worker " << m_id << " starting task\n";
				if(m_usefunction) {
					m_task();
				} else {
					doWork();
				}
				m_state = WorkerState::EIdle;
				auto timep2 = std::chrono::system_clock::now();
				std::chrono::duration<float> elapsed(timep2 - timep1);
				LOG_INFO() << "Worker [" << m_id << "] task complete, time spent: " << std::to_string(elapsed.count());
			}
			else {
				LOG_INFO() << "Worker: " << m_id << " @" << Worker::WorkerStateToString(m_state);
				std::this_thread::sleep_for(std::chrono::seconds(KThreadWaitSeconds));
			}
		}
	}

private:
	std::thread m_thread;
	std::mutex m_mutex;
	std::function<void(void)> m_task;
	WorkerState m_state;
	std::uint16_t m_id;
	bool m_running;
	bool m_usefunction;
};

#endif //  __WORKER_H_DEFINED__
