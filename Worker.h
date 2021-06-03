#ifndef __WORKER_H_DEFINED__
#define __WORKER_H_DEFINED__

#include <cstdint>
#include <chrono>
#include <thread>
#include <string>
#include <mutex>

class Worker
{
public: // enums
	enum class WorkerState : uint16_t {
		ECreated,
		EIdle,
		EWorking,
		EExiting
	};

	static std::string WorkerStateToString(const WorkerState& state) {
		std::string statestr;
		switch (state) {
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
	Worker();
	Worker(uint16_t id);
	virtual ~Worker();

public:
	std::uint16_t id() const { return m_id; };
	const WorkerState& state() const { return m_state; }
	void start();
	void stop();
	void work();

protected:
	virtual void doWork();

private:
	void doRun();

private:
	std::thread m_thread;
	std::mutex m_mutex;
	WorkerState m_state;
	bool m_running;
	std::uint16_t m_id;
};

#endif //  __WORKER_H_DEFINED__
