#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>

typedef enum LogLevel {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_ERROR = 3,
    LOG_LEVEL_NONE = 4
} LogLevel;

void logger_set_level(LogLevel level);
LogLevel logger_get_level(void);
void logger_log(LogLevel level, const char* file, int line, const char* fmt, ...);
void logger_vlog(LogLevel level, const char* file, int line, const char* fmt, va_list args);

#define LOG_DEBUG(fmt, ...) logger_log(LOG_LEVEL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) logger_log(LOG_LEVEL_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) logger_log(LOG_LEVEL_WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) logger_log(LOG_LEVEL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif
