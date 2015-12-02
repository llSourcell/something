#include <stdio.h>
#include <stdarg.h>
#include "TwilioIPMessagingLogger.h"

static int log_level = TIPM_LOG_LEVEL_ERROR;

#ifdef __ANDROID__
#include <android/log.h>

static int
level_to_android_level(TIPMLogLevel level)
{
    switch (level) {
        case TIPM_LOG_LEVEL_ERROR:
            return ANDROID_LOG_ERROR;
        case TIPM_LOG_LEVEL_WARN:
            return ANDROID_LOG_WARN;
        case TIPM_LOG_LEVEL_INFO:
            return ANDROID_LOG_INFO;
        case TIPM_LOG_LEVEL_DEBUG:
            return ANDROID_LOG_DEBUG;
        default:
            return ANDROID_LOG_VERBOSE;
    }
}
#else
static const char *
level_to_str(TIPMLogLevel level)
{
    switch (level) {
        case TIPM_LOG_LEVEL_ERROR:
            return "ERROR";
        case TIPM_LOG_LEVEL_WARN:
            return "WARN";
        case TIPM_LOG_LEVEL_INFO:
            return "INFO";
        case TIPM_LOG_LEVEL_DEBUG:
            return "DEBUG";
        default:
            return "??";
    }
}
#endif

void
tipm_jni_set_log_level(TIPMLogLevel level)
{
    log_level = level;
}

void
_tipm_jni_log_printer(const char *function,
                     int line,
                     int level,
                     const char *tag,
                     const char *fmt,
                     ...)
{
    va_list var_args;
    if (level > log_level) {
        return;
    }

    va_start(var_args, fmt);

#ifdef __ANDROID__
    __android_log_vprint(level_to_android_level(level), tag, fmt, var_args);
#else
    fprintf(stderr, "%s: [%s():%d: %s] ", level_to_str(level), function, line, tag);
    vfprintf(stderr, fmt, var_args);
    fprintf(stderr, "\n");
#endif

    va_end(var_args);
}
