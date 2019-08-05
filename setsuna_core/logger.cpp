#include <setsuna/logger.h>
#include <cstdio>
#include <cstdarg>

namespace setsuna {

logger::logger() :
    m_level{LL_ERROR} {}

void logger::log(log_level level, const char* fmt, ...) {
	if (level > m_level) return;

	char* prefix;
	switch (level) {
	case LL_ERROR:
		prefix = "[ERROR]";
		break;
	case LL_WARNING:
		prefix = "[WARNING]";
		break;
	case LL_MESSAGE:
		prefix = "[MESSAGE]";
		break;
	default:
		prefix = "[DEBUG]";
		break;
	}

	char tmp[512], *msg = tmp;
	va_list vl;
	va_start(vl, fmt);
	std::size_t size = (std::size_t)_vscprintf(fmt, vl) + 1;
	if (size >= sizeof(tmp)) msg = new char[size];
	vsnprintf_s(msg, size, size - 1, fmt, vl);
	printf_s("%-12s%s\n", prefix, msg);
	va_end(vl);

	if (msg != tmp) delete[] msg;
}

void logger::set_log_level(log_level level) {
	m_level = level;
}

}  // namespace setsuna
