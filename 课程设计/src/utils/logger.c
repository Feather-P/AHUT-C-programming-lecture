#include "logger.h"

#include <stdio.h>
#include <time.h>

static LogLevel g_log_level = LOG_LEVEL_INFO;

static const char* logger_level_to_string(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
        case LOG_LEVEL_INFO:
            return "INFO";
        case LOG_LEVEL_WARN:
            return "WARN";
        case LOG_LEVEL_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

void logger_set_level(LogLevel level) {
    g_log_level = level;
}

LogLevel logger_get_level(void) {
    return g_log_level;
}

void logger_vlog(LogLevel level, const char* file, int line, const char* fmt, va_list args) {
    time_t now;
    struct tm tm_snapshot;
    char timebuf[20];

    if (fmt == NULL) return;
    if (level < g_log_level || level >= LOG_LEVEL_NONE) return;

    now = time(NULL);
    tm_snapshot = *localtime(&now);
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm_snapshot);

    fprintf(stderr, "[%s] [%s] [%s:%d] ", timebuf, logger_level_to_string(level), file == NULL ? "<unknown>" : file, line);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
}

void logger_log(LogLevel level, const char* file, int line, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    logger_vlog(level, file, line, fmt, args);
    va_end(args);
}
