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
			if (m_state == WorkerState::EWorking) {
				m_task();
				m_state = WorkerState::EIdle;
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
