#ifndef LOG_H
#define LOG_H

void log_open(const char *path);
void log_msg(const char *format, ...);
void log_close(void);

#endif