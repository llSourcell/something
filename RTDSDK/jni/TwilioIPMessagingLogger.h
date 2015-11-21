#ifndef _TwilioIPMessagingLogger_H_
#define _TwilioIPMessagingLogger_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    TIPM_LOG_LEVEL_NONE = 0,
    TIPM_LOG_LEVEL_ERROR = 1,
    TIPM_LOG_LEVEL_WARN = 2,
    TIPM_LOG_LEVEL_INFO = 3,
    TIPM_LOG_LEVEL_DEBUG = 4
} TIPMLogLevel;

void tipm_jni_set_log_level(TIPMLogLevel level);

#define LOG_DEBUG(tag, fmt, ...)  _tipm_jni_log_printer(__func__, __LINE__, TIPM_LOG_LEVEL_DEBUG, tag, fmt, ##__VA_ARGS__)
#define LOG_INFO(tag, fmt, ...)  _tipm_jni_log_printer(__func__, __LINE__, TIPM_LOG_LEVEL_INFO, tag, fmt, ##__VA_ARGS__)
#define LOG_WARN(tag, fmt, ...)  _tipm_jni_log_printer(__func__, __LINE__, TIPM_LOG_LEVEL_WARN, tag, fmt, ##__VA_ARGS__)
#define LOG_ERROR(tag, fmt, ...)  _tipm_jni_log_printer(__func__, __LINE__, TIPM_LOG_LEVEL_ERROR, tag, fmt, ##__VA_ARGS__)

void _tipm_jni_log_printer(const char *function,
                        int line,
                        int level,
                        const char *tag,
                        const char *fmt,
                        ...);

#ifdef __cplusplus
}
#endif

#endif  /* !_TwilioIPMessagingLogger_H_ */
