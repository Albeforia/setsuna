#pragma once

#include <cstdint>

/** @file
@brief Header for @ref setsuna::logger
*/

namespace setsuna {

/**
@brief Simple logger

@see @ref LOG_ERROR() @ref LOG_WARNING() @ref LOG_MESSAGE() @ref LOG_DEBUG()
*/
class logger {

public:
	/**
	@brief Log level

	Since @p LL_DEBUG has the lowest level, @ref LOG_DEBUG() will log its message
	despite the current log level setting in a debug build. In a release build, @ref LOG_DEBUG()
	will be completely ignored.

	@p LL_ERROR is the default log level.
	*/
	enum log_level : uint32_t {
		LL_DEBUG,
		LL_MESSAGE,
		LL_WARNING,
		LL_ERROR,
	};

	/**
	@brief Get the logger singleton
	*/
	static logger& instance() {
		static logger _instance;
		return _instance;
	}

	logger(const logger&) = delete;
	logger& operator=(const logger&) = delete;

	void log(log_level level, const char* fmt, ...);

	/**
	@brief Set current log level
	*/
	void set_log_level(log_level level);

private:
	logger();

	log_level m_level;
};

/**
@brief Log error message
*/
#define LOG_ERROR(fmt, ...) setsuna::logger::instance().log(setsuna::logger::LL_ERROR, fmt, __VA_ARGS__);

/**
@brief Log warning message
*/
#define LOG_WARNING(fmt, ...) setsuna::logger::instance().log(setsuna::logger::LL_WARNING, fmt, __VA_ARGS__);

/**
@brief Log normal message
*/
#define LOG_MESSAGE(fmt, ...) setsuna::logger::instance().log(setsuna::logger::LL_MESSAGE, fmt, __VA_ARGS__);

/**
@brief Log debug message
*/
#ifdef _DEBUG
#	define LOG_DEBUG(fmt, ...) setsuna::logger::instance().log(setsuna::logger::LL_DEBUG, fmt, __VA_ARGS__);
#else
#	define LOG_DEBUG(fmt, ...)
#endif

}  // namespace setsuna
