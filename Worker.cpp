#include "Worker.h"

#include <iostream>
#include <thread>
#include <chrono>
#include "DebugLogger.h"

Worker::Worker() : Worker(0)
{
}

Worker::Worker(uint16_t id) : m_thread{}, m_state(WorkerState::ECreated), m_running(false), m_id(id)
{
	LOG_INFO() << "Created Worker number: " << m_id;
}

Worker::~Worker()
{
	LOG_INFO() << "Destroyed Worker number: " << m_id;
}

void Worker::start() 
{
	LOG_INFO() << "Starting worker: " << m_id;
	m_running = true;
	m_state = WorkerState::EIdle;
	m_thread = std::thread([this]() { doRun(); });
}

void Worker::stop()
{
	LOG_INFO() << "Stopping worker: " << m_id;
	if (m_running) {
		m_running = false;
		m_thread.join();
	}
}

void Worker::doRun()
{
	while(m_running) {	
		LOG_INFO() << "Worker: " << m_id << " @" << Worker::WorkerStateToString(m_state);
		std::this_thread::sleep_for(std::chrono::seconds(KThreadWaitSeconds));
	}
}
