 /**
  * Simple console debug logger macro
  * 
  * This is a simple macro which implements a debug logger with or without time stamp.
  * The logging can be used in streams with the stream '<<' operator
  * 
  * Usage with the time stamp:
  *		LOG_INFO() << "You failed " << 20 << "Times!"
  *		LOG_WARN() << "You failed " << 20 << "Times!"
  *		LOG_ERROR() << "You failed " << 20 << "Times!"
  *
  * Usage without the time stamp:
  *		LOG_INFO() << "You failed " << 20 << "Times!"
  *		LOG_WARN() << "You failed " << 20 << "Times!"
  *		LOG_ERROR() << "You failed " << 20 << "Times!"
  *
  * Author: Erno Pakarinen
  * Email: codesmith.fi@gmail.com
  * Date: 25th of May 2021
  *
  * MIT License
  * Free to use, modify, copy, what ever. <3
  */
#ifndef __DEBUGLOGGER_DEFINED_H__
#define __DEBUGLOGGER_DEFINED_H__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <mutex>

static std::mutex g_debuglogger_mutex;

namespace codesmith
{
	namespace debug
	{
		/**
		 * Defines the supported warning levels for the macro.
		 * The level defines a warning level text displayed by the macro
		 */
		enum class DebugLogLevel : int
		{
			EINFO = 0,
			EWARN = 1,
			EERROR = 2
		};

		/**
		 * DebugLogger class definition
		 * Class has stream output operation with operator <<
		 * Constructor creates the initial output with (or without) a time stamp
		 * and the desired warning level (severity).
		 * 
		 * Default warning level is ERROR
		 * 
		 * When class instance is destructed
		 */
		class DebugLogger
		{
		private: // constants
			static constexpr const char* KTextLevelInfo = "INFO";
			static constexpr const char* KTextLevelWarn = "WARN";
			static constexpr const char* KTextLevelError = "ERROR";

		public:
			DebugLogger(DebugLogLevel severity = DebugLogLevel::EERROR, bool showtime = true) 
				: m_buffer(), m_stm{}
			{	
				g_debuglogger_mutex.lock();

				if (showtime) {
					make_time();
					m_buffer << "[" << std::put_time(&m_stm, "%F %T") << "] ";
				}

				switch(severity) {
					case DebugLogLevel::EWARN:
					{
						m_buffer << KTextLevelWarn;
						break;
					}
				case DebugLogLevel::EERROR:
					{
						m_buffer << KTextLevelError;
						break;
					}
					default:
					{
						m_buffer << KTextLevelInfo;
						break;
					}
				}
				m_buffer << ": ";
			}

			// Destructor, causes the debug info to be outputted with new line
			virtual ~DebugLogger() {
				std::cerr << m_buffer.str() << std::endl;
				g_debuglogger_mutex.unlock();
			}

			/**
			 * Stream output operator, appends a value to the debug output stream
			 */
			template <typename T>
			DebugLogger& operator<<(const T& value) {
				m_buffer << value;
				return *this;
			}
		private:
			void make_time() {
				std::time_t t_now = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
				localtime_s(&m_stm, &t_now);
			}

		private:
			std::ostringstream m_buffer;
			struct std::tm m_stm;
		};
	} // debug
} // codesmith

using namespace codesmith::debug;

/**
 * Helper macros/defines for using the DebugLogger
 */

// Default logger, severity level is ERROR, with a time stamp
#define LOG() DebugLogger()

// Default logger, severity level is ERROR, without a time stamp
#define LOG_NT() DebugLogger()

// These variants show system time
#define LOG_INFO() DebugLogger(DebugLogLevel::EINFO)
#define LOG_WARN() DebugLogger(DebugLogLevel::EWARN)
#define LOG_ERROR() DebugLogger(DebugLogLevel::EERROR)

// These variants omit the system time and only show the warning level
#define LOG_INFO_NT() DebugLogger(DebugLogLevel::EINFO, false)
#define LOG_WARN_NT() DebugLogger(DebugLogLevel::EWARN, false)
#define LOG_ERROR_NT() DebugLogger(DebugLogLevel::EERROR, false)

#endif // __DEBUGLOGGER_DEFINED_H__
